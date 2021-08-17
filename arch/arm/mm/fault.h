#ifndef __ARCH_ARM_FAULT_H
#define __ARCH_ARM_FAULT_H

/*
 * Fault status register encodings.  We steal bit 31 for our own purposes.
 */
#define FSR_LNX_PF		(1 << 31)
<<<<<<< HEAD
=======
#define FSR_CM			(1 << 13)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define FSR_WRITE		(1 << 11)
#define FSR_FS4			(1 << 10)
#define FSR_FS3_0		(15)
#define FSR_FS5_0		(0x3f)

#ifdef CONFIG_ARM_LPAE
<<<<<<< HEAD
=======
#define FSR_FS_AEA		17

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int fsr_fs(unsigned int fsr)
{
	return fsr & FSR_FS5_0;
}
#else
<<<<<<< HEAD
=======
#define FSR_FS_AEA		22

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int fsr_fs(unsigned int fsr)
{
	return (fsr & FSR_FS3_0) | (fsr & FSR_FS4) >> 6;
}
#endif

void do_bad_area(unsigned long addr, unsigned int fsr, struct pt_regs *regs);
<<<<<<< HEAD
unsigned long search_exception_table(unsigned long addr);

#ifdef CONFIG_HISI_RDR
extern unsigned int arm_exc_type;
#endif
=======
void early_abt_enable(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif	/* __ARCH_ARM_FAULT_H */
