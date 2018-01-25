// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for detailes.
//

#ifndef _SYSTEM_KSTDIO_H
#define _SYSTEM_KSTDIO_H

#include <stdarg.h>

int kprintf(const char *format, ...);
int ksprintf(char *out, const char *format, ...);

#endif

