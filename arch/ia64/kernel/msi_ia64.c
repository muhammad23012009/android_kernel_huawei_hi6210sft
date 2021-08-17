/*
 * MSI hooks for standard x86 apic
 */

#include <linux/pci.h>
#include <linux/irq.h>
#include <linux/msi.h>
#include <linux/dmar.h>
#include <asm/smp.h>
#include <asm/msidef.h>

static struct irq_chip	ia64_msi_chip;

#ifdef CONFIG_SMP
static int ia64_set_msi_irq_affinity(struct irq_data *idata,
				     const cpumask_t *cpu_mask, bool force)
{
	struct msi_msg msg;
	u32 addr, data;
<<<<<<< HEAD
	int cpu = first_cpu(*cpu_mask);
	unsigned int irq = idata->irq;

	if (!cpu_online(cpu))
		return -1;

	if (irq_prepare_move(irq, cpu))
		return -1;

	get_cached_msi_msg(irq, &msg);
=======
	int cpu = cpumask_first_and(cpu_mask, cpu_online_mask);
	unsigned int irq = idata->irq;

	if (irq_prepare_move(irq, cpu))
		return -1;

	__get_cached_msi_msg(irq_data_get_msi_desc(idata), &msg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	addr = msg.address_lo;
	addr &= MSI_ADDR_DEST_ID_MASK;
	addr |= MSI_ADDR_DEST_ID_CPU(cpu_physical_id(cpu));
	msg.address_lo = addr;

	data = msg.data;
	data &= MSI_DATA_VECTOR_MASK;
	data |= MSI_DATA_VECTOR(irq_to_vector(irq));
	msg.data = data;

<<<<<<< HEAD
	write_msi_msg(irq, &msg);
	cpumask_copy(idata->affinity, cpumask_of(cpu));
=======
	pci_write_msi_msg(irq, &msg);
	cpumask_copy(irq_data_get_affinity_mask(idata), cpumask_of(cpu));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
#endif /* CONFIG_SMP */

int ia64_setup_msi_irq(struct pci_dev *pdev, struct msi_desc *desc)
{
	struct msi_msg	msg;
	unsigned long	dest_phys_id;
	int	irq, vector;
<<<<<<< HEAD
	cpumask_t mask;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	irq = create_irq();
	if (irq < 0)
		return irq;

	irq_set_msi_desc(irq, desc);
<<<<<<< HEAD
	cpumask_and(&mask, &(irq_to_domain(irq)), cpu_online_mask);
	dest_phys_id = cpu_physical_id(first_cpu(mask));
=======
	dest_phys_id = cpu_physical_id(cpumask_any_and(&(irq_to_domain(irq)),
						       cpu_online_mask));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vector = irq_to_vector(irq);

	msg.address_hi = 0;
	msg.address_lo =
		MSI_ADDR_HEADER |
		MSI_ADDR_DEST_MODE_PHYS |
		MSI_ADDR_REDIRECTION_CPU |
		MSI_ADDR_DEST_ID_CPU(dest_phys_id);

	msg.data =
		MSI_DATA_TRIGGER_EDGE |
		MSI_DATA_LEVEL_ASSERT |
		MSI_DATA_DELIVERY_FIXED |
		MSI_DATA_VECTOR(vector);

<<<<<<< HEAD
	write_msi_msg(irq, &msg);
=======
	pci_write_msi_msg(irq, &msg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	irq_set_chip_and_handler(irq, &ia64_msi_chip, handle_edge_irq);

	return 0;
}

void ia64_teardown_msi_irq(unsigned int irq)
{
	destroy_irq(irq);
}

static void ia64_ack_msi_irq(struct irq_data *data)
{
	irq_complete_move(data->irq);
	irq_move_irq(data);
	ia64_eoi();
}

static int ia64_msi_retrigger_irq(struct irq_data *data)
{
	unsigned int vector = irq_to_vector(data->irq);
	ia64_resend_irq(vector);

	return 1;
}

/*
 * Generic ops used on most IA64 platforms.
 */
static struct irq_chip ia64_msi_chip = {
	.name			= "PCI-MSI",
<<<<<<< HEAD
	.irq_mask		= mask_msi_irq,
	.irq_unmask		= unmask_msi_irq,
=======
	.irq_mask		= pci_msi_mask_irq,
	.irq_unmask		= pci_msi_unmask_irq,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.irq_ack		= ia64_ack_msi_irq,
#ifdef CONFIG_SMP
	.irq_set_affinity	= ia64_set_msi_irq_affinity,
#endif
	.irq_retrigger		= ia64_msi_retrigger_irq,
};


int arch_setup_msi_irq(struct pci_dev *pdev, struct msi_desc *desc)
{
	if (platform_setup_msi_irq)
		return platform_setup_msi_irq(pdev, desc);

	return ia64_setup_msi_irq(pdev, desc);
}

void arch_teardown_msi_irq(unsigned int irq)
{
	if (platform_teardown_msi_irq)
		return platform_teardown_msi_irq(irq);

	return ia64_teardown_msi_irq(irq);
}

#ifdef CONFIG_INTEL_IOMMU
#ifdef CONFIG_SMP
static int dmar_msi_set_affinity(struct irq_data *data,
				 const struct cpumask *mask, bool force)
{
	unsigned int irq = data->irq;
	struct irq_cfg *cfg = irq_cfg + irq;
	struct msi_msg msg;
<<<<<<< HEAD
	int cpu = cpumask_first(mask);

	if (!cpu_online(cpu))
		return -1;
=======
	int cpu = cpumask_first_and(mask, cpu_online_mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (irq_prepare_move(irq, cpu))
		return -1;

	dmar_msi_read(irq, &msg);

	msg.data &= ~MSI_DATA_VECTOR_MASK;
	msg.data |= MSI_DATA_VECTOR(cfg->vector);
	msg.address_lo &= ~MSI_ADDR_DEST_ID_MASK;
	msg.address_lo |= MSI_ADDR_DEST_ID_CPU(cpu_physical_id(cpu));

	dmar_msi_write(irq, &msg);
<<<<<<< HEAD
	cpumask_copy(data->affinity, mask);
=======
	cpumask_copy(irq_data_get_affinity_mask(data), mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
#endif /* CONFIG_SMP */

static struct irq_chip dmar_msi_type = {
	.name = "DMAR_MSI",
	.irq_unmask = dmar_msi_unmask,
	.irq_mask = dmar_msi_mask,
	.irq_ack = ia64_ack_msi_irq,
#ifdef CONFIG_SMP
	.irq_set_affinity = dmar_msi_set_affinity,
#endif
	.irq_retrigger = ia64_msi_retrigger_irq,
};

<<<<<<< HEAD
static int
=======
static void
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
msi_compose_msg(struct pci_dev *pdev, unsigned int irq, struct msi_msg *msg)
{
	struct irq_cfg *cfg = irq_cfg + irq;
	unsigned dest;
<<<<<<< HEAD
	cpumask_t mask;

	cpumask_and(&mask, &(irq_to_domain(irq)), cpu_online_mask);
	dest = cpu_physical_id(first_cpu(mask));
=======

	dest = cpu_physical_id(cpumask_first_and(&(irq_to_domain(irq)),
						 cpu_online_mask));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	msg->address_hi = 0;
	msg->address_lo =
		MSI_ADDR_HEADER |
		MSI_ADDR_DEST_MODE_PHYS |
		MSI_ADDR_REDIRECTION_CPU |
		MSI_ADDR_DEST_ID_CPU(dest);

	msg->data =
		MSI_DATA_TRIGGER_EDGE |
		MSI_DATA_LEVEL_ASSERT |
		MSI_DATA_DELIVERY_FIXED |
		MSI_DATA_VECTOR(cfg->vector);
<<<<<<< HEAD
	return 0;
}

int arch_setup_dmar_msi(unsigned int irq)
{
	int ret;
	struct msi_msg msg;

	ret = msi_compose_msg(NULL, irq, &msg);
	if (ret < 0)
		return ret;
	dmar_msi_write(irq, &msg);
	irq_set_chip_and_handler_name(irq, &dmar_msi_type, handle_edge_irq,
				      "edge");
	return 0;
=======
}

int dmar_alloc_hwirq(int id, int node, void *arg)
{
	int irq;
	struct msi_msg msg;

	irq = create_irq();
	if (irq > 0) {
		irq_set_handler_data(irq, arg);
		irq_set_chip_and_handler_name(irq, &dmar_msi_type,
					      handle_edge_irq, "edge");
		msi_compose_msg(NULL, irq, &msg);
		dmar_msi_write(irq, &msg);
	}

	return irq;
}

void dmar_free_hwirq(int irq)
{
	irq_set_handler_data(irq, NULL);
	destroy_irq(irq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif /* CONFIG_INTEL_IOMMU */

