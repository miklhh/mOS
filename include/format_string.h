// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#ifndef _FORMAT_STRING_H
#define _FORMAT_STRING_H

// Recomended length of string for usage by the format memory function.
#define FORMAT_MEMORY_STRING_LEN    10

// Function for formating memory. The result is a NULL terminated string with 
// with no more than three digits, followd by suitible suffix, stored in 'dst'.
// 'dst' should be long enought to contain the result (at least 10 char).
void format_memory(char *dst, unsigned bytes);

#endif

