/*
 * Clock driver for the ARM Integrator/IM-PD1 board
<<<<<<< HEAD
 * Copyright (C) 2012 Linus Walleij
=======
 * Copyright (C) 2012-2013 Linus Walleij
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/clk-provider.h>
<<<<<<< HEAD
#include <linux/clk.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/clkdev.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_data/clk-integrator.h>

<<<<<<< HEAD
#include <mach/impd1.h>

#include "clk-icst.h"

struct impd1_clk {
	struct clk *vcoclk;
	struct clk *uartclk;
	struct clk_lookup *clks[3];
};

static struct impd1_clk impd1_clks[4];

/*
 * There are two VCO's on the IM-PD1 but only one is used by the
 * kernel, that is why we are only implementing the control of
 * IMPD1_OSC1 here.
 */

static const struct icst_params impd1_vco_params = {
=======
#include "clk-icst.h"

#define IMPD1_OSC1	0x00
#define IMPD1_OSC2	0x04
#define IMPD1_LOCK	0x08

struct impd1_clk {
	char *pclkname;
	struct clk *pclk;
	char *vco1name;
	struct clk *vco1clk;
	char *vco2name;
	struct clk *vco2clk;
	struct clk *mmciclk;
	char *uartname;
	struct clk *uartclk;
	char *spiname;
	struct clk *spiclk;
	char *scname;
	struct clk *scclk;
	struct clk_lookup *clks[15];
};

/* One entry for each connected IM-PD1 LM */
static struct impd1_clk impd1_clks[4];

/*
 * There are two VCO's on the IM-PD1
 */

static const struct icst_params impd1_vco1_params = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.ref		= 24000000,	/* 24 MHz */
	.vco_max	= ICST525_VCO_MAX_3V,
	.vco_min	= ICST525_VCO_MIN,
	.vd_min		= 12,
	.vd_max		= 519,
	.rd_min		= 3,
	.rd_max		= 120,
	.s2div		= icst525_s2div,
	.idx2s		= icst525_idx2s,
};

static const struct clk_icst_desc impd1_icst1_desc = {
<<<<<<< HEAD
	.params = &impd1_vco_params,
=======
	.params = &impd1_vco1_params,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.vco_offset = IMPD1_OSC1,
	.lock_offset = IMPD1_LOCK,
};

<<<<<<< HEAD
=======
static const struct icst_params impd1_vco2_params = {
	.ref		= 24000000,	/* 24 MHz */
	.vco_max	= ICST525_VCO_MAX_3V,
	.vco_min	= ICST525_VCO_MIN,
	.vd_min		= 12,
	.vd_max		= 519,
	.rd_min		= 3,
	.rd_max		= 120,
	.s2div		= icst525_s2div,
	.idx2s		= icst525_idx2s,
};

