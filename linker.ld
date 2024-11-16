ENTRY(entry)
OUTPUT_FORMAT("binary")
SECTIONS {
	. = 0x8E00;
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

