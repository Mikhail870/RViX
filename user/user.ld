ENTRY(start)
SECTIONS
{
    . = 0x0; /* Адрес начала виртуальной памяти и бинарника */
    .text : {
        KEEP(*(.text.start));
        *(.text .text.*);
    }
    .rodata : ALIGN(4) { *(.rodata .rodata.*); }
    .data   : ALIGN(4) { *(.data .data.*); }
    .bss    : ALIGN(4) { *(.bss .bss.*); }
}
