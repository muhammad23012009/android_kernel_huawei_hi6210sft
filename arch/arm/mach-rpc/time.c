/*
 *  linux/arch/arm/common/time-acorn.c
 *
 *  Copyright (c) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   24-Sep-1996	RMK	Created
 *   10-Oct-1996	RMK	Brought up to date with arch-sa110eval
 *   04-Dec-1997	RMK	Updated for new arch/arm/time.c
 *   13=Jun-2004	DS	Moved to arch/arm/common b/c shared w/CLPS7500
 */
#include <linux/timex.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/hardware/ioc.h>

#include <asm/mach/time.h>

<<<<<<< HEAD
=======
#define RPC_CLOCK_FREQ 2000000
#define RPC_LATCH DIV_ROUND_CLOSEST(RPC_CLOCK_FREQ, HZ)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static u32 ioc_timer_gettimeoffset(void)
{
	unsigned int count1, count2, status;
	long offset;

	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count1 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);
	barrier ();
	status = ioc_readb(IOC_IRQREQA);
	barrier ();
	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count2 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);

	offset = count2;
	if (count2 < count1) {
		/*
		 * We have not had an interrupt between reading count1
		 * and count2.
		 */
		if (status & (1 << 5))
<<<<<<< HEAD
			offset -= LATCH;
=======
			offset -= RPC_LATCH;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else if (count2 > count1) {
		/*
		 * We have just had another interrupt between reading
		 * count1 and count2.
		 */
<<<<<<< HEAD
		offset -= LATCH;
	}

	offset = (LATCH - offset) * (tick_nsec / 1000);
	return ((offset + LATCH/2) / LATCH) * 1000;
=======
		offset -= RPC_LATCH;
	}

	offset = (RPC_LATCH - offset) * (tick_nsec / 1000);
	return DIV_ROUND_CLOSEST(offset, RPC_LATCH) * 1000;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __init ioctime_init(void)
{
<<<<<<< HEAD
	ioc_writeb(LATCH & 255, IOC_T0LTCHL);
	ioc_writeb(LATCH >> 8, IOC_T0LTCHH);
=======
	ioc_writeb(RPC_LATCH & 255, IOC_T0LTCHL);
	ioc_writeb(RPC_LATCH >> 8, IOC_T0LTCHH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ioc_writeb(0, IOC_T0GO);
}

static irqreturn_t
ioc_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction ioc_timer_irq = {
	.name		= "timer",
<<<<<<< HEAD
	.flags		= IRQF_DISABLED,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.handler	= ioc_timer_interrupt
};

/*
 * Set up timer interrupt.
 */
void __init ioc_timer_init(void)
{
	arch_gettimeoffset = ioc_timer_gettimeoffset;
	ioctime_init();
	setup_irq(IRQ_TIMER0, &ioc_timer_irq);
}
