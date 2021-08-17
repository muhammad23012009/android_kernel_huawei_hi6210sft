/*
 *  Atheros AR71XX/AR724X/AR913X common definitions
 *
 *  Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  Parts of this file are based on Atheros' 2.6.15 BSP
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef __ATH79_COMMON_H
#define __ATH79_COMMON_H

#include <linux/types.h>
<<<<<<< HEAD
#include <linux/init.h>

#define ATH79_MEM_SIZE_MIN	(2 * 1024 * 1024)
#define ATH79_MEM_SIZE_MAX	(128 * 1024 * 1024)

void ath79_clocks_init(void);
void ath79_ddr_wb_flush(unsigned int reg);

void ath79_gpio_function_enable(u32 mask);
void ath79_gpio_function_disable(u32 mask);
void ath79_gpio_function_setup(u32 set, u32 clear);
=======

#define ATH79_MEM_SIZE_MIN	(2 * 1024 * 1024)
#define ATH79_MEM_SIZE_MAX	(256 * 1024 * 1024)

void ath79_clocks_init(void);
unsigned long ath79_get_sys_clk_rate(const char *id);

void ath79_ddr_ctrl_init(void);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void ath79_gpio_init(void);

#endif /* __ATH79_COMMON_H */
