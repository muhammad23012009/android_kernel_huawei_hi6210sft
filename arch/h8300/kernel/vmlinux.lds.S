#include <asm-generic/vmlinux.lds.h>
#include <asm/page.h>
<<<<<<< HEAD

/* target memory map */
#ifdef CONFIG_H8300H_GENERIC
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x400000
#endif

#ifdef CONFIG_H8300H_AKI3068NET
#define ROMTOP  0x000000
#define ROMSIZE 0x080000
#define RAMTOP  0x400000
#define RAMSIZE 0x200000
#endif

#ifdef CONFIG_H8300H_H8MAX
#define ROMTOP  0x000000
#define ROMSIZE 0x080000
#define RAMTOP  0x400000
#define RAMSIZE 0x200000
#endif

#ifdef CONFIG_H8300H_SIM
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x400000
#endif

#ifdef CONFIG_H8S_SIM
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x800000
#endif

#ifdef CONFIG_H8S_EDOSK2674
#define ROMTOP  0x000000
#define ROMSIZE 0x400000
#define RAMTOP  0x400000
#define RAMSIZE 0x800000
#endif

#if defined(CONFIG_H8300H_SIM) || defined(CONFIG_H8S_SIM)
INPUT(romfs.o)
#endif

_jiffies = _jiffies_64 + 4;

ENTRY(__start)
=======
#include <asm/thread_info.h>

#define ROMTOP 0x000000
#define RAMTOP 0x400000

jiffies = jiffies_64 + 4;

ENTRY(_start)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

SECTIONS
{
#if defined(CONFIG_ROMKERNEL)
<<<<<<< HEAD
	. = ROMTOP; 
	.vectors :
	{
	__vector = . ;
		*(.vectors*)
	}
#else
	. = RAMTOP; 
	.bootvec :	
	{
		*(.bootvec)
	}
#endif
        .text :
	{
	_text = .;
#if defined(CONFIG_ROMKERNEL)
	*(.int_redirect)
#endif
	__stext = . ;
	TEXT_TEXT
	SCHED_TEXT
	LOCK_TEXT
	__etext = . ;
	}
	EXCEPTION_TABLE(16)

	RODATA
#if defined(CONFIG_ROMKERNEL)
	SECURITY_INIT
#endif
	ROEND = .; 
#if defined(CONFIG_ROMKERNEL)
	. = RAMTOP;
	.data :	AT(ROEND)
#else
	.data :	
#endif
	{
	__sdata = . ;
	___data_start = . ;

	INIT_TASK_DATA(0x2000)
	. = ALIGN(0x4) ;
		DATA_DATA
	. = ALIGN(0x4) ;
		*(.data.*)	

	. = ALIGN(0x4) ;
	___init_begin = .;
	__sinittext = .; 
		INIT_TEXT
	__einittext = .; 
		INIT_DATA
	. = ALIGN(0x4) ;
	INIT_SETUP(0x4)
	___setup_start = .;
		*(.init.setup)
	. = ALIGN(0x4) ;
	___setup_end = .;
	INIT_CALLS
	CON_INITCALL
		EXIT_TEXT
		EXIT_DATA
	INIT_RAM_FS
	. = ALIGN(0x4) ;
	___init_end = .;
	__edata = . ;
	}
#if defined(CONFIG_RAMKERNEL)
	SECURITY_INIT
#endif
	__begin_data = LOADADDR(.data);
        .bss : 
        {
	. = ALIGN(0x4) ;
	__sbss = . ;
		*(.bss*)
	. = ALIGN(0x4) ;
		*(COMMON)
	. = ALIGN(0x4) ;
	__ebss = . ;
	__end = . ;
	__ramstart = .;
	}
        .romfs :	
	{
		*(.romfs*)
	}
	. = RAMTOP+RAMSIZE;
        .dummy :
        {
	COMMAND_START = . - 0x200 ;
	__ramend = . ;
	}

=======
	. = ROMTOP;
	.vectors :
	{
	_vector = . ;
		*(.vector*)
	}
#else
	. = RAMTOP;
	_ramstart = .;
	. = . + CONFIG_OFFSET;
#endif
	_text = .;
	HEAD_TEXT_SECTION
	.text : {
	_stext = . ;
		TEXT_TEXT
		SCHED_TEXT
		CPUIDLE_TEXT
		LOCK_TEXT
#if defined(CONFIG_ROMKERNEL)
		*(.int_redirect)
#endif
	_etext = . ;
	}
	EXCEPTION_TABLE(16)
	NOTES
	RO_DATA_SECTION(4)
	ROMEND = .;
#if defined(CONFIG_ROMKERNEL)
	. = RAMTOP;
	_ramstart = .;
#define ADDR(x) ROMEND
#endif
	_sdata = . ;
	__data_start = . ;
	RW_DATA_SECTION(0, PAGE_SIZE, THREAD_SIZE)
#if defined(CONFIG_ROMKERNEL)
#undef ADDR
#endif
	. = ALIGN(0x4) ;
	__init_begin = .;
	INIT_TEXT_SECTION(4)
	INIT_DATA_SECTION(4)
	SECURITY_INIT
	__init_end = .;
	_edata = . ;
	_begin_data = LOADADDR(.data);
	_sbss =.;
	BSS_SECTION(0, 0 ,0)
	_ebss =.;
	_ramend = .;
	_end = .;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	DISCARDS
}
