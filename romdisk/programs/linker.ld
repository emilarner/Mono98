ENTRY(__entry)
OUTPUT_FORMAT("binary")
SECTIONS {
	. = 0x33399c;
	.text :
	{
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