static const struct clk_icst_desc impd1_icst2_desc = {
	.params = &impd1_vco2_params,
	.vco_offset = IMPD1_OSC2,
	.lock_offset = IMPD1_LOCK,
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * integrator_impd1_clk_init() - set up the integrator clock tree
 * @base: base address of the logic module (LM)
 * @id: the ID of this LM
 */
void integrator_impd1_clk_init(void __iomem *base, unsigned int id)
{
	struct impd1_clk *imc;
	struct clk *clk;
<<<<<<< HEAD
=======
	struct clk *pclk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i;

	if (id > 3) {
		pr_crit("no more than 4 LMs can be attached\n");
		return;
	}
	imc = &impd1_clks[id];

<<<<<<< HEAD
	clk = icst_clk_register(NULL, &impd1_icst1_desc, base);
	imc->vcoclk = clk;
	imc->clks[0] = clkdev_alloc(clk, NULL, "lm%x:01000", id);

	/* UART reference clock */
	clk = clk_register_fixed_rate(NULL, "uartclk", NULL, CLK_IS_ROOT,
				14745600);
	imc->uartclk = clk;
	imc->clks[1] = clkdev_alloc(clk, NULL, "lm%x:00100", id);
	imc->clks[2] = clkdev_alloc(clk, NULL, "lm%x:00200", id);
=======
	/* Register the fixed rate PCLK */
	imc->pclkname = kasprintf(GFP_KERNEL, "lm%x-pclk", id);
	pclk = clk_register_fixed_rate(NULL, imc->pclkname, NULL, 0, 0);
	imc->pclk = pclk;

	imc->vco1name = kasprintf(GFP_KERNEL, "lm%x-vco1", id);
	clk = icst_clk_register(NULL, &impd1_icst1_desc, imc->vco1name, NULL,
				base);
	imc->vco1clk = clk;
	imc->clks[0] = clkdev_alloc(pclk, "apb_pclk", "lm%x:01000", id);
	imc->clks[1] = clkdev_alloc(clk, NULL, "lm%x:01000", id);

	/* VCO2 is also called "CLK2" */
	imc->vco2name = kasprintf(GFP_KERNEL, "lm%x-vco2", id);
	clk = icst_clk_register(NULL, &impd1_icst2_desc, imc->vco2name, NULL,
				base);
	imc->vco2clk = clk;

	/* MMCI uses CLK2 right off */
	imc->clks[2] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00700", id);
	imc->clks[3] = clkdev_alloc(clk, NULL, "lm%x:00700", id);

	/* UART reference clock divides CLK2 by a fixed factor 4 */
	imc->uartname = kasprintf(GFP_KERNEL, "lm%x-uartclk", id);
	clk = clk_register_fixed_factor(NULL, imc->uartname, imc->vco2name,
				   CLK_IGNORE_UNUSED, 1, 4);
	imc->uartclk = clk;
	imc->clks[4] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00100", id);
	imc->clks[5] = clkdev_alloc(clk, NULL, "lm%x:00100", id);
	imc->clks[6] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00200", id);
	imc->clks[7] = clkdev_alloc(clk, NULL, "lm%x:00200", id);

	/* SPI PL022 clock divides CLK2 by a fixed factor 64 */
	imc->spiname = kasprintf(GFP_KERNEL, "lm%x-spiclk", id);
	clk = clk_register_fixed_factor(NULL, imc->spiname, imc->vco2name,
				   CLK_IGNORE_UNUSED, 1, 64);
	imc->clks[8] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00300", id);
	imc->clks[9] = clkdev_alloc(clk, NULL, "lm%x:00300", id);

	/* The GPIO blocks and AACI have only PCLK */
	imc->clks[10] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00400", id);
	imc->clks[11] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00500", id);
	imc->clks[12] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00800", id);

	/* Smart Card clock divides CLK2 by a fixed factor 4 */
	imc->scname = kasprintf(GFP_KERNEL, "lm%x-scclk", id);
	clk = clk_register_fixed_factor(NULL, imc->scname, imc->vco2name,
				   CLK_IGNORE_UNUSED, 1, 4);
	imc->scclk = clk;
	imc->clks[13] = clkdev_alloc(pclk, "apb_pclk", "lm%x:00600", id);
	imc->clks[14] = clkdev_alloc(clk, NULL, "lm%x:00600", id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < ARRAY_SIZE(imc->clks); i++)
		clkdev_add(imc->clks[i]);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(integrator_impd1_clk_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void integrator_impd1_clk_exit(unsigned int id)
{
	int i;
	struct impd1_clk *imc;

	if (id > 3)
		return;
	imc = &impd1_clks[id];

	for (i = 0; i < ARRAY_SIZE(imc->clks); i++)
		clkdev_drop(imc->clks[i]);
<<<<<<< HEAD
	clk_unregister(imc->uartclk);
	clk_unregister(imc->vcoclk);
}
=======
	clk_unregister(imc->spiclk);
	clk_unregister(imc->uartclk);
	clk_unregister(imc->vco2clk);
	clk_unregister(imc->vco1clk);
	clk_unregister(imc->pclk);
	kfree(imc->scname);
	kfree(imc->spiname);
	kfree(imc->uartname);
	kfree(imc->vco2name);
	kfree(imc->vco1name);
	kfree(imc->pclkname);
}
EXPORT_SYMBOL_GPL(integrator_impd1_clk_exit);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
