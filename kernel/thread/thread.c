#include <thread.h>
#include <synch.h>
#include <list.h>
#include <idt.h>
#include <assert.h>
#include <vaddr.h>
#include <string.h>

// "Magic" thread number.
#define THREAD_MAGIC 0xBEEFDEED

// Stack offset in the struct thread aggregate.
uint32_t thread_struct_stack_offset = offsetof(struct thread, stack);

// Is kernel threading initialized=
static int kernel_threading_initialized = 0;

// List of ready threads.
static struct list ready_list;

// Idle thread.
static struct thread *idle_thread;

// Initial thread, the thread running kmain.c:kmain(...)
static struct thread *initial_thread;

// Lock used by allocate_tid().
static struct lock tid_lock;

// switch_threads stack frame.
struct __attribute__((packed)) switch_threads_frame
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    void (*eip)();
    struct thread *cur;
    struct thread *next;
};

// switch_entry stack frame:
struct switch_entry_frame
{
    void (*eip)();
};

// Stack frame for kernel_thread().
struct kernel_thread_frame
{
    void *eip;
    void (* func)(void *aux);
    void *aux;
};

// Scheduling.
#define TIME_SLICE 4            // Thread time slice.
static unsigned thread_ticks;   // Ticks since last yield.

// Statistics.
static long long idle_ticks;    // Ticks spent idle.
static long long kernel_ticks;  // Ticks spent by kernel.
static long long user_ticks;    // Ticks spent by user.

// External assembly function for switching thread.
extern struct thread *switch_threads(struct thread *cur, struct thread *next);


// Returns zero if object pointed to by t is not considered a thread. Returns 
// one otherwise.
static int is_thread(struct thread *t)
{
    return t != NULL && t->magic == THREAD_MAGIC;
}


// Returns the running thread. Threading must be initialzed before calling this
// function.
struct thread *thread_current()
{
    uint32_t *esp;
    asm ("mov %%esp, %0" : "=g"(esp));
    //struct thread *t = (struct thread *) esp;
    //assert(is_thread(t));
    //assert(t->status == THREAD_RUNNING);
    return pg_round_down(esp);
}


// Called by timer interrupt handler each timer tick. This function runs 
// exclusivle in an interrupt handler.
void thread_tick()
{
    if (kernel_threading_initialized)
    {
        struct thread *t = thread_current();

        // Update statistics.
        if (t == idle_thread)
            idle_ticks++;
        else if (t->pagedir != NULL)
            user_ticks++;
        else
            kernel_ticks++;

        // Enforce preemption
        if (++thread_ticks >= TIME_SLICE)
        {
            extern void set_yield_on_return_flag();
            set_yield_on_return_flag();
        }
    }
}


// This function schedule_tail, completes a thread switch. This is done by 
// activating the next threads page tables and destroying the previous thread
// if so be needed.
void schedule_tail(struct thread *prev)
{
    struct thread *cur = thread_current();
    assert(intr_get_level() == INTR_OFF);

    // Mark current thread as running.
    cur->status = THREAD_RUNNING;

    // Reset thread time slice.
    thread_ticks = 0;

#ifdef USERPROG
    // Activate the new address space.
    process_activate();
#endif

    // Cleanup dying thread.
    if (prev != NULL && prev->status == THREAD_DYING && prev != initial_thread)
    {
        assert(prev != cur);
        //TODO: Free page here.
        //palloc_free_page(prev);
    }
}

static struct thread *next_thread_to_run()
{
    if (list_empty(&ready_list))
        return idle_thread;
    else
        return list_entry(list_pop_front(&ready_list), struct thread, elem);
}


// Scheduel a thread. When calling this function interrupts must be dissabled
// and the current procces (when implemented) must have changed its state from
// running to some other state. This functions finds andother thread to run and
// switches to that thread.
static void schedule()
{
    struct thread *cur = thread_current();
    struct thread *next = next_thread_to_run();
    struct thread *prev = NULL;
    assert(intr_get_level() == INTR_OFF);
    assert(cur->status != THREAD_RUNNING);
    assert(is_thread(next));

    if (cur != next)
    {
        prev = switch_threads(cur, next);
    }
    schedule_tail(prev);
}


