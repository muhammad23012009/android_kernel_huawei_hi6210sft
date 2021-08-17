/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

#include <linux/console.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/irqflags.h>
#include <linux/printk.h>
#include <asm/setup.h>
#include <hv/hypervisor.h>

static void early_hv_write(struct console *con, const char *s, unsigned n)
{
<<<<<<< HEAD
	hv_console_write((HV_VirtAddr) s, n);
=======
	tile_console_write(s, n);

	/*
	 * Convert NL to NLCR (close enough to CRNL) during early boot.
	 * We assume newlines are at the ends of strings, which turns out
	 * to be good enough for early boot console output.
	 */
	if (n && s[n-1] == '\n')
		tile_console_write("\r", 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct console early_hv_console = {
	.name =		"earlyhv",
	.write =	early_hv_write,
<<<<<<< HEAD
	.flags =	CON_PRINTBUFFER,
	.index =	-1,
};

/* Direct interface for emergencies */
static int early_console_complete;

void early_panic(const char *fmt, ...)
{
	va_list ap;
	arch_local_irq_disable_all();
	va_start(ap, fmt);
	early_printk("Kernel panic - not syncing: ");
	early_vprintk(fmt, ap);
	early_console->write(early_console, "\n", 1);
	va_end(ap);
=======
	.flags =	CON_PRINTBUFFER | CON_BOOT,
	.index =	-1,
};

void early_panic(const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;

	arch_local_irq_disable_all();

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	early_printk("Kernel panic - not syncing: %pV", &vaf);

	va_end(args);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dump_stack();
	hv_halt();
}

<<<<<<< HEAD
static int __initdata keep_early;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int __init setup_early_printk(char *str)
{
	if (early_console)
		return 1;

<<<<<<< HEAD
	if (str != NULL && strncmp(str, "keep", 4) == 0)
		keep_early = 1;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	early_console = &early_hv_console;
	register_console(early_console);

	return 0;
}

<<<<<<< HEAD
void __init disable_early_printk(void)
{
	early_console_complete = 1;
	if (!early_console)
		return;
	if (!keep_early) {
		early_printk("disabling early console\n");
		unregister_console(early_console);
		early_console = NULL;
	} else {
		early_printk("keeping early console\n");
	}
}

void warn_early_printk(void)
{
	if (early_console_complete || early_console)
		return;
	early_printk("\
Machine shutting down before console output is fully initialized.\n\
You may wish to reboot and add the option 'earlyprintk' to your\n\
boot command line to see any diagnostic early console output.\n\
");
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
early_param("earlyprintk", setup_early_printk);
