// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _KALLOC_H
#define _KALLOC_H

#include <memory/heap_allocator/heap.h>

// Routine for initialzeing the kernel allocation system.
void system_init_kalloc();

// Routine called to switch malloc from placement allocation to heap allocation.
uintptr_t heap_install();

// Allocate 'size' bytes of memory on the kernel heap.
void * __attribute__((malloc)) kmalloc(size_t size);

// Create a page alligned allocation of 'size' bytes.
void * __attribute__((malloc)) kvmalloc(size_t size);

// Allocate 'size' bytes of memory on the kernel heap and get the physical
// address of the allocation.
void * __attribute__((malloc)) kmalloc_p(size_t size, uintptr_t *phys);

// Create a page alligned allocation of 'size' bytes and get the physical
// address of the allocation.
void * __attribute__((malloc)) kvmalloc_p(size_t size, uintptr_t *phys);

// Reallocates an already allocaed memory area. 
void * __attribute__((malloc)) krealloc(void *ptr, size_t size);

// Create a zero-initialized array of 'num' elements, each of size 'size'.
void * __attribute__((malloc)) kcalloc(size_t num, size_t size);

// Return memory to the kernel.
void kfree(void *ptr);

#endif
