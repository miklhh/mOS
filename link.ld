ENTRY(loader_phys)
SECTIONS
{
    . = 0xC0100000;
    .text BLOCK(4K) : AT(ADDR(.text) - 0xC0000000) ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata BLOCK(4K) : AT(ADDR(.rodata) - 0xC0000000) ALIGN(4K)
    {
        *(.rodata)
    }
    .data BLOCK(4K) : AT(ADDR(.data) - 0xC0000000) ALIGN(4K)
    {
        *(.data)
    }
    .bss BLOCK(4K) : AT(ADDR(.bss) - 0xC0000000) ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
    /* Possible other sections go here. */
}
