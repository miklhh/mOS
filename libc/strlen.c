// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#include <string.h>

size_t strlen(const char *str)
{
    char null = (char) 0;
    size_t len = 0;
    while (str[len] != null)
    {
        ++len;
    }
    return len;
}
