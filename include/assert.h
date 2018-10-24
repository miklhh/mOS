// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _K_ASSERT_H
#define _K_ASSERT_H

extern void __assert_func(const char *file, int line, const char *func, const char *failedexpr);
#define assert(statement) ( (statement) ? (void) 0 : __assert_func(__FILE__, __LINE__, __FUNCTION__, #statement) )

#endif
