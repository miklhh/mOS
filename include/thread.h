// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <stdint.h>
#include <gdt.h>
#include <list.h>

// Thread states.
enum thread_status
{
    THREAD_RUNNING,     // Currently running thread.
    THREAD_READY,       // A thread that is ready to run.
    THREAD_BLOCKED,     // A blocked thread (waiting for something).
    THREAD_DYING        // A dying thread waiting to be cleaned up.
};

// Thread identifier type.
typedef int tid_t;

// Thread priorites.
#define PRI_MIN         0
#define PRI_DEFAULT     31
#define PRI_MAX         63

/* A kernel thread or user process. Taken from MiT/Stanford/LiU's PintOS.
 *
 * Each thread structure is stored in its own 4 kB page.  The
 * thread structure itself sits at the very bottom of the page
 * (at offset 0).  The rest of the page is reserved for the
 * thread's kernel stack, which grows downward from the top of
 * the page (at offset 4 kB).  Here's an illustration:
 *
 *      4 kB +---------------------------------+
 *           |          kernel stack           |
 *           |                |                |
 *           |                |                |
 *           |                V                |
 *           |         grows downward          |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           |                                 |
 *           +---------------------------------+
 *           |              magic              |
 *           |                :                |
 *           |                :                |
 *           |               name              |
 *           |              status             |
 *      0 kB +---------------------------------+
 *
 * The upshot of this is twofold:
 *
 *    1. First, `struct thread' must not be allowed to grow too
 *       big.  If it does, then there will not be enough room for
 *       the kernel stack.  Our base `struct thread' is only a
 *       few bytes in size.  It probably should stay well under 1
 *       kB.
 *
 *    2. Second, kernel stacks must not be allowed to grow too
 *       large.  If a stack overflows, it will corrupt the thread
 *       state.  Thus, kernel functions should not allocate large
 *       structures or arrays as non-static local variables.  Use
 *       dynamic allocation with malloc() or palloc_get_page()
 *       instead.
 *
 * The first symptom of either of these problems will probably be
 * an assertion failure in thread_current(), which checks that
 * the `magic' member of the running thread's `struct thread' is
 * set to THREAD_MAGIC.  Stack overflow will normally change this
 * value, triggering the assertion. 
 */
struct thread
{
    tid_t tid;
    enum thread_status status;
    char name[16];
    uint8_t *stack;
    int priority;
    uint32_t *pagedir;
    unsigned magic;

    // Shared between thread.c and synch.c.
    struct list_elem elem;
};

// Switch_threads routine stack frame after register preservation.
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

// Switch_entry stack frame.
struct switch_entry_frame
{
    void (*eip)();
};

// Stack frame for kernel_thread.
struct kernel_thread_frame
{
    void *eip;
    void (* func)(void *aux);
    void *aux;
};

// Initialization routines.
void system_init_threading();
void system_init_threading_final();

// Block thread.
void thread_block();

// Unblock thread.
void thread_unblock(struct thread *t);

// Get the running thread.
struct thread *thread_current();

// Yield the thread.
void thread_yield();

// Exit current thread.
void thread_exit();

// Create a new kernel thread.
tid_t thread_create(const char *name, int priority, 
                    void (* thread_fun)(void *), void *aux);

// This function schedule_tail, completes a thread switch. This is done by 
// activating the next threads page tables and destroying the previous thread
// if so be needed.
void schedule_tail(struct thread *prev);

// Thread ticking function, should be called by the timer.
void thread_tick();
