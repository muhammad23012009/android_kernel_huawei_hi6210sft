/*
 *  Copyright IBM Corp. 2008
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#define KMSG_COMPONENT "cpu"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <asm/elf.h>
#include <asm/lowcore.h>
#include <asm/param.h>

static DEFINE_PER_CPU(struct cpuid, cpu_id);
=======
#include <linux/cpufeature.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <asm/diag.h>
#include <asm/facility.h>
#include <asm/elf.h>
#include <asm/lowcore.h>
#include <asm/param.h>
#include <asm/smp.h>

struct cpu_info {
	unsigned int cpu_mhz_dynamic;
	unsigned int cpu_mhz_static;
	struct cpuid cpu_id;
};

static DEFINE_PER_CPU(struct cpu_info, cpu_info);

static bool machine_has_cpu_mhz;

void __init cpu_detect_mhz_feature(void)
{
	if (test_facility(34) && __ecag(ECAG_CPU_ATTRIBUTE, 0) != -1UL)
		machine_has_cpu_mhz = 1;
}

static void update_cpu_mhz(void *arg)
{
	unsigned long mhz;
	struct cpu_info *c;

	mhz = __ecag(ECAG_CPU_ATTRIBUTE, 0);
	c = this_cpu_ptr(&cpu_info);
	c->cpu_mhz_dynamic = mhz >> 32;
	c->cpu_mhz_static = mhz & 0xffffffff;
}

void s390_update_cpu_mhz(void)
{
	s390_adjust_jiffies();
	if (machine_has_cpu_mhz)
		on_each_cpu(update_cpu_mhz, NULL, 0);
}

void notrace cpu_relax(void)
{
	if (!smp_cpu_mtid && MACHINE_HAS_DIAG44) {
		diag_stat_inc(DIAG_STAT_X044);
		asm volatile("diag 0,0,0x44");
	}
	barrier();
}
EXPORT_SYMBOL(cpu_relax);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * cpu_init - initializes state that is per-CPU.
 */
