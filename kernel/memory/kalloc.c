// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <multiboot.h>
#include <memory/kalloc.h>
#include <memory/heap_allocator/heap.h>

#define ALIGN       1
#define NO_ALIGN    0

//extern void *__kernel_end;
//static uintptr_t placement_pointer = (uintptr_t) &__kernel_end;
static uintptr_t placement_pointer = (uintptr_t) NULL;
static uintptr_t local_heap_end = (uintptr_t) NULL;

// Routine for initialzeing the kernel allocation system. It will use mem_start 
// and above to accomplish this.
void system_init_kalloc(uintptr_t mem_start)
{
    assert(local_heap_end == 0);
    placement_pointer = mem_start;
}

// Routine for aqcuiering the end of all bootloaded content.
uintptr_t get_kalloc_start(struct multiboot_info *info)
{
    // Fallback if no modules was loaded.
    extern void *__kernel_end;
    uintptr_t kernel_end = (uintptr_t) &__kernel_end;

    if ( !(info->flags & MULTIBOOT_INFO_MODS) )
    {
        // No module data from bootloader.
        return kernel_end;
    }
    else
    {
        if (info->mods_count == 0)
        {
            // No modules loaded by bootloader.
            return kernel_end;
        }
        else
        {
            // Get the ending address of the last module.
            multiboot_module_t *module = (multiboot_module_t *)info->mods_addr;
            return module[info->mods_count - 1].mod_end + 0x20;
        }
    }
}

// This function is called by the system init heap. This will end the placement
// heap allocator and initialize the real heap allocator. Calling this routine
// more than once causes undefined behaviour.
uintptr_t heap_install()
{
    // Make sure that local_heap_end ends 4 kB alligned.
    if (placement_pointer & 0xfff)
    {
        local_heap_end = (placement_pointer + 0x1000) & 0xfffff000;
    }
    else
    {
        local_heap_end = placement_pointer;
    }
    return local_heap_end;
}

// The static kernel static allocation routine.
static void * __attribute__((malloc)) kmalloc_real(
        size_t size, 
        int align, 
        uintptr_t *phys)
{
    // Depending on the kernel launch stage, the kernel allocation works a
    // a little different. If we are in the early boot stage (i.e local_heap_end
    // == NULL) then we want to use James Mollys placement allocator. When the 
    // paging system is enabled we will use the heap allocation system.
    if (local_heap_end == 0)
    {
        // James Mollys placment allocator
        if (align && (placement_pointer & 0x00000fff))
        {
            // Align the placement pointer.
            placement_pointer &= 0xfffff000;
            placement_pointer += 0x00001000;
        }
        if (phys)
        {
            *phys = placement_pointer;
        }
        uintptr_t address = placement_pointer;
        placement_pointer += size;
        return (void *) address;
    }
    else
    {
        // Heap allocation. 
        if (align)
        {
            assert(0 && "This is not implemented yet");
            return 0;
        }
        else
        {
            extern heap_t *kernel_heap;
            return heap_alloc(kernel_heap, size);
        }
    }
}


// Allocate 'size' bytes of memory on the kernel heap.
void * __attribute__((malloc)) kmalloc(size_t size)
{
    return kmalloc_real(size, NO_ALIGN, NULL);
}

// Create a page alligned allocation of 'size' bytes.
void * __attribute__((malloc)) kvmalloc(size_t size)
{
    return kmalloc_real(size, ALIGN, NULL);
}

// Allocate 'size' bytes of memory on the kernel heap and get the physical
// address of the allocation.
void * __attribute__((malloc)) kmalloc_p(size_t size, uintptr_t *phys)
{
    return kmalloc_real(size, NO_ALIGN, phys);
}

// Create a page alligned allocation of 'size' bytes and get the physical
// address of the allocation.
void * __attribute__((malloc)) kvmalloc_p(size_t size, uintptr_t *phys)
{
    return kmalloc_real(size, ALIGN, phys);
}

// Free memory. This is only available when the heap have been initialized.
void kfree(void *ptr)
{
    assert(local_heap_end != 0 && "No heap initialzed. Fatal error.");
    extern heap_t *kernel_heap;
    heap_free(kernel_heap, ptr);
}
