//
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <kstdio.h>
#include <stdint.h>
#include <format_string.h>
#include <assert.h>

#define PAGE_SIZE 0x1000

static uint32_t pages;              // Number of pages.
static uint32_t bitset_size_pages;  // Bitmap size in pages.
static uint8_t *page_bitset;        // Pointer to the begining of page bitset.

void system_init_page_frame_allocator(
        uintptr_t wilderness_start,
        uintptr_t wilderness_end)
{
    // Wilderness start and end should already be page aligned.
    assert( (wilderness_start & ~0xFFF) == wilderness_start);
    assert( (wilderness_end & ~0xFFF) == wilderness_end);

    // Acquire initial information.
    pages = (wilderness_end - wilderness_start)/0x1000;
    bitset_size_pages = (pages/0x8 - 1)/0x1000 + 1;

    // Setup the bitset and adjust the new wilderness_start;
    page_bitset = (uint8_t *) wilderness_start;
    wilderness_start += PAGE_SIZE * bitset_size_pages;


    // Print PFA info.
    char size_str[10];
    format_memory(size_str, wilderness_end - wilderness_start);
    kprintf("[ PFA ] Initialized PFA: %d pages @ 0x%08x -> 0x%08x [%s]\n",
            pages, wilderness_start, wilderness_end, size_str);
}

