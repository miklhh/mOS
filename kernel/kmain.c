#include <boot.h>
#include <stdint.h>

void kmain(struct mboot_info *info, uint32_t magic, uintptr_t stack_bottom)
{
    if (magic != MULTIBOOT_HEADER_MAGIC)
    {
        // PANIC
    }
    while(1) { }
}
