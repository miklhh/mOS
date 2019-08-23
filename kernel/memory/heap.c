// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <memory/heap.h>
#include <memory/heap_allocator/heap.h>
#include <memory/kalloc.h>
#include <format_string.h>
#include <stdint.h>
#include <string.h>

// The kernel heap structure.
heap_t *kernel_heap;

// The kernel heap memory area.
void *kernel_heap_memory_area;

// Initialization routine.
void system_init_heap(uintptr_t mem_end)
{
    // Setup the kernel memory area (960 kB, 4 kB alligned).
    kernel_heap_memory_area = kvmalloc(HEAP_INIT_SIZE);
    memset(kernel_heap_memory_area, 0, HEAP_INIT_SIZE);

    // Setup heap data structure.
    kernel_heap = kmalloc(sizeof(heap_t));
    memset(kernel_heap, 0, sizeof(heap_t));

    // Initialze all heap buckets.
    for (int i = 0; i < BIN_COUNT; ++i)
    {
        kernel_heap->bins[i] = kmalloc(sizeof(bin_t));
        memset(kernel_heap->bins[i], 0, sizeof(bin_t));
    }

    // Initialze the heap. From here on the kalloc family of functions wil use
    // the heap instead of the placement allocation system.
    init_heap(kernel_heap, (long) kernel_heap_memory_area);

    // The result of heap_install will be 4 kB alligned.
    uintptr_t mem_start = heap_install();
    char available_mem_str[10];
    format_memory(available_mem_str, mem_end - mem_start);
}
