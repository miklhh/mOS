// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _SYSTEM_STRING_H
#define _SYSTEM_STRING_H

#include <stddef.h>

// Copying routines.
void *memcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src);

// Other routines:
void *memset(void *ptr, int value, size_t n);
size_t strlen(const char *str);

#endif
