ENTRY(_loader)
SECTIONS
{
    . = 0x00100000;
    __kernel_start = .;
    .text BLOCK(4K) : AT(ADDR(.text) - 0x00000000) ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata BLOCK(4K) : AT(ADDR(.rodata) - 0x00000000) ALIGN(4K)
    {
        *(.rodata)
    }
    .data BLOCK(4K) : AT(ADDR(.data) - 0x00000000) ALIGN(4K)
    {
        *(.data)
        *(.symbols)
    }
    .bss BLOCK(4K) : AT(ADDR(.bss) - 0x00000000) ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
    __kernel_end = .;

    /* Possible other sections go here. */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
