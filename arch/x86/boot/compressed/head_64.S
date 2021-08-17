/*
 *  linux/boot/head.S
 *
 *  Copyright (C) 1991, 1992, 1993  Linus Torvalds
 */

/*
 *  head.S contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00001000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory. [According to comments etc elsewhere on a compressed
 * kernel it will end up at 0x1000 + 1Mb I hope so as I assume this. - AC]
 *
 * Page 0 is deliberately kept safe, since System Management Mode code in 
 * laptops may need to access the BIOS data stored there.  This is also
 * useful for future device drivers that either access the BIOS via VM86 
 * mode.
 */

/*
 * High loaded stuff by Hans Lermen & Werner Almesberger, Feb. 1996
 */
	.code32
	.text

#include <linux/init.h>
#include <linux/linkage.h>
#include <asm/segment.h>
<<<<<<< HEAD
#include <asm/pgtable_types.h>
#include <asm/page_types.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/boot.h>
#include <asm/msr.h>
#include <asm/processor-flags.h>
#include <asm/asm-offsets.h>
<<<<<<< HEAD
=======
#include <asm/bootparam.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Locally defined symbols should be marked hidden:
 */
	.hidden _bss
	.hidden _ebss
	.hidden _got
	.hidden _egot
<<<<<<< HEAD
=======
	.hidden _end
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	__HEAD
	.code32
ENTRY(startup_32)
	/*
	 * 32bit entry is 0 and it is ABI so immutable!
	 * If we come here directly from a bootloader,
	 * kernel(text+data+bss+brk) ramdisk, zero_page, command line
	 * all need to be under the 4G limit.
	 */
	cld
	/*
	 * Test KEEP_SEGMENTS flag to see if the bootloader is asking
	 * us to not reload segments
	 */
<<<<<<< HEAD
	testb $(1<<6), BP_loadflags(%esi)
=======
	testb $KEEP_SEGMENTS, BP_loadflags(%esi)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	jnz 1f

	cli
	movl	$(__BOOT_DS), %eax
	movl	%eax, %ds
	movl	%eax, %es
	movl	%eax, %ss
1:

/*
 * Calculate the delta between where we were compiled to run
 * at and where we were actually loaded at.  This can only be done
 * with a short local call on x86.  Nothing  else will tell us what
 * address we are running at.  The reserved chunk of the real-mode
 * data at 0x1e4 (defined as a scratch field) are used as the stack
 * for this calculation. Only 4 bytes are needed.
 */
	leal	(BP_scratch+4)(%esi), %esp
	call	1f
1:	popl	%ebp
	subl	$1b, %ebp

/* setup a stack and make sure cpu supports long mode. */
	movl	$boot_stack_end, %eax
	addl	%ebp, %eax
	movl	%eax, %esp

	call	verify_cpu
	testl	%eax, %eax
	jnz	no_longmode

/*
 * Compute the delta between where we were compiled to run at
 * and where the code will actually run at.
 *
 * %ebp contains the address we are loaded at by the boot loader and %ebx
 * contains the address where we should move the kernel image temporarily
 * for safe in-place decompression.
 */

#ifdef CONFIG_RELOCATABLE
	movl	%ebp, %ebx
	movl	BP_kernel_alignment(%esi), %eax
	decl	%eax
	addl	%eax, %ebx
	notl	%eax
	andl	%eax, %ebx
<<<<<<< HEAD
#else
	movl	$LOAD_PHYSICAL_ADDR, %ebx
#endif

	/* Target address to relocate to for decompression */
	addl	$z_extract_offset, %ebx
=======
	cmpl	$LOAD_PHYSICAL_ADDR, %ebx
	jae	1f
#endif
	movl	$LOAD_PHYSICAL_ADDR, %ebx
1:

	/* Target address to relocate to for decompression */
	movl	BP_init_size(%esi), %eax
	subl	$_end, %eax
	addl	%eax, %ebx
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Prepare for entering 64 bit mode
 */

	/* Load new GDT with the 64bit segments using 32bit descriptor */
	leal	gdt(%ebp), %eax
	movl	%eax, gdt+2(%ebp)
	lgdt	gdt(%ebp)

	/* Enable PAE mode */
