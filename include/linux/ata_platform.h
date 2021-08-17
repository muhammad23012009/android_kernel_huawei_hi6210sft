#ifndef __LINUX_ATA_PLATFORM_H
#define __LINUX_ATA_PLATFORM_H

struct pata_platform_info {
	/*
	 * I/O port shift, for platforms with ports that are
	 * constantly spaced and need larger than the 1-byte
	 * spacing used by ata_std_ports().
	 */
	unsigned int ioport_shift;
<<<<<<< HEAD
	/* 
	 * Indicate platform specific irq types and initial
	 * IRQ flags when call request_irq()
	 */
	unsigned int irq_flags;
};

=======
};

struct scsi_host_template;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int __pata_platform_probe(struct device *dev,
				 struct resource *io_res,
				 struct resource *ctl_res,
				 struct resource *irq_res,
				 unsigned int ioport_shift,
<<<<<<< HEAD
				 int __pio_mask);
=======
				 int __pio_mask,
				 struct scsi_host_template *sht);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Marvell SATA private data
 */
struct mv_sata_platform_data {
	int	n_ports; /* number of sata ports */
};

#endif /* __LINUX_ATA_PLATFORM_H */
