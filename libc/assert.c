// 
// Part of mOS (Minimalistic Operating System).
// This software is distributed under the MIT License.
// See the file LICENSE for details.
//

#include <kstdio.h>

// Later on, we want this function to print to 'stderr', be useable in user land
// and it should not freeze the system (just halt the application).
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    kprintf("Assertion failed in: %s:%d (%s): %s\n", file, line, func, failedexpr);

    while(1)
    {
        asm("cli; hlt");
    }
}
