/*
 *  linux/arch/arm/kernel/setup.c
 *
 *  Copyright (C) 1995-2001 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
=======
#include <linux/efi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/utsname.h>
#include <linux/initrd.h>
#include <linux/console.h>
#include <linux/bootmem.h>
#include <linux/seq_file.h>
#include <linux/screen_info.h>
#include <linux/of_platform.h>
#include <linux/init.h>
#include <linux/kexec.h>
#include <linux/of_fdt.h>
#include <linux/cpu.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/proc_fs.h>
#include <linux/memblock.h>
#include <linux/bug.h>
#include <linux/compiler.h>
#include <linux/sort.h>
<<<<<<< HEAD
=======
#include <linux/psci.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/unified.h>
#include <asm/cp15.h>
#include <asm/cpu.h>
#include <asm/cputype.h>
<<<<<<< HEAD
#include <asm/elf.h>
=======
#include <asm/efi.h>
#include <asm/elf.h>
#include <asm/early_ioremap.h>
#include <asm/fixmap.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/procinfo.h>
#include <asm/psci.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/smp_plat.h>
#include <asm/mach-types.h>
#include <asm/cacheflush.h>
#include <asm/cachetype.h>
#include <asm/tlbflush.h>
<<<<<<< HEAD
=======
#include <asm/xen/hypervisor.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/prom.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/system_info.h>
#include <asm/system_misc.h>
#include <asm/traps.h>
#include <asm/unwind.h>
#include <asm/memblock.h>
#include <asm/virt.h>

#include "atags.h"
<<<<<<< HEAD
/* DTS2013031107868 qidechun 2013-03-11 begin */
#ifdef CONFIG_DUMP_SYS_INFO
#include <linux/module.h>
#include <linux/srecorder.h>
#endif
/* DTS2013031107868 qidechun 2013-03-11 end */
#include <asm/fiq_glue.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414


#if defined(CONFIG_FPE_NWFPE) || defined(CONFIG_FPE_FASTFPE)
char fpe_type[8];

static int __init fpe_setup(char *line)
{
	memcpy(fpe_type, line, 8);
	return 1;
}

__setup("fpe=", fpe_setup);
#endif

<<<<<<< HEAD
extern void paging_init(struct machine_desc *desc);
extern void sanity_check_meminfo(void);
extern void reboot_setup(char *str);
extern void setup_dma_zone(struct machine_desc *desc);
=======
extern void init_default_cache_policy(unsigned long);
extern void paging_init(const struct machine_desc *desc);
extern void early_paging_init(const struct machine_desc *);
extern void adjust_lowmem_bounds(void);
extern enum reboot_mode reboot_mode;
extern void setup_dma_zone(const struct machine_desc *desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

unsigned int processor_id;
EXPORT_SYMBOL(processor_id);
unsigned int __machine_arch_type __read_mostly;
EXPORT_SYMBOL(__machine_arch_type);
unsigned int cacheid __read_mostly;
EXPORT_SYMBOL(cacheid);

unsigned int __atags_pointer __initdata;

unsigned int system_rev;
EXPORT_SYMBOL(system_rev);

<<<<<<< HEAD
=======
const char *system_serial;
EXPORT_SYMBOL(system_serial);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unsigned int system_serial_low;
EXPORT_SYMBOL(system_serial_low);

unsigned int system_serial_high;
EXPORT_SYMBOL(system_serial_high);

unsigned int elf_hwcap __read_mostly;
EXPORT_SYMBOL(elf_hwcap);

<<<<<<< HEAD

#ifdef MULTI_CPU
struct processor processor __read_mostly;
#endif
#ifdef MULTI_TLB
struct cpu_tlb_fns cpu_tlb __read_mostly;
#endif
#ifdef MULTI_USER
struct cpu_user_fns cpu_user __read_mostly;
#endif
#ifdef MULTI_CACHE
struct cpu_cache_fns cpu_cache __read_mostly;
#endif
#ifdef CONFIG_OUTER_CACHE
struct outer_cache_fns outer_cache __read_mostly;
=======
unsigned int elf_hwcap2 __read_mostly;
EXPORT_SYMBOL(elf_hwcap2);


#ifdef MULTI_CPU
struct processor processor __ro_after_init;
#if defined(CONFIG_BIG_LITTLE) && defined(CONFIG_HARDEN_BRANCH_PREDICTOR)
struct processor *cpu_vtable[NR_CPUS] = {
	[0] = &processor,
};
#endif
#endif
#ifdef MULTI_TLB
struct cpu_tlb_fns cpu_tlb __ro_after_init;
#endif
#ifdef MULTI_USER
struct cpu_user_fns cpu_user __ro_after_init;
#endif
#ifdef MULTI_CACHE
struct cpu_cache_fns cpu_cache __ro_after_init;
#endif
#ifdef CONFIG_OUTER_CACHE
struct outer_cache_fns outer_cache __ro_after_init;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL(outer_cache);
#endif

/*
 * Cached cpu_architecture() result for use by assembler code.
 * C code should use the cpu_architecture() function instead of accessing this
 * variable directly.
 */
int __cpu_architecture __read_mostly = CPU_ARCH_UNKNOWN;

struct stack {
	u32 irq[3];
	u32 abt[3];
	u32 und[3];
<<<<<<< HEAD
} ____cacheline_aligned;

static struct stack stacks[NR_CPUS];
=======
	u32 fiq[3];
} ____cacheline_aligned;

#ifndef CONFIG_CPU_V7M
static struct stack stacks[NR_CPUS];
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

char elf_platform[ELF_PLATFORM_SIZE];
EXPORT_SYMBOL(elf_platform);

static const char *cpu_name;
static const char *machine_name;
<<<<<<< HEAD

#ifdef CONFIG_HISI_ALTER_HARDWARE_NAME
#define MAX_HARDWARE_NAME 128
char alter_hardware_name[MAX_HARDWARE_NAME]={0};
EXPORT_SYMBOL(alter_hardware_name);
#endif

/* DTS2013031107868 qidechun 2013-03-11 begin */
#ifdef CONFIG_DUMP_SYS_INFO
unsigned long get_cpu_name(void)
{
    return (unsigned long)&cpu_name;
}
EXPORT_SYMBOL(get_cpu_name);

unsigned long get_machine_name(void)
{
    return (unsigned long)&machine_name;
}
EXPORT_SYMBOL(get_machine_name);
#endif
/* DTS2013031107868 qidechun 2013-03-11 end */

static char __initdata cmd_line[COMMAND_LINE_SIZE];
struct machine_desc *machine_desc __initdata;
=======
static char __initdata cmd_line[COMMAND_LINE_SIZE];
const struct machine_desc *machine_desc __initdata;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static union { char c[4]; unsigned long l; } endian_test __initdata = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.l)

DEFINE_PER_CPU(struct cpuinfo_arm, cpu_data);

/*
 * Standard memory resources
 */