<<<<<<< HEAD
	movl	$(X86_CR4_PAE), %eax
=======
	movl	%cr4, %eax
	orl	$X86_CR4_PAE, %eax
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	movl	%eax, %cr4

 /*
  * Build early 4G boot pagetable
  */
	/* Initialize Page tables to 0 */
	leal	pgtable(%ebx), %edi
	xorl	%eax, %eax
<<<<<<< HEAD
	movl	$((4096*6)/4), %ecx
=======
	movl	$(BOOT_INIT_PGT_SIZE/4), %ecx
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rep	stosl

	/* Build Level 4 */
	leal	pgtable + 0(%ebx), %edi
	leal	0x1007 (%edi), %eax
	movl	%eax, 0(%edi)

	/* Build Level 3 */
	leal	pgtable + 0x1000(%ebx), %edi
	leal	0x1007(%edi), %eax
	movl	$4, %ecx
1:	movl	%eax, 0x00(%edi)
	addl	$0x00001000, %eax
	addl	$8, %edi
	decl	%ecx
	jnz	1b

	/* Build Level 2 */
	leal	pgtable + 0x2000(%ebx), %edi
	movl	$0x00000183, %eax
	movl	$2048, %ecx
1:	movl	%eax, 0(%edi)
	addl	$0x00200000, %eax
	addl	$8, %edi
	decl	%ecx
	jnz	1b

	/* Enable the boot page tables */
	leal	pgtable(%ebx), %eax
	movl	%eax, %cr3

	/* Enable Long mode in EFER (Extended Feature Enable Register) */
	movl	$MSR_EFER, %ecx
	rdmsr
	btsl	$_EFER_LME, %eax
	wrmsr

	/* After gdt is loaded */
	xorl	%eax, %eax
	lldt	%ax
<<<<<<< HEAD
	movl    $0x20, %eax
=======
	movl    $__BOOT_TSS, %eax
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ltr	%ax

	/*
	 * Setup for the jump to 64bit mode
	 *
	 * When the jump is performend we will be in long mode but
	 * in 32bit compatibility mode with EFER.LME = 1, CS.L = 0, CS.D = 1
	 * (and in turn EFER.LMA = 1).	To jump into 64bit mode we use
	 * the new gdt/idt that has __KERNEL_CS with CS.L = 1.
	 * We place all of the values on our mini stack so lret can
	 * used to perform that far jump.
	 */
	pushl	$__KERNEL_CS
	leal	startup_64(%ebp), %eax
<<<<<<< HEAD
=======
#ifdef CONFIG_EFI_MIXED
	movl	efi32_config(%ebp), %ebx
	cmp	$0, %ebx
	jz	1f
	leal	handover_entry(%ebp), %eax
1:
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pushl	%eax

	/* Enter paged protected Mode, activating Long Mode */
	movl	$(X86_CR0_PG | X86_CR0_PE), %eax /* Enable Paging and Protected mode */
	movl	%eax, %cr0

	/* Jump from 32bit compatibility mode into 64bit mode. */
	lret
ENDPROC(startup_32)

<<<<<<< HEAD
=======
#ifdef CONFIG_EFI_MIXED
	.org 0x190
ENTRY(efi32_stub_entry)
	add	$0x4, %esp		/* Discard return address */
	popl	%ecx
	popl	%edx
	popl	%esi

	leal	(BP_scratch+4)(%esi), %esp
	call	1f
1:	pop	%ebp
	subl	$1b, %ebp

	movl	%ecx, efi32_config(%ebp)
	movl	%edx, efi32_config+8(%ebp)
	sgdtl	efi32_boot_gdt(%ebp)

	leal	efi32_config(%ebp), %eax
	movl	%eax, efi_config(%ebp)

	/* Disable paging */
	movl	%cr0, %eax
	btrl	$X86_CR0_PG_BIT, %eax
	movl	%eax, %cr0

	jmp	startup_32
ENDPROC(efi32_stub_entry)
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.code64
	.org 0x200