<<<<<<< HEAD
void __cpuinit cpu_init(void)
{
	struct s390_idle_data *idle = &__get_cpu_var(s390_idle);
	struct cpuid *id = &__get_cpu_var(cpu_id);

	get_cpu_id(id);
=======
void cpu_init(void)
{
	struct cpuid *id = this_cpu_ptr(&cpu_info.cpu_id);

	get_cpu_id(id);
	if (machine_has_cpu_mhz)
		update_cpu_mhz(NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	atomic_inc(&init_mm.mm_count);
	current->active_mm = &init_mm;
	BUG_ON(current->mm);
	enter_lazy_tlb(&init_mm, current);
<<<<<<< HEAD
	memset(idle, 0, sizeof(*idle));
}

/*
 * show_cpuinfo - Get information on one CPU for use by procfs.
 */
static int show_cpuinfo(struct seq_file *m, void *v)
{
	static const char *hwcap_str[] = {
		"esan3", "zarch", "stfle", "msa", "ldisp", "eimm", "dfp",
		"edat", "etf3eh", "highgprs", "te"
	};
	unsigned long n = (unsigned long) v - 1;
	int i;

	if (!n) {
		s390_adjust_jiffies();
		seq_printf(m, "vendor_id       : IBM/S390\n"
			   "# processors    : %i\n"
			   "bogomips per cpu: %lu.%02lu\n",
			   num_online_cpus(), loops_per_jiffy/(500000/HZ),
			   (loops_per_jiffy/(5000/HZ))%100);
		seq_puts(m, "features\t: ");
		for (i = 0; i < ARRAY_SIZE(hwcap_str); i++)
			if (hwcap_str[i] && (elf_hwcap & (1UL << i)))
				seq_printf(m, "%s ", hwcap_str[i]);
		seq_puts(m, "\n");
		show_cacheinfo(m);
	}
	get_online_cpus();
	if (cpu_online(n)) {
		struct cpuid *id = &per_cpu(cpu_id, n);
		seq_printf(m, "processor %li: "
			   "version = %02X,  "
			   "identification = %06X,  "
			   "machine = %04X\n",
			   n, id->version, id->ident, id->machine);
	}
	put_online_cpus();
	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < nr_cpu_ids ? (void *)((unsigned long) *pos + 1) : NULL;
=======
}

/*
 * cpu_have_feature - Test CPU features on module initialization
 */
int cpu_have_feature(unsigned int num)
{
	return elf_hwcap & (1UL << num);
}
EXPORT_SYMBOL(cpu_have_feature);

static void show_cpu_summary(struct seq_file *m, void *v)
{
	static const char *hwcap_str[] = {
		"esan3", "zarch", "stfle", "msa", "ldisp", "eimm", "dfp",
		"edat", "etf3eh", "highgprs", "te", "vx"
	};
	static const char * const int_hwcap_str[] = {
		"sie"
	};
	int i, cpu;

	seq_printf(m, "vendor_id       : IBM/S390\n"
		   "# processors    : %i\n"
		   "bogomips per cpu: %lu.%02lu\n",
		   num_online_cpus(), loops_per_jiffy/(500000/HZ),
		   (loops_per_jiffy/(5000/HZ))%100);
	seq_printf(m, "max thread id   : %d\n", smp_cpu_mtid);
	seq_puts(m, "features\t: ");
	for (i = 0; i < ARRAY_SIZE(hwcap_str); i++)
		if (hwcap_str[i] && (elf_hwcap & (1UL << i)))
			seq_printf(m, "%s ", hwcap_str[i]);
	for (i = 0; i < ARRAY_SIZE(int_hwcap_str); i++)
		if (int_hwcap_str[i] && (int_hwcap & (1UL << i)))
			seq_printf(m, "%s ", int_hwcap_str[i]);
	seq_puts(m, "\n");
	show_cacheinfo(m);
	for_each_online_cpu(cpu) {
		struct cpuid *id = &per_cpu(cpu_info.cpu_id, cpu);

		seq_printf(m, "processor %d: "
			   "version = %02X,  "
			   "identification = %06X,  "
			   "machine = %04X\n",
			   cpu, id->version, id->ident, id->machine);
	}
}

static void show_cpu_mhz(struct seq_file *m, unsigned long n)
{
	struct cpu_info *c = per_cpu_ptr(&cpu_info, n);

	seq_printf(m, "cpu MHz dynamic : %d\n", c->cpu_mhz_dynamic);
	seq_printf(m, "cpu MHz static  : %d\n", c->cpu_mhz_static);
}

/*
 * show_cpuinfo - Get information on one CPU for use by procfs.
 */
static int show_cpuinfo(struct seq_file *m, void *v)
{
	unsigned long n = (unsigned long) v - 1;
	unsigned long first = cpumask_first(cpu_online_mask);

	if (n == first)
		show_cpu_summary(m, v);
	if (!machine_has_cpu_mhz)
		return 0;
	seq_printf(m, "\ncpu number      : %ld\n", n);
	show_cpu_mhz(m, n);
	return 0;
}

static inline void *c_update(loff_t *pos)
{
	if (*pos)
		*pos = cpumask_next(*pos - 1, cpu_online_mask);
	else
		*pos = cpumask_first(cpu_online_mask);
	return *pos < nr_cpu_ids ? (void *)*pos + 1 : NULL;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	get_online_cpus();
	return c_update(pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
<<<<<<< HEAD
	return c_start(m, pos);
=======
	return c_update(pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void c_stop(struct seq_file *m, void *v)
{
<<<<<<< HEAD
=======
	put_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= show_cpuinfo,
};

<<<<<<< HEAD
=======
int s390_isolate_bp(void)
{
	if (!test_facility(82))
		return -EOPNOTSUPP;
	set_thread_flag(TIF_ISOLATE_BP);
	return 0;
}
EXPORT_SYMBOL(s390_isolate_bp);

int s390_isolate_bp_guest(void)
{
	if (!test_facility(82))
		return -EOPNOTSUPP;
	set_thread_flag(TIF_ISOLATE_BP_GUEST);
	return 0;
}
EXPORT_SYMBOL(s390_isolate_bp_guest);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
