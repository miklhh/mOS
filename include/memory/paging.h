// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>
#include <system.h>

typedef struct page
{
    uint32_t present    : 1;   // Page present in memory.
    uint32_t rw         : 1;   // Read only if clear, readwrite if set.
    uint32_t user       : 1;   // Ring 0 only access only if cleard
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Unused and reserved.
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits).
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[1024];
    uintptr_t tables_physical[1024];
    uintptr_t physical_address;
} page_directory_t;

// Test if the paging system is enabled. Returns one if enabled, zero otherwise.
int paging_enabled();

// Initialze the paging.
void system_init_paging(unsigned mem_size);

// Switch page directory.
void switch_page_directory(page_directory_t *dir);

// Get the page from a page directory. The page is created if not found if 
// 'make' > 0.
page_t *get_page(uintptr_t address, int make, page_directory_t *dir);

#endif
