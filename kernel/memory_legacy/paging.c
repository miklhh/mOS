// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <memory/paging.h>
#include <memory/kalloc.h>
#include <assert.h>
#include <string.h>
#include <kstdio.h>
#include <irq.h>

// Macros used in the bitset algorithm.
#define INDEX_FROM_BIT(b)   (b / 0x20)
#define OFFSET_FROM_BIT(b)  (b % 0x20)

uint32_t *frames;
uint32_t nframes;

// Current page directory.
static page_directory_t *current_directory;
static page_directory_t *kernel_directory;


static void set_frame(uintptr_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] |= (uint32_t) 0x01 << offset;
}

static void clear_frame(uintptr_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~( (uint32_t) 0x01 << offset );
}

static uint32_t test_frame(uintptr_t frame_addr)
{
    uint32_t frame = frame_addr / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return frames[index] & ((uint32_t) 0x01 << offset);
}

static uint32_t first_frame()
{
    for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); ++i)
    {
        if (frames[i] != 0xffffffff)
        {
            for (uint32_t j = 0; j < 32; ++j)
            {
                uint32_t test_frame = (uint32_t) 0x01 << j;
                if ( !(frames[i] & test_frame) )
                {
                    return i * 0x20 + j;
                }
            }
        }
    }

    // The function should never come to this place.
    kprintf("Failed to retrive the first frame from the bitset. Fatal error.");
    halt_and_shutdown();
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        // Frame was already allocated.
        return;
    }
    else
    {
        uint32_t index = first_frame();
        if (index == 0xffffffff)
        {
            // This should not happen.
            kprintf("No free frames.");
            halt_and_shutdown();
        }
        set_frame(index * 0x1000);
        page->present = 1;
        page->rw = is_writeable ? 1 : 0;
        page->user = is_kernel  ? 0 : 1;
        page->frame = index;
    }
}


void dma_frame(page_t *page, int is_kernel, int is_writeable, uintptr_t address)
{
    assert(page != NULL && "Page is NULL.");
    page->present = 1;
    page->rw      = is_writeable ? 1 : 0;
    page->user    = is_kernel    ? 0 : 1;
    page->frame   = address / 0x1000;
    set_frame(address);
}

void free_frame(page_t *page)
{
    uint32_t frame;
    if ( !(frame=page->frame) )
    {
        // The given page didn't actually have an allocated frame!
        return;
    }
    else
    {
        // Clear the frame and deassociate the frame.
        clear_frame(frame);
        page->frame = 0x00;
    }
}

void system_init_paging(unsigned mem_size)
{
    // Setup the bitset.
    nframes = mem_size / 4;
    frames = (uint32_t *) kmalloc(INDEX_FROM_BIT(nframes * 8));
    memset(frames, 0, INDEX_FROM_BIT(nframes * 8));

    // Create kernel page directory.
    uintptr_t phys;
    kernel_directory = (page_directory_t *) kvmalloc_p(sizeof(page_directory_t), &phys);
    memset(kernel_directory, 0, sizeof(page_directory_t));

    // Identity map the kernel which is located at 0x00100000. Everything after
    // this address is considered a part of the kernel, and so it will be mapped
    // accordingly.
    for (uintptr_t i = 0; i < mem_size + 0x00100000; i += 0x1000)
    {
        // Kernel code is readable but not writeable from userspace.
        uintptr_t address = 0x00000000 + i;
        dma_frame( get_page(i, 1, kernel_directory), 0, 0, address );
    }

    // Set the kernel page directory.
    kernel_directory->physical_address = (uintptr_t) kernel_directory->tables_physical;
    current_directory = kernel_directory;
    switch_page_directory(kernel_directory);

    // Tha page-fault handler was already initialized and added to the vector
    // table earlier.
}

page_t *get_page(uintptr_t address, int make, page_directory_t *dir)
{
    address /= 0x1000;
    uint32_t table_index = address / 1024;
    if(dir->tables[table_index])
    {
        return &dir->tables[table_index]->pages[address % 1024];
    }
    else if (make)
    {
        uint32_t temp;
        dir->tables[table_index] = (page_table_t *)kvmalloc_p(sizeof(page_table_t), (uintptr_t *) &temp);
        assert(dir->tables[table_index] != NULL);
        memset(dir->tables[table_index], 0, sizeof(page_table_t));
        dir->tables_physical[table_index] = temp | 0x7; // Present | R/w | User.
        return &dir->tables[table_index]->pages[address % 1024];
    }
    else
    {
        return 0;
    }
}

// Test if paging is enabled. Returns zero if paging is dissabled and one
// otherwise.
int paging_enabled()
{
    uint32_t cr0_reg;
    asm volatile("mov %%cr0, %0" : "=r"(cr0_reg));
    return cr0_reg & 0x80000000 ? 1 : 0;
}

// Switch active page directory.
void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    uint32_t cr0_reg;

    // Set the 31 bit of controll register one to enable paging.
    asm volatile("mov %0, %%cr3" : : "r"(&dir->tables_physical));
    asm volatile("movl %%cr0, %0" : "=r"(cr0_reg));
    cr0_reg |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r"(cr0_reg));
}