ENTRY(startup_64)
	/*
	 * 64bit entry is 0x200 and it is ABI so immutable!
	 * We come here either from startup_32 or directly from a
	 * 64bit bootloader.
	 * If we come here from a bootloader, kernel(text+data+bss+brk),
	 * ramdisk, zero_page, command line could be above 4G.
	 * We depend on an identity mapped page table being provided
	 * that maps our entire kernel(text+data+bss+brk), zero page
	 * and command line.
	 */
#ifdef CONFIG_EFI_STUB
	/*
	 * The entry point for the PE/COFF executable is efi_pe_entry, so
	 * only legacy boot loaders will execute this jmp.
	 */
	jmp	preferred_addr

ENTRY(efi_pe_entry)
<<<<<<< HEAD
	mov	%rcx, %rdi
	mov	%rdx, %rsi
	pushq	%rdi
	pushq	%rsi
	call	make_boot_params
	cmpq	$0,%rax
	je	1f
	mov	%rax, %rdx
	leaq	startup_32(%rip), %rax
	movl	%eax, BP_code32_start(%rdx)
	popq	%rsi
	popq	%rdi

ENTRY(efi_stub_entry)
=======
	movq	%rcx, efi64_config(%rip)	/* Handle */
	movq	%rdx, efi64_config+8(%rip) /* EFI System table pointer */

	leaq	efi64_config(%rip), %rax
	movq	%rax, efi_config(%rip)

	call	1f
1:	popq	%rbp
	subq	$1b, %rbp

	/*
	 * Relocate efi_config->call().
	 */
	addq	%rbp, efi64_config+32(%rip)

	movq	%rax, %rdi
	call	make_boot_params
	cmpq	$0,%rax
	je	fail
	mov	%rax, %rsi
	leaq	startup_32(%rip), %rax
	movl	%eax, BP_code32_start(%rsi)
	jmp	2f		/* Skip the relocation */

handover_entry:
	call	1f
1:	popq	%rbp
	subq	$1b, %rbp

	/*
	 * Relocate efi_config->call().
	 */
	movq	efi_config(%rip), %rax
	addq	%rbp, 32(%rax)
2:
	movq	efi_config(%rip), %rdi
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	call	efi_main
	movq	%rax,%rsi
	cmpq	$0,%rax
	jne	2f
<<<<<<< HEAD
1:
	/* EFI init failed, so hang. */
	hlt
	jmp	1b
=======
fail:
	/* EFI init failed, so hang. */
	hlt
	jmp	fail
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
2:
	movl	BP_code32_start(%esi), %eax
	leaq	preferred_addr(%rax), %rax
	jmp	*%rax

preferred_addr:
#endif

	/* Setup data segments. */
	xorl	%eax, %eax
	movl	%eax, %ds
	movl	%eax, %es
	movl	%eax, %ss
	movl	%eax, %fs
	movl	%eax, %gs

	/*
	 * Compute the decompressed kernel start address.  It is where
	 * we were loaded at aligned to a 2M boundary. %rbp contains the
	 * decompressed kernel start address.
	 *
	 * If it is a relocatable kernel then decompress and run the kernel
	 * from load address aligned to 2MB addr, otherwise decompress and
	 * run the kernel from LOAD_PHYSICAL_ADDR
	 *
	 * We cannot rely on the calculation done in 32-bit mode, since we
	 * may have been invoked via the 64-bit entry point.
	 */

	/* Start with the delta to where the kernel will run at. */
#ifdef CONFIG_RELOCATABLE
	leaq	startup_32(%rip) /* - $startup_32 */, %rbp
	movl	BP_kernel_alignment(%rsi), %eax
	decl	%eax
	addq	%rax, %rbp
	notq	%rax
	andq	%rax, %rbp
<<<<<<< HEAD
#else
	movq	$LOAD_PHYSICAL_ADDR, %rbp
#endif

	/* Target address to relocate to for decompression */
	leaq	z_extract_offset(%rbp), %rbx
=======
	cmpq	$LOAD_PHYSICAL_ADDR, %rbp
	jae	1f
#endif
	movq	$LOAD_PHYSICAL_ADDR, %rbp
1:

	/* Target address to relocate to for decompression */
	movl	BP_init_size(%rsi), %ebx
	subl	$_end, %ebx
	addq	%rbp, %rbx
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Set up the stack */
	leaq	boot_stack_end(%rbx), %rsp

	/* Zero EFLAGS */
	pushq	$0
	popfq