// Returns a new and fresh (not used) tid.
static tid_t allocate_tid()
{
    static tid_t next_tid = 1;
    tid_t tid;

    lock_acquire(&tid_lock);
    tid = next_tid++;
    lock_release(&tid_lock);

    return tid;
}


// Initialze a thread aggregate.
static void init_thread(struct thread *t, const char *name, int priority)
{
    assert(t != NULL);
    assert(PRI_MIN <= priority && priority <= PRI_MAX);
    assert(name != NULL);

    memset(t, 0, sizeof(*t));
    t->status = THREAD_BLOCKED;
    strlcpy(t->name, name, sizeof(t->name));
    t->stack = (uint8_t *) t + PGSIZE;
    t->priority = priority;
    t->magic = THREAD_MAGIC;
}


// Initialze the threading system.
void system_init_threading()
{
    lock_init(&tid_lock);
    list_init(&ready_list);

    // Setup thread structure for the running thread. Here we assume that the
    // kernel thread is page aligned 0x400.
    initial_thread = thread_current();
    init_thread(initial_thread, "kernel_thread", PRI_DEFAULT);
    initial_thread->status = THREAD_RUNNING;
    initial_thread->tid = allocate_tid();
    kernel_threading_initialized = 1;
}


// Allocates a SIZE-byte frame at the top of threads T's stack and returns a
// pointer to the frame's base.
static void * alloc_frame(struct thread *t, size_t size)
{
    assert(is_thread(t));
    assert(size % sizeof(uint32_t) == 0);
    t->stack -= size;
    return t->stack;
}


// Function used as the basis for a kernel thread.
static void kernel_thread(void (*thread_func)(), void *aux)
{
    assert(thread_func != NULL);

    intr_enable();
    thread_func(aux);
    thread_exit();
}


// Exit current thread.
void thread_exit()
{
    assert(!intr_context());
#ifdef USERPROG
    process_cleanup();
#endif
    intr_disable();
    thread_current()->status = THREAD_DYING;
    schedule();
}


// Function for creating a new kernel thread.
tid_t thread_create(const char *name, int priority, 
                    void (* thread_func)(void *), void *aux)
{
    static uint8_t fake_page_unaligned[PGSIZE*2];
    uintptr_t overshoot = (uintptr_t)fake_page_unaligned % PGSIZE;
    uintptr_t fake_page_aligned = (uintptr_t)fake_page_unaligned + PGSIZE - overshoot;
    struct thread *t = (struct thread *) fake_page_aligned;
    struct kernel_thread_frame *kf;
    struct switch_entry_frame *ef;
    struct switch_threads_frame *sf;
    tid_t tid;

    assert(thread_func != NULL);

    // Initialze thread.
    init_thread(t, name, priority);
    tid = t->tid = allocate_tid();

    // Stack frame for kernel_thread().
    kf = alloc_frame(t, sizeof(*kf));
    kf->eip = NULL;
    kf->func = thread_func;
    kf->aux = aux;

    // Stack frame for switch_entry().
    ef = alloc_frame(t, sizeof(*ef));
    ef->eip = (void (*)()) kernel_thread;

    // Stack frame for switch_entry().
    sf = alloc_frame(t, sizeof(*sf));
    extern void switch_entry();
    sf->eip = switch_entry;

    thread_unblock(t);
    return tid;
}


// Block current thread.
void thread_block()
{
    thread_current()->status = THREAD_BLOCKED;
    schedule();
}


// Unblock a thread.
void thread_unblock(struct thread *t)
{
  enum intr_level old_level;
  assert(is_thread(t));
  old_level = intr_disable();
  assert (t->status == THREAD_BLOCKED);
  list_push_back (&ready_list, &t->elem);
  t->status = THREAD_READY;
  intr_set_level (old_level);
}


// Yields the current thread. The current thread is put to sleep, but might be
// woken again immediatly by the scheduler. This routine should only be invoked
// from an IRQ interrupt handler!
void thread_yield()
{
    if (kernel_threading_initialized)
    {
        struct thread *cur = thread_current();
        enum intr_level old_level;

        assert(!intr_context());

        old_level = intr_disable();
        if (cur != idle_thread)
            list_push_back(&ready_list, &cur->elem);
        cur->status = THREAD_READY;
        schedule();
        intr_set_level(old_level);
    }
}
