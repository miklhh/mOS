#include <memory/heap.h>
#include <memory/heap_allocator/heap.h>
#include <memory/kalloc.h>
#include <format_string.h>
#include <stdint.h>
#include <string.h>

// The kernel heap structure.
heap_t *kernel_heap;

// The kernel wilderness.
void *kernel_wilderness;

// Initialization routine.
void system_init_heap(uintptr_t mem_end)
{
    // Setup the kernel wilderness (16 MB, 4 kB alligned).
    kernel_wilderness = kvmalloc(0x1000000);
    memset(kernel_wilderness, 0, 0x1000000);

    // Setup heap data structure.
    kernel_heap = kmalloc(sizeof(heap_t));
    memset(kernel_heap, 0, sizeof(heap_t));

    // Initialze all heap buckets.
    for (int i = 0; i < BIN_COUNT; ++i)
    {
        kernel_heap->bins[i] = kmalloc(sizeof(bin_t));
        memset(kernel_heap->bins[i], 0, sizeof(bin_t));
    }

    // Initialze the heap.
    init_heap(kernel_heap, (long) kernel_wilderness);

    // The result of heap_install will be 4 kB alligned.
    uintptr_t mem_start = heap_install();
    char available_mem_str[10];
    format_memory(available_mem_str, mem_end - mem_start);
}
