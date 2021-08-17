#ifndef _ASM_X86_EMERGENCY_RESTART_H
#define _ASM_X86_EMERGENCY_RESTART_H

<<<<<<< HEAD
enum reboot_type {
	BOOT_TRIPLE = 't',
	BOOT_KBD = 'k',
	BOOT_BIOS = 'b',
	BOOT_ACPI = 'a',
	BOOT_EFI = 'e',
	BOOT_CF9 = 'p',
	BOOT_CF9_COND = 'q',
};

extern enum reboot_type reboot_type;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void machine_emergency_restart(void);

#endif /* _ASM_X86_EMERGENCY_RESTART_H */
