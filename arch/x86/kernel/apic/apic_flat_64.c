/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
 *
 * Flat APIC subarch code.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */
#include <linux/errno.h>
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/hardirq.h>
#include <linux/module.h>
=======
#include <linux/hardirq.h>
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/smp.h>
#include <asm/apic.h>
#include <asm/ipi.h>

<<<<<<< HEAD
#ifdef CONFIG_ACPI
#include <acpi/acpi_bus.h>
#endif
=======
#include <linux/acpi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct apic apic_physflat;
static struct apic apic_flat;

<<<<<<< HEAD
struct apic __read_mostly *apic = &apic_flat;
=======
struct apic *apic __ro_after_init = &apic_flat;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(apic);

static int flat_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	return 1;
}

/*
 * Set up the logical destination ID.
 *
 * Intel recommends to set DFR, LDR and TPR before enabling
 * an APIC.  See e.g. "AP-388 82489DX User's Manual" (Intel
 * document number 292116).  So here it goes...
 */
void flat_init_apic_ldr(void)
{
	unsigned long val;
	unsigned long num, id;

	num = smp_processor_id();
	id = 1UL << num;
	apic_write(APIC_DFR, APIC_DFR_FLAT);
	val = apic_read(APIC_LDR) & ~APIC_LDR_MASK;
	val |= SET_APIC_LOGICAL_ID(id);
	apic_write(APIC_LDR, val);
}

<<<<<<< HEAD
static inline void _flat_send_IPI_mask(unsigned long mask, int vector)
=======
static void _flat_send_IPI_mask(unsigned long mask, int vector)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long flags;

	local_irq_save(flags);
	__default_send_IPI_dest_field(mask, vector, apic->dest_logical);
	local_irq_restore(flags);
}

static void flat_send_IPI_mask(const struct cpumask *cpumask, int vector)
{
	unsigned long mask = cpumask_bits(cpumask)[0];

	_flat_send_IPI_mask(mask, vector);
}

static void
flat_send_IPI_mask_allbutself(const struct cpumask *cpumask, int vector)
{
	unsigned long mask = cpumask_bits(cpumask)[0];
	int cpu = smp_processor_id();

	if (cpu < BITS_PER_LONG)
		clear_bit(cpu, &mask);

	_flat_send_IPI_mask(mask, vector);
}

static void flat_send_IPI_allbutself(int vector)
{
	int cpu = smp_processor_id();
#ifdef	CONFIG_HOTPLUG_CPU
	int hotplug = 1;
#else
	int hotplug = 0;
#endif
	if (hotplug || vector == NMI_VECTOR) {
		if (!cpumask_equal(cpu_online_mask, cpumask_of(cpu))) {
			unsigned long mask = cpumask_bits(cpu_online_mask)[0];

			if (cpu < BITS_PER_LONG)
				clear_bit(cpu, &mask);

			_flat_send_IPI_mask(mask, vector);
		}
	} else if (num_online_cpus() > 1) {
		__default_send_IPI_shortcut(APIC_DEST_ALLBUT,
					    vector, apic->dest_logical);
	}
}

static void flat_send_IPI_all(int vector)
{
	if (vector == NMI_VECTOR) {
		flat_send_IPI_mask(cpu_online_mask, vector);
	} else {
		__default_send_IPI_shortcut(APIC_DEST_ALLINC,
					    vector, apic->dest_logical);
	}
}

