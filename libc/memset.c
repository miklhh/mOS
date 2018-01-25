// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <string.h>
#include <stdint.h>

void *memset(void *ptr, int val, size_t n)
{
    unsigned char *b = ptr;
    for (size_t i = 0; i < n; ++i)
    {
        b[i] = val;
    }
    return ptr;
}