static struct resource mem_res[] = {
	{
		.name = "Video RAM",
		.start = 0,
		.end = 0,
		.flags = IORESOURCE_MEM
	},
	{
		.name = "Kernel code",
		.start = 0,
		.end = 0,
<<<<<<< HEAD
		.flags = IORESOURCE_MEM
=======
		.flags = IORESOURCE_SYSTEM_RAM
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.name = "Kernel data",
		.start = 0,
		.end = 0,
<<<<<<< HEAD
		.flags = IORESOURCE_MEM
=======
		.flags = IORESOURCE_SYSTEM_RAM
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
};

#define video_ram   mem_res[0]
#define kernel_code mem_res[1]
#define kernel_data mem_res[2]

static struct resource io_res[] = {
	{
		.name = "reserved",
		.start = 0x3bc,
		.end = 0x3be,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	},
	{
		.name = "reserved",
		.start = 0x378,
		.end = 0x37f,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	},
	{
		.name = "reserved",
		.start = 0x278,
		.end = 0x27f,
		.flags = IORESOURCE_IO | IORESOURCE_BUSY
	}
};

#define lp0 io_res[0]
#define lp1 io_res[1]
#define lp2 io_res[2]

static const char *proc_arch[] = {
	"undefined/unknown",
	"3",
	"4",
	"4T",
	"5",
	"5T",
	"5TE",
	"5TEJ",
	"6TEJ",
	"7",
<<<<<<< HEAD
	"?(11)",
=======
	"7M",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"?(12)",
	"?(13)",
	"?(14)",
	"?(15)",
	"?(16)",
	"?(17)",
};

<<<<<<< HEAD
=======
#ifdef CONFIG_CPU_V7M
static int __get_cpu_architecture(void)
{
	return CPU_ARCH_ARMv7M;
}
#else
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int __get_cpu_architecture(void)
{
	int cpu_arch;

	if ((read_cpuid_id() & 0x0008f000) == 0) {
		cpu_arch = CPU_ARCH_UNKNOWN;
	} else if ((read_cpuid_id() & 0x0008f000) == 0x00007000) {
		cpu_arch = (read_cpuid_id() & (1 << 23)) ? CPU_ARCH_ARMv4T : CPU_ARCH_ARMv3;
	} else if ((read_cpuid_id() & 0x00080000) == 0x00000000) {
		cpu_arch = (read_cpuid_id() >> 16) & 7;
		if (cpu_arch)
			cpu_arch += CPU_ARCH_ARMv3;
	} else if ((read_cpuid_id() & 0x000f0000) == 0x000f0000) {
<<<<<<< HEAD
		unsigned int mmfr0;

		/* Revised CPUID format. Read the Memory Model Feature
		 * Register 0 and check for VMSAv7 or PMSAv7 */
		asm("mrc	p15, 0, %0, c0, c1, 4"
		    : "=r" (mmfr0));
=======
		/* Revised CPUID format. Read the Memory Model Feature
		 * Register 0 and check for VMSAv7 or PMSAv7 */
		unsigned int mmfr0 = read_cpuid_ext(CPUID_EXT_MMFR0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if ((mmfr0 & 0x0000000f) >= 0x00000003 ||
		    (mmfr0 & 0x000000f0) >= 0x00000030)
			cpu_arch = CPU_ARCH_ARMv7;
		else if ((mmfr0 & 0x0000000f) == 0x00000002 ||
			 (mmfr0 & 0x000000f0) == 0x00000020)
			cpu_arch = CPU_ARCH_ARMv6;
		else
			cpu_arch = CPU_ARCH_UNKNOWN;
	} else
		cpu_arch = CPU_ARCH_UNKNOWN;

	return cpu_arch;
}
<<<<<<< HEAD
=======
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int __pure cpu_architecture(void)
{
	BUG_ON(__cpu_architecture == CPU_ARCH_UNKNOWN);

	return __cpu_architecture;
}

static int cpu_has_aliasing_icache(unsigned int arch)
{
	int aliasing_icache;
	unsigned int id_reg, num_sets, line_size;

<<<<<<< HEAD
#ifdef CONFIG_BIG_LITTLE
	/*
	 * We expect a combination of Cortex-A15 and Cortex-A7 cores.
	 * A7 = VIPT aliasing I-cache
	 * A15 = PIPT (non-aliasing) I-cache
	 * To cater for this discrepancy, let's assume aliasing I-cache
	 * all the time.  This means unneeded extra work on the A15 but
	 * only ptrace is affected which is not performance critical.
	 */
	if ((read_cpuid_id() & 0xff0ffff0) == 0x410fc0f0)
		return 1;
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* PIPT caches never alias. */
	if (icache_is_pipt())
		return 0;

	/* arch specifies the register format */
	switch (arch) {
	case CPU_ARCH_ARMv7:
<<<<<<< HEAD
		asm("mcr	p15, 2, %0, c0, c0, 0 @ set CSSELR"
		    : /* No output operands */
		    : "r" (1));
		isb();
		asm("mrc	p15, 1, %0, c0, c0, 0 @ read CCSIDR"
		    : "=r" (id_reg));
=======
		set_csselr(CSSELR_ICACHE | CSSELR_L1);
		isb();
		id_reg = read_ccsidr();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		line_size = 4 << ((id_reg & 0x7) + 2);
		num_sets = ((id_reg >> 13) & 0x7fff) + 1;
		aliasing_icache = (line_size * num_sets) > PAGE_SIZE;
		break;
	case CPU_ARCH_ARMv6:
		aliasing_icache = read_cpuid_cachetype() & (1 << 11);
		break;
	default:
		/* I-cache aliases will be handled by D-cache aliasing code */
		aliasing_icache = 0;
	}

	return aliasing_icache;
}

static void __init cacheid_init(void)
{
	unsigned int arch = cpu_architecture();

	if (arch >= CPU_ARCH_ARMv6) {
		unsigned int cachetype = read_cpuid_cachetype();
<<<<<<< HEAD
		if ((cachetype & (7 << 29)) == 4 << 29) {
=======

		if ((arch == CPU_ARCH_ARMv7M) && !cachetype) {
			cacheid = 0;
		} else if ((cachetype & (7 << 29)) == 4 << 29) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* ARMv7 register format */
			arch = CPU_ARCH_ARMv7;
			cacheid = CACHEID_VIPT_NONALIASING;
			switch (cachetype & (3 << 14)) {
			case (1 << 14):
				cacheid |= CACHEID_ASID_TAGGED;
				break;
			case (3 << 14):
				cacheid |= CACHEID_PIPT;
				break;
			}
		} else {
			arch = CPU_ARCH_ARMv6;
			if (cachetype & (1 << 23))
				cacheid = CACHEID_VIPT_ALIASING;
			else
				cacheid = CACHEID_VIPT_NONALIASING;
		}
		if (cpu_has_aliasing_icache(arch))
			cacheid |= CACHEID_VIPT_I_ALIASING;
	} else {
		cacheid = CACHEID_VIVT;
	}

<<<<<<< HEAD
	printk("CPU: %s data cache, %s instruction cache\n",
=======
	pr_info("CPU: %s data cache, %s instruction cache\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cache_is_vivt() ? "VIVT" :
		cache_is_vipt_aliasing() ? "VIPT aliasing" :
		cache_is_vipt_nonaliasing() ? "PIPT / VIPT nonaliasing" : "unknown",
		cache_is_vivt() ? "VIVT" :
		icache_is_vivt_asid_tagged() ? "VIVT ASID tagged" :
		icache_is_vipt_aliasing() ? "VIPT aliasing" :
		icache_is_pipt() ? "PIPT" :
		cache_is_vipt_nonaliasing() ? "VIPT nonaliasing" : "unknown");
}

/*
 * These functions re-use the assembly code in head.S, which
 * already provide the required functionality.
 */
extern struct proc_info_list *lookup_processor_type(unsigned int);

void __init early_print(const char *str, ...)
{
	extern void printascii(const char *);
	char buf[256];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, sizeof(buf), str, ap);
	va_end(ap);

#ifdef CONFIG_DEBUG_LL
	printascii(buf);
#endif
	printk("%s", buf);
}

<<<<<<< HEAD
static void __init cpuid_init_hwcaps(void)
{
	unsigned int divide_instrs;
=======
#ifdef CONFIG_ARM_PATCH_IDIV

static inline u32 __attribute_const__ sdiv_instruction(void)
{
	if (IS_ENABLED(CONFIG_THUMB2_KERNEL)) {
		/* "sdiv r0, r0, r1" */
		u32 insn = __opcode_thumb32_compose(0xfb90, 0xf0f1);
		return __opcode_to_mem_thumb32(insn);
	}

	/* "sdiv r0, r0, r1" */
	return __opcode_to_mem_arm(0xe710f110);
}

static inline u32 __attribute_const__ udiv_instruction(void)
{
	if (IS_ENABLED(CONFIG_THUMB2_KERNEL)) {
		/* "udiv r0, r0, r1" */
		u32 insn = __opcode_thumb32_compose(0xfbb0, 0xf0f1);
		return __opcode_to_mem_thumb32(insn);
	}

	/* "udiv r0, r0, r1" */
	return __opcode_to_mem_arm(0xe730f110);
}

static inline u32 __attribute_const__ bx_lr_instruction(void)
{
	if (IS_ENABLED(CONFIG_THUMB2_KERNEL)) {
		/* "bx lr; nop" */
		u32 insn = __opcode_thumb32_compose(0x4770, 0x46c0);
		return __opcode_to_mem_thumb32(insn);
	}

	/* "bx lr" */
	return __opcode_to_mem_arm(0xe12fff1e);
}

static void __init patch_aeabi_idiv(void)
{
	extern void __aeabi_uidiv(void);
	extern void __aeabi_idiv(void);
	uintptr_t fn_addr;
	unsigned int mask;

	mask = IS_ENABLED(CONFIG_THUMB2_KERNEL) ? HWCAP_IDIVT : HWCAP_IDIVA;
	if (!(elf_hwcap & mask))
		return;

	pr_info("CPU: div instructions available: patching division code\n");

	fn_addr = ((uintptr_t)&__aeabi_uidiv) & ~1;
	asm ("" : "+g" (fn_addr));
	((u32 *)fn_addr)[0] = udiv_instruction();
	((u32 *)fn_addr)[1] = bx_lr_instruction();
	flush_icache_range(fn_addr, fn_addr + 8);

	fn_addr = ((uintptr_t)&__aeabi_idiv) & ~1;
	asm ("" : "+g" (fn_addr));
	((u32 *)fn_addr)[0] = sdiv_instruction();
	((u32 *)fn_addr)[1] = bx_lr_instruction();
	flush_icache_range(fn_addr, fn_addr + 8);
}

#else
static inline void patch_aeabi_idiv(void) { }
#endif

static void __init cpuid_init_hwcaps(void)
{
	int block;
	u32 isar5;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (cpu_architecture() < CPU_ARCH_ARMv7)
		return;

<<<<<<< HEAD
	divide_instrs = (read_cpuid_ext(CPUID_EXT_ISAR0) & 0x0f000000) >> 24;

	switch (divide_instrs) {
	case 2:
		elf_hwcap |= HWCAP_IDIVA;
	case 1:
		elf_hwcap |= HWCAP_IDIVT;
	}
}

static void __init feat_v6_fixup(void)
{
	int id = read_cpuid_id();

	if ((id & 0xff0f0000) != 0x41070000)
		return;
=======
	block = cpuid_feature_extract(CPUID_EXT_ISAR0, 24);
	if (block >= 2)
		elf_hwcap |= HWCAP_IDIVA;
	if (block >= 1)
		elf_hwcap |= HWCAP_IDIVT;

	/* LPAE implies atomic ldrd/strd instructions */
	block = cpuid_feature_extract(CPUID_EXT_MMFR0, 0);
	if (block >= 5)
		elf_hwcap |= HWCAP_LPAE;

	/* check for supported v8 Crypto instructions */
	isar5 = read_cpuid_ext(CPUID_EXT_ISAR5);

	block = cpuid_feature_extract_field(isar5, 4);
	if (block >= 2)
		elf_hwcap2 |= HWCAP2_PMULL;
	if (block >= 1)
		elf_hwcap2 |= HWCAP2_AES;

	block = cpuid_feature_extract_field(isar5, 8);
	if (block >= 1)
		elf_hwcap2 |= HWCAP2_SHA1;

	block = cpuid_feature_extract_field(isar5, 12);
	if (block >= 1)
		elf_hwcap2 |= HWCAP2_SHA2;

	block = cpuid_feature_extract_field(isar5, 16);
	if (block >= 1)
		elf_hwcap2 |= HWCAP2_CRC32;
}

static void __init elf_hwcap_fixup(void)
{
	unsigned id = read_cpuid_id();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * HWCAP_TLS is available only on 1136 r1p0 and later,
	 * see also kuser_get_tls_init.
	 */
<<<<<<< HEAD
	if ((((id >> 4) & 0xfff) == 0xb36) && (((id >> 20) & 3) == 0))
		elf_hwcap &= ~HWCAP_TLS;
=======
	if (read_cpuid_part() == ARM_CPU_PART_ARM1136 &&
	    ((id >> 20) & 3) == 0) {
		elf_hwcap &= ~HWCAP_TLS;
		return;
	}

	/* Verify if CPUID scheme is implemented */
	if ((id & 0x000f0000) != 0x000f0000)
		return;

	/*
	 * If the CPU supports LDREX/STREX and LDREXB/STREXB,
	 * avoid advertising SWP; it may not be atomic with
	 * multiprocessing cores.
	 */
	if (cpuid_feature_extract(CPUID_EXT_ISAR3, 12) > 1 ||
	    (cpuid_feature_extract(CPUID_EXT_ISAR3, 12) == 1 &&
	     cpuid_feature_extract(CPUID_EXT_ISAR4, 20) >= 3))
		elf_hwcap &= ~HWCAP_SWP;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * cpu_init - initialise one CPU.
 *
 * cpu_init sets up the per-CPU stacks.
 */
void notrace cpu_init(void)
{
<<<<<<< HEAD
=======
#ifndef CONFIG_CPU_V7M
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int cpu = smp_processor_id();
	struct stack *stk = &stacks[cpu];

	if (cpu >= NR_CPUS) {
<<<<<<< HEAD
		printk(KERN_CRIT "CPU%u: bad primary CPU number\n", cpu);
=======
		pr_crit("CPU%u: bad primary CPU number\n", cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		BUG();
	}

	/*
	 * This only works on resume and secondary cores. For booting on the
	 * boot cpu, smp_prepare_boot_cpu is called after percpu area setup.
	 */
	set_my_cpu_offset(per_cpu_offset(cpu));

	cpu_proc_init();

	/*
	 * Define the placement constraint for the inline asm directive below.
	 * In Thumb-2, msr with an immediate value is not allowed.
	 */
#ifdef CONFIG_THUMB2_KERNEL
<<<<<<< HEAD
#define PLC	"r"
#else
#define PLC	"I"
=======
#define PLC_l	"l"
#define PLC_r	"r"
#else
#define PLC_l	"I"
#define PLC_r	"I"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

	/*
	 * setup stacks for re-entrant exception handlers
	 */
	__asm__ (
	"msr	cpsr_c, %1\n\t"
	"add	r14, %0, %2\n\t"
	"mov	sp, r14\n\t"
	"msr	cpsr_c, %3\n\t"
	"add	r14, %0, %4\n\t"
	"mov	sp, r14\n\t"
	"msr	cpsr_c, %5\n\t"
	"add	r14, %0, %6\n\t"
	"mov	sp, r14\n\t"
<<<<<<< HEAD
	"msr	cpsr_c, %7"
	    :
	    : "r" (stk),
	      PLC (PSR_F_BIT | PSR_I_BIT | IRQ_MODE),
	      "I" (offsetof(struct stack, irq[0])),
	      PLC (PSR_F_BIT | PSR_I_BIT | ABT_MODE),
	      "I" (offsetof(struct stack, abt[0])),
	      PLC (PSR_F_BIT | PSR_I_BIT | UND_MODE),
	      "I" (offsetof(struct stack, und[0])),
	      PLC (PSR_F_BIT | PSR_I_BIT | SVC_MODE)
	    : "r14");

	fiq_glue_resume();
=======
	"msr	cpsr_c, %7\n\t"
	"add	r14, %0, %8\n\t"
	"mov	sp, r14\n\t"
	"msr	cpsr_c, %9"
	    :
	    : "r" (stk),
	      PLC_r (PSR_F_BIT | PSR_I_BIT | IRQ_MODE),
	      "I" (offsetof(struct stack, irq[0])),
	      PLC_r (PSR_F_BIT | PSR_I_BIT | ABT_MODE),
	      "I" (offsetof(struct stack, abt[0])),
	      PLC_r (PSR_F_BIT | PSR_I_BIT | UND_MODE),
	      "I" (offsetof(struct stack, und[0])),
	      PLC_r (PSR_F_BIT | PSR_I_BIT | FIQ_MODE),
	      "I" (offsetof(struct stack, fiq[0])),
	      PLC_l (PSR_F_BIT | PSR_I_BIT | SVC_MODE)
	    : "r14");
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

u32 __cpu_logical_map[NR_CPUS] = { [0 ... NR_CPUS-1] = MPIDR_INVALID };

void __init smp_setup_processor_id(void)
{
	int i;
	u32 mpidr = is_smp() ? read_cpuid_mpidr() & MPIDR_HWID_BITMASK : 0;
	u32 cpu = MPIDR_AFFINITY_LEVEL(mpidr, 0);

	cpu_logical_map(0) = cpu;
	for (i = 1; i < nr_cpu_ids; ++i)
		cpu_logical_map(i) = i == cpu ? 0 : i;

	/*
	 * clear __my_cpu_offset on boot CPU to avoid hang caused by
	 * using percpu variable early, for example, lockdep will
	 * access percpu variable inside lock_release
	 */
	set_my_cpu_offset(0);

<<<<<<< HEAD
	printk(KERN_INFO "Booting Linux on physical CPU 0x%x\n", mpidr);
}

static void __init setup_processor(void)
{
	struct proc_info_list *list;

	/*
	 * locate processor in the list of supported processor
	 * types.  The linker builds this table for us from the
	 * entries in arch/arm/mm/proc-*.S
	 */
	list = lookup_processor_type(read_cpuid_id());
	if (!list) {
		printk("CPU configuration botched (ID %08x), unable "
		       "to continue.\n", read_cpuid_id());
		while (1);
	}

	cpu_name = list->cpu_name;
	__cpu_architecture = __get_cpu_architecture();

#ifdef MULTI_CPU
	processor = *list->proc;
#endif
=======
	pr_info("Booting Linux on physical CPU 0x%x\n", mpidr);
}

struct mpidr_hash mpidr_hash;
#ifdef CONFIG_SMP
/**
 * smp_build_mpidr_hash - Pre-compute shifts required at each affinity
 *			  level in order to build a linear index from an
 *			  MPIDR value. Resulting algorithm is a collision
 *			  free hash carried out through shifting and ORing
 */
static void __init smp_build_mpidr_hash(void)
{
	u32 i, affinity;
	u32 fs[3], bits[3], ls, mask = 0;
	/*
	 * Pre-scan the list of MPIDRS and filter out bits that do
	 * not contribute to affinity levels, ie they never toggle.
	 */
	for_each_possible_cpu(i)
		mask |= (cpu_logical_map(i) ^ cpu_logical_map(0));
	pr_debug("mask of set bits 0x%x\n", mask);
	/*
	 * Find and stash the last and first bit set at all affinity levels to
	 * check how many bits are required to represent them.
	 */
	for (i = 0; i < 3; i++) {
		affinity = MPIDR_AFFINITY_LEVEL(mask, i);
		/*
		 * Find the MSB bit and LSB bits position
		 * to determine how many bits are required
		 * to express the affinity level.
		 */
		ls = fls(affinity);
		fs[i] = affinity ? ffs(affinity) - 1 : 0;
		bits[i] = ls - fs[i];
	}
	/*
	 * An index can be created from the MPIDR by isolating the
	 * significant bits at each affinity level and by shifting
	 * them in order to compress the 24 bits values space to a
	 * compressed set of values. This is equivalent to hashing
	 * the MPIDR through shifting and ORing. It is a collision free
	 * hash though not minimal since some levels might contain a number
	 * of CPUs that is not an exact power of 2 and their bit
	 * representation might contain holes, eg MPIDR[7:0] = {0x2, 0x80}.
	 */
	mpidr_hash.shift_aff[0] = fs[0];
	mpidr_hash.shift_aff[1] = MPIDR_LEVEL_BITS + fs[1] - bits[0];
	mpidr_hash.shift_aff[2] = 2*MPIDR_LEVEL_BITS + fs[2] -
						(bits[1] + bits[0]);
	mpidr_hash.mask = mask;
	mpidr_hash.bits = bits[2] + bits[1] + bits[0];
	pr_debug("MPIDR hash: aff0[%u] aff1[%u] aff2[%u] mask[0x%x] bits[%u]\n",
				mpidr_hash.shift_aff[0],
				mpidr_hash.shift_aff[1],
				mpidr_hash.shift_aff[2],
				mpidr_hash.mask,
				mpidr_hash.bits);
	/*
	 * 4x is an arbitrary value used to warn on a hash table much bigger
	 * than expected on most systems.
	 */
	if (mpidr_hash_size() > 4 * num_possible_cpus())
		pr_warn("Large number of MPIDR hash buckets detected\n");
	sync_cache_w(&mpidr_hash);
}
#endif

/*
 * locate processor in the list of supported processor types.  The linker
 * builds this table for us from the entries in arch/arm/mm/proc-*.S
 */
struct proc_info_list *lookup_processor(u32 midr)
{
	struct proc_info_list *list = lookup_processor_type(midr);

	if (!list) {
		pr_err("CPU%u: configuration botched (ID %08x), CPU halted\n",
		       smp_processor_id(), midr);
		while (1)
		/* can't use cpu_relax() here as it may require MMU setup */;
	}

	return list;
}

static void __init setup_processor(void)
{
	unsigned int midr = read_cpuid_id();
	struct proc_info_list *list = lookup_processor(midr);

	cpu_name = list->cpu_name;
	__cpu_architecture = __get_cpu_architecture();

	init_proc_vtable(list->proc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef MULTI_TLB
	cpu_tlb = *list->tlb;
#endif
#ifdef MULTI_USER
	cpu_user = *list->user;
#endif
#ifdef MULTI_CACHE
	cpu_cache = *list->cache;
#endif

<<<<<<< HEAD
	printk("CPU: %s [%08x] revision %d (ARMv%s), cr=%08lx\n",
	       cpu_name, read_cpuid_id(), read_cpuid_id() & 15,
	       proc_arch[cpu_architecture()], cr_alignment);
=======
	pr_info("CPU: %s [%08x] revision %d (ARMv%s), cr=%08lx\n",
		list->cpu_name, midr, midr & 15,
		proc_arch[cpu_architecture()], get_cr());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	snprintf(init_utsname()->machine, __NEW_UTS_LEN + 1, "%s%c",
		 list->arch_name, ENDIANNESS);
	snprintf(elf_platform, ELF_PLATFORM_SIZE, "%s%c",
		 list->elf_name, ENDIANNESS);
	elf_hwcap = list->elf_hwcap;

	cpuid_init_hwcaps();
<<<<<<< HEAD
=======
	patch_aeabi_idiv();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifndef CONFIG_ARM_THUMB
	elf_hwcap &= ~(HWCAP_THUMB | HWCAP_IDIVT);
#endif
<<<<<<< HEAD

	feat_v6_fixup();
=======
#ifdef CONFIG_MMU
	init_default_cache_policy(list->__cpu_mm_mmu_flags);
#endif
	erratum_a15_798181_init();

	elf_hwcap_fixup();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cacheid_init();
	cpu_init();
}

void __init dump_machine_table(void)
{
<<<<<<< HEAD
	struct machine_desc *p;
=======
	const struct machine_desc *p;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	early_print("Available machine support:\n\nID (hex)\tNAME\n");
	for_each_machine_desc(p)
		early_print("%08x\t%s\n", p->nr, p->name);

	early_print("\nPlease check your kernel config and/or bootloader.\n");

	while (true)
		/* can't use cpu_relax() here as it may require MMU setup */;
}

<<<<<<< HEAD
int __init arm_add_memory(phys_addr_t start, phys_addr_t size)
{
	struct membank *bank = &meminfo.bank[meminfo.nr_banks];
	u64 aligned_start;

	if (meminfo.nr_banks >= NR_BANKS) {
		printk(KERN_CRIT "NR_BANKS too low, "
			"ignoring memory at 0x%08llx\n", (long long)start);
		return -EINVAL;
	}

	/*
	 * Ensure that start/size are aligned to a page boundary.
	 * Size is appropriately rounded down, start is rounded up.
	 */
	size -= start & ~PAGE_MASK;
	aligned_start = PAGE_ALIGN(start);

#ifndef CONFIG_ARCH_PHYS_ADDR_T_64BIT
	if (aligned_start > ULONG_MAX) {
		printk(KERN_CRIT "Ignoring memory at 0x%08llx outside "
		       "32-bit physical address space\n", (long long)start);
=======
int __init arm_add_memory(u64 start, u64 size)
{
	u64 aligned_start;

	/*
	 * Ensure that start/size are aligned to a page boundary.
	 * Size is rounded down, start is rounded up.
	 */
	aligned_start = PAGE_ALIGN(start);
	if (aligned_start > start + size)
		size = 0;
	else
		size -= aligned_start - start;

#ifndef CONFIG_ARCH_PHYS_ADDR_T_64BIT
	if (aligned_start > ULONG_MAX) {
		pr_crit("Ignoring memory at 0x%08llx outside 32-bit physical address space\n",
			(long long)start);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;
	}

	if (aligned_start + size > ULONG_MAX) {
<<<<<<< HEAD
		printk(KERN_CRIT "Truncating memory at 0x%08llx to fit in "
			"32-bit physical address space\n", (long long)start);
=======
		pr_crit("Truncating memory at 0x%08llx to fit in 32-bit physical address space\n",
			(long long)start);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * To ensure bank->start + bank->size is representable in
		 * 32 bits, we use ULONG_MAX as the upper limit rather than 4GB.
		 * This means we lose a page after masking.
		 */
		size = ULONG_MAX - aligned_start;
	}
#endif

	if (aligned_start < PHYS_OFFSET) {
		if (aligned_start + size <= PHYS_OFFSET) {
			pr_info("Ignoring memory below PHYS_OFFSET: 0x%08llx-0x%08llx\n",
				aligned_start, aligned_start + size);
			return -EINVAL;
		}

		pr_info("Ignoring memory below PHYS_OFFSET: 0x%08llx-0x%08llx\n",
			aligned_start, (u64)PHYS_OFFSET);

		size -= PHYS_OFFSET - aligned_start;
		aligned_start = PHYS_OFFSET;
	}

<<<<<<< HEAD
	bank->start = aligned_start;
	bank->size = size & ~(phys_addr_t)(PAGE_SIZE - 1);
=======
	start = aligned_start;
	size = size & ~(phys_addr_t)(PAGE_SIZE - 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Check whether this memory region has non-zero size or
	 * invalid node number.
	 */
<<<<<<< HEAD
	if (bank->size == 0)
		return -EINVAL;

	meminfo.nr_banks++;
=======
	if (size == 0)
		return -EINVAL;

	memblock_add(start, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/*
 * Pick out the memory size.  We look for mem=size@start,
 * where start and size are "size[KkMm]"
 */
<<<<<<< HEAD
static int __init early_mem(char *p)
{
	static int usermem __initdata = 0;
	phys_addr_t size;
	phys_addr_t start;
=======

static int __init early_mem(char *p)
{
	static int usermem __initdata = 0;
	u64 size;
	u64 start;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char *endp;

	/*
	 * If the user specifies memory size, we
	 * blow away any automatically generated
	 * size.
	 */
	if (usermem == 0) {
		usermem = 1;
<<<<<<< HEAD
		meminfo.nr_banks = 0;
=======
		memblock_remove(memblock_start_of_DRAM(),
			memblock_end_of_DRAM() - memblock_start_of_DRAM());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	start = PHYS_OFFSET;
	size  = memparse(p, &endp);
	if (*endp == '@')
		start = memparse(endp + 1, NULL);

	arm_add_memory(start, size);

	return 0;
}
early_param("mem", early_mem);

<<<<<<< HEAD
/* BEGIN PN:DTS2014081505452,Added by chenyu, 2014-08-14*/
#ifdef CONFIG_FEATURE_HUAWEI_EMERGENCY_DATA
#define DATAMOUNT_FLAG_FAIL 0x0587C90A
#define DATAMOUNT_FLAG_SUCCESS 0 // datamount_flag initialization value

/*
 * global variable datamount_flag has two values:
 * 0 - init value
 * 1 - reboot caused by ext4_handle_error, or sync_blockdev return EIO
 */
static unsigned int datamount_flag = DATAMOUNT_FLAG_SUCCESS;
unsigned int get_datamount_flag(void)
{
    return datamount_flag;
}
EXPORT_SYMBOL(get_datamount_flag);

void set_datamount_flag(int value)
{
    datamount_flag = value;
}
EXPORT_SYMBOL(set_datamount_flag);

static int __init early_param_boottype(char * p)
{
    if (p) {
        if (!strcmp(p,"mountfail")) {
            datamount_flag = DATAMOUNT_FLAG_FAIL;
        }
    }
	//printk(KERN_ALERT "kernel_log: func = %s, doubledata p = %s, datamount_flag = %d\n", __FUNCTION__, p, datamount_flag);
    return 0;
}
early_param("boottype", early_param_boottype);
#endif
/* END PN:DTS2014081505452,Added by chenyu, 2014-08-14*/

static void __init request_standard_resources(struct machine_desc *mdesc)
=======
static void __init request_standard_resources(const struct machine_desc *mdesc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct memblock_region *region;
	struct resource *res;

	kernel_code.start   = virt_to_phys(_text);
<<<<<<< HEAD
	kernel_code.end     = virt_to_phys(_etext - 1);
=======
	kernel_code.end     = virt_to_phys(__init_begin - 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kernel_data.start   = virt_to_phys(_sdata);
	kernel_data.end     = virt_to_phys(_end - 1);

	for_each_memblock(memory, region) {
<<<<<<< HEAD
		res = alloc_bootmem_low(sizeof(*res));
		res->name  = "System RAM";
		res->start = __pfn_to_phys(memblock_region_memory_base_pfn(region));
		res->end = __pfn_to_phys(memblock_region_memory_end_pfn(region)) - 1;
		res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
=======
		phys_addr_t start = __pfn_to_phys(memblock_region_memory_base_pfn(region));
		phys_addr_t end = __pfn_to_phys(memblock_region_memory_end_pfn(region)) - 1;
		unsigned long boot_alias_start;

		/*
		 * Some systems have a special memory alias which is only
		 * used for booting.  We need to advertise this region to
		 * kexec-tools so they know where bootable RAM is located.
		 */
		boot_alias_start = phys_to_idmap(start);
		if (arm_has_idmap_alias() && boot_alias_start != IDMAP_INVALID_ADDR) {
			res = memblock_virt_alloc(sizeof(*res), 0);
			res->name = "System RAM (boot alias)";
			res->start = boot_alias_start;
			res->end = phys_to_idmap(end);
			res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
			request_resource(&iomem_resource, res);
		}

		res = memblock_virt_alloc(sizeof(*res), 0);
		res->name  = "System RAM";
		res->start = start;
		res->end = end;
		res->flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		request_resource(&iomem_resource, res);

		if (kernel_code.start >= res->start &&
		    kernel_code.end <= res->end)
			request_resource(res, &kernel_code);
		if (kernel_data.start >= res->start &&
		    kernel_data.end <= res->end)
			request_resource(res, &kernel_data);
	}

	if (mdesc->video_start) {
		video_ram.start = mdesc->video_start;
		video_ram.end   = mdesc->video_end;
		request_resource(&iomem_resource, &video_ram);
	}

	/*
	 * Some machines don't have the possibility of ever
	 * possessing lp0, lp1 or lp2
	 */
	if (mdesc->reserve_lp0)
		request_resource(&ioport_resource, &lp0);
	if (mdesc->reserve_lp1)
		request_resource(&ioport_resource, &lp1);
	if (mdesc->reserve_lp2)
		request_resource(&ioport_resource, &lp2);
}

<<<<<<< HEAD
#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_DUMMY_CONSOLE)
=======
#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_DUMMY_CONSOLE) || \
    defined(CONFIG_EFI)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct screen_info screen_info = {
 .orig_video_lines	= 30,
 .orig_video_cols	= 80,
 .orig_video_mode	= 0,
 .orig_video_ega_bx	= 0,
 .orig_video_isVGA	= 1,
 .orig_video_points	= 8
};
#endif

static int __init customize_machine(void)
{
	/*
	 * customizes platform devices, or adds new ones
	 * On DT based machines, we fall back to populating the
	 * machine from the device tree, if no callback is provided,
	 * otherwise we would always need an init_machine callback.
	 */
	if (machine_desc->init_machine)
		machine_desc->init_machine();
<<<<<<< HEAD
#ifdef CONFIG_OF
	else
		of_platform_populate(NULL, of_default_bus_match_table,
					NULL, NULL);
#endif
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
arch_initcall(customize_machine);

static int __init init_machine_late(void)
{
<<<<<<< HEAD
	if (machine_desc->init_late)
		machine_desc->init_late();
=======
	struct device_node *root;
	int ret;

	if (machine_desc->init_late)
		machine_desc->init_late();

	root = of_find_node_by_path("/");
	if (root) {
		ret = of_property_read_string(root, "serial-number",
					      &system_serial);
		if (ret)
			system_serial = NULL;
	}

	if (!system_serial)
		system_serial = kasprintf(GFP_KERNEL, "%08x%08x",
					  system_serial_high,
					  system_serial_low);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
late_initcall(init_machine_late);

#ifdef CONFIG_KEXEC
<<<<<<< HEAD
=======
/*
 * The crash region must be aligned to 128MB to avoid
 * zImage relocating below the reserved region.
 */
#define CRASH_ALIGN	(128 << 20)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline unsigned long long get_total_mem(void)
{
	unsigned long total;

	total = max_low_pfn - min_low_pfn;
	return total << PAGE_SHIFT;
}

/**
 * reserve_crashkernel() - reserves memory are for crash kernel
 *
 * This function reserves memory area given in "crashkernel=" kernel command
 * line parameter. The memory reserved is used by a dump capture kernel when
 * primary kernel is crashing.
 */
static void __init reserve_crashkernel(void)
{
	unsigned long long crash_size, crash_base;
	unsigned long long total_mem;
	int ret;

	total_mem = get_total_mem();
	ret = parse_crashkernel(boot_command_line, total_mem,
				&crash_size, &crash_base);
	if (ret)
		return;

<<<<<<< HEAD
	ret = reserve_bootmem(crash_base, crash_size, BOOTMEM_EXCLUSIVE);
	if (ret < 0) {
		printk(KERN_WARNING "crashkernel reservation failed - "
		       "memory is in use (0x%lx)\n", (unsigned long)crash_base);
		return;
	}

	printk(KERN_INFO "Reserving %ldMB of memory at %ldMB "
	       "for crashkernel (System RAM: %ldMB)\n",
	       (unsigned long)(crash_size >> 20),
	       (unsigned long)(crash_base >> 20),
	       (unsigned long)(total_mem >> 20));

	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
	insert_resource(&iomem_resource, &crashk_res);
=======
	if (crash_base <= 0) {
		unsigned long long crash_max = idmap_to_phys((u32)~0);
		crash_base = memblock_find_in_range(CRASH_ALIGN, crash_max,
						    crash_size, CRASH_ALIGN);
		if (!crash_base) {
			pr_err("crashkernel reservation failed - No suitable area found.\n");
			return;
		}
	} else {
		unsigned long long start;

		start = memblock_find_in_range(crash_base,
					       crash_base + crash_size,
					       crash_size, SECTION_SIZE);
		if (start != crash_base) {
			pr_err("crashkernel reservation failed - memory is in use.\n");
			return;
		}
	}

	ret = memblock_reserve(crash_base, crash_size);
	if (ret < 0) {
		pr_warn("crashkernel reservation failed - memory is in use (0x%lx)\n",
			(unsigned long)crash_base);
		return;
	}

	pr_info("Reserving %ldMB of memory at %ldMB for crashkernel (System RAM: %ldMB)\n",
		(unsigned long)(crash_size >> 20),
		(unsigned long)(crash_base >> 20),
		(unsigned long)(total_mem >> 20));

	/* The crashk resource must always be located in normal mem */
	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
	insert_resource(&iomem_resource, &crashk_res);

	if (arm_has_idmap_alias()) {
		/*
		 * If we have a special RAM alias for use at boot, we
		 * need to advertise to kexec tools where the alias is.
		 */
		static struct resource crashk_boot_res = {
			.name = "Crash kernel (boot alias)",
			.flags = IORESOURCE_BUSY | IORESOURCE_MEM,
		};

		crashk_boot_res.start = phys_to_idmap(crash_base);
		crashk_boot_res.end = crashk_boot_res.start + crash_size - 1;
		insert_resource(&iomem_resource, &crashk_boot_res);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#else
static inline void reserve_crashkernel(void) {}
#endif /* CONFIG_KEXEC */

<<<<<<< HEAD
static int __init meminfo_cmp(const void *_a, const void *_b)
{
	const struct membank *a = _a, *b = _b;
	long cmp = bank_pfn_start(a) - bank_pfn_start(b);
	return cmp < 0 ? -1 : cmp > 0 ? 1 : 0;
}

void __init hyp_mode_check(void)
{
#ifdef CONFIG_ARM_VIRT_EXT
=======
void __init hyp_mode_check(void)
{
#ifdef CONFIG_ARM_VIRT_EXT
	sync_boot_mode();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (is_hyp_mode_available()) {
		pr_info("CPU: All CPU(s) started in HYP mode.\n");
		pr_info("CPU: Virtualization extensions available.\n");
	} else if (is_hyp_mode_mismatched()) {
		pr_warn("CPU: WARNING: CPU(s) started in wrong/inconsistent modes (primary CPU mode 0x%x)\n",
			__boot_cpu_mode & MODE_MASK);
		pr_warn("CPU: This may indicate a broken bootloader or firmware.\n");
	} else
		pr_info("CPU: All CPU(s) started in SVC mode.\n");
#endif
}

void __init setup_arch(char **cmdline_p)
{
<<<<<<< HEAD
	struct machine_desc *mdesc;
=======
	const struct machine_desc *mdesc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	setup_processor();
	mdesc = setup_machine_fdt(__atags_pointer);
	if (!mdesc)
		mdesc = setup_machine_tags(__atags_pointer, __machine_arch_type);
	machine_desc = mdesc;
	machine_name = mdesc->name;
<<<<<<< HEAD

	setup_dma_zone(mdesc);

	if (mdesc->restart_mode)
		reboot_setup(&mdesc->restart_mode);
=======
	dump_stack_set_arch_desc("%s", mdesc->name);

	if (mdesc->reboot_mode != REBOOT_HARD)
		reboot_mode = mdesc->reboot_mode;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	init_mm.start_code = (unsigned long) _text;
	init_mm.end_code   = (unsigned long) _etext;
	init_mm.end_data   = (unsigned long) _edata;
	init_mm.brk	   = (unsigned long) _end;

	/* populate cmd_line too for later use, preserving boot_command_line */
	strlcpy(cmd_line, boot_command_line, COMMAND_LINE_SIZE);
	*cmdline_p = cmd_line;

<<<<<<< HEAD
	parse_early_param();

	sort(&meminfo.bank, meminfo.nr_banks, sizeof(meminfo.bank[0]), meminfo_cmp, NULL);
	sanity_check_meminfo();
	arm_memblock_init(&meminfo, mdesc);
=======
	early_fixmap_init();
	early_ioremap_init();

	parse_early_param();

#ifdef CONFIG_MMU
	early_paging_init(mdesc);
#endif
	setup_dma_zone(mdesc);
	xen_early_init();
	efi_init();
	/*
	 * Make sure the calculation for lowmem/highmem is set appropriately
	 * before reserving/allocating any mmeory
	 */
	adjust_lowmem_bounds();
	arm_memblock_init(mdesc);
	/* Memory may have been removed so recalculate the bounds. */
	adjust_lowmem_bounds();

	early_ioremap_reset();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	paging_init(mdesc);
	request_standard_resources(mdesc);

	if (mdesc->restart)
		arm_pm_restart = mdesc->restart;

	unflatten_device_tree();

	arm_dt_init_cpu_maps();
<<<<<<< HEAD
	psci_init();
=======
	psci_dt_init();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_SMP
	if (is_smp()) {
		if (!mdesc->smp_init || !mdesc->smp_init()) {
			if (psci_smp_available())
				smp_set_ops(&psci_smp_ops);
			else if (mdesc->smp)
				smp_set_ops(mdesc->smp);
		}
		smp_init_cpus();
<<<<<<< HEAD
=======
		smp_build_mpidr_hash();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
#endif

	if (!is_smp())
		hyp_mode_check();

	reserve_crashkernel();

#ifdef CONFIG_MULTI_IRQ_HANDLER
	handle_arch_irq = mdesc->handle_irq;
#endif

#ifdef CONFIG_VT
#if defined(CONFIG_VGA_CONSOLE)
	conswitchp = &vga_con;
#elif defined(CONFIG_DUMMY_CONSOLE)
	conswitchp = &dummy_con;
#endif
#endif

	if (mdesc->init_early)
		mdesc->init_early();
}


static int __init topology_init(void)
{
	int cpu;

	for_each_possible_cpu(cpu) {
		struct cpuinfo_arm *cpuinfo = &per_cpu(cpu_data, cpu);
<<<<<<< HEAD
		cpuinfo->cpu.hotpluggable = 1;
=======
		cpuinfo->cpu.hotpluggable = platform_can_hotplug_cpu(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		register_cpu(&cpuinfo->cpu, cpu);
	}

	return 0;
}
subsys_initcall(topology_init);

#ifdef CONFIG_HAVE_PROC_CPU
static int __init proc_cpu_init(void)
{
	struct proc_dir_entry *res;

	res = proc_mkdir("cpu", NULL);
	if (!res)
		return -ENOMEM;
	return 0;
}
fs_initcall(proc_cpu_init);
#endif

static const char *hwcap_str[] = {
	"swp",
	"half",
	"thumb",
	"26bit",
	"fastmult",
	"fpa",
	"vfp",
	"edsp",
	"java",
	"iwmmxt",
	"crunch",
	"thumbee",
	"neon",
	"vfpv3",
	"vfpv3d16",
	"tls",
	"vfpv4",
	"idiva",
	"idivt",
	"vfpd32",
	"lpae",
	"evtstrm",
	NULL
};

<<<<<<< HEAD
#if defined(CONFIG_SMP)
static int find_in_array(u32 *source, u32 length, u32 cpuid)
{
	int i;

	for (i=0; i<length; i++)
		if (source[i] == cpuid)
			return 1;

	return 0;
}

static void print_cpuinfo_array(struct seq_file *m, char *pre, u32 width, u32 *source, u32 length, u32 bit, u32 mask)
{
	int i;
	char str[10] = {0};
	u32 value, old_value;

	if (pre){
		snprintf(str, sizeof(str), "%s%%0%dx", pre, width);
	} else {
	       snprintf(str, sizeof(str), "%s%%0%dx", str, width);
	}

	old_value = (source[0] >> bit) & mask;
	seq_printf(m, str, old_value);
	for (i=1; i<length; i++) {
		value = (source[i] >> bit) & mask;
		if (old_value != value) {
			seq_puts(m, " & ");
			seq_printf(m, str, value);
			old_value = value;
		}
	}

	seq_puts(m, "\n");
}
#endif

static int c_show(struct seq_file *m, void *v)
{
	int i;
	u32 cpuid;
#if defined(CONFIG_SMP)
	u32 cpuid_array[NR_CPUS];
	u32 index = 0;
#endif

	cpuid = read_cpuid_id();
	seq_printf(m, "Processor\t: %s rev %d (%s)\n",
		   cpu_name, cpuid & 15, elf_platform);

#if defined(CONFIG_SMP)
    for_each_online_cpu(i) {
=======
static const char *hwcap2_str[] = {
	"aes",
	"pmull",
	"sha1",
	"sha2",
	"crc32",
	NULL
};

static int c_show(struct seq_file *m, void *v)
{
	int i, j;
	u32 cpuid;

	for_each_online_cpu(i) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * glibc reads /proc/cpuinfo to determine the number of
		 * online processors, looking for lines beginning with
		 * "processor".  Give glibc what it expects.
		 */
		seq_printf(m, "processor\t: %d\n", i);
<<<<<<< HEAD
		cpuid = per_cpu(cpu_data, i).cpuid;
		if (!find_in_array(cpuid_array, index, cpuid))
			cpuid_array[index++] = cpuid;

		seq_printf(m, "BogoMIPS\t: %lu.%02lu\n\n",
			   per_cpu(cpu_data, i).loops_per_jiffy / (500000UL/HZ),
			   (per_cpu(cpu_data, i).loops_per_jiffy / (5000UL/HZ)) % 100);
	}
=======
		cpuid = is_smp() ? per_cpu(cpu_data, i).cpuid : read_cpuid_id();
		seq_printf(m, "model name\t: %s rev %d (%s)\n",
			   cpu_name, cpuid & 15, elf_platform);

#if defined(CONFIG_SMP)
		seq_printf(m, "BogoMIPS\t: %lu.%02lu\n",
			   per_cpu(cpu_data, i).loops_per_jiffy / (500000UL/HZ),
			   (per_cpu(cpu_data, i).loops_per_jiffy / (5000UL/HZ)) % 100);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
		seq_printf(m, "BogoMIPS\t: %lu.%02lu\n",
			   loops_per_jiffy / (500000/HZ),
			   (loops_per_jiffy / (5000/HZ)) % 100);
#endif
		/* dump out the processor features */
		seq_puts(m, "Features\t: ");

<<<<<<< HEAD
		for (i = 0; hwcap_str[i]; i++)
			if (elf_hwcap & (1 << i))
				seq_printf(m, "%s ", hwcap_str[i]);

		seq_puts(m, "\nCPU implementer\t: ");
#if defined(CONFIG_SMP)
		print_cpuinfo_array(m, "0x", 2, cpuid_array, index, 24, 255);
#else
		seq_printf(m, "0x%02x\n", cpuid >> 24);
#endif

=======
		for (j = 0; hwcap_str[j]; j++)
			if (elf_hwcap & (1 << j))
				seq_printf(m, "%s ", hwcap_str[j]);

		for (j = 0; hwcap2_str[j]; j++)
			if (elf_hwcap2 & (1 << j))
				seq_printf(m, "%s ", hwcap2_str[j]);

		seq_printf(m, "\nCPU implementer\t: 0x%02x\n", cpuid >> 24);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		seq_printf(m, "CPU architecture: %s\n",
			   proc_arch[cpu_architecture()]);

		if ((cpuid & 0x0008f000) == 0x00000000) {
			/* pre-ARM7 */
<<<<<<< HEAD
			seq_puts(m, "CPU part\t: ");
#if defined(CONFIG_SMP)
			print_cpuinfo_array(m, NULL, 7, cpuid_array, index, 4, 255);
#else
			seq_printf(m, "%07x\n", cpuid >> 4);
#endif
		} else {
			if ((cpuid & 0x0008f000) == 0x00007000) {
				/* ARM7 */
				seq_puts(m, "CPU variant\t: ");
#if defined(CONFIG_SMP)
				print_cpuinfo_array(m, "0x", 2, cpuid_array, index, 16, 127);
#else
				seq_printf(m, "0x%02x\n", (cpuid >> 16) & 127);
#endif
			} else {
				/* post-ARM7 */
				seq_puts(m, "CPU variant\t: ");
#if defined(CONFIG_SMP)
				print_cpuinfo_array(m, "0x", 1, cpuid_array, index, 20, 15);
#else
				seq_printf(m, "0x%x\n", (cpuid >> 20) & 15);
#endif
			}
			seq_puts(m, "CPU part\t: ");
#if defined(CONFIG_SMP)
			print_cpuinfo_array(m, "0x", 3, cpuid_array, index, 4, 0xfff);
#else
			seq_printf(m, "0x%03x\n", (cpuid >> 4) & 0xfff);
#endif
		}
		seq_puts(m, "CPU revision\t: ");
#if defined(CONFIG_SMP)
		print_cpuinfo_array(m, NULL, 1, cpuid_array, index, 0, 15);
		seq_puts(m, "\n");
#else
		seq_printf(m, "%d\n\n", cpuid & 15);
#endif

#ifdef CONFIG_HISI_ALTER_HARDWARE_NAME
    if(alter_hardware_name[0] != 0) {
        seq_printf(m, "Hardware\t: %s\n", alter_hardware_name);
    }
    else {
        seq_printf(m, "Hardware\t: %s\n", machine_name);
    }
#else
	seq_printf(m, "Hardware\t: %s\n", machine_name);
#endif
	seq_printf(m, "Revision\t: %04x\n", system_rev);
	seq_printf(m, "Serial\t\t: %08x%08x\n",
		   system_serial_high, system_serial_low);
=======
			seq_printf(m, "CPU part\t: %07x\n", cpuid >> 4);
		} else {
			if ((cpuid & 0x0008f000) == 0x00007000) {
				/* ARM7 */
				seq_printf(m, "CPU variant\t: 0x%02x\n",
					   (cpuid >> 16) & 127);
			} else {
				/* post-ARM7 */
				seq_printf(m, "CPU variant\t: 0x%x\n",
					   (cpuid >> 20) & 15);
			}
			seq_printf(m, "CPU part\t: 0x%03x\n",
				   (cpuid >> 4) & 0xfff);
		}
		seq_printf(m, "CPU revision\t: %d\n\n", cpuid & 15);
	}

	seq_printf(m, "Hardware\t: %s\n", machine_name);
	seq_printf(m, "Revision\t: %04x\n", system_rev);
	seq_printf(m, "Serial\t\t: %s\n", system_serial);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < 1 ? (void *)1 : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return NULL;
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= c_show
};
