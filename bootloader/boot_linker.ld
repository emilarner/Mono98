ENTRY(bootloader)
OUTPUT_FORMAT("binary")
SECTIONS {
	. = 0x7E00;
	.text :
	{
        *(.boot)
		*(.text)
	}
    
    .rodata :
    {
    	*(.rodata)
    }

    .data :
    {
    	*(.data)
    }

    .bss :
    {
    	*(.bss)
    }
}

