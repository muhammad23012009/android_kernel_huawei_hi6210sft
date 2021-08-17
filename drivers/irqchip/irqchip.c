/*
 * Copyright (C) 2012 Thomas Petazzoni
 *
 * Thomas Petazzoni <thomas.petazzoni@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#include <linux/init.h>
#include <linux/of_irq.h>

#include "irqchip.h"
=======
#include <linux/acpi.h>
#include <linux/init.h>
#include <linux/of_irq.h>
#include <linux/irqchip.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * This special of_device_id is the sentinel at the end of the
 * of_device_id[] array of all irqchips. It is automatically placed at
 * the end of the array by the linker, thanks to being part of a
 * special section.
 */
static const struct of_device_id
<<<<<<< HEAD
irqchip_of_match_end __used __section(__irqchip_of_end);

extern struct of_device_id __irqchip_begin[];

void __init irqchip_init(void)
{
	of_irq_init(__irqchip_begin);
=======
irqchip_of_match_end __used __section(__irqchip_of_table_end);

extern struct of_device_id __irqchip_of_table[];

void __init irqchip_init(void)
{
	of_irq_init(__irqchip_of_table);
	acpi_probe_device_table(irqchip);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
