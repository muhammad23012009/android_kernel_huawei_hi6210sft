#ifndef _LINUX_REBOOT_H
#define _LINUX_REBOOT_H


#include <linux/notifier.h>
#include <uapi/linux/reboot.h>

#define SYS_DOWN	0x0001	/* Notify of system down */
#define SYS_RESTART	SYS_DOWN
#define SYS_HALT	0x0002	/* Notify of system halt */
#define SYS_POWER_OFF	0x0003	/* Notify of system power off */

enum reboot_mode {
	REBOOT_COLD = 0,
	REBOOT_WARM,
<<<<<<< HEAD
};
=======
	REBOOT_HARD,
	REBOOT_SOFT,
	REBOOT_GPIO,
};
extern enum reboot_mode reboot_mode;

enum reboot_type {
	BOOT_TRIPLE	= 't',
	BOOT_KBD	= 'k',
	BOOT_BIOS	= 'b',
	BOOT_ACPI	= 'a',
	BOOT_EFI	= 'e',
	BOOT_CF9_FORCE	= 'p',
	BOOT_CF9_SAFE	= 'q',
};
extern enum reboot_type reboot_type;

extern int reboot_default;
extern int reboot_cpu;
extern int reboot_force;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern int register_reboot_notifier(struct notifier_block *);
extern int unregister_reboot_notifier(struct notifier_block *);

<<<<<<< HEAD
/* DTS2013031107868 qidechun 2013-03-11 begin */
#ifdef CONFIG_SRECORDER
#ifdef CONFIG_POWERCOLLAPSE
#ifndef CONFIG_KPROBES
extern int register_emergency_reboot_notifier(struct notifier_block *);
extern int unregister_emergency_reboot_notifier(struct notifier_block *);
#endif
#endif
#endif /* CONFIG_SRECORDER */
/* DTS2013031107868 qidechun 2013-03-11 end */
=======
extern int register_restart_handler(struct notifier_block *);
extern int unregister_restart_handler(struct notifier_block *);
extern void do_kernel_restart(char *cmd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Architecture-specific implementations of sys_reboot commands.
 */

<<<<<<< HEAD
=======
extern void migrate_to_reboot_cpu(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void machine_restart(char *cmd);
extern void machine_halt(void);
extern void machine_power_off(void);

extern void machine_shutdown(void);
struct pt_regs;
extern void machine_crash_shutdown(struct pt_regs *);

<<<<<<< HEAD
/* 
=======
/*
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Architecture independent implemenations of sys_reboot commands.
 */

extern void kernel_restart_prepare(char *cmd);
extern void kernel_restart(char *cmd);
extern void kernel_halt(void);
extern void kernel_power_off(void);

extern int C_A_D; /* for sysctl */
void ctrl_alt_del(void);

#define POWEROFF_CMD_PATH_LEN	256
extern char poweroff_cmd[POWEROFF_CMD_PATH_LEN];

<<<<<<< HEAD
extern int orderly_poweroff(bool force);
=======
extern void orderly_poweroff(bool force);
extern void orderly_reboot(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Emergency restart, callable from an interrupt handler.
 */

extern void emergency_restart(void);
#include <asm/emergency-restart.h>

#endif /* _LINUX_REBOOT_H */
