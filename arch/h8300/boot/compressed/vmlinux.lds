SECTIONS
{
        .text :
        {
        __stext = . ;
	__text = .;
	       *(.text..startup)
	       *(.text)
        __etext = . ;
        }

	.rodata :
	{
		*(.rodata)
	}
<<<<<<< HEAD
        .data :

        {
=======
        . = ALIGN(0x4) ;
        .data :

        {
        . = ALIGN(0x4) ;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
        __sdata = . ;
        ___data_start = . ;
                *(.data.*)
	}
<<<<<<< HEAD
        .bss :
        {
        . = ALIGN(0x4) ;
=======
        . = ALIGN(0x4) ;
        .bss :
        {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
        __sbss = . ;
                *(.bss*)
        . = ALIGN(0x4) ;
        __ebss = . ;
<<<<<<< HEAD
        __end = . ;
        }
=======
        }
        _end = . ;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
