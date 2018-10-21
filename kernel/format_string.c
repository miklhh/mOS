// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

// 
// This file contains functions for formating special string sequences. See each
// function for its usage.
//

#include <string.h>
#include <kstdio.h>

// Function for formating memory. The result is a NULL terminated string with 
// with no more than three digits, followd by suitible suffix, stored in 'dst'.
// 'dst' should be long enought to contain the result (at least 8 char!).
void format_memory(char *dst, unsigned bytes)
{
    if (bytes < 0x400)
    {
        // Byte format.
        ksprintf(dst, "%u B", bytes);
    }
    else if (bytes < 0x100000)
    {
        bytes /= 0x400;
        ksprintf(dst, "%u kB", bytes);
    }
    else if (bytes < 0x40000000)
    {
        bytes /= 0x100000;
        ksprintf(dst, "%u MB", bytes);
    }
    else
    {
        bytes /= 0x40000000;
        ksprintf(dst, "%u GB", bytes);
    }
    return;
}