static unsigned int flat_get_apic_id(unsigned long x)
{
<<<<<<< HEAD
	unsigned int id;

	id = (((x)>>24) & 0xFFu);

	return id;
=======
	return (x >> 24) & 0xFF;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static unsigned long set_apic_id(unsigned int id)
{
<<<<<<< HEAD
	unsigned long x;

	x = ((id & 0xFFu)<<24);
	return x;
=======
	return (id & 0xFF) << 24;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static unsigned int read_xapic_id(void)
{
<<<<<<< HEAD
	unsigned int id;

	id = flat_get_apic_id(apic_read(APIC_ID));
	return id;
=======
	return flat_get_apic_id(apic_read(APIC_ID));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int flat_apic_id_registered(void)
{
	return physid_isset(read_xapic_id(), phys_cpu_present_map);
}

static int flat_phys_pkg_id(int initial_apic_id, int index_msb)
{
	return initial_apic_id >> index_msb;
}

static int flat_probe(void)
{
	return 1;
}

<<<<<<< HEAD
static struct apic apic_flat =  {
=======
static struct apic apic_flat __ro_after_init = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.name				= "flat",
	.probe				= flat_probe,
	.acpi_madt_oem_check		= flat_acpi_madt_oem_check,
	.apic_id_valid			= default_apic_id_valid,
	.apic_id_registered		= flat_apic_id_registered,

	.irq_delivery_mode		= dest_LowestPrio,
	.irq_dest_mode			= 1, /* logical */

	.target_cpus			= online_target_cpus,
	.disable_esr			= 0,
	.dest_logical			= APIC_DEST_LOGICAL,
	.check_apicid_used		= NULL,
<<<<<<< HEAD
	.check_apicid_present		= NULL,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.vector_allocation_domain	= flat_vector_allocation_domain,
	.init_apic_ldr			= flat_init_apic_ldr,

	.ioapic_phys_id_map		= NULL,
	.setup_apic_routing		= NULL,
<<<<<<< HEAD
	.multi_timer_check		= NULL,
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.setup_portio_remap		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.enable_apic_mode		= NULL,
	.phys_pkg_id			= flat_phys_pkg_id,
	.mps_oem_check			= NULL,

	.get_apic_id			= flat_get_apic_id,
	.set_apic_id			= set_apic_id,
	.apic_id_mask			= 0xFFu << 24,

	.cpu_mask_to_apicid_and		= flat_cpu_mask_to_apicid_and,

=======
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.phys_pkg_id			= flat_phys_pkg_id,

	.get_apic_id			= flat_get_apic_id,
	.set_apic_id			= set_apic_id,

	.cpu_mask_to_apicid_and		= flat_cpu_mask_to_apicid_and,

	.send_IPI			= default_send_IPI_single,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.send_IPI_mask			= flat_send_IPI_mask,
	.send_IPI_mask_allbutself	= flat_send_IPI_mask_allbutself,
	.send_IPI_allbutself		= flat_send_IPI_allbutself,
	.send_IPI_all			= flat_send_IPI_all,
	.send_IPI_self			= apic_send_IPI_self,

<<<<<<< HEAD
	.trampoline_phys_low		= DEFAULT_TRAMPOLINE_PHYS_LOW,
	.trampoline_phys_high		= DEFAULT_TRAMPOLINE_PHYS_HIGH,
	.wait_for_init_deassert		= NULL,
	.smp_callin_clear_local_apic	= NULL,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.inquire_remote_apic		= default_inquire_remote_apic,

	.read				= native_apic_mem_read,
	.write				= native_apic_mem_write,
	.eoi_write			= native_apic_mem_write,
	.icr_read			= native_apic_icr_read,
	.icr_write			= native_apic_icr_write,
	.wait_icr_idle			= native_apic_wait_icr_idle,
	.safe_wait_icr_idle		= native_safe_apic_wait_icr_idle,
};

/*
 * Physflat mode is used when there are more than 8 CPUs on a system.
 * We cannot use logical delivery in this case because the mask
 * overflows, so use physical mode.
 */
static int physflat_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
#ifdef CONFIG_ACPI
	/*
	 * Quirk: some x86_64 machines can only use physical APIC mode
	 * regardless of how many processors are present (x86_64 ES7000
	 * is an example).
	 */
	if (acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID &&
		(acpi_gbl_FADT.flags & ACPI_FADT_APIC_PHYSICAL)) {
		printk(KERN_DEBUG "system APIC only can use physical flat");
		return 1;
	}

	if (!strncmp(oem_id, "IBM", 3) && !strncmp(oem_table_id, "EXA", 3)) {
		printk(KERN_DEBUG "IBM Summit detected, will use apic physical");
		return 1;
	}
#endif

	return 0;
}

<<<<<<< HEAD
static void physflat_send_IPI_mask(const struct cpumask *cpumask, int vector)
{
	default_send_IPI_mask_sequence_phys(cpumask, vector);
}

static void physflat_send_IPI_mask_allbutself(const struct cpumask *cpumask,
					      int vector)
{
	default_send_IPI_mask_allbutself_phys(cpumask, vector);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void physflat_send_IPI_allbutself(int vector)
{
	default_send_IPI_mask_allbutself_phys(cpu_online_mask, vector);
}

static void physflat_send_IPI_all(int vector)
{
<<<<<<< HEAD
	physflat_send_IPI_mask(cpu_online_mask, vector);
=======
	default_send_IPI_mask_sequence_phys(cpu_online_mask, vector);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int physflat_probe(void)
{
	if (apic == &apic_physflat || num_possible_cpus() > 8)
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct apic apic_physflat =  {
=======
static struct apic apic_physflat __ro_after_init = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.name				= "physical flat",
	.probe				= physflat_probe,
	.acpi_madt_oem_check		= physflat_acpi_madt_oem_check,
	.apic_id_valid			= default_apic_id_valid,
	.apic_id_registered		= flat_apic_id_registered,

	.irq_delivery_mode		= dest_Fixed,
	.irq_dest_mode			= 0, /* physical */

	.target_cpus			= online_target_cpus,
	.disable_esr			= 0,
	.dest_logical			= 0,
	.check_apicid_used		= NULL,
<<<<<<< HEAD
	.check_apicid_present		= NULL,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.vector_allocation_domain	= default_vector_allocation_domain,
	/* not needed, but shouldn't hurt: */
	.init_apic_ldr			= flat_init_apic_ldr,

	.ioapic_phys_id_map		= NULL,
	.setup_apic_routing		= NULL,
<<<<<<< HEAD
	.multi_timer_check		= NULL,
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.setup_portio_remap		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.enable_apic_mode		= NULL,
	.phys_pkg_id			= flat_phys_pkg_id,
	.mps_oem_check			= NULL,

	.get_apic_id			= flat_get_apic_id,
	.set_apic_id			= set_apic_id,
	.apic_id_mask			= 0xFFu << 24,

	.cpu_mask_to_apicid_and		= default_cpu_mask_to_apicid_and,

	.send_IPI_mask			= physflat_send_IPI_mask,
	.send_IPI_mask_allbutself	= physflat_send_IPI_mask_allbutself,
=======
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.phys_pkg_id			= flat_phys_pkg_id,

	.get_apic_id			= flat_get_apic_id,
	.set_apic_id			= set_apic_id,

	.cpu_mask_to_apicid_and		= default_cpu_mask_to_apicid_and,

	.send_IPI			= default_send_IPI_single_phys,
	.send_IPI_mask			= default_send_IPI_mask_sequence_phys,
	.send_IPI_mask_allbutself	= default_send_IPI_mask_allbutself_phys,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.send_IPI_allbutself		= physflat_send_IPI_allbutself,
	.send_IPI_all			= physflat_send_IPI_all,
	.send_IPI_self			= apic_send_IPI_self,

<<<<<<< HEAD
	.trampoline_phys_low		= DEFAULT_TRAMPOLINE_PHYS_LOW,
	.trampoline_phys_high		= DEFAULT_TRAMPOLINE_PHYS_HIGH,
	.wait_for_init_deassert		= NULL,
	.smp_callin_clear_local_apic	= NULL,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.inquire_remote_apic		= default_inquire_remote_apic,

	.read				= native_apic_mem_read,
	.write				= native_apic_mem_write,
	.eoi_write			= native_apic_mem_write,
	.icr_read			= native_apic_icr_read,
	.icr_write			= native_apic_icr_write,
	.wait_icr_idle			= native_apic_wait_icr_idle,
	.safe_wait_icr_idle		= native_safe_apic_wait_icr_idle,
};

/*
 * We need to check for physflat first, so this order is important.
 */
apic_drivers(apic_physflat, apic_flat);