/*
 * Copy the compressed kernel to the end of our buffer
 * where decompression in place becomes safe.
 */
	pushq	%rsi
	leaq	(_bss-8)(%rip), %rsi
	leaq	(_bss-8)(%rbx), %rdi
	movq	$_bss /* - $startup_32 */, %rcx
	shrq	$3, %rcx
	std
	rep	movsq
	cld
	popq	%rsi

/*
 * Jump to the relocated address.
 */
	leaq	relocated(%rbx), %rax
	jmp	*%rax

<<<<<<< HEAD
=======
#ifdef CONFIG_EFI_STUB
	.org 0x390
ENTRY(efi64_stub_entry)
	movq	%rdi, efi64_config(%rip)	/* Handle */
	movq	%rsi, efi64_config+8(%rip) /* EFI System table pointer */

	leaq	efi64_config(%rip), %rax
	movq	%rax, efi_config(%rip)

	movq	%rdx, %rsi
	jmp	handover_entry
ENDPROC(efi64_stub_entry)
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.text
relocated:

/*
 * Clear BSS (stack is currently empty)
 */
	xorl	%eax, %eax
	leaq    _bss(%rip), %rdi
	leaq    _ebss(%rip), %rcx
	subq	%rdi, %rcx
	shrq	$3, %rcx
	rep	stosq

/*
 * Adjust our own GOT
 */
	leaq	_got(%rip), %rdx
	leaq	_egot(%rip), %rcx
1:
	cmpq	%rcx, %rdx
	jae	2f
	addq	%rbx, (%rdx)
	addq	$8, %rdx
	jmp	1b
2:
	
/*
<<<<<<< HEAD
 * Do the decompression, and jump to the new kernel..
=======
 * Do the extraction, and jump to the new kernel..
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
	pushq	%rsi			/* Save the real mode argument */
	movq	%rsi, %rdi		/* real mode address */
	leaq	boot_heap(%rip), %rsi	/* malloc area for uncompression */
	leaq	input_data(%rip), %rdx  /* input_data */
	movl	$z_input_len, %ecx	/* input_len */
	movq	%rbp, %r8		/* output target address */
<<<<<<< HEAD
	call	decompress_kernel
=======
	movq	$z_output_len, %r9	/* decompressed length, end of relocs */
	call	extract_kernel		/* returns kernel location in %rax */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	popq	%rsi

/*
 * Jump to the decompressed kernel.
 */
<<<<<<< HEAD
	jmp	*%rbp
=======
	jmp	*%rax
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.code32
no_longmode:
	/* This isn't an x86-64 CPU so hang */
1:
	hlt
	jmp     1b

#include "../../kernel/verify_cpu.S"

	.data
gdt:
	.word	gdt_end - gdt
	.long	gdt
	.word	0
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00af9a000000ffff	/* __KERNEL_CS */
	.quad	0x00cf92000000ffff	/* __KERNEL_DS */
	.quad	0x0080890000000000	/* TS descriptor */
	.quad   0x0000000000000000	/* TS continued */
gdt_end:

<<<<<<< HEAD
=======
#ifdef CONFIG_EFI_STUB
efi_config:
	.quad	0

#ifdef CONFIG_EFI_MIXED
	.global efi32_config
efi32_config:
	.fill	4,8,0
	.quad	efi64_thunk
	.byte	0
#endif

	.global efi64_config
efi64_config:
	.fill	4,8,0
	.quad	efi_call
	.byte	1
#endif /* CONFIG_EFI_STUB */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Stack and heap for uncompression
 */
	.bss
	.balign 4
boot_heap:
	.fill BOOT_HEAP_SIZE, 1, 0
boot_stack:
	.fill BOOT_STACK_SIZE, 1, 0
boot_stack_end:

/*
 * Space for page tables (not in .bss so not zeroed)
 */
	.section ".pgtable","a",@nobits
	.balign 4096
pgtable:
<<<<<<< HEAD
	.fill 6*4096, 1, 0
=======
	.fill BOOT_PGT_SIZE, 1, 0
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
