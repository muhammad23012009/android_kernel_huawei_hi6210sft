#ifndef _ASM_X86_MMCONFIG_H
#define _ASM_X86_MMCONFIG_H

#ifdef CONFIG_PCI_MMCONFIG
<<<<<<< HEAD
extern void __cpuinit fam10h_check_enable_mmcfg(void);
extern void __cpuinit check_enable_amd_mmconf_dmi(void);
=======
extern void fam10h_check_enable_mmcfg(void);
extern void check_enable_amd_mmconf_dmi(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline void fam10h_check_enable_mmcfg(void) { }
static inline void check_enable_amd_mmconf_dmi(void) { }
#endif

#endif /* _ASM_X86_MMCONFIG_H */
