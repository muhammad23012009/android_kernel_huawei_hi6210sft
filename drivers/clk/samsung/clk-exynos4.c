/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 * Copyright (c) 2013 Linaro Ltd.
 * Author: Thomas Abraham <thomas.ab@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Common Clock Framework support for all Exynos4 SoCs.
*/

<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include "clk.h"
#include "clk-pll.h"
=======
#include <dt-bindings/clock/exynos4.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/syscore_ops.h>

#include "clk.h"
#include "clk-cpu.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Exynos4 clock controller register offsets */
#define SRC_LEFTBUS		0x4200
#define DIV_LEFTBUS		0x4500
#define GATE_IP_LEFTBUS		0x4800
#define E4X12_GATE_IP_IMAGE	0x4930
<<<<<<< HEAD
=======
#define CLKOUT_CMU_LEFTBUS	0x4a00
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SRC_RIGHTBUS		0x8200
#define DIV_RIGHTBUS		0x8500
#define GATE_IP_RIGHTBUS	0x8800
#define E4X12_GATE_IP_PERIR	0x8960
<<<<<<< HEAD
=======
#define CLKOUT_CMU_RIGHTBUS	0x8a00
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define EPLL_LOCK		0xc010
#define VPLL_LOCK		0xc020
#define EPLL_CON0		0xc110
#define EPLL_CON1		0xc114
#define EPLL_CON2		0xc118
#define VPLL_CON0		0xc120
#define VPLL_CON1		0xc124
#define VPLL_CON2		0xc128
#define SRC_TOP0		0xc210
#define SRC_TOP1		0xc214
#define SRC_CAM			0xc220
#define SRC_TV			0xc224
#define SRC_MFC			0xc228
#define SRC_G3D			0xc22c
#define E4210_SRC_IMAGE		0xc230
#define SRC_LCD0		0xc234
#define E4210_SRC_LCD1		0xc238
#define E4X12_SRC_ISP		0xc238
#define SRC_MAUDIO		0xc23c
#define SRC_FSYS		0xc240
#define SRC_PERIL0		0xc250
#define SRC_PERIL1		0xc254
#define E4X12_SRC_CAM1		0xc258
#define SRC_MASK_TOP		0xc310
#define SRC_MASK_CAM		0xc320
#define SRC_MASK_TV		0xc324
#define SRC_MASK_LCD0		0xc334
#define E4210_SRC_MASK_LCD1	0xc338
#define E4X12_SRC_MASK_ISP	0xc338
#define SRC_MASK_MAUDIO		0xc33c
#define SRC_MASK_FSYS		0xc340
#define SRC_MASK_PERIL0		0xc350
#define SRC_MASK_PERIL1		0xc354
#define DIV_TOP			0xc510
#define DIV_CAM			0xc520
#define DIV_TV			0xc524
#define DIV_MFC			0xc528
#define DIV_G3D			0xc52c
#define DIV_IMAGE		0xc530
#define DIV_LCD0		0xc534
#define E4210_DIV_LCD1		0xc538
#define E4X12_DIV_ISP		0xc538
#define DIV_MAUDIO		0xc53c
#define DIV_FSYS0		0xc540
#define DIV_FSYS1		0xc544
#define DIV_FSYS2		0xc548
#define DIV_FSYS3		0xc54c
#define DIV_PERIL0		0xc550
#define DIV_PERIL1		0xc554
#define DIV_PERIL2		0xc558
#define DIV_PERIL3		0xc55c
#define DIV_PERIL4		0xc560
#define DIV_PERIL5		0xc564
#define E4X12_DIV_CAM1		0xc568
<<<<<<< HEAD
=======
#define E4X12_GATE_BUS_FSYS1	0xc744
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define GATE_SCLK_CAM		0xc820
#define GATE_IP_CAM		0xc920
#define GATE_IP_TV		0xc924
#define GATE_IP_MFC		0xc928
#define GATE_IP_G3D		0xc92c
#define E4210_GATE_IP_IMAGE	0xc930
#define GATE_IP_LCD0		0xc934
#define E4210_GATE_IP_LCD1	0xc938
#define E4X12_GATE_IP_ISP	0xc938
#define E4X12_GATE_IP_MAUDIO	0xc93c
#define GATE_IP_FSYS		0xc940
#define GATE_IP_GPS		0xc94c
#define GATE_IP_PERIL		0xc950
#define E4210_GATE_IP_PERIR	0xc960
#define GATE_BLOCK		0xc970
<<<<<<< HEAD
=======
#define CLKOUT_CMU_TOP		0xca00
#define E4X12_MPLL_LOCK		0x10008
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define E4X12_MPLL_CON0		0x10108
#define SRC_DMC			0x10200
#define SRC_MASK_DMC		0x10300
#define DIV_DMC0		0x10500
#define DIV_DMC1		0x10504
#define GATE_IP_DMC		0x10900
<<<<<<< HEAD
=======
#define CLKOUT_CMU_DMC		0x10a00
#define APLL_LOCK		0x14000
#define E4210_MPLL_LOCK		0x14008
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define APLL_CON0		0x14100
#define E4210_MPLL_CON0		0x14108
#define SRC_CPU			0x14200
#define DIV_CPU0		0x14500
#define DIV_CPU1		0x14504
#define GATE_SCLK_CPU		0x14800
#define GATE_IP_CPU		0x14900
<<<<<<< HEAD
=======
#define CLKOUT_CMU_CPU		0x14a00
#define PWR_CTRL1		0x15020
#define E4X12_PWR_CTRL2		0x15024
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define E4X12_DIV_ISP0		0x18300
#define E4X12_DIV_ISP1		0x18304
#define E4X12_GATE_ISP0		0x18800
#define E4X12_GATE_ISP1		0x18804

<<<<<<< HEAD
=======
/* Below definitions are used for PWR_CTRL settings */
#define PWR_CTRL1_CORE2_DOWN_RATIO(x)		(((x) & 0x7) << 28)
#define PWR_CTRL1_CORE1_DOWN_RATIO(x)		(((x) & 0x7) << 16)
#define PWR_CTRL1_DIV2_DOWN_EN			(1 << 9)
#define PWR_CTRL1_DIV1_DOWN_EN			(1 << 8)
#define PWR_CTRL1_USE_CORE3_WFE			(1 << 7)
#define PWR_CTRL1_USE_CORE2_WFE			(1 << 6)
#define PWR_CTRL1_USE_CORE1_WFE			(1 << 5)
#define PWR_CTRL1_USE_CORE0_WFE			(1 << 4)
#define PWR_CTRL1_USE_CORE3_WFI			(1 << 3)
#define PWR_CTRL1_USE_CORE2_WFI			(1 << 2)
#define PWR_CTRL1_USE_CORE1_WFI			(1 << 1)
#define PWR_CTRL1_USE_CORE0_WFI			(1 << 0)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* the exynos4 soc type */
enum exynos4_soc {
	EXYNOS4210,
	EXYNOS4X12,
};

<<<<<<< HEAD
/*
 * Let each supported clock get a unique id. This id is used to lookup the clock
 * for device tree based platforms. The clocks are categorized into three
 * sections: core, sclk gate and bus interface gate clocks.
 *
 * When adding a new clock to this list, it is advised to choose a clock
 * category and add it to the end of that category. That is because the the
 * device tree source file is referring to these ids and any change in the
 * sequence number of existing clocks will require corresponding change in the
 * device tree files. This limitation would go away when pre-processor support
 * for dtc would be available.
 */
enum exynos4_clks {
	none,

	/* core clocks */
	xxti, xusbxti, fin_pll, fout_apll, fout_mpll, fout_epll, fout_vpll,
	sclk_apll, sclk_mpll, sclk_epll, sclk_vpll, arm_clk, aclk200, aclk100,
	aclk160, aclk133, mout_mpll_user_t, mout_mpll_user_c, mout_core,
	mout_apll, /* 20 */

	/* gate for special clocks (sclk) */
	sclk_fimc0 = 128, sclk_fimc1, sclk_fimc2, sclk_fimc3, sclk_cam0,
	sclk_cam1, sclk_csis0, sclk_csis1, sclk_hdmi, sclk_mixer, sclk_dac,
	sclk_pixel, sclk_fimd0, sclk_mdnie0, sclk_mdnie_pwm0, sclk_mipi0,
	sclk_audio0, sclk_mmc0, sclk_mmc1, sclk_mmc2, sclk_mmc3, sclk_mmc4,
	sclk_sata, sclk_uart0, sclk_uart1, sclk_uart2, sclk_uart3, sclk_uart4,
	sclk_audio1, sclk_audio2, sclk_spdif, sclk_spi0, sclk_spi1, sclk_spi2,
	sclk_slimbus, sclk_fimd1, sclk_mipi1, sclk_pcm1, sclk_pcm2, sclk_i2s1,
	sclk_i2s2, sclk_mipihsi, sclk_mfc, sclk_pcm0, sclk_g3d, sclk_pwm_isp,
	sclk_spi0_isp, sclk_spi1_isp, sclk_uart_isp,

	/* gate clocks */
	fimc0 = 256, fimc1, fimc2, fimc3, csis0, csis1, jpeg, smmu_fimc0,
	smmu_fimc1, smmu_fimc2, smmu_fimc3, smmu_jpeg, vp, mixer, tvenc, hdmi,
	smmu_tv, mfc, smmu_mfcl, smmu_mfcr, g3d, g2d, rotator, mdma, smmu_g2d,
	smmu_rotator, smmu_mdma, fimd0, mie0, mdnie0, dsim0, smmu_fimd0, fimd1,
	mie1, dsim1, smmu_fimd1, pdma0, pdma1, pcie_phy, sata_phy, tsi, sdmmc0,
	sdmmc1, sdmmc2, sdmmc3, sdmmc4, sata, sromc, usb_host, usb_device, pcie,
	onenand, nfcon, smmu_pcie, gps, smmu_gps, uart0, uart1, uart2, uart3,
	uart4, i2c0, i2c1, i2c2, i2c3, i2c4, i2c5, i2c6, i2c7, i2c_hdmi, tsadc,
	spi0, spi1, spi2, i2s1, i2s2, pcm0, i2s0, pcm1, pcm2, pwm, slimbus,
	spdif, ac97, modemif, chipid, sysreg, hdmi_cec, mct, wdt, rtc, keyif,
	audss, mipi_hsi, mdma2, pixelasyncm0, pixelasyncm1, fimc_lite0,
	fimc_lite1, ppmuispx, ppmuispmx, fimc_isp, fimc_drc, fimc_fd, mcuisp,
	gicisp, smmu_isp, smmu_drc, smmu_fd, smmu_lite0, smmu_lite1, mcuctl_isp,
	mpwm_isp, i2c0_isp, i2c1_isp, mtcadc_isp, pwm_isp, wdt_isp, uart_isp,
	asyncaxim, smmu_ispcx, spi0_isp, spi1_isp, pwm_isp_sclk, spi0_isp_sclk,
	spi1_isp_sclk, uart_isp_sclk,

	/* mux clocks */
	mout_fimc0 = 384, mout_fimc1, mout_fimc2, mout_fimc3, mout_cam0,
	mout_cam1, mout_csis0, mout_csis1, mout_g3d0, mout_g3d1, mout_g3d,
	aclk400_mcuisp,

	/* div clocks */
	div_isp0 = 450, div_isp1, div_mcuisp0, div_mcuisp1, div_aclk200,
	div_aclk400_mcuisp,

	nr_clks,
};
=======
/* list of PLLs to be registered */
enum exynos4_plls {
	apll, mpll, epll, vpll,
	nr_plls			/* number of PLLs */
};

static void __iomem *reg_base;
static enum exynos4_soc exynos4_soc;

/*
 * Support for CMU save/restore across system suspends
 */
#ifdef CONFIG_PM_SLEEP
static struct samsung_clk_reg_dump *exynos4_save_common;
static struct samsung_clk_reg_dump *exynos4_save_soc;
static struct samsung_clk_reg_dump *exynos4_save_pll;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * list of controller registers to be saved and restored during a
 * suspend/resume cycle.
 */
<<<<<<< HEAD
static __initdata unsigned long exynos4210_clk_save[] = {
=======
static const unsigned long exynos4210_clk_save[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	E4210_SRC_IMAGE,
	E4210_SRC_LCD1,
	E4210_SRC_MASK_LCD1,
	E4210_DIV_LCD1,
	E4210_GATE_IP_IMAGE,
	E4210_GATE_IP_LCD1,
	E4210_GATE_IP_PERIR,
	E4210_MPLL_CON0,
<<<<<<< HEAD
};

static __initdata unsigned long exynos4x12_clk_save[] = {
=======
	PWR_CTRL1,
};

static const unsigned long exynos4x12_clk_save[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	E4X12_GATE_IP_IMAGE,
	E4X12_GATE_IP_PERIR,
	E4X12_SRC_CAM1,
	E4X12_DIV_ISP,
	E4X12_DIV_CAM1,
	E4X12_MPLL_CON0,
<<<<<<< HEAD
};

static __initdata unsigned long exynos4_clk_regs[] = {
	SRC_LEFTBUS,
	DIV_LEFTBUS,
	GATE_IP_LEFTBUS,
	SRC_RIGHTBUS,
	DIV_RIGHTBUS,
	GATE_IP_RIGHTBUS,
=======
	PWR_CTRL1,
	E4X12_PWR_CTRL2,
};

static const unsigned long exynos4_clk_pll_regs[] __initconst = {
	EPLL_LOCK,
	VPLL_LOCK,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	EPLL_CON0,
	EPLL_CON1,
	EPLL_CON2,
	VPLL_CON0,
	VPLL_CON1,
	VPLL_CON2,
<<<<<<< HEAD
=======
};

static const unsigned long exynos4_clk_regs[] __initconst = {
	SRC_LEFTBUS,
	DIV_LEFTBUS,
	GATE_IP_LEFTBUS,
	SRC_RIGHTBUS,
	DIV_RIGHTBUS,
	GATE_IP_RIGHTBUS,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	SRC_TOP0,
	SRC_TOP1,
	SRC_CAM,
	SRC_TV,
	SRC_MFC,
	SRC_G3D,
	SRC_LCD0,
	SRC_MAUDIO,
	SRC_FSYS,
	SRC_PERIL0,
	SRC_PERIL1,
	SRC_MASK_TOP,
	SRC_MASK_CAM,
	SRC_MASK_TV,
	SRC_MASK_LCD0,
	SRC_MASK_MAUDIO,
	SRC_MASK_FSYS,
	SRC_MASK_PERIL0,
	SRC_MASK_PERIL1,
	DIV_TOP,
	DIV_CAM,
	DIV_TV,
	DIV_MFC,
	DIV_G3D,
	DIV_IMAGE,
	DIV_LCD0,
	DIV_MAUDIO,
	DIV_FSYS0,
	DIV_FSYS1,
	DIV_FSYS2,
	DIV_FSYS3,
	DIV_PERIL0,
	DIV_PERIL1,
	DIV_PERIL2,
	DIV_PERIL3,
	DIV_PERIL4,
	DIV_PERIL5,
	GATE_SCLK_CAM,
	GATE_IP_CAM,
	GATE_IP_TV,
	GATE_IP_MFC,
	GATE_IP_G3D,
	GATE_IP_LCD0,
	GATE_IP_FSYS,
	GATE_IP_GPS,
	GATE_IP_PERIL,
	GATE_BLOCK,
	SRC_MASK_DMC,
	SRC_DMC,
	DIV_DMC0,
	DIV_DMC1,
	GATE_IP_DMC,
	APLL_CON0,
	SRC_CPU,
	DIV_CPU0,
	DIV_CPU1,
	GATE_SCLK_CPU,
	GATE_IP_CPU,
<<<<<<< HEAD
};

=======
	CLKOUT_CMU_LEFTBUS,
	CLKOUT_CMU_RIGHTBUS,
	CLKOUT_CMU_TOP,
	CLKOUT_CMU_DMC,
	CLKOUT_CMU_CPU,
};

static const struct samsung_clk_reg_dump src_mask_suspend[] = {
	{ .offset = SRC_MASK_TOP,		.value = 0x00000001, },
	{ .offset = SRC_MASK_CAM,		.value = 0x11111111, },
	{ .offset = SRC_MASK_TV,		.value = 0x00000111, },
	{ .offset = SRC_MASK_LCD0,		.value = 0x00001111, },
	{ .offset = SRC_MASK_MAUDIO,		.value = 0x00000001, },
	{ .offset = SRC_MASK_FSYS,		.value = 0x01011111, },
	{ .offset = SRC_MASK_PERIL0,		.value = 0x01111111, },
	{ .offset = SRC_MASK_PERIL1,		.value = 0x01110111, },
	{ .offset = SRC_MASK_DMC,		.value = 0x00010000, },
};

static const struct samsung_clk_reg_dump src_mask_suspend_e4210[] = {
	{ .offset = E4210_SRC_MASK_LCD1,	.value = 0x00001111, },
};

#define PLL_ENABLED	(1 << 31)
#define PLL_LOCKED	(1 << 29)

static void exynos4_clk_wait_for_pll(u32 reg)
{
	u32 pll_con;

	pll_con = readl(reg_base + reg);
	if (!(pll_con & PLL_ENABLED))
		return;

	while (!(pll_con & PLL_LOCKED)) {
		cpu_relax();
		pll_con = readl(reg_base + reg);
	}
}

static int exynos4_clk_suspend(void)
{
	samsung_clk_save(reg_base, exynos4_save_common,
				ARRAY_SIZE(exynos4_clk_regs));
	samsung_clk_save(reg_base, exynos4_save_pll,
				ARRAY_SIZE(exynos4_clk_pll_regs));

	if (exynos4_soc == EXYNOS4210) {
		samsung_clk_save(reg_base, exynos4_save_soc,
					ARRAY_SIZE(exynos4210_clk_save));
		samsung_clk_restore(reg_base, src_mask_suspend_e4210,
					ARRAY_SIZE(src_mask_suspend_e4210));
	} else {
		samsung_clk_save(reg_base, exynos4_save_soc,
					ARRAY_SIZE(exynos4x12_clk_save));
	}

	samsung_clk_restore(reg_base, src_mask_suspend,
					ARRAY_SIZE(src_mask_suspend));

	return 0;
}

static void exynos4_clk_resume(void)
{
	samsung_clk_restore(reg_base, exynos4_save_pll,
				ARRAY_SIZE(exynos4_clk_pll_regs));

	exynos4_clk_wait_for_pll(EPLL_CON0);
	exynos4_clk_wait_for_pll(VPLL_CON0);

	samsung_clk_restore(reg_base, exynos4_save_common,
				ARRAY_SIZE(exynos4_clk_regs));

	if (exynos4_soc == EXYNOS4210)
		samsung_clk_restore(reg_base, exynos4_save_soc,
					ARRAY_SIZE(exynos4210_clk_save));
	else
		samsung_clk_restore(reg_base, exynos4_save_soc,
					ARRAY_SIZE(exynos4x12_clk_save));
}

static struct syscore_ops exynos4_clk_syscore_ops = {
	.suspend = exynos4_clk_suspend,
	.resume = exynos4_clk_resume,
};

static void __init exynos4_clk_sleep_init(void)
{
	exynos4_save_common = samsung_clk_alloc_reg_dump(exynos4_clk_regs,
					ARRAY_SIZE(exynos4_clk_regs));
	if (!exynos4_save_common)
		goto err_warn;

	if (exynos4_soc == EXYNOS4210)
		exynos4_save_soc = samsung_clk_alloc_reg_dump(
					exynos4210_clk_save,
					ARRAY_SIZE(exynos4210_clk_save));
	else
		exynos4_save_soc = samsung_clk_alloc_reg_dump(
					exynos4x12_clk_save,
					ARRAY_SIZE(exynos4x12_clk_save));
	if (!exynos4_save_soc)
		goto err_common;

	exynos4_save_pll = samsung_clk_alloc_reg_dump(exynos4_clk_pll_regs,
					ARRAY_SIZE(exynos4_clk_pll_regs));
	if (!exynos4_save_pll)
		goto err_soc;

	register_syscore_ops(&exynos4_clk_syscore_ops);
	return;

err_soc:
	kfree(exynos4_save_soc);
err_common:
	kfree(exynos4_save_common);
err_warn:
	pr_warn("%s: failed to allocate sleep save data, no sleep support!\n",
		__func__);
}
#else
static void __init exynos4_clk_sleep_init(void) {}
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* list of all parent clock list */
PNAME(mout_apll_p)	= { "fin_pll", "fout_apll", };
PNAME(mout_mpll_p)	= { "fin_pll", "fout_mpll", };
PNAME(mout_epll_p)	= { "fin_pll", "fout_epll", };
PNAME(mout_vpllsrc_p)	= { "fin_pll", "sclk_hdmi24m", };
PNAME(mout_vpll_p)	= { "fin_pll", "fout_vpll", };
PNAME(sclk_evpll_p)	= { "sclk_epll", "sclk_vpll", };
PNAME(mout_mfc_p)	= { "mout_mfc0", "mout_mfc1", };
PNAME(mout_g3d_p)	= { "mout_g3d0", "mout_g3d1", };
PNAME(mout_g2d_p)	= { "mout_g2d0", "mout_g2d1", };
PNAME(mout_hdmi_p)	= { "sclk_pixel", "sclk_hdmiphy", };
PNAME(mout_jpeg_p)	= { "mout_jpeg0", "mout_jpeg1", };
PNAME(mout_spdif_p)	= { "sclk_audio0", "sclk_audio1", "sclk_audio2",
				"spdif_extclk", };
PNAME(mout_onenand_p)  = {"aclk133", "aclk160", };
PNAME(mout_onenand1_p) = {"mout_onenand", "sclk_vpll", };

/* Exynos 4210-specific parent groups */
PNAME(sclk_vpll_p4210)	= { "mout_vpllsrc", "fout_vpll", };
PNAME(mout_core_p4210)	= { "mout_apll", "sclk_mpll", };
PNAME(sclk_ampll_p4210)	= { "sclk_mpll", "sclk_apll", };
PNAME(group1_p4210)	= { "xxti", "xusbxti", "sclk_hdmi24m",
				"sclk_usbphy0", "none",	"sclk_hdmiphy",
				"sclk_mpll", "sclk_epll", "sclk_vpll", };
PNAME(mout_audio0_p4210) = { "cdclk0", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti", "sclk_mpll",
				"sclk_epll", "sclk_vpll" };
PNAME(mout_audio1_p4210) = { "cdclk1", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti", "sclk_mpll",
				"sclk_epll", "sclk_vpll", };
PNAME(mout_audio2_p4210) = { "cdclk2", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti", "sclk_mpll",
				"sclk_epll", "sclk_vpll", };
PNAME(mout_mixer_p4210)	= { "sclk_dac", "sclk_hdmi", };
PNAME(mout_dac_p4210)	= { "sclk_vpll", "sclk_hdmiphy", };
<<<<<<< HEAD
=======
PNAME(mout_pwi_p4210) = { "xxti", "xusbxti", "sclk_hdmi24m", "sclk_usbphy0",
				"sclk_usbphy1", "sclk_hdmiphy", "none",
				"sclk_epll", "sclk_vpll" };
PNAME(clkout_left_p4210) = { "sclk_mpll_div_2", "sclk_apll_div_2",
				"div_gdl", "div_gpl" };
PNAME(clkout_right_p4210) = { "sclk_mpll_div_2", "sclk_apll_div_2",
				"div_gdr", "div_gpr" };
PNAME(clkout_top_p4210) = { "fout_epll", "fout_vpll", "sclk_hdmi24m",
				"sclk_usbphy0", "sclk_usbphy1", "sclk_hdmiphy",
				"cdclk0", "cdclk1", "cdclk2", "spdif_extclk",
				"aclk160", "aclk133", "aclk200", "aclk100",
				"sclk_mfc", "sclk_g3d", "sclk_g2d",
				"cam_a_pclk", "cam_b_pclk", "s_rxbyteclkhs0_2l",
				"s_rxbyteclkhs0_4l" };
PNAME(clkout_dmc_p4210) = { "div_dmcd", "div_dmcp", "div_acp_pclk", "div_dmc",
				"div_dphy", "none", "div_pwi" };
PNAME(clkout_cpu_p4210) = { "fout_apll_div_2", "none", "fout_mpll_div_2",
				"none", "arm_clk_div_2", "div_corem0",
				"div_corem1", "div_corem0", "div_atb",
				"div_periph", "div_pclk_dbg", "div_hpm" };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Exynos 4x12-specific parent groups */
PNAME(mout_mpll_user_p4x12) = { "fin_pll", "sclk_mpll", };
PNAME(mout_core_p4x12)	= { "mout_apll", "mout_mpll_user_c", };
<<<<<<< HEAD
=======
PNAME(mout_gdl_p4x12)	= { "mout_mpll_user_l", "sclk_apll", };
PNAME(mout_gdr_p4x12)	= { "mout_mpll_user_r", "sclk_apll", };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
PNAME(sclk_ampll_p4x12)	= { "mout_mpll_user_t", "sclk_apll", };
PNAME(group1_p4x12)	= { "xxti", "xusbxti", "sclk_hdmi24m", "sclk_usbphy0",
				"none",	"sclk_hdmiphy", "mout_mpll_user_t",
				"sclk_epll", "sclk_vpll", };
PNAME(mout_audio0_p4x12) = { "cdclk0", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti",
				"mout_mpll_user_t", "sclk_epll", "sclk_vpll" };
PNAME(mout_audio1_p4x12) = { "cdclk1", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti",
				"mout_mpll_user_t", "sclk_epll", "sclk_vpll", };
PNAME(mout_audio2_p4x12) = { "cdclk2", "none", "sclk_hdmi24m",
				"sclk_usbphy0", "xxti", "xusbxti",
				"mout_mpll_user_t", "sclk_epll", "sclk_vpll", };
PNAME(aclk_p4412)	= { "mout_mpll_user_t", "sclk_apll", };
PNAME(mout_user_aclk400_mcuisp_p4x12) = {"fin_pll", "div_aclk400_mcuisp", };
PNAME(mout_user_aclk200_p4x12) = {"fin_pll", "div_aclk200", };
PNAME(mout_user_aclk266_gps_p4x12) = {"fin_pll", "div_aclk266_gps", };
<<<<<<< HEAD

/* fixed rate clocks generated outside the soc */
struct samsung_fixed_rate_clock exynos4_fixed_rate_ext_clks[] __initdata = {
	FRATE(xxti, "xxti", NULL, CLK_IS_ROOT, 0),
	FRATE(xusbxti, "xusbxti", NULL, CLK_IS_ROOT, 0),
};

/* fixed rate clocks generated inside the soc */
struct samsung_fixed_rate_clock exynos4_fixed_rate_clks[] __initdata = {
	FRATE(none, "sclk_hdmi24m", NULL, CLK_IS_ROOT, 24000000),
	FRATE(none, "sclk_hdmiphy", NULL, CLK_IS_ROOT, 27000000),
	FRATE(none, "sclk_usbphy0", NULL, CLK_IS_ROOT, 48000000),
};

struct samsung_fixed_rate_clock exynos4210_fixed_rate_clks[] __initdata = {
	FRATE(none, "sclk_usbphy1", NULL, CLK_IS_ROOT, 48000000),
};

/* list of mux clocks supported in all exynos4 soc's */
struct samsung_mux_clock exynos4_mux_clks[] __initdata = {
	MUX_F(mout_apll, "mout_apll", mout_apll_p, SRC_CPU, 0, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(none, "mout_hdmi", mout_hdmi_p, SRC_TV, 0, 1),
	MUX(none, "mout_mfc1", sclk_evpll_p, SRC_MFC, 4, 1),
	MUX(none, "mout_mfc", mout_mfc_p, SRC_MFC, 8, 1),
	MUX_F(mout_g3d1, "mout_g3d1", sclk_evpll_p, SRC_G3D, 4, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX_F(mout_g3d, "mout_g3d", mout_g3d_p, SRC_G3D, 8, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(none, "mout_spdif", mout_spdif_p, SRC_PERIL1, 8, 2),
	MUX(none, "mout_onenand1", mout_onenand1_p, SRC_TOP0, 0, 1),
	MUX_A(sclk_epll, "sclk_epll", mout_epll_p, SRC_TOP0, 4, 1, "sclk_epll"),
	MUX(none, "mout_onenand", mout_onenand_p, SRC_TOP0, 28, 1),
};

/* list of mux clocks supported in exynos4210 soc */
struct samsung_mux_clock exynos4210_mux_clks[] __initdata = {
	MUX(none, "mout_aclk200", sclk_ampll_p4210, SRC_TOP0, 12, 1),
	MUX(none, "mout_aclk100", sclk_ampll_p4210, SRC_TOP0, 16, 1),
	MUX(none, "mout_aclk160", sclk_ampll_p4210, SRC_TOP0, 20, 1),
	MUX(none, "mout_aclk133", sclk_ampll_p4210, SRC_TOP0, 24, 1),
	MUX(none, "mout_vpllsrc", mout_vpllsrc_p, SRC_TOP1, 0, 1),
	MUX(none, "mout_mixer", mout_mixer_p4210, SRC_TV, 4, 1),
	MUX(none, "mout_dac", mout_dac_p4210, SRC_TV, 8, 1),
	MUX(none, "mout_g2d0", sclk_ampll_p4210, E4210_SRC_IMAGE, 0, 1),
	MUX(none, "mout_g2d1", sclk_evpll_p, E4210_SRC_IMAGE, 4, 1),
	MUX(none, "mout_g2d", mout_g2d_p, E4210_SRC_IMAGE, 8, 1),
	MUX(none, "mout_fimd1", group1_p4210, E4210_SRC_LCD1, 0, 4),
	MUX(none, "mout_mipi1", group1_p4210, E4210_SRC_LCD1, 12, 4),
	MUX_A(sclk_mpll, "sclk_mpll", mout_mpll_p, SRC_CPU, 8, 1, "sclk_mpll"),
	MUX_A(mout_core, "mout_core", mout_core_p4210,
			SRC_CPU, 16, 1, "mout_core"),
	MUX_A(sclk_vpll, "sclk_vpll", sclk_vpll_p4210,
			SRC_TOP0, 8, 1, "sclk_vpll"),
	MUX(mout_fimc0, "mout_fimc0", group1_p4210, SRC_CAM, 0, 4),
	MUX(mout_fimc1, "mout_fimc1", group1_p4210, SRC_CAM, 4, 4),
	MUX(mout_fimc2, "mout_fimc2", group1_p4210, SRC_CAM, 8, 4),
	MUX(mout_fimc3, "mout_fimc3", group1_p4210, SRC_CAM, 12, 4),
	MUX(mout_cam0, "mout_cam0", group1_p4210, SRC_CAM, 16, 4),
	MUX(mout_cam1, "mout_cam1", group1_p4210, SRC_CAM, 20, 4),
	MUX(mout_csis0, "mout_csis0", group1_p4210, SRC_CAM, 24, 4),
	MUX(mout_csis1, "mout_csis1", group1_p4210, SRC_CAM, 28, 4),
	MUX(none, "mout_mfc0", sclk_ampll_p4210, SRC_MFC, 0, 1),
	MUX_F(mout_g3d0, "mout_g3d0", sclk_ampll_p4210, SRC_G3D, 0, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(none, "mout_fimd0", group1_p4210, SRC_LCD0, 0, 4),
	MUX(none, "mout_mipi0", group1_p4210, SRC_LCD0, 12, 4),
	MUX(none, "mout_audio0", mout_audio0_p4210, SRC_MAUDIO, 0, 4),
	MUX(none, "mout_mmc0", group1_p4210, SRC_FSYS, 0, 4),
	MUX(none, "mout_mmc1", group1_p4210, SRC_FSYS, 4, 4),
	MUX(none, "mout_mmc2", group1_p4210, SRC_FSYS, 8, 4),
	MUX(none, "mout_mmc3", group1_p4210, SRC_FSYS, 12, 4),
	MUX(none, "mout_mmc4", group1_p4210, SRC_FSYS, 16, 4),
	MUX(none, "mout_sata", sclk_ampll_p4210, SRC_FSYS, 24, 1),
	MUX(none, "mout_uart0", group1_p4210, SRC_PERIL0, 0, 4),
	MUX(none, "mout_uart1", group1_p4210, SRC_PERIL0, 4, 4),
	MUX(none, "mout_uart2", group1_p4210, SRC_PERIL0, 8, 4),
	MUX(none, "mout_uart3", group1_p4210, SRC_PERIL0, 12, 4),
	MUX(none, "mout_uart4", group1_p4210, SRC_PERIL0, 16, 4),
	MUX(none, "mout_audio1", mout_audio1_p4210, SRC_PERIL1, 0, 4),
	MUX(none, "mout_audio2", mout_audio2_p4210, SRC_PERIL1, 4, 4),
	MUX(none, "mout_spi0", group1_p4210, SRC_PERIL1, 16, 4),
	MUX(none, "mout_spi1", group1_p4210, SRC_PERIL1, 20, 4),
	MUX(none, "mout_spi2", group1_p4210, SRC_PERIL1, 24, 4),
};

/* list of mux clocks supported in exynos4x12 soc */
struct samsung_mux_clock exynos4x12_mux_clks[] __initdata = {
	MUX(mout_mpll_user_c, "mout_mpll_user_c", mout_mpll_user_p4x12,
			SRC_CPU, 24, 1),
	MUX(none, "mout_aclk266_gps", aclk_p4412, SRC_TOP1, 4, 1),
	MUX(none, "mout_aclk400_mcuisp", aclk_p4412, SRC_TOP1, 8, 1),
	MUX(mout_mpll_user_t, "mout_mpll_user_t", mout_mpll_user_p4x12,
			SRC_TOP1, 12, 1),
	MUX(none, "mout_user_aclk266_gps", mout_user_aclk266_gps_p4x12,
			SRC_TOP1, 16, 1),
	MUX(aclk200, "aclk200", mout_user_aclk200_p4x12, SRC_TOP1, 20, 1),
	MUX(aclk400_mcuisp, "aclk400_mcuisp", mout_user_aclk400_mcuisp_p4x12,
			SRC_TOP1, 24, 1),
	MUX(none, "mout_aclk200", aclk_p4412, SRC_TOP0, 12, 1),
	MUX(none, "mout_aclk100", aclk_p4412, SRC_TOP0, 16, 1),
	MUX(none, "mout_aclk160", aclk_p4412, SRC_TOP0, 20, 1),
	MUX(none, "mout_aclk133", aclk_p4412, SRC_TOP0, 24, 1),
	MUX(none, "mout_mdnie0", group1_p4x12, SRC_LCD0, 4, 4),
	MUX(none, "mout_mdnie_pwm0", group1_p4x12, SRC_LCD0, 8, 4),
	MUX(none, "mout_sata", sclk_ampll_p4x12, SRC_FSYS, 24, 1),
	MUX(none, "mout_jpeg0", sclk_ampll_p4x12, E4X12_SRC_CAM1, 0, 1),
	MUX(none, "mout_jpeg1", sclk_evpll_p, E4X12_SRC_CAM1, 4, 1),
	MUX(none, "mout_jpeg", mout_jpeg_p, E4X12_SRC_CAM1, 8, 1),
	MUX_A(sclk_mpll, "sclk_mpll", mout_mpll_p,
			SRC_DMC, 12, 1, "sclk_mpll"),
	MUX_A(sclk_vpll, "sclk_vpll", mout_vpll_p,
			SRC_TOP0, 8, 1, "sclk_vpll"),
	MUX(mout_core, "mout_core", mout_core_p4x12, SRC_CPU, 16, 1),
	MUX(mout_fimc0, "mout_fimc0", group1_p4x12, SRC_CAM, 0, 4),
	MUX(mout_fimc1, "mout_fimc1", group1_p4x12, SRC_CAM, 4, 4),
	MUX(mout_fimc2, "mout_fimc2", group1_p4x12, SRC_CAM, 8, 4),
	MUX(mout_fimc3, "mout_fimc3", group1_p4x12, SRC_CAM, 12, 4),
	MUX(mout_cam0, "mout_cam0", group1_p4x12, SRC_CAM, 16, 4),
	MUX(mout_cam1, "mout_cam1", group1_p4x12, SRC_CAM, 20, 4),
	MUX(mout_csis0, "mout_csis0", group1_p4x12, SRC_CAM, 24, 4),
	MUX(mout_csis1, "mout_csis1", group1_p4x12, SRC_CAM, 28, 4),
	MUX(none, "mout_mfc0", sclk_ampll_p4x12, SRC_MFC, 0, 1),
	MUX_F(mout_g3d0, "mout_g3d0", sclk_ampll_p4x12, SRC_G3D, 0, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(none, "mout_fimd0", group1_p4x12, SRC_LCD0, 0, 4),
	MUX(none, "mout_mipi0", group1_p4x12, SRC_LCD0, 12, 4),
	MUX(none, "mout_audio0", mout_audio0_p4x12, SRC_MAUDIO, 0, 4),
	MUX(none, "mout_mmc0", group1_p4x12, SRC_FSYS, 0, 4),
	MUX(none, "mout_mmc1", group1_p4x12, SRC_FSYS, 4, 4),
	MUX(none, "mout_mmc2", group1_p4x12, SRC_FSYS, 8, 4),
	MUX(none, "mout_mmc3", group1_p4x12, SRC_FSYS, 12, 4),
	MUX(none, "mout_mmc4", group1_p4x12, SRC_FSYS, 16, 4),
	MUX(none, "mout_mipihsi", aclk_p4412, SRC_FSYS, 24, 1),
	MUX(none, "mout_uart0", group1_p4x12, SRC_PERIL0, 0, 4),
	MUX(none, "mout_uart1", group1_p4x12, SRC_PERIL0, 4, 4),
	MUX(none, "mout_uart2", group1_p4x12, SRC_PERIL0, 8, 4),
	MUX(none, "mout_uart3", group1_p4x12, SRC_PERIL0, 12, 4),
	MUX(none, "mout_uart4", group1_p4x12, SRC_PERIL0, 16, 4),
	MUX(none, "mout_audio1", mout_audio1_p4x12, SRC_PERIL1, 0, 4),
	MUX(none, "mout_audio2", mout_audio2_p4x12, SRC_PERIL1, 4, 4),
	MUX(none, "mout_spi0", group1_p4x12, SRC_PERIL1, 16, 4),
	MUX(none, "mout_spi1", group1_p4x12, SRC_PERIL1, 20, 4),
	MUX(none, "mout_spi2", group1_p4x12, SRC_PERIL1, 24, 4),
	MUX(none, "mout_pwm_isp", group1_p4x12, E4X12_SRC_ISP, 0, 4),
	MUX(none, "mout_spi0_isp", group1_p4x12, E4X12_SRC_ISP, 4, 4),
	MUX(none, "mout_spi1_isp", group1_p4x12, E4X12_SRC_ISP, 8, 4),
	MUX(none, "mout_uart_isp", group1_p4x12, E4X12_SRC_ISP, 12, 4),
};

/* list of divider clocks supported in all exynos4 soc's */
struct samsung_div_clock exynos4_div_clks[] __initdata = {
	DIV(none, "div_core", "mout_core", DIV_CPU0, 0, 3),
	DIV(none, "div_core2", "div_core", DIV_CPU0, 28, 3),
	DIV(none, "div_fimc0", "mout_fimc0", DIV_CAM, 0, 4),
	DIV(none, "div_fimc1", "mout_fimc1", DIV_CAM, 4, 4),
	DIV(none, "div_fimc2", "mout_fimc2", DIV_CAM, 8, 4),
	DIV(none, "div_fimc3", "mout_fimc3", DIV_CAM, 12, 4),
	DIV(none, "div_cam0", "mout_cam0", DIV_CAM, 16, 4),
	DIV(none, "div_cam1", "mout_cam1", DIV_CAM, 20, 4),
	DIV(none, "div_csis0", "mout_csis0", DIV_CAM, 24, 4),
	DIV(none, "div_csis1", "mout_csis1", DIV_CAM, 28, 4),
	DIV(sclk_mfc, "sclk_mfc", "mout_mfc", DIV_MFC, 0, 4),
	DIV_F(none, "div_g3d", "mout_g3d", DIV_G3D, 0, 4,
			CLK_SET_RATE_PARENT, 0),
	DIV(none, "div_fimd0", "mout_fimd0", DIV_LCD0, 0, 4),
	DIV(none, "div_mipi0", "mout_mipi0", DIV_LCD0, 16, 4),
	DIV(none, "div_audio0", "mout_audio0", DIV_MAUDIO, 0, 4),
	DIV(sclk_pcm0, "sclk_pcm0", "sclk_audio0", DIV_MAUDIO, 4, 8),
	DIV(none, "div_mmc0", "mout_mmc0", DIV_FSYS1, 0, 4),
	DIV(none, "div_mmc1", "mout_mmc1", DIV_FSYS1, 16, 4),
	DIV(none, "div_mmc2", "mout_mmc2", DIV_FSYS2, 0, 4),
	DIV(none, "div_mmc3", "mout_mmc3", DIV_FSYS2, 16, 4),
	DIV(sclk_pixel, "sclk_pixel", "sclk_vpll", DIV_TV, 0, 4),
	DIV(aclk100, "aclk100", "mout_aclk100", DIV_TOP, 4, 4),
	DIV(aclk160, "aclk160", "mout_aclk160", DIV_TOP, 8, 3),
	DIV(aclk133, "aclk133", "mout_aclk133", DIV_TOP, 12, 3),
	DIV(none, "div_onenand", "mout_onenand1", DIV_TOP, 16, 3),
	DIV(sclk_slimbus, "sclk_slimbus", "sclk_epll", DIV_PERIL3, 4, 4),
	DIV(sclk_pcm1, "sclk_pcm1", "sclk_audio1", DIV_PERIL4, 4, 8),
	DIV(sclk_pcm2, "sclk_pcm2", "sclk_audio2", DIV_PERIL4, 20, 8),
	DIV(sclk_i2s1, "sclk_i2s1", "sclk_audio1", DIV_PERIL5, 0, 6),
	DIV(sclk_i2s2, "sclk_i2s2", "sclk_audio2", DIV_PERIL5, 8, 6),
	DIV(none, "div_mmc4", "mout_mmc4", DIV_FSYS3, 0, 4),
	DIV(none, "div_mmc_pre4", "div_mmc4", DIV_FSYS3, 8, 8),
	DIV(none, "div_uart0", "mout_uart0", DIV_PERIL0, 0, 4),
	DIV(none, "div_uart1", "mout_uart1", DIV_PERIL0, 4, 4),
	DIV(none, "div_uart2", "mout_uart2", DIV_PERIL0, 8, 4),
	DIV(none, "div_uart3", "mout_uart3", DIV_PERIL0, 12, 4),
	DIV(none, "div_uart4", "mout_uart4", DIV_PERIL0, 16, 4),
	DIV(none, "div_spi0", "mout_spi0", DIV_PERIL1, 0, 4),
	DIV(none, "div_spi_pre0", "div_spi0", DIV_PERIL1, 8, 8),
	DIV(none, "div_spi1", "mout_spi1", DIV_PERIL1, 16, 4),
	DIV(none, "div_spi_pre1", "div_spi1", DIV_PERIL1, 24, 8),
	DIV(none, "div_spi2", "mout_spi2", DIV_PERIL2, 0, 4),
	DIV(none, "div_spi_pre2", "div_spi2", DIV_PERIL2, 8, 8),
	DIV(none, "div_audio1", "mout_audio1", DIV_PERIL4, 0, 4),
	DIV(none, "div_audio2", "mout_audio2", DIV_PERIL4, 16, 4),
	DIV_A(arm_clk, "arm_clk", "div_core2", DIV_CPU0, 28, 3, "arm_clk"),
	DIV_A(sclk_apll, "sclk_apll", "mout_apll",
			DIV_CPU0, 24, 3, "sclk_apll"),
	DIV_F(none, "div_mipi_pre0", "div_mipi0", DIV_LCD0, 20, 4,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(none, "div_mmc_pre0", "div_mmc0", DIV_FSYS1, 8, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(none, "div_mmc_pre1", "div_mmc1", DIV_FSYS1, 24, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(none, "div_mmc_pre2", "div_mmc2", DIV_FSYS2, 8, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(none, "div_mmc_pre3", "div_mmc3", DIV_FSYS2, 24, 8,
			CLK_SET_RATE_PARENT, 0),
};

/* list of divider clocks supported in exynos4210 soc */
struct samsung_div_clock exynos4210_div_clks[] __initdata = {
	DIV(aclk200, "aclk200", "mout_aclk200", DIV_TOP, 0, 3),
	DIV(none, "div_g2d", "mout_g2d", DIV_IMAGE, 0, 4),
	DIV(none, "div_fimd1", "mout_fimd1", E4210_DIV_LCD1, 0, 4),
	DIV(none, "div_mipi1", "mout_mipi1", E4210_DIV_LCD1, 16, 4),
	DIV(none, "div_sata", "mout_sata", DIV_FSYS0, 20, 4),
	DIV_F(none, "div_mipi_pre1", "div_mipi1", E4210_DIV_LCD1, 20, 4,
=======
PNAME(mout_pwi_p4x12) = { "xxti", "xusbxti", "sclk_hdmi24m", "sclk_usbphy0",
				"none", "sclk_hdmiphy", "sclk_mpll",
				"sclk_epll", "sclk_vpll" };
PNAME(clkout_left_p4x12) = { "sclk_mpll_user_l_div_2", "sclk_apll_div_2",
				"div_gdl", "div_gpl" };
PNAME(clkout_right_p4x12) = { "sclk_mpll_user_r_div_2", "sclk_apll_div_2",
				"div_gdr", "div_gpr" };
PNAME(clkout_top_p4x12) = { "fout_epll", "fout_vpll", "sclk_hdmi24m",
				"sclk_usbphy0", "none", "sclk_hdmiphy",
				"cdclk0", "cdclk1", "cdclk2", "spdif_extclk",
				"aclk160", "aclk133", "aclk200", "aclk100",
				"sclk_mfc", "sclk_g3d", "aclk400_mcuisp",
				"cam_a_pclk", "cam_b_pclk", "s_rxbyteclkhs0_2l",
				"s_rxbyteclkhs0_4l", "rx_half_byte_clk_csis0",
				"rx_half_byte_clk_csis1", "div_jpeg",
				"sclk_pwm_isp", "sclk_spi0_isp",
				"sclk_spi1_isp", "sclk_uart_isp",
				"sclk_mipihsi", "sclk_hdmi", "sclk_fimd0",
				"sclk_pcm0" };
PNAME(clkout_dmc_p4x12) = { "div_dmcd", "div_dmcp", "aclk_acp", "div_acp_pclk",
				"div_dmc", "div_dphy", "fout_mpll_div_2",
				"div_pwi", "none", "div_c2c", "div_c2c_aclk" };
PNAME(clkout_cpu_p4x12) = { "fout_apll_div_2", "none", "none", "none",
				"arm_clk_div_2", "div_corem0", "div_corem1",
				"div_cores", "div_atb", "div_periph",
				"div_pclk_dbg", "div_hpm" };

/* fixed rate clocks generated outside the soc */
static struct samsung_fixed_rate_clock exynos4_fixed_rate_ext_clks[] __initdata = {
	FRATE(CLK_XXTI, "xxti", NULL, 0, 0),
	FRATE(CLK_XUSBXTI, "xusbxti", NULL, 0, 0),
};

/* fixed rate clocks generated inside the soc */
static const struct samsung_fixed_rate_clock exynos4_fixed_rate_clks[] __initconst = {
	FRATE(0, "sclk_hdmi24m", NULL, 0, 24000000),
	FRATE(CLK_SCLK_HDMIPHY, "sclk_hdmiphy", "hdmi", 0, 27000000),
	FRATE(0, "sclk_usbphy0", NULL, 0, 48000000),
};

static const struct samsung_fixed_rate_clock exynos4210_fixed_rate_clks[] __initconst = {
	FRATE(0, "sclk_usbphy1", NULL, 0, 48000000),
};

static const struct samsung_fixed_factor_clock exynos4_fixed_factor_clks[] __initconst = {
	FFACTOR(0, "sclk_apll_div_2", "sclk_apll", 1, 2, 0),
	FFACTOR(0, "fout_mpll_div_2", "fout_mpll", 1, 2, 0),
	FFACTOR(0, "fout_apll_div_2", "fout_apll", 1, 2, 0),
	FFACTOR(0, "arm_clk_div_2", "div_core2", 1, 2, 0),
};

static const struct samsung_fixed_factor_clock exynos4210_fixed_factor_clks[] __initconst = {
	FFACTOR(0, "sclk_mpll_div_2", "sclk_mpll", 1, 2, 0),
};

static const struct samsung_fixed_factor_clock exynos4x12_fixed_factor_clks[] __initconst = {
	FFACTOR(0, "sclk_mpll_user_l_div_2", "mout_mpll_user_l", 1, 2, 0),
	FFACTOR(0, "sclk_mpll_user_r_div_2", "mout_mpll_user_r", 1, 2, 0),
	FFACTOR(0, "sclk_mpll_user_t_div_2", "mout_mpll_user_t", 1, 2, 0),
	FFACTOR(0, "sclk_mpll_user_c_div_2", "mout_mpll_user_c", 1, 2, 0),
};

/* list of mux clocks supported in all exynos4 soc's */
static const struct samsung_mux_clock exynos4_mux_clks[] __initconst = {
	MUX_FA(CLK_MOUT_APLL, "mout_apll", mout_apll_p, SRC_CPU, 0, 1,
			CLK_SET_RATE_PARENT | CLK_RECALC_NEW_RATES, 0,
			"mout_apll"),
	MUX(CLK_MOUT_HDMI, "mout_hdmi", mout_hdmi_p, SRC_TV, 0, 1),
	MUX(0, "mout_mfc1", sclk_evpll_p, SRC_MFC, 4, 1),
	MUX(0, "mout_mfc", mout_mfc_p, SRC_MFC, 8, 1),
	MUX_F(CLK_MOUT_G3D1, "mout_g3d1", sclk_evpll_p, SRC_G3D, 4, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX_F(CLK_MOUT_G3D, "mout_g3d", mout_g3d_p, SRC_G3D, 8, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(0, "mout_spdif", mout_spdif_p, SRC_PERIL1, 8, 2),
	MUX(0, "mout_onenand1", mout_onenand1_p, SRC_TOP0, 0, 1),
	MUX(CLK_SCLK_EPLL, "sclk_epll", mout_epll_p, SRC_TOP0, 4, 1),
	MUX(0, "mout_onenand", mout_onenand_p, SRC_TOP0, 28, 1),

	MUX(0, "mout_dmc_bus", sclk_ampll_p4210, SRC_DMC, 4, 1),
	MUX(0, "mout_dphy", sclk_ampll_p4210, SRC_DMC, 8, 1),
};

/* list of mux clocks supported in exynos4210 soc */
static const struct samsung_mux_clock exynos4210_mux_early[] __initconst = {
	MUX(0, "mout_vpllsrc", mout_vpllsrc_p, SRC_TOP1, 0, 1),
};

static const struct samsung_mux_clock exynos4210_mux_clks[] __initconst = {
	MUX(0, "mout_gdl", sclk_ampll_p4210, SRC_LEFTBUS, 0, 1),
	MUX(0, "mout_clkout_leftbus", clkout_left_p4210,
			CLKOUT_CMU_LEFTBUS, 0, 5),

	MUX(0, "mout_gdr", sclk_ampll_p4210, SRC_RIGHTBUS, 0, 1),
	MUX(0, "mout_clkout_rightbus", clkout_right_p4210,
			CLKOUT_CMU_RIGHTBUS, 0, 5),

	MUX(0, "mout_aclk200", sclk_ampll_p4210, SRC_TOP0, 12, 1),
	MUX(0, "mout_aclk100", sclk_ampll_p4210, SRC_TOP0, 16, 1),
	MUX(0, "mout_aclk160", sclk_ampll_p4210, SRC_TOP0, 20, 1),
	MUX(0, "mout_aclk133", sclk_ampll_p4210, SRC_TOP0, 24, 1),
	MUX(CLK_MOUT_MIXER, "mout_mixer", mout_mixer_p4210, SRC_TV, 4, 1),
	MUX(0, "mout_dac", mout_dac_p4210, SRC_TV, 8, 1),
	MUX(0, "mout_g2d0", sclk_ampll_p4210, E4210_SRC_IMAGE, 0, 1),
	MUX(0, "mout_g2d1", sclk_evpll_p, E4210_SRC_IMAGE, 4, 1),
	MUX(0, "mout_g2d", mout_g2d_p, E4210_SRC_IMAGE, 8, 1),
	MUX(0, "mout_fimd1", group1_p4210, E4210_SRC_LCD1, 0, 4),
	MUX(0, "mout_mipi1", group1_p4210, E4210_SRC_LCD1, 12, 4),
	MUX(CLK_SCLK_MPLL, "sclk_mpll", mout_mpll_p, SRC_CPU, 8, 1),
	MUX(CLK_MOUT_CORE, "mout_core", mout_core_p4210, SRC_CPU, 16, 1),
	MUX(0, "mout_hpm", mout_core_p4210, SRC_CPU, 20, 1),
	MUX(CLK_SCLK_VPLL, "sclk_vpll", sclk_vpll_p4210, SRC_TOP0, 8, 1),
	MUX(CLK_MOUT_FIMC0, "mout_fimc0", group1_p4210, SRC_CAM, 0, 4),
	MUX(CLK_MOUT_FIMC1, "mout_fimc1", group1_p4210, SRC_CAM, 4, 4),
	MUX(CLK_MOUT_FIMC2, "mout_fimc2", group1_p4210, SRC_CAM, 8, 4),
	MUX(CLK_MOUT_FIMC3, "mout_fimc3", group1_p4210, SRC_CAM, 12, 4),
	MUX(CLK_MOUT_CAM0, "mout_cam0", group1_p4210, SRC_CAM, 16, 4),
	MUX(CLK_MOUT_CAM1, "mout_cam1", group1_p4210, SRC_CAM, 20, 4),
	MUX(CLK_MOUT_CSIS0, "mout_csis0", group1_p4210, SRC_CAM, 24, 4),
	MUX(CLK_MOUT_CSIS1, "mout_csis1", group1_p4210, SRC_CAM, 28, 4),
	MUX(0, "mout_mfc0", sclk_ampll_p4210, SRC_MFC, 0, 1),
	MUX_F(CLK_MOUT_G3D0, "mout_g3d0", sclk_ampll_p4210, SRC_G3D, 0, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(0, "mout_fimd0", group1_p4210, SRC_LCD0, 0, 4),
	MUX(0, "mout_mipi0", group1_p4210, SRC_LCD0, 12, 4),
	MUX(0, "mout_audio0", mout_audio0_p4210, SRC_MAUDIO, 0, 4),
	MUX(0, "mout_mmc0", group1_p4210, SRC_FSYS, 0, 4),
	MUX(0, "mout_mmc1", group1_p4210, SRC_FSYS, 4, 4),
	MUX(0, "mout_mmc2", group1_p4210, SRC_FSYS, 8, 4),
	MUX(0, "mout_mmc3", group1_p4210, SRC_FSYS, 12, 4),
	MUX(0, "mout_mmc4", group1_p4210, SRC_FSYS, 16, 4),
	MUX(0, "mout_sata", sclk_ampll_p4210, SRC_FSYS, 24, 1),
	MUX(0, "mout_uart0", group1_p4210, SRC_PERIL0, 0, 4),
	MUX(0, "mout_uart1", group1_p4210, SRC_PERIL0, 4, 4),
	MUX(0, "mout_uart2", group1_p4210, SRC_PERIL0, 8, 4),
	MUX(0, "mout_uart3", group1_p4210, SRC_PERIL0, 12, 4),
	MUX(0, "mout_uart4", group1_p4210, SRC_PERIL0, 16, 4),
	MUX(0, "mout_audio1", mout_audio1_p4210, SRC_PERIL1, 0, 4),
	MUX(0, "mout_audio2", mout_audio2_p4210, SRC_PERIL1, 4, 4),
	MUX(0, "mout_spi0", group1_p4210, SRC_PERIL1, 16, 4),
	MUX(0, "mout_spi1", group1_p4210, SRC_PERIL1, 20, 4),
	MUX(0, "mout_spi2", group1_p4210, SRC_PERIL1, 24, 4),
	MUX(0, "mout_clkout_top", clkout_top_p4210, CLKOUT_CMU_TOP, 0, 5),

	MUX(0, "mout_pwi", mout_pwi_p4210, SRC_DMC, 16, 4),
	MUX(0, "mout_clkout_dmc", clkout_dmc_p4210, CLKOUT_CMU_DMC, 0, 5),

	MUX(0, "mout_clkout_cpu", clkout_cpu_p4210, CLKOUT_CMU_CPU, 0, 5),
};

/* list of mux clocks supported in exynos4x12 soc */
static const struct samsung_mux_clock exynos4x12_mux_clks[] __initconst = {
	MUX(0, "mout_mpll_user_l", mout_mpll_p, SRC_LEFTBUS, 4, 1),
	MUX(0, "mout_gdl", mout_gdl_p4x12, SRC_LEFTBUS, 0, 1),
	MUX(0, "mout_clkout_leftbus", clkout_left_p4x12,
			CLKOUT_CMU_LEFTBUS, 0, 5),

	MUX(0, "mout_mpll_user_r", mout_mpll_p, SRC_RIGHTBUS, 4, 1),
	MUX(0, "mout_gdr", mout_gdr_p4x12, SRC_RIGHTBUS, 0, 1),
	MUX(0, "mout_clkout_rightbus", clkout_right_p4x12,
			CLKOUT_CMU_RIGHTBUS, 0, 5),

	MUX(CLK_MOUT_MPLL_USER_C, "mout_mpll_user_c", mout_mpll_user_p4x12,
			SRC_CPU, 24, 1),
	MUX(0, "mout_clkout_cpu", clkout_cpu_p4x12, CLKOUT_CMU_CPU, 0, 5),

	MUX(0, "mout_aclk266_gps", aclk_p4412, SRC_TOP1, 4, 1),
	MUX(0, "mout_aclk400_mcuisp", aclk_p4412, SRC_TOP1, 8, 1),
	MUX(CLK_MOUT_MPLL_USER_T, "mout_mpll_user_t", mout_mpll_user_p4x12,
			SRC_TOP1, 12, 1),
	MUX(0, "mout_user_aclk266_gps", mout_user_aclk266_gps_p4x12,
			SRC_TOP1, 16, 1),
	MUX(CLK_ACLK200, "aclk200", mout_user_aclk200_p4x12, SRC_TOP1, 20, 1),
	MUX(CLK_ACLK400_MCUISP, "aclk400_mcuisp",
		mout_user_aclk400_mcuisp_p4x12, SRC_TOP1, 24, 1),
	MUX(0, "mout_aclk200", aclk_p4412, SRC_TOP0, 12, 1),
	MUX(0, "mout_aclk100", aclk_p4412, SRC_TOP0, 16, 1),
	MUX(0, "mout_aclk160", aclk_p4412, SRC_TOP0, 20, 1),
	MUX(0, "mout_aclk133", aclk_p4412, SRC_TOP0, 24, 1),
	MUX(0, "mout_mdnie0", group1_p4x12, SRC_LCD0, 4, 4),
	MUX(0, "mout_mdnie_pwm0", group1_p4x12, SRC_LCD0, 8, 4),
	MUX(0, "mout_sata", sclk_ampll_p4x12, SRC_FSYS, 24, 1),
	MUX(0, "mout_jpeg0", sclk_ampll_p4x12, E4X12_SRC_CAM1, 0, 1),
	MUX(0, "mout_jpeg1", sclk_evpll_p, E4X12_SRC_CAM1, 4, 1),
	MUX(0, "mout_jpeg", mout_jpeg_p, E4X12_SRC_CAM1, 8, 1),
	MUX(CLK_SCLK_MPLL, "sclk_mpll", mout_mpll_p, SRC_DMC, 12, 1),
	MUX(CLK_SCLK_VPLL, "sclk_vpll", mout_vpll_p, SRC_TOP0, 8, 1),
	MUX(CLK_MOUT_CORE, "mout_core", mout_core_p4x12, SRC_CPU, 16, 1),
	MUX(0, "mout_hpm", mout_core_p4x12, SRC_CPU, 20, 1),
	MUX(CLK_MOUT_FIMC0, "mout_fimc0", group1_p4x12, SRC_CAM, 0, 4),
	MUX(CLK_MOUT_FIMC1, "mout_fimc1", group1_p4x12, SRC_CAM, 4, 4),
	MUX(CLK_MOUT_FIMC2, "mout_fimc2", group1_p4x12, SRC_CAM, 8, 4),
	MUX(CLK_MOUT_FIMC3, "mout_fimc3", group1_p4x12, SRC_CAM, 12, 4),
	MUX(CLK_MOUT_CAM0, "mout_cam0", group1_p4x12, SRC_CAM, 16, 4),
	MUX(CLK_MOUT_CAM1, "mout_cam1", group1_p4x12, SRC_CAM, 20, 4),
	MUX(CLK_MOUT_CSIS0, "mout_csis0", group1_p4x12, SRC_CAM, 24, 4),
	MUX(CLK_MOUT_CSIS1, "mout_csis1", group1_p4x12, SRC_CAM, 28, 4),
	MUX(0, "mout_mfc0", sclk_ampll_p4x12, SRC_MFC, 0, 1),
	MUX_F(CLK_MOUT_G3D0, "mout_g3d0", sclk_ampll_p4x12, SRC_G3D, 0, 1,
			CLK_SET_RATE_PARENT, 0),
	MUX(0, "mout_fimd0", group1_p4x12, SRC_LCD0, 0, 4),
	MUX(0, "mout_mipi0", group1_p4x12, SRC_LCD0, 12, 4),
	MUX(0, "mout_audio0", mout_audio0_p4x12, SRC_MAUDIO, 0, 4),
	MUX(0, "mout_mmc0", group1_p4x12, SRC_FSYS, 0, 4),
	MUX(0, "mout_mmc1", group1_p4x12, SRC_FSYS, 4, 4),
	MUX(0, "mout_mmc2", group1_p4x12, SRC_FSYS, 8, 4),
	MUX(0, "mout_mmc3", group1_p4x12, SRC_FSYS, 12, 4),
	MUX(0, "mout_mmc4", group1_p4x12, SRC_FSYS, 16, 4),
	MUX(0, "mout_mipihsi", aclk_p4412, SRC_FSYS, 24, 1),
	MUX(0, "mout_uart0", group1_p4x12, SRC_PERIL0, 0, 4),
	MUX(0, "mout_uart1", group1_p4x12, SRC_PERIL0, 4, 4),
	MUX(0, "mout_uart2", group1_p4x12, SRC_PERIL0, 8, 4),
	MUX(0, "mout_uart3", group1_p4x12, SRC_PERIL0, 12, 4),
	MUX(0, "mout_uart4", group1_p4x12, SRC_PERIL0, 16, 4),
	MUX(0, "mout_audio1", mout_audio1_p4x12, SRC_PERIL1, 0, 4),
	MUX(0, "mout_audio2", mout_audio2_p4x12, SRC_PERIL1, 4, 4),
	MUX(0, "mout_spi0", group1_p4x12, SRC_PERIL1, 16, 4),
	MUX(0, "mout_spi1", group1_p4x12, SRC_PERIL1, 20, 4),
	MUX(0, "mout_spi2", group1_p4x12, SRC_PERIL1, 24, 4),
	MUX(0, "mout_pwm_isp", group1_p4x12, E4X12_SRC_ISP, 0, 4),
	MUX(0, "mout_spi0_isp", group1_p4x12, E4X12_SRC_ISP, 4, 4),
	MUX(0, "mout_spi1_isp", group1_p4x12, E4X12_SRC_ISP, 8, 4),
	MUX(0, "mout_uart_isp", group1_p4x12, E4X12_SRC_ISP, 12, 4),
	MUX(0, "mout_clkout_top", clkout_top_p4x12, CLKOUT_CMU_TOP, 0, 5),

	MUX(0, "mout_c2c", sclk_ampll_p4210, SRC_DMC, 0, 1),
	MUX(0, "mout_pwi", mout_pwi_p4x12, SRC_DMC, 16, 4),
	MUX(0, "mout_g2d0", sclk_ampll_p4210, SRC_DMC, 20, 1),
	MUX(0, "mout_g2d1", sclk_evpll_p, SRC_DMC, 24, 1),
	MUX(0, "mout_g2d", mout_g2d_p, SRC_DMC, 28, 1),
	MUX(0, "mout_clkout_dmc", clkout_dmc_p4x12, CLKOUT_CMU_DMC, 0, 5),
};

/* list of divider clocks supported in all exynos4 soc's */
static const struct samsung_div_clock exynos4_div_clks[] __initconst = {
	DIV(CLK_DIV_GDL, "div_gdl", "mout_gdl", DIV_LEFTBUS, 0, 3),
	DIV(0, "div_gpl", "div_gdl", DIV_LEFTBUS, 4, 3),
	DIV(0, "div_clkout_leftbus", "mout_clkout_leftbus",
			CLKOUT_CMU_LEFTBUS, 8, 6),

	DIV(CLK_DIV_GDR, "div_gdr", "mout_gdr", DIV_RIGHTBUS, 0, 3),
	DIV(0, "div_gpr", "div_gdr", DIV_RIGHTBUS, 4, 3),
	DIV(0, "div_clkout_rightbus", "mout_clkout_rightbus",
			CLKOUT_CMU_RIGHTBUS, 8, 6),

	DIV(0, "div_core", "mout_core", DIV_CPU0, 0, 3),
	DIV(0, "div_corem0", "div_core2", DIV_CPU0, 4, 3),
	DIV(0, "div_corem1", "div_core2", DIV_CPU0, 8, 3),
	DIV(0, "div_periph", "div_core2", DIV_CPU0, 12, 3),
	DIV(0, "div_atb", "mout_core", DIV_CPU0, 16, 3),
	DIV(0, "div_pclk_dbg", "div_atb", DIV_CPU0, 20, 3),
	DIV(CLK_ARM_CLK, "div_core2", "div_core", DIV_CPU0, 28, 3),
	DIV(0, "div_copy", "mout_hpm", DIV_CPU1, 0, 3),
	DIV(0, "div_hpm", "div_copy", DIV_CPU1, 4, 3),
	DIV(0, "div_clkout_cpu", "mout_clkout_cpu", CLKOUT_CMU_CPU, 8, 6),

	DIV(0, "div_fimc0", "mout_fimc0", DIV_CAM, 0, 4),
	DIV(0, "div_fimc1", "mout_fimc1", DIV_CAM, 4, 4),
	DIV(0, "div_fimc2", "mout_fimc2", DIV_CAM, 8, 4),
	DIV(0, "div_fimc3", "mout_fimc3", DIV_CAM, 12, 4),
	DIV(0, "div_cam0", "mout_cam0", DIV_CAM, 16, 4),
	DIV(0, "div_cam1", "mout_cam1", DIV_CAM, 20, 4),
	DIV(0, "div_csis0", "mout_csis0", DIV_CAM, 24, 4),
	DIV(0, "div_csis1", "mout_csis1", DIV_CAM, 28, 4),
	DIV(CLK_SCLK_MFC, "sclk_mfc", "mout_mfc", DIV_MFC, 0, 4),
	DIV(CLK_SCLK_G3D, "sclk_g3d", "mout_g3d", DIV_G3D, 0, 4),
	DIV(0, "div_fimd0", "mout_fimd0", DIV_LCD0, 0, 4),
	DIV(0, "div_mipi0", "mout_mipi0", DIV_LCD0, 16, 4),
	DIV(0, "div_audio0", "mout_audio0", DIV_MAUDIO, 0, 4),
	DIV(CLK_SCLK_PCM0, "sclk_pcm0", "sclk_audio0", DIV_MAUDIO, 4, 8),
	DIV(0, "div_mmc0", "mout_mmc0", DIV_FSYS1, 0, 4),
	DIV(0, "div_mmc1", "mout_mmc1", DIV_FSYS1, 16, 4),
	DIV(0, "div_mmc2", "mout_mmc2", DIV_FSYS2, 0, 4),
	DIV(0, "div_mmc3", "mout_mmc3", DIV_FSYS2, 16, 4),
	DIV(CLK_SCLK_PIXEL, "sclk_pixel", "sclk_vpll", DIV_TV, 0, 4),
	DIV(CLK_ACLK100, "aclk100", "mout_aclk100", DIV_TOP, 4, 4),
	DIV(CLK_ACLK160, "aclk160", "mout_aclk160", DIV_TOP, 8, 3),
	DIV(CLK_ACLK133, "aclk133", "mout_aclk133", DIV_TOP, 12, 3),
	DIV(0, "div_onenand", "mout_onenand1", DIV_TOP, 16, 3),
	DIV(CLK_SCLK_SLIMBUS, "sclk_slimbus", "sclk_epll", DIV_PERIL3, 4, 4),
	DIV(CLK_SCLK_PCM1, "sclk_pcm1", "sclk_audio1", DIV_PERIL4, 4, 8),
	DIV(CLK_SCLK_PCM2, "sclk_pcm2", "sclk_audio2", DIV_PERIL4, 20, 8),
	DIV(CLK_SCLK_I2S1, "sclk_i2s1", "sclk_audio1", DIV_PERIL5, 0, 6),
	DIV(CLK_SCLK_I2S2, "sclk_i2s2", "sclk_audio2", DIV_PERIL5, 8, 6),
	DIV(0, "div_mmc4", "mout_mmc4", DIV_FSYS3, 0, 4),
	DIV_F(0, "div_mmc_pre4", "div_mmc4", DIV_FSYS3, 8, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV(0, "div_uart0", "mout_uart0", DIV_PERIL0, 0, 4),
	DIV(0, "div_uart1", "mout_uart1", DIV_PERIL0, 4, 4),
	DIV(0, "div_uart2", "mout_uart2", DIV_PERIL0, 8, 4),
	DIV(0, "div_uart3", "mout_uart3", DIV_PERIL0, 12, 4),
	DIV(0, "div_uart4", "mout_uart4", DIV_PERIL0, 16, 4),
	DIV(0, "div_spi0", "mout_spi0", DIV_PERIL1, 0, 4),
	DIV(0, "div_spi_pre0", "div_spi0", DIV_PERIL1, 8, 8),
	DIV(0, "div_spi1", "mout_spi1", DIV_PERIL1, 16, 4),
	DIV(0, "div_spi_pre1", "div_spi1", DIV_PERIL1, 24, 8),
	DIV(0, "div_spi2", "mout_spi2", DIV_PERIL2, 0, 4),
	DIV(0, "div_spi_pre2", "div_spi2", DIV_PERIL2, 8, 8),
	DIV(0, "div_audio1", "mout_audio1", DIV_PERIL4, 0, 4),
	DIV(0, "div_audio2", "mout_audio2", DIV_PERIL4, 16, 4),
	DIV(CLK_SCLK_APLL, "sclk_apll", "mout_apll", DIV_CPU0, 24, 3),
	DIV_F(0, "div_mipi_pre0", "div_mipi0", DIV_LCD0, 20, 4,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(0, "div_mmc_pre0", "div_mmc0", DIV_FSYS1, 8, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(0, "div_mmc_pre1", "div_mmc1", DIV_FSYS1, 24, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(0, "div_mmc_pre2", "div_mmc2", DIV_FSYS2, 8, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV_F(0, "div_mmc_pre3", "div_mmc3", DIV_FSYS2, 24, 8,
			CLK_SET_RATE_PARENT, 0),
	DIV(0, "div_clkout_top", "mout_clkout_top", CLKOUT_CMU_TOP, 8, 6),

	DIV(CLK_DIV_ACP, "div_acp", "mout_dmc_bus", DIV_DMC0, 0, 3),
	DIV(0, "div_acp_pclk", "div_acp", DIV_DMC0, 4, 3),
	DIV(0, "div_dphy", "mout_dphy", DIV_DMC0, 8, 3),
	DIV(CLK_DIV_DMC, "div_dmc", "mout_dmc_bus", DIV_DMC0, 12, 3),
	DIV(0, "div_dmcd", "div_dmc", DIV_DMC0, 16, 3),
	DIV(0, "div_dmcp", "div_dmcd", DIV_DMC0, 20, 3),
	DIV(0, "div_pwi", "mout_pwi", DIV_DMC1, 8, 4),
	DIV(0, "div_clkout_dmc", "mout_clkout_dmc", CLKOUT_CMU_DMC, 8, 6),
};

/* list of divider clocks supported in exynos4210 soc */
static const struct samsung_div_clock exynos4210_div_clks[] __initconst = {
	DIV(CLK_ACLK200, "aclk200", "mout_aclk200", DIV_TOP, 0, 3),
	DIV(CLK_SCLK_FIMG2D, "sclk_fimg2d", "mout_g2d", DIV_IMAGE, 0, 4),
	DIV(0, "div_fimd1", "mout_fimd1", E4210_DIV_LCD1, 0, 4),
	DIV(0, "div_mipi1", "mout_mipi1", E4210_DIV_LCD1, 16, 4),
	DIV(0, "div_sata", "mout_sata", DIV_FSYS0, 20, 4),
	DIV_F(0, "div_mipi_pre1", "div_mipi1", E4210_DIV_LCD1, 20, 4,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			CLK_SET_RATE_PARENT, 0),
};

/* list of divider clocks supported in exynos4x12 soc */
<<<<<<< HEAD
struct samsung_div_clock exynos4x12_div_clks[] __initdata = {
	DIV(none, "div_mdnie0", "mout_mdnie0", DIV_LCD0, 4, 4),
	DIV(none, "div_mdnie_pwm0", "mout_mdnie_pwm0", DIV_LCD0, 8, 4),
	DIV(none, "div_mdnie_pwm_pre0", "div_mdnie_pwm0", DIV_LCD0, 12, 4),
	DIV(none, "div_mipihsi", "mout_mipihsi", DIV_FSYS0, 20, 4),
	DIV(none, "div_jpeg", "mout_jpeg", E4X12_DIV_CAM1, 0, 4),
	DIV(div_aclk200, "div_aclk200", "mout_aclk200", DIV_TOP, 0, 3),
	DIV(none, "div_aclk266_gps", "mout_aclk266_gps", DIV_TOP, 20, 3),
	DIV(div_aclk400_mcuisp, "div_aclk400_mcuisp", "mout_aclk400_mcuisp",
						DIV_TOP, 24, 3),
	DIV(none, "div_pwm_isp", "mout_pwm_isp", E4X12_DIV_ISP, 0, 4),
	DIV(none, "div_spi0_isp", "mout_spi0_isp", E4X12_DIV_ISP, 4, 4),
	DIV(none, "div_spi0_isp_pre", "div_spi0_isp", E4X12_DIV_ISP, 8, 8),
	DIV(none, "div_spi1_isp", "mout_spi1_isp", E4X12_DIV_ISP, 16, 4),
	DIV(none, "div_spi1_isp_pre", "div_spi1_isp", E4X12_DIV_ISP, 20, 8),
	DIV(none, "div_uart_isp", "mout_uart_isp", E4X12_DIV_ISP, 28, 4),
	DIV(div_isp0, "div_isp0", "aclk200", E4X12_DIV_ISP0, 0, 3),
	DIV(div_isp1, "div_isp1", "aclk200", E4X12_DIV_ISP0, 4, 3),
	DIV(none, "div_mpwm", "div_isp1", E4X12_DIV_ISP1, 0, 3),
	DIV(div_mcuisp0, "div_mcuisp0", "aclk400_mcuisp", E4X12_DIV_ISP1, 4, 3),
	DIV(div_mcuisp1, "div_mcuisp1", "div_mcuisp0", E4X12_DIV_ISP1, 8, 3),
};

/* list of gate clocks supported in all exynos4 soc's */
struct samsung_gate_clock exynos4_gate_clks[] __initdata = {
=======
static const struct samsung_div_clock exynos4x12_div_clks[] __initconst = {
	DIV(0, "div_mdnie0", "mout_mdnie0", DIV_LCD0, 4, 4),
	DIV(0, "div_mdnie_pwm0", "mout_mdnie_pwm0", DIV_LCD0, 8, 4),
	DIV(0, "div_mdnie_pwm_pre0", "div_mdnie_pwm0", DIV_LCD0, 12, 4),
	DIV(0, "div_mipihsi", "mout_mipihsi", DIV_FSYS0, 20, 4),
	DIV(0, "div_jpeg", "mout_jpeg", E4X12_DIV_CAM1, 0, 4),
	DIV(CLK_DIV_ACLK200, "div_aclk200", "mout_aclk200", DIV_TOP, 0, 3),
	DIV(0, "div_aclk266_gps", "mout_aclk266_gps", DIV_TOP, 20, 3),
	DIV(CLK_DIV_ACLK400_MCUISP, "div_aclk400_mcuisp", "mout_aclk400_mcuisp",
						DIV_TOP, 24, 3),
	DIV(0, "div_pwm_isp", "mout_pwm_isp", E4X12_DIV_ISP, 0, 4),
	DIV(0, "div_spi0_isp", "mout_spi0_isp", E4X12_DIV_ISP, 4, 4),
	DIV(0, "div_spi0_isp_pre", "div_spi0_isp", E4X12_DIV_ISP, 8, 8),
	DIV(0, "div_spi1_isp", "mout_spi1_isp", E4X12_DIV_ISP, 16, 4),
	DIV(0, "div_spi1_isp_pre", "div_spi1_isp", E4X12_DIV_ISP, 20, 8),
	DIV(0, "div_uart_isp", "mout_uart_isp", E4X12_DIV_ISP, 28, 4),
	DIV_F(CLK_DIV_ISP0, "div_isp0", "aclk200", E4X12_DIV_ISP0, 0, 3,
						CLK_GET_RATE_NOCACHE, 0),
	DIV_F(CLK_DIV_ISP1, "div_isp1", "aclk200", E4X12_DIV_ISP0, 4, 3,
						CLK_GET_RATE_NOCACHE, 0),
	DIV(0, "div_mpwm", "div_isp1", E4X12_DIV_ISP1, 0, 3),
	DIV_F(CLK_DIV_MCUISP0, "div_mcuisp0", "aclk400_mcuisp", E4X12_DIV_ISP1,
						4, 3, CLK_GET_RATE_NOCACHE, 0),
	DIV_F(CLK_DIV_MCUISP1, "div_mcuisp1", "div_mcuisp0", E4X12_DIV_ISP1,
						8, 3, CLK_GET_RATE_NOCACHE, 0),
	DIV(CLK_SCLK_FIMG2D, "sclk_fimg2d", "mout_g2d", DIV_DMC1, 0, 4),
	DIV(CLK_DIV_C2C, "div_c2c", "mout_c2c", DIV_DMC1, 4, 3),
	DIV(0, "div_c2c_aclk", "div_c2c", DIV_DMC1, 12, 3),
};

/* list of gate clocks supported in all exynos4 soc's */
static const struct samsung_gate_clock exynos4_gate_clks[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * After all Exynos4 based platforms are migrated to use device tree,
	 * the device name and clock alias names specified below for some
	 * of the clocks can be removed.
	 */
<<<<<<< HEAD
	GATE(sclk_hdmi, "sclk_hdmi", "mout_hdmi", SRC_MASK_TV, 0, 0, 0),
	GATE(sclk_spdif, "sclk_spdif", "mout_spdif", SRC_MASK_PERIL1, 8, 0, 0),
	GATE(jpeg, "jpeg", "aclk160", GATE_IP_CAM, 6, 0, 0),
	GATE(mie0, "mie0", "aclk160", GATE_IP_LCD0, 1, 0, 0),
	GATE(dsim0, "dsim0", "aclk160", GATE_IP_LCD0, 3, 0, 0),
	GATE(fimd1, "fimd1", "aclk160", E4210_GATE_IP_LCD1, 0, 0, 0),
	GATE(mie1, "mie1", "aclk160", E4210_GATE_IP_LCD1, 1, 0, 0),
	GATE(dsim1, "dsim1", "aclk160", E4210_GATE_IP_LCD1, 3, 0, 0),
	GATE(smmu_fimd1, "smmu_fimd1", "aclk160", E4210_GATE_IP_LCD1, 4, 0, 0),
	GATE(tsi, "tsi", "aclk133", GATE_IP_FSYS, 4, 0, 0),
	GATE(sromc, "sromc", "aclk133", GATE_IP_FSYS, 11, 0, 0),
	GATE(sclk_g3d, "sclk_g3d", "div_g3d", GATE_IP_G3D, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(usb_device, "usb_device", "aclk133", GATE_IP_FSYS, 13, 0, 0),
	GATE(onenand, "onenand", "aclk133", GATE_IP_FSYS, 15, 0, 0),
	GATE(nfcon, "nfcon", "aclk133", GATE_IP_FSYS, 16, 0, 0),
	GATE(gps, "gps", "aclk133", GATE_IP_GPS, 0, 0, 0),
	GATE(smmu_gps, "smmu_gps", "aclk133", GATE_IP_GPS, 1, 0, 0),
	GATE(slimbus, "slimbus", "aclk100", GATE_IP_PERIL, 25, 0, 0),
	GATE(sclk_cam0, "sclk_cam0", "div_cam0", GATE_SCLK_CAM, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(sclk_cam1, "sclk_cam1", "div_cam1", GATE_SCLK_CAM, 5,
			CLK_SET_RATE_PARENT, 0),
	GATE(sclk_mipi0, "sclk_mipi0", "div_mipi_pre0",
			SRC_MASK_LCD0, 12, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_audio0, "sclk_audio0", "div_audio0", SRC_MASK_MAUDIO, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(sclk_audio1, "sclk_audio1", "div_audio1", SRC_MASK_PERIL1, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE_D(vp, "s5p-mixer", "vp", "aclk160", GATE_IP_TV, 0, 0, 0),
	GATE_D(mixer, "s5p-mixer", "mixer", "aclk160", GATE_IP_TV, 1, 0, 0),
	GATE_D(hdmi, "exynos4-hdmi", "hdmi", "aclk160", GATE_IP_TV, 3, 0, 0),
	GATE_A(pwm, "pwm", "aclk100", GATE_IP_PERIL, 24, 0, 0, "timers"),
	GATE_A(sdmmc4, "sdmmc4", "aclk133", GATE_IP_FSYS, 9, 0, 0, "biu"),
	GATE_A(usb_host, "usb_host", "aclk133",
			GATE_IP_FSYS, 12, 0, 0, "usbhost"),
	GATE_DA(sclk_fimc0, "exynos4-fimc.0", "sclk_fimc0", "div_fimc0",
			SRC_MASK_CAM, 0, CLK_SET_RATE_PARENT, 0, "sclk_fimc"),
	GATE_DA(sclk_fimc1, "exynos4-fimc.1", "sclk_fimc1", "div_fimc1",
			SRC_MASK_CAM, 4, CLK_SET_RATE_PARENT, 0, "sclk_fimc"),
	GATE_DA(sclk_fimc2, "exynos4-fimc.2", "sclk_fimc2", "div_fimc2",
			SRC_MASK_CAM, 8, CLK_SET_RATE_PARENT, 0, "sclk_fimc"),
	GATE_DA(sclk_fimc3, "exynos4-fimc.3", "sclk_fimc3", "div_fimc3",
			SRC_MASK_CAM, 12, CLK_SET_RATE_PARENT, 0, "sclk_fimc"),
	GATE_DA(sclk_csis0, "s5p-mipi-csis.0", "sclk_csis0", "div_csis0",
			SRC_MASK_CAM, 24, CLK_SET_RATE_PARENT, 0, "sclk_csis"),
	GATE_DA(sclk_csis1, "s5p-mipi-csis.1", "sclk_csis1", "div_csis1",
			SRC_MASK_CAM, 28, CLK_SET_RATE_PARENT, 0, "sclk_csis"),
	GATE_DA(sclk_fimd0, "exynos4-fb.0", "sclk_fimd0", "div_fimd0",
			SRC_MASK_LCD0, 0, CLK_SET_RATE_PARENT, 0, "sclk_fimd"),
	GATE_DA(sclk_mmc0, "exynos4-sdhci.0", "sclk_mmc0", "div_mmc_pre0",
			SRC_MASK_FSYS, 0, CLK_SET_RATE_PARENT, 0,
			"mmc_busclk.2"),
	GATE_DA(sclk_mmc1, "exynos4-sdhci.1", "sclk_mmc1", "div_mmc_pre1",
			SRC_MASK_FSYS, 4, CLK_SET_RATE_PARENT, 0,
			"mmc_busclk.2"),
	GATE_DA(sclk_mmc2, "exynos4-sdhci.2", "sclk_mmc2", "div_mmc_pre2",
			SRC_MASK_FSYS, 8, CLK_SET_RATE_PARENT, 0,
			"mmc_busclk.2"),
	GATE_DA(sclk_mmc3, "exynos4-sdhci.3", "sclk_mmc3", "div_mmc_pre3",
			SRC_MASK_FSYS, 12, CLK_SET_RATE_PARENT, 0,
			"mmc_busclk.2"),
	GATE_DA(sclk_mmc4, NULL, "sclk_mmc4", "div_mmc_pre4",
			SRC_MASK_FSYS, 16, CLK_SET_RATE_PARENT, 0, "ciu"),
	GATE_DA(sclk_uart0, "exynos4210-uart.0", "uclk0", "div_uart0",
			SRC_MASK_PERIL0, 0, CLK_SET_RATE_PARENT,
			0, "clk_uart_baud0"),
	GATE_DA(sclk_uart1, "exynos4210-uart.1", "uclk1", "div_uart1",
			SRC_MASK_PERIL0, 4, CLK_SET_RATE_PARENT,
			0, "clk_uart_baud0"),
	GATE_DA(sclk_uart2, "exynos4210-uart.2", "uclk2", "div_uart2",
			SRC_MASK_PERIL0, 8, CLK_SET_RATE_PARENT,
			0, "clk_uart_baud0"),
	GATE_DA(sclk_uart3, "exynos4210-uart.3", "uclk3", "div_uart3",
			SRC_MASK_PERIL0, 12, CLK_SET_RATE_PARENT,
			0, "clk_uart_baud0"),
	GATE_DA(sclk_uart4, "exynos4210-uart.4", "uclk4", "div_uart4",
			SRC_MASK_PERIL0, 16, CLK_SET_RATE_PARENT,
			0, "clk_uart_baud0"),
	GATE(sclk_audio2, "sclk_audio2", "div_audio2", SRC_MASK_PERIL1, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE_DA(sclk_spi0, "exynos4210-spi.0", "sclk_spi0", "div_spi_pre0",
			SRC_MASK_PERIL1, 16, CLK_SET_RATE_PARENT,
			0, "spi_busclk0"),
	GATE_DA(sclk_spi1, "exynos4210-spi.1", "sclk_spi1", "div_spi_pre1",
			SRC_MASK_PERIL1, 20, CLK_SET_RATE_PARENT,
			0, "spi_busclk0"),
	GATE_DA(sclk_spi2, "exynos4210-spi.2", "sclk_spi2", "div_spi_pre2",
			SRC_MASK_PERIL1, 24, CLK_SET_RATE_PARENT,
			0, "spi_busclk0"),
	GATE_DA(fimc0, "exynos4-fimc.0", "fimc0", "aclk160",
			GATE_IP_CAM, 0, 0, 0, "fimc"),
	GATE_DA(fimc1, "exynos4-fimc.1", "fimc1", "aclk160",
			GATE_IP_CAM, 1, 0, 0, "fimc"),
	GATE_DA(fimc2, "exynos4-fimc.2", "fimc2", "aclk160",
			GATE_IP_CAM, 2, 0, 0, "fimc"),
	GATE_DA(fimc3, "exynos4-fimc.3", "fimc3", "aclk160",
			GATE_IP_CAM, 3, 0, 0, "fimc"),
	GATE_DA(csis0, "s5p-mipi-csis.0", "csis0", "aclk160",
			GATE_IP_CAM, 4, 0, 0, "fimc"),
	GATE_DA(csis1, "s5p-mipi-csis.1", "csis1", "aclk160",
			GATE_IP_CAM, 5, 0, 0, "fimc"),
	GATE_DA(smmu_fimc0, "exynos-sysmmu.5", "smmu_fimc0", "aclk160",
			GATE_IP_CAM, 7, 0, 0, "sysmmu"),
	GATE_DA(smmu_fimc1, "exynos-sysmmu.6", "smmu_fimc1", "aclk160",
			GATE_IP_CAM, 8, 0, 0, "sysmmu"),
	GATE_DA(smmu_fimc2, "exynos-sysmmu.7", "smmu_fimc2", "aclk160",
			GATE_IP_CAM, 9, 0, 0, "sysmmu"),
	GATE_DA(smmu_fimc3, "exynos-sysmmu.8", "smmu_fimc3", "aclk160",
			GATE_IP_CAM, 10, 0, 0, "sysmmu"),
	GATE_DA(smmu_jpeg, "exynos-sysmmu.3", "smmu_jpeg", "aclk160",
			GATE_IP_CAM, 11, 0, 0, "sysmmu"),
	GATE(pixelasyncm0, "pxl_async0", "aclk160", GATE_IP_CAM, 17, 0, 0),
	GATE(pixelasyncm1, "pxl_async1", "aclk160", GATE_IP_CAM, 18, 0, 0),
	GATE_DA(smmu_tv, "exynos-sysmmu.2", "smmu_tv", "aclk160",
			GATE_IP_TV, 4, 0, 0, "sysmmu"),
	GATE_DA(mfc, "s5p-mfc", "mfc", "aclk100", GATE_IP_MFC, 0, 0, 0, "mfc"),
	GATE_DA(smmu_mfcl, "exynos-sysmmu.0", "smmu_mfcl", "aclk100",
			GATE_IP_MFC, 1, 0, 0, "sysmmu"),
	GATE_DA(smmu_mfcr, "exynos-sysmmu.1", "smmu_mfcr", "aclk100",
			GATE_IP_MFC, 2, 0, 0, "sysmmu"),
	GATE_DA(fimd0, "exynos4-fb.0", "fimd0", "aclk160",
			GATE_IP_LCD0, 0, 0, 0, "fimd"),
	GATE_DA(smmu_fimd0, "exynos-sysmmu.10", "smmu_fimd0", "aclk160",
			GATE_IP_LCD0, 4, 0, 0, "sysmmu"),
	GATE_DA(pdma0, "dma-pl330.0", "pdma0", "aclk133",
			GATE_IP_FSYS, 0, 0, 0, "dma"),
	GATE_DA(pdma1, "dma-pl330.1", "pdma1", "aclk133",
			GATE_IP_FSYS, 1, 0, 0, "dma"),
	GATE_DA(sdmmc0, "exynos4-sdhci.0", "sdmmc0", "aclk133",
			GATE_IP_FSYS, 5, 0, 0, "hsmmc"),
	GATE_DA(sdmmc1, "exynos4-sdhci.1", "sdmmc1", "aclk133",
			GATE_IP_FSYS, 6, 0, 0, "hsmmc"),
	GATE_DA(sdmmc2, "exynos4-sdhci.2", "sdmmc2", "aclk133",
			GATE_IP_FSYS, 7, 0, 0, "hsmmc"),
	GATE_DA(sdmmc3, "exynos4-sdhci.3", "sdmmc3", "aclk133",
			GATE_IP_FSYS, 8, 0, 0, "hsmmc"),
	GATE_DA(uart0, "exynos4210-uart.0", "uart0", "aclk100",
			GATE_IP_PERIL, 0, 0, 0, "uart"),
	GATE_DA(uart1, "exynos4210-uart.1", "uart1", "aclk100",
			GATE_IP_PERIL, 1, 0, 0, "uart"),
	GATE_DA(uart2, "exynos4210-uart.2", "uart2", "aclk100",
			GATE_IP_PERIL, 2, 0, 0, "uart"),
	GATE_DA(uart3, "exynos4210-uart.3", "uart3", "aclk100",
			GATE_IP_PERIL, 3, 0, 0, "uart"),
	GATE_DA(uart4, "exynos4210-uart.4", "uart4", "aclk100",
			GATE_IP_PERIL, 4, 0, 0, "uart"),
	GATE_DA(i2c0, "s3c2440-i2c.0", "i2c0", "aclk100",
			GATE_IP_PERIL, 6, 0, 0, "i2c"),
	GATE_DA(i2c1, "s3c2440-i2c.1", "i2c1", "aclk100",
			GATE_IP_PERIL, 7, 0, 0, "i2c"),
	GATE_DA(i2c2, "s3c2440-i2c.2", "i2c2", "aclk100",
			GATE_IP_PERIL, 8, 0, 0, "i2c"),
	GATE_DA(i2c3, "s3c2440-i2c.3", "i2c3", "aclk100",
			GATE_IP_PERIL, 9, 0, 0, "i2c"),
	GATE_DA(i2c4, "s3c2440-i2c.4", "i2c4", "aclk100",
			GATE_IP_PERIL, 10, 0, 0, "i2c"),
	GATE_DA(i2c5, "s3c2440-i2c.5", "i2c5", "aclk100",
			GATE_IP_PERIL, 11, 0, 0, "i2c"),
	GATE_DA(i2c6, "s3c2440-i2c.6", "i2c6", "aclk100",
			GATE_IP_PERIL, 12, 0, 0, "i2c"),
	GATE_DA(i2c7, "s3c2440-i2c.7", "i2c7", "aclk100",
			GATE_IP_PERIL, 13, 0, 0, "i2c"),
	GATE_DA(i2c_hdmi, "s3c2440-hdmiphy-i2c", "i2c-hdmi", "aclk100",
			GATE_IP_PERIL, 14, 0, 0, "i2c"),
	GATE_DA(spi0, "exynos4210-spi.0", "spi0", "aclk100",
			GATE_IP_PERIL, 16, 0, 0, "spi"),
	GATE_DA(spi1, "exynos4210-spi.1", "spi1", "aclk100",
			GATE_IP_PERIL, 17, 0, 0, "spi"),
	GATE_DA(spi2, "exynos4210-spi.2", "spi2", "aclk100",
			GATE_IP_PERIL, 18, 0, 0, "spi"),
	GATE_DA(i2s1, "samsung-i2s.1", "i2s1", "aclk100",
			GATE_IP_PERIL, 20, 0, 0, "iis"),
	GATE_DA(i2s2, "samsung-i2s.2", "i2s2", "aclk100",
			GATE_IP_PERIL, 21, 0, 0, "iis"),
	GATE_DA(pcm1, "samsung-pcm.1", "pcm1", "aclk100",
			GATE_IP_PERIL, 22, 0, 0, "pcm"),
	GATE_DA(pcm2, "samsung-pcm.2", "pcm2", "aclk100",
			GATE_IP_PERIL, 23, 0, 0, "pcm"),
	GATE_DA(spdif, "samsung-spdif", "spdif", "aclk100",
			GATE_IP_PERIL, 26, 0, 0, "spdif"),
	GATE_DA(ac97, "samsung-ac97", "ac97", "aclk100",
			GATE_IP_PERIL, 27, 0, 0, "ac97"),
};

/* list of gate clocks supported in exynos4210 soc */
struct samsung_gate_clock exynos4210_gate_clks[] __initdata = {
	GATE(tvenc, "tvenc", "aclk160", GATE_IP_TV, 2, 0, 0),
	GATE(g2d, "g2d", "aclk200", E4210_GATE_IP_IMAGE, 0, 0, 0),
	GATE(rotator, "rotator", "aclk200", E4210_GATE_IP_IMAGE, 1, 0, 0),
	GATE(mdma, "mdma", "aclk200", E4210_GATE_IP_IMAGE, 2, 0, 0),
	GATE(smmu_g2d, "smmu_g2d", "aclk200", E4210_GATE_IP_IMAGE, 3, 0, 0),
	GATE(smmu_mdma, "smmu_mdma", "aclk200", E4210_GATE_IP_IMAGE, 5, 0, 0),
	GATE(pcie_phy, "pcie_phy", "aclk133", GATE_IP_FSYS, 2, 0, 0),
	GATE(sata_phy, "sata_phy", "aclk133", GATE_IP_FSYS, 3, 0, 0),
	GATE(sata, "sata", "aclk133", GATE_IP_FSYS, 10, 0, 0),
	GATE(pcie, "pcie", "aclk133", GATE_IP_FSYS, 14, 0, 0),
	GATE(smmu_pcie, "smmu_pcie", "aclk133", GATE_IP_FSYS, 18, 0, 0),
	GATE(modemif, "modemif", "aclk100", GATE_IP_PERIL, 28, 0, 0),
	GATE(chipid, "chipid", "aclk100", E4210_GATE_IP_PERIR, 0, 0, 0),
	GATE(sysreg, "sysreg", "aclk100", E4210_GATE_IP_PERIR, 0,
			CLK_IGNORE_UNUSED, 0),
	GATE(hdmi_cec, "hdmi_cec", "aclk100", E4210_GATE_IP_PERIR, 11, 0, 0),
	GATE(smmu_rotator, "smmu_rotator", "aclk200",
			E4210_GATE_IP_IMAGE, 4, 0, 0),
	GATE(sclk_mipi1, "sclk_mipi1", "div_mipi_pre1",
			E4210_SRC_MASK_LCD1, 12, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_sata, "sclk_sata", "div_sata",
			SRC_MASK_FSYS, 24, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_mixer, "sclk_mixer", "mout_mixer", SRC_MASK_TV, 4, 0, 0),
	GATE(sclk_dac, "sclk_dac", "mout_dac", SRC_MASK_TV, 8, 0, 0),
	GATE_A(tsadc, "tsadc", "aclk100", GATE_IP_PERIL, 15, 0, 0, "adc"),
	GATE_A(mct, "mct", "aclk100", E4210_GATE_IP_PERIR, 13, 0, 0, "mct"),
	GATE_A(wdt, "watchdog", "aclk100", E4210_GATE_IP_PERIR, 14, 0, 0, "watchdog"),
	GATE_A(rtc, "rtc", "aclk100", E4210_GATE_IP_PERIR, 15, 0, 0, "rtc"),
	GATE_A(keyif, "keyif", "aclk100", E4210_GATE_IP_PERIR, 16, 0, 0, "keypad"),
	GATE_DA(sclk_fimd1, "exynos4-fb.1", "sclk_fimd1", "div_fimd1",
			E4210_SRC_MASK_LCD1, 0, CLK_SET_RATE_PARENT, 0, "sclk_fimd"),
};

/* list of gate clocks supported in exynos4x12 soc */
struct samsung_gate_clock exynos4x12_gate_clks[] __initdata = {
	GATE(audss, "audss", "sclk_epll", E4X12_GATE_IP_MAUDIO, 0, 0, 0),
	GATE(mdnie0, "mdnie0", "aclk160", GATE_IP_LCD0, 2, 0, 0),
	GATE(rotator, "rotator", "aclk200", E4X12_GATE_IP_IMAGE, 1, 0, 0),
	GATE(mdma2, "mdma2", "aclk200", E4X12_GATE_IP_IMAGE, 2, 0, 0),
	GATE(smmu_mdma, "smmu_mdma", "aclk200", E4X12_GATE_IP_IMAGE, 5, 0, 0),
	GATE(mipi_hsi, "mipi_hsi", "aclk133", GATE_IP_FSYS, 10, 0, 0),
	GATE(chipid, "chipid", "aclk100", E4X12_GATE_IP_PERIR, 0, 0, 0),
	GATE(sysreg, "sysreg", "aclk100", E4X12_GATE_IP_PERIR, 1,
			CLK_IGNORE_UNUSED, 0),
	GATE(hdmi_cec, "hdmi_cec", "aclk100", E4X12_GATE_IP_PERIR, 11, 0, 0),
	GATE(sclk_mdnie0, "sclk_mdnie0", "div_mdnie0",
			SRC_MASK_LCD0, 4, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_mdnie_pwm0, "sclk_mdnie_pwm0", "div_mdnie_pwm_pre0",
			SRC_MASK_LCD0, 8, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_mipihsi, "sclk_mipihsi", "div_mipihsi",
			SRC_MASK_FSYS, 24, CLK_SET_RATE_PARENT, 0),
	GATE(smmu_rotator, "smmu_rotator", "aclk200",
			E4X12_GATE_IP_IMAGE, 4, 0, 0),
	GATE_A(mct, "mct", "aclk100", E4X12_GATE_IP_PERIR, 13, 0, 0, "mct"),
	GATE_A(rtc, "rtc", "aclk100", E4X12_GATE_IP_PERIR, 15, 0, 0, "rtc"),
	GATE_A(keyif, "keyif", "aclk100",
			E4X12_GATE_IP_PERIR, 16, 0, 0, "keypad"),
	GATE(sclk_pwm_isp, "sclk_pwm_isp", "div_pwm_isp",
			E4X12_SRC_MASK_ISP, 0, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_spi0_isp, "sclk_spi0_isp", "div_spi0_isp_pre",
			E4X12_SRC_MASK_ISP, 4, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_spi1_isp, "sclk_spi1_isp", "div_spi1_isp_pre",
			E4X12_SRC_MASK_ISP, 8, CLK_SET_RATE_PARENT, 0),
	GATE(sclk_uart_isp, "sclk_uart_isp", "div_uart_isp",
			E4X12_SRC_MASK_ISP, 12, CLK_SET_RATE_PARENT, 0),
	GATE(pwm_isp_sclk, "pwm_isp_sclk", "sclk_pwm_isp",
			E4X12_GATE_IP_ISP, 0, 0, 0),
	GATE(spi0_isp_sclk, "spi0_isp_sclk", "sclk_spi0_isp",
			E4X12_GATE_IP_ISP, 1, 0, 0),
	GATE(spi1_isp_sclk, "spi1_isp_sclk", "sclk_spi1_isp",
			E4X12_GATE_IP_ISP, 2, 0, 0),
	GATE(uart_isp_sclk, "uart_isp_sclk", "sclk_uart_isp",
			E4X12_GATE_IP_ISP, 3, 0, 0),
	GATE_A(wdt, "watchdog", "aclk100",
			E4X12_GATE_IP_PERIR, 14, 0, 0, "watchdog"),
	GATE_DA(pcm0, "samsung-pcm.0", "pcm0", "aclk100",
			E4X12_GATE_IP_MAUDIO, 2, 0, 0, "pcm"),
	GATE_DA(i2s0, "samsung-i2s.0", "i2s0", "aclk100",
			E4X12_GATE_IP_MAUDIO, 3, 0, 0, "iis"),
	GATE(fimc_isp, "isp", "aclk200", E4X12_GATE_ISP0, 0,
			CLK_IGNORE_UNUSED, 0),
	GATE(fimc_drc, "drc", "aclk200", E4X12_GATE_ISP0, 1,
			CLK_IGNORE_UNUSED, 0),
	GATE(fimc_fd, "fd", "aclk200", E4X12_GATE_ISP0, 2,
			CLK_IGNORE_UNUSED, 0),
	GATE(fimc_lite0, "lite0", "aclk200", E4X12_GATE_ISP0, 3,
			CLK_IGNORE_UNUSED, 0),
	GATE(fimc_lite1, "lite1", "aclk200", E4X12_GATE_ISP0, 4,
			CLK_IGNORE_UNUSED, 0),
	GATE(mcuisp, "mcuisp", "aclk200", E4X12_GATE_ISP0, 5,
			CLK_IGNORE_UNUSED, 0),
	GATE(gicisp, "gicisp", "aclk200", E4X12_GATE_ISP0, 7,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_isp, "smmu_isp", "aclk200", E4X12_GATE_ISP0, 8,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_drc, "smmu_drc", "aclk200", E4X12_GATE_ISP0, 9,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_fd, "smmu_fd", "aclk200", E4X12_GATE_ISP0, 10,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_lite0, "smmu_lite0", "aclk200", E4X12_GATE_ISP0, 11,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_lite1, "smmu_lite1", "aclk200", E4X12_GATE_ISP0, 12,
			CLK_IGNORE_UNUSED, 0),
	GATE(ppmuispmx, "ppmuispmx", "aclk200", E4X12_GATE_ISP0, 20,
			CLK_IGNORE_UNUSED, 0),
	GATE(ppmuispx, "ppmuispx", "aclk200", E4X12_GATE_ISP0, 21,
			CLK_IGNORE_UNUSED, 0),
	GATE(mcuctl_isp, "mcuctl_isp", "aclk200", E4X12_GATE_ISP0, 23,
			CLK_IGNORE_UNUSED, 0),
	GATE(mpwm_isp, "mpwm_isp", "aclk200", E4X12_GATE_ISP0, 24,
			CLK_IGNORE_UNUSED, 0),
	GATE(i2c0_isp, "i2c0_isp", "aclk200", E4X12_GATE_ISP0, 25,
			CLK_IGNORE_UNUSED, 0),
	GATE(i2c1_isp, "i2c1_isp", "aclk200", E4X12_GATE_ISP0, 26,
			CLK_IGNORE_UNUSED, 0),
	GATE(mtcadc_isp, "mtcadc_isp", "aclk200", E4X12_GATE_ISP0, 27,
			CLK_IGNORE_UNUSED, 0),
	GATE(pwm_isp, "pwm_isp", "aclk200", E4X12_GATE_ISP0, 28,
			CLK_IGNORE_UNUSED, 0),
	GATE(wdt_isp, "wdt_isp", "aclk200", E4X12_GATE_ISP0, 30,
			CLK_IGNORE_UNUSED, 0),
	GATE(uart_isp, "uart_isp", "aclk200", E4X12_GATE_ISP0, 31,
			CLK_IGNORE_UNUSED, 0),
	GATE(asyncaxim, "asyncaxim", "aclk200", E4X12_GATE_ISP1, 0,
			CLK_IGNORE_UNUSED, 0),
	GATE(smmu_ispcx, "smmu_ispcx", "aclk200", E4X12_GATE_ISP1, 4,
			CLK_IGNORE_UNUSED, 0),
	GATE(spi0_isp, "spi0_isp", "aclk200", E4X12_GATE_ISP1, 12,
			CLK_IGNORE_UNUSED, 0),
	GATE(spi1_isp, "spi1_isp", "aclk200", E4X12_GATE_ISP1, 13,
			CLK_IGNORE_UNUSED, 0),
=======
	GATE(CLK_PPMULEFT, "ppmuleft", "aclk200", GATE_IP_LEFTBUS, 1, 0, 0),
	GATE(CLK_PPMURIGHT, "ppmuright", "aclk200", GATE_IP_RIGHTBUS, 1, 0, 0),
	GATE(CLK_SCLK_HDMI, "sclk_hdmi", "mout_hdmi", SRC_MASK_TV, 0, 0, 0),
	GATE(CLK_SCLK_SPDIF, "sclk_spdif", "mout_spdif", SRC_MASK_PERIL1, 8, 0,
		0),
	GATE(CLK_JPEG, "jpeg", "aclk160", GATE_IP_CAM, 6, 0, 0),
	GATE(CLK_MIE0, "mie0", "aclk160", GATE_IP_LCD0, 1, 0, 0),
	GATE(CLK_DSIM0, "dsim0", "aclk160", GATE_IP_LCD0, 3, 0, 0),
	GATE(CLK_FIMD1, "fimd1", "aclk160", E4210_GATE_IP_LCD1, 0, 0, 0),
	GATE(CLK_MIE1, "mie1", "aclk160", E4210_GATE_IP_LCD1, 1, 0, 0),
	GATE(CLK_DSIM1, "dsim1", "aclk160", E4210_GATE_IP_LCD1, 3, 0, 0),
	GATE(CLK_SMMU_FIMD1, "smmu_fimd1", "aclk160", E4210_GATE_IP_LCD1, 4, 0,
		0),
	GATE(CLK_TSI, "tsi", "aclk133", GATE_IP_FSYS, 4, 0, 0),
	GATE(CLK_SROMC, "sromc", "aclk133", GATE_IP_FSYS, 11, 0, 0),
	GATE(CLK_G3D, "g3d", "aclk200", GATE_IP_G3D, 0, 0, 0),
	GATE(CLK_PPMUG3D, "ppmug3d", "aclk200", GATE_IP_G3D, 1, 0, 0),
	GATE(CLK_USB_DEVICE, "usb_device", "aclk133", GATE_IP_FSYS, 13, 0, 0),
	GATE(CLK_ONENAND, "onenand", "aclk133", GATE_IP_FSYS, 15, 0, 0),
	GATE(CLK_NFCON, "nfcon", "aclk133", GATE_IP_FSYS, 16, 0, 0),
	GATE(CLK_GPS, "gps", "aclk133", GATE_IP_GPS, 0, 0, 0),
	GATE(CLK_SMMU_GPS, "smmu_gps", "aclk133", GATE_IP_GPS, 1, 0, 0),
	GATE(CLK_PPMUGPS, "ppmugps", "aclk200", GATE_IP_GPS, 2, 0, 0),
	GATE(CLK_SLIMBUS, "slimbus", "aclk100", GATE_IP_PERIL, 25, 0, 0),
	GATE(CLK_SCLK_CAM0, "sclk_cam0", "div_cam0", GATE_SCLK_CAM, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_CAM1, "sclk_cam1", "div_cam1", GATE_SCLK_CAM, 5,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MIPI0, "sclk_mipi0", "div_mipi_pre0",
			SRC_MASK_LCD0, 12, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_AUDIO0, "sclk_audio0", "div_audio0", SRC_MASK_MAUDIO, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_AUDIO1, "sclk_audio1", "div_audio1", SRC_MASK_PERIL1, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_VP, "vp", "aclk160", GATE_IP_TV, 0, 0, 0),
	GATE(CLK_MIXER, "mixer", "aclk160", GATE_IP_TV, 1, 0, 0),
	GATE(CLK_HDMI, "hdmi", "aclk160", GATE_IP_TV, 3, 0, 0),
	GATE(CLK_PWM, "pwm", "aclk100", GATE_IP_PERIL, 24, 0, 0),
	GATE(CLK_SDMMC4, "sdmmc4", "aclk133", GATE_IP_FSYS, 9, 0, 0),
	GATE(CLK_USB_HOST, "usb_host", "aclk133", GATE_IP_FSYS, 12, 0, 0),
	GATE(CLK_SCLK_FIMC0, "sclk_fimc0", "div_fimc0", SRC_MASK_CAM, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_FIMC1, "sclk_fimc1", "div_fimc1", SRC_MASK_CAM, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_FIMC2, "sclk_fimc2", "div_fimc2", SRC_MASK_CAM, 8,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_FIMC3, "sclk_fimc3", "div_fimc3", SRC_MASK_CAM, 12,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_CSIS0, "sclk_csis0", "div_csis0", SRC_MASK_CAM, 24,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_CSIS1, "sclk_csis1", "div_csis1", SRC_MASK_CAM, 28,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_FIMD0, "sclk_fimd0", "div_fimd0", SRC_MASK_LCD0, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MMC0, "sclk_mmc0", "div_mmc_pre0", SRC_MASK_FSYS, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MMC1, "sclk_mmc1", "div_mmc_pre1", SRC_MASK_FSYS, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MMC2, "sclk_mmc2", "div_mmc_pre2", SRC_MASK_FSYS, 8,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MMC3, "sclk_mmc3", "div_mmc_pre3", SRC_MASK_FSYS, 12,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MMC4, "sclk_mmc4", "div_mmc_pre4", SRC_MASK_FSYS, 16,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_UART0, "uclk0", "div_uart0", SRC_MASK_PERIL0, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_UART1, "uclk1", "div_uart1", SRC_MASK_PERIL0, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_UART2, "uclk2", "div_uart2", SRC_MASK_PERIL0, 8,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_UART3, "uclk3", "div_uart3", SRC_MASK_PERIL0, 12,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_UART4, "uclk4", "div_uart4", SRC_MASK_PERIL0, 16,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_AUDIO2, "sclk_audio2", "div_audio2", SRC_MASK_PERIL1, 4,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_SPI0, "sclk_spi0", "div_spi_pre0", SRC_MASK_PERIL1, 16,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_SPI1, "sclk_spi1", "div_spi_pre1", SRC_MASK_PERIL1, 20,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_SPI2, "sclk_spi2", "div_spi_pre2", SRC_MASK_PERIL1, 24,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_FIMC0, "fimc0", "aclk160", GATE_IP_CAM, 0,
			0, 0),
	GATE(CLK_FIMC1, "fimc1", "aclk160", GATE_IP_CAM, 1,
			0, 0),
	GATE(CLK_FIMC2, "fimc2", "aclk160", GATE_IP_CAM, 2,
			0, 0),
	GATE(CLK_FIMC3, "fimc3", "aclk160", GATE_IP_CAM, 3,
			0, 0),
	GATE(CLK_CSIS0, "csis0", "aclk160", GATE_IP_CAM, 4,
			0, 0),
	GATE(CLK_CSIS1, "csis1", "aclk160", GATE_IP_CAM, 5,
			0, 0),
	GATE(CLK_SMMU_FIMC0, "smmu_fimc0", "aclk160", GATE_IP_CAM, 7,
			0, 0),
	GATE(CLK_SMMU_FIMC1, "smmu_fimc1", "aclk160", GATE_IP_CAM, 8,
			0, 0),
	GATE(CLK_SMMU_FIMC2, "smmu_fimc2", "aclk160", GATE_IP_CAM, 9,
			0, 0),
	GATE(CLK_SMMU_FIMC3, "smmu_fimc3", "aclk160", GATE_IP_CAM, 10,
			0, 0),
	GATE(CLK_SMMU_JPEG, "smmu_jpeg", "aclk160", GATE_IP_CAM, 11,
			0, 0),
	GATE(CLK_PPMUCAMIF, "ppmucamif", "aclk160", GATE_IP_CAM, 16, 0, 0),
	GATE(CLK_PIXELASYNCM0, "pxl_async0", "aclk160", GATE_IP_CAM, 17, 0, 0),
	GATE(CLK_PIXELASYNCM1, "pxl_async1", "aclk160", GATE_IP_CAM, 18, 0, 0),
	GATE(CLK_SMMU_TV, "smmu_tv", "aclk160", GATE_IP_TV, 4,
			0, 0),
	GATE(CLK_PPMUTV, "ppmutv", "aclk160", GATE_IP_TV, 5, 0, 0),
	GATE(CLK_MFC, "mfc", "aclk100", GATE_IP_MFC, 0, 0, 0),
	GATE(CLK_SMMU_MFCL, "smmu_mfcl", "aclk100", GATE_IP_MFC, 1,
			0, 0),
	GATE(CLK_SMMU_MFCR, "smmu_mfcr", "aclk100", GATE_IP_MFC, 2,
			0, 0),
	GATE(CLK_PPMUMFC_L, "ppmumfc_l", "aclk100", GATE_IP_MFC, 3, 0, 0),
	GATE(CLK_PPMUMFC_R, "ppmumfc_r", "aclk100", GATE_IP_MFC, 4, 0, 0),
	GATE(CLK_FIMD0, "fimd0", "aclk160", GATE_IP_LCD0, 0,
			0, 0),
	GATE(CLK_SMMU_FIMD0, "smmu_fimd0", "aclk160", GATE_IP_LCD0, 4,
			0, 0),
	GATE(CLK_PPMULCD0, "ppmulcd0", "aclk160", GATE_IP_LCD0, 5, 0, 0),
	GATE(CLK_PDMA0, "pdma0", "aclk133", GATE_IP_FSYS, 0,
			0, 0),
	GATE(CLK_PDMA1, "pdma1", "aclk133", GATE_IP_FSYS, 1,
			0, 0),
	GATE(CLK_SDMMC0, "sdmmc0", "aclk133", GATE_IP_FSYS, 5,
			0, 0),
	GATE(CLK_SDMMC1, "sdmmc1", "aclk133", GATE_IP_FSYS, 6,
			0, 0),
	GATE(CLK_SDMMC2, "sdmmc2", "aclk133", GATE_IP_FSYS, 7,
			0, 0),
	GATE(CLK_SDMMC3, "sdmmc3", "aclk133", GATE_IP_FSYS, 8,
			0, 0),
	GATE(CLK_PPMUFILE, "ppmufile", "aclk133", GATE_IP_FSYS, 17, 0, 0),
	GATE(CLK_UART0, "uart0", "aclk100", GATE_IP_PERIL, 0,
			0, 0),
	GATE(CLK_UART1, "uart1", "aclk100", GATE_IP_PERIL, 1,
			0, 0),
	GATE(CLK_UART2, "uart2", "aclk100", GATE_IP_PERIL, 2,
			0, 0),
	GATE(CLK_UART3, "uart3", "aclk100", GATE_IP_PERIL, 3,
			0, 0),
	GATE(CLK_UART4, "uart4", "aclk100", GATE_IP_PERIL, 4,
			0, 0),
	GATE(CLK_I2C0, "i2c0", "aclk100", GATE_IP_PERIL, 6,
			0, 0),
	GATE(CLK_I2C1, "i2c1", "aclk100", GATE_IP_PERIL, 7,
			0, 0),
	GATE(CLK_I2C2, "i2c2", "aclk100", GATE_IP_PERIL, 8,
			0, 0),
	GATE(CLK_I2C3, "i2c3", "aclk100", GATE_IP_PERIL, 9,
			0, 0),
	GATE(CLK_I2C4, "i2c4", "aclk100", GATE_IP_PERIL, 10,
			0, 0),
	GATE(CLK_I2C5, "i2c5", "aclk100", GATE_IP_PERIL, 11,
			0, 0),
	GATE(CLK_I2C6, "i2c6", "aclk100", GATE_IP_PERIL, 12,
			0, 0),
	GATE(CLK_I2C7, "i2c7", "aclk100", GATE_IP_PERIL, 13,
			0, 0),
	GATE(CLK_I2C_HDMI, "i2c-hdmi", "aclk100", GATE_IP_PERIL, 14,
			0, 0),
	GATE(CLK_SPI0, "spi0", "aclk100", GATE_IP_PERIL, 16,
			0, 0),
	GATE(CLK_SPI1, "spi1", "aclk100", GATE_IP_PERIL, 17,
			0, 0),
	GATE(CLK_SPI2, "spi2", "aclk100", GATE_IP_PERIL, 18,
			0, 0),
	GATE(CLK_I2S1, "i2s1", "aclk100", GATE_IP_PERIL, 20,
			0, 0),
	GATE(CLK_I2S2, "i2s2", "aclk100", GATE_IP_PERIL, 21,
			0, 0),
	GATE(CLK_PCM1, "pcm1", "aclk100", GATE_IP_PERIL, 22,
			0, 0),
	GATE(CLK_PCM2, "pcm2", "aclk100", GATE_IP_PERIL, 23,
			0, 0),
	GATE(CLK_SPDIF, "spdif", "aclk100", GATE_IP_PERIL, 26,
			0, 0),
	GATE(CLK_AC97, "ac97", "aclk100", GATE_IP_PERIL, 27,
			0, 0),
	GATE(CLK_SSS, "sss", "aclk133", GATE_IP_DMC, 4, 0, 0),
	GATE(CLK_PPMUDMC0, "ppmudmc0", "aclk133", GATE_IP_DMC, 8, 0, 0),
	GATE(CLK_PPMUDMC1, "ppmudmc1", "aclk133", GATE_IP_DMC, 9, 0, 0),
	GATE(CLK_PPMUCPU, "ppmucpu", "aclk133", GATE_IP_DMC, 10, 0, 0),
	GATE(CLK_PPMUACP, "ppmuacp", "aclk133", GATE_IP_DMC, 16, 0, 0),

	GATE(CLK_OUT_LEFTBUS, "clkout_leftbus", "div_clkout_leftbus",
			CLKOUT_CMU_LEFTBUS, 16, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_OUT_RIGHTBUS, "clkout_rightbus", "div_clkout_rightbus",
			CLKOUT_CMU_RIGHTBUS, 16, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_OUT_TOP, "clkout_top", "div_clkout_top",
			CLKOUT_CMU_TOP, 16, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_OUT_DMC, "clkout_dmc", "div_clkout_dmc",
			CLKOUT_CMU_DMC, 16, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_OUT_CPU, "clkout_cpu", "div_clkout_cpu",
			CLKOUT_CMU_CPU, 16, CLK_SET_RATE_PARENT, 0),
};

/* list of gate clocks supported in exynos4210 soc */
static const struct samsung_gate_clock exynos4210_gate_clks[] __initconst = {
	GATE(CLK_TVENC, "tvenc", "aclk160", GATE_IP_TV, 2, 0, 0),
	GATE(CLK_G2D, "g2d", "aclk200", E4210_GATE_IP_IMAGE, 0, 0, 0),
	GATE(CLK_ROTATOR, "rotator", "aclk200", E4210_GATE_IP_IMAGE, 1, 0, 0),
	GATE(CLK_MDMA, "mdma", "aclk200", E4210_GATE_IP_IMAGE, 2, 0, 0),
	GATE(CLK_SMMU_G2D, "smmu_g2d", "aclk200", E4210_GATE_IP_IMAGE, 3, 0, 0),
	GATE(CLK_SMMU_MDMA, "smmu_mdma", "aclk200", E4210_GATE_IP_IMAGE, 5, 0,
		0),
	GATE(CLK_PPMUIMAGE, "ppmuimage", "aclk200", E4210_GATE_IP_IMAGE, 9, 0,
		0),
	GATE(CLK_PPMULCD1, "ppmulcd1", "aclk160", E4210_GATE_IP_LCD1, 5, 0, 0),
	GATE(CLK_PCIE_PHY, "pcie_phy", "aclk133", GATE_IP_FSYS, 2, 0, 0),
	GATE(CLK_SATA_PHY, "sata_phy", "aclk133", GATE_IP_FSYS, 3, 0, 0),
	GATE(CLK_SATA, "sata", "aclk133", GATE_IP_FSYS, 10, 0, 0),
	GATE(CLK_PCIE, "pcie", "aclk133", GATE_IP_FSYS, 14, 0, 0),
	GATE(CLK_SMMU_PCIE, "smmu_pcie", "aclk133", GATE_IP_FSYS, 18, 0, 0),
	GATE(CLK_MODEMIF, "modemif", "aclk100", GATE_IP_PERIL, 28, 0, 0),
	GATE(CLK_CHIPID, "chipid", "aclk100", E4210_GATE_IP_PERIR, 0, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_SYSREG, "sysreg", "aclk100", E4210_GATE_IP_PERIR, 0,
			CLK_IGNORE_UNUSED, 0),
	GATE(CLK_HDMI_CEC, "hdmi_cec", "aclk100", E4210_GATE_IP_PERIR, 11, 0,
		0),
	GATE(CLK_SMMU_ROTATOR, "smmu_rotator", "aclk200",
			E4210_GATE_IP_IMAGE, 4, 0, 0),
	GATE(CLK_SCLK_MIPI1, "sclk_mipi1", "div_mipi_pre1",
			E4210_SRC_MASK_LCD1, 12, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_SATA, "sclk_sata", "div_sata",
			SRC_MASK_FSYS, 24, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MIXER, "sclk_mixer", "mout_mixer", SRC_MASK_TV, 4, 0, 0),
	GATE(CLK_SCLK_DAC, "sclk_dac", "mout_dac", SRC_MASK_TV, 8, 0, 0),
	GATE(CLK_TSADC, "tsadc", "aclk100", GATE_IP_PERIL, 15,
			0, 0),
	GATE(CLK_MCT, "mct", "aclk100", E4210_GATE_IP_PERIR, 13,
			0, 0),
	GATE(CLK_WDT, "watchdog", "aclk100", E4210_GATE_IP_PERIR, 14,
			0, 0),
	GATE(CLK_RTC, "rtc", "aclk100", E4210_GATE_IP_PERIR, 15,
			0, 0),
	GATE(CLK_KEYIF, "keyif", "aclk100", E4210_GATE_IP_PERIR, 16,
			0, 0),
	GATE(CLK_SCLK_FIMD1, "sclk_fimd1", "div_fimd1", E4210_SRC_MASK_LCD1, 0,
			CLK_SET_RATE_PARENT, 0),
	GATE(CLK_TMU_APBIF, "tmu_apbif", "aclk100", E4210_GATE_IP_PERIR, 17, 0,
		0),
};

/* list of gate clocks supported in exynos4x12 soc */
static const struct samsung_gate_clock exynos4x12_gate_clks[] __initconst = {
	GATE(CLK_AUDSS, "audss", "sclk_epll", E4X12_GATE_IP_MAUDIO, 0, 0, 0),
	GATE(CLK_MDNIE0, "mdnie0", "aclk160", GATE_IP_LCD0, 2, 0, 0),
	GATE(CLK_ROTATOR, "rotator", "aclk200", E4X12_GATE_IP_IMAGE, 1, 0, 0),
	GATE(CLK_MDMA, "mdma", "aclk200", E4X12_GATE_IP_IMAGE, 2, 0, 0),
	GATE(CLK_SMMU_MDMA, "smmu_mdma", "aclk200", E4X12_GATE_IP_IMAGE, 5, 0,
		0),
	GATE(CLK_PPMUIMAGE, "ppmuimage", "aclk200", E4X12_GATE_IP_IMAGE, 9, 0,
		0),
	GATE(CLK_TSADC, "tsadc", "aclk133", E4X12_GATE_BUS_FSYS1, 16, 0, 0),
	GATE(CLK_MIPI_HSI, "mipi_hsi", "aclk133", GATE_IP_FSYS, 10, 0, 0),
	GATE(CLK_CHIPID, "chipid", "aclk100", E4X12_GATE_IP_PERIR, 0, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_SYSREG, "sysreg", "aclk100", E4X12_GATE_IP_PERIR, 1,
			CLK_IGNORE_UNUSED, 0),
	GATE(CLK_HDMI_CEC, "hdmi_cec", "aclk100", E4X12_GATE_IP_PERIR, 11, 0,
		0),
	GATE(CLK_SCLK_MDNIE0, "sclk_mdnie0", "div_mdnie0",
			SRC_MASK_LCD0, 4, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MDNIE_PWM0, "sclk_mdnie_pwm0", "div_mdnie_pwm_pre0",
			SRC_MASK_LCD0, 8, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SCLK_MIPIHSI, "sclk_mipihsi", "div_mipihsi",
			SRC_MASK_FSYS, 24, CLK_SET_RATE_PARENT, 0),
	GATE(CLK_SMMU_ROTATOR, "smmu_rotator", "aclk200",
			E4X12_GATE_IP_IMAGE, 4, 0, 0),
	GATE(CLK_MCT, "mct", "aclk100", E4X12_GATE_IP_PERIR, 13,
			0, 0),
	GATE(CLK_RTC, "rtc", "aclk100", E4X12_GATE_IP_PERIR, 15,
			0, 0),
	GATE(CLK_KEYIF, "keyif", "aclk100", E4X12_GATE_IP_PERIR, 16, 0, 0),
	GATE(CLK_PWM_ISP_SCLK, "pwm_isp_sclk", "div_pwm_isp",
			E4X12_GATE_IP_ISP, 0, 0, 0),
	GATE(CLK_SPI0_ISP_SCLK, "spi0_isp_sclk", "div_spi0_isp_pre",
			E4X12_GATE_IP_ISP, 1, 0, 0),
	GATE(CLK_SPI1_ISP_SCLK, "spi1_isp_sclk", "div_spi1_isp_pre",
			E4X12_GATE_IP_ISP, 2, 0, 0),
	GATE(CLK_UART_ISP_SCLK, "uart_isp_sclk", "div_uart_isp",
			E4X12_GATE_IP_ISP, 3, 0, 0),
	GATE(CLK_WDT, "watchdog", "aclk100", E4X12_GATE_IP_PERIR, 14, 0, 0),
	GATE(CLK_PCM0, "pcm0", "aclk100", E4X12_GATE_IP_MAUDIO, 2,
			0, 0),
	GATE(CLK_I2S0, "i2s0", "aclk100", E4X12_GATE_IP_MAUDIO, 3,
			0, 0),
	GATE(CLK_FIMC_ISP, "isp", "aclk200", E4X12_GATE_ISP0, 0,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_FIMC_DRC, "drc", "aclk200", E4X12_GATE_ISP0, 1,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_FIMC_FD, "fd", "aclk200", E4X12_GATE_ISP0, 2,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_FIMC_LITE0, "lite0", "aclk200", E4X12_GATE_ISP0, 3,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_FIMC_LITE1, "lite1", "aclk200", E4X12_GATE_ISP0, 4,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_MCUISP, "mcuisp", "aclk200", E4X12_GATE_ISP0, 5,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_GICISP, "gicisp", "aclk200", E4X12_GATE_ISP0, 7,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_ISP, "smmu_isp", "aclk200", E4X12_GATE_ISP0, 8,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_DRC, "smmu_drc", "aclk200", E4X12_GATE_ISP0, 9,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_FD, "smmu_fd", "aclk200", E4X12_GATE_ISP0, 10,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_LITE0, "smmu_lite0", "aclk200", E4X12_GATE_ISP0, 11,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_LITE1, "smmu_lite1", "aclk200", E4X12_GATE_ISP0, 12,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_PPMUISPMX, "ppmuispmx", "aclk200", E4X12_GATE_ISP0, 20,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_PPMUISPX, "ppmuispx", "aclk200", E4X12_GATE_ISP0, 21,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_MCUCTL_ISP, "mcuctl_isp", "aclk200", E4X12_GATE_ISP0, 23,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_MPWM_ISP, "mpwm_isp", "aclk200", E4X12_GATE_ISP0, 24,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_I2C0_ISP, "i2c0_isp", "aclk200", E4X12_GATE_ISP0, 25,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_I2C1_ISP, "i2c1_isp", "aclk200", E4X12_GATE_ISP0, 26,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_MTCADC_ISP, "mtcadc_isp", "aclk200", E4X12_GATE_ISP0, 27,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_PWM_ISP, "pwm_isp", "aclk200", E4X12_GATE_ISP0, 28,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_WDT_ISP, "wdt_isp", "aclk200", E4X12_GATE_ISP0, 30,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_UART_ISP, "uart_isp", "aclk200", E4X12_GATE_ISP0, 31,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_ASYNCAXIM, "asyncaxim", "aclk200", E4X12_GATE_ISP1, 0,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SMMU_ISPCX, "smmu_ispcx", "aclk200", E4X12_GATE_ISP1, 4,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SPI0_ISP, "spi0_isp", "aclk200", E4X12_GATE_ISP1, 12,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_SPI1_ISP, "spi1_isp", "aclk200", E4X12_GATE_ISP1, 13,
			CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE, 0),
	GATE(CLK_G2D, "g2d", "aclk200", GATE_IP_DMC, 23, 0, 0),
	GATE(CLK_SMMU_G2D, "smmu_g2d", "aclk200", GATE_IP_DMC, 24, 0, 0),
	GATE(CLK_TMU_APBIF, "tmu_apbif", "aclk100", E4X12_GATE_IP_PERIR, 17, 0,
		0),
};

static const struct samsung_clock_alias exynos4_aliases[] __initconst = {
	ALIAS(CLK_MOUT_CORE, NULL, "moutcore"),
	ALIAS(CLK_ARM_CLK, NULL, "armclk"),
	ALIAS(CLK_SCLK_APLL, NULL, "mout_apll"),
};

static const struct samsung_clock_alias exynos4210_aliases[] __initconst = {
	ALIAS(CLK_SCLK_MPLL, NULL, "mout_mpll"),
};

static const struct samsung_clock_alias exynos4x12_aliases[] __initconst = {
	ALIAS(CLK_MOUT_MPLL_USER_C, NULL, "mout_mpll"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * The parent of the fin_pll clock is selected by the XOM[0] bit. This bit
 * resides in chipid register space, outside of the clock controller memory
 * mapped space. So to determine the parent of fin_pll clock, the chipid
 * controller is first remapped and the value of XOM[0] bit is read to
 * determine the parent clock.
 */
<<<<<<< HEAD
static unsigned long exynos4_get_xom(void)
=======
static unsigned long __init exynos4_get_xom(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long xom = 0;
	void __iomem *chipid_base;
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "samsung,exynos4210-chipid");
	if (np) {
		chipid_base = of_iomap(np, 0);

		if (chipid_base)
			xom = readl(chipid_base + 8);

		iounmap(chipid_base);
<<<<<<< HEAD
=======
		of_node_put(np);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return xom;
}

<<<<<<< HEAD
static void __init exynos4_clk_register_finpll(unsigned long xom)
=======
static void __init exynos4_clk_register_finpll(struct samsung_clk_provider *ctx)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct samsung_fixed_rate_clock fclk;
	struct clk *clk;
	unsigned long finpll_f = 24000000;
	char *parent_name;
<<<<<<< HEAD
=======
	unsigned int xom = exynos4_get_xom();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	parent_name = xom & 1 ? "xusbxti" : "xxti";
	clk = clk_get(NULL, parent_name);
	if (IS_ERR(clk)) {
		pr_err("%s: failed to lookup parent clock %s, assuming "
			"fin_pll clock frequency is 24MHz\n", __func__,
			parent_name);
	} else {
		finpll_f = clk_get_rate(clk);
	}

<<<<<<< HEAD
	fclk.id = fin_pll;
	fclk.name = "fin_pll";
	fclk.parent_name = NULL;
	fclk.flags = CLK_IS_ROOT;
	fclk.fixed_rate = finpll_f;
	samsung_clk_register_fixed_rate(&fclk, 1);

}

/*
 * This function allows non-dt platforms to specify the clock speed of the
 * xxti and xusbxti clocks. These clocks are then registered with the specified
 * clock speed.
 */
void __init exynos4_clk_register_fixed_ext(unsigned long xxti_f,
						unsigned long xusbxti_f)
{
	exynos4_fixed_rate_ext_clks[0].fixed_rate = xxti_f;
	exynos4_fixed_rate_ext_clks[1].fixed_rate = xusbxti_f;
	samsung_clk_register_fixed_rate(exynos4_fixed_rate_ext_clks,
			ARRAY_SIZE(exynos4_fixed_rate_ext_clks));
}

static __initdata struct of_device_id ext_clk_match[] = {
=======
	fclk.id = CLK_FIN_PLL;
	fclk.name = "fin_pll";
	fclk.parent_name = NULL;
	fclk.flags = 0;
	fclk.fixed_rate = finpll_f;
	samsung_clk_register_fixed_rate(ctx, &fclk, 1);

}

static const struct of_device_id ext_clk_match[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ .compatible = "samsung,clock-xxti", .data = (void *)0, },
	{ .compatible = "samsung,clock-xusbxti", .data = (void *)1, },
	{},
};

<<<<<<< HEAD
/* register exynos4 clocks */
void __init exynos4_clk_init(struct device_node *np, enum exynos4_soc exynos4_soc, void __iomem *reg_base, unsigned long xom)
{
	struct clk *apll, *mpll, *epll, *vpll;

	if (np) {
		reg_base = of_iomap(np, 0);
		if (!reg_base)
			panic("%s: failed to map registers\n", __func__);
	}

	if (exynos4_soc == EXYNOS4210)
		samsung_clk_init(np, reg_base, nr_clks,
			exynos4_clk_regs, ARRAY_SIZE(exynos4_clk_regs),
			exynos4210_clk_save, ARRAY_SIZE(exynos4210_clk_save));
	else
		samsung_clk_init(np, reg_base, nr_clks,
			exynos4_clk_regs, ARRAY_SIZE(exynos4_clk_regs),
			exynos4x12_clk_save, ARRAY_SIZE(exynos4x12_clk_save));

	if (np)
		samsung_clk_of_register_fixed_ext(exynos4_fixed_rate_ext_clks,
			ARRAY_SIZE(exynos4_fixed_rate_ext_clks),
			ext_clk_match);

	exynos4_clk_register_finpll(xom);

	if (exynos4_soc == EXYNOS4210) {
		apll = samsung_clk_register_pll45xx("fout_apll", "fin_pll",
					reg_base + APLL_CON0, pll_4508);
		mpll = samsung_clk_register_pll45xx("fout_mpll", "fin_pll",
					reg_base + E4210_MPLL_CON0, pll_4508);
		epll = samsung_clk_register_pll46xx("fout_epll", "fin_pll",
					reg_base + EPLL_CON0, pll_4600);
		vpll = samsung_clk_register_pll46xx("fout_vpll", "mout_vpllsrc",
					reg_base + VPLL_CON0, pll_4650c);
	} else {
		apll = samsung_clk_register_pll35xx("fout_apll", "fin_pll",
					reg_base + APLL_CON0);
		mpll = samsung_clk_register_pll35xx("fout_mpll", "fin_pll",
					reg_base + E4X12_MPLL_CON0);
		epll = samsung_clk_register_pll36xx("fout_epll", "fin_pll",
					reg_base + EPLL_CON0);
		vpll = samsung_clk_register_pll36xx("fout_vpll", "fin_pll",
					reg_base + VPLL_CON0);
	}

	samsung_clk_add_lookup(apll, fout_apll);
	samsung_clk_add_lookup(mpll, fout_mpll);
	samsung_clk_add_lookup(epll, fout_epll);
	samsung_clk_add_lookup(vpll, fout_vpll);

	samsung_clk_register_fixed_rate(exynos4_fixed_rate_clks,
			ARRAY_SIZE(exynos4_fixed_rate_clks));
	samsung_clk_register_mux(exynos4_mux_clks,
			ARRAY_SIZE(exynos4_mux_clks));
	samsung_clk_register_div(exynos4_div_clks,
			ARRAY_SIZE(exynos4_div_clks));
	samsung_clk_register_gate(exynos4_gate_clks,
			ARRAY_SIZE(exynos4_gate_clks));

	if (exynos4_soc == EXYNOS4210) {
		samsung_clk_register_fixed_rate(exynos4210_fixed_rate_clks,
			ARRAY_SIZE(exynos4210_fixed_rate_clks));
		samsung_clk_register_mux(exynos4210_mux_clks,
			ARRAY_SIZE(exynos4210_mux_clks));
		samsung_clk_register_div(exynos4210_div_clks,
			ARRAY_SIZE(exynos4210_div_clks));
		samsung_clk_register_gate(exynos4210_gate_clks,
			ARRAY_SIZE(exynos4210_gate_clks));
	} else {
		samsung_clk_register_mux(exynos4x12_mux_clks,
			ARRAY_SIZE(exynos4x12_mux_clks));
		samsung_clk_register_div(exynos4x12_div_clks,
			ARRAY_SIZE(exynos4x12_div_clks));
		samsung_clk_register_gate(exynos4x12_gate_clks,
			ARRAY_SIZE(exynos4x12_gate_clks));
	}

=======
/* PLLs PMS values */
static const struct samsung_pll_rate_table exynos4210_apll_rates[] __initconst = {
	PLL_45XX_RATE(1200000000, 150,  3, 1, 28),
	PLL_45XX_RATE(1000000000, 250,  6, 1, 28),
	PLL_45XX_RATE( 800000000, 200,  6, 1, 28),
	PLL_45XX_RATE( 666857142, 389, 14, 1, 13),
	PLL_45XX_RATE( 600000000, 100,  4, 1, 13),
	PLL_45XX_RATE( 533000000, 533, 24, 1,  5),
	PLL_45XX_RATE( 500000000, 250,  6, 2, 28),
	PLL_45XX_RATE( 400000000, 200,  6, 2, 28),
	PLL_45XX_RATE( 200000000, 200,  6, 3, 28),
	{ /* sentinel */ }
};

static const struct samsung_pll_rate_table exynos4210_epll_rates[] __initconst = {
	PLL_4600_RATE(192000000, 48, 3, 1,     0, 0),
	PLL_4600_RATE(180633605, 45, 3, 1, 10381, 0),
	PLL_4600_RATE(180000000, 45, 3, 1,     0, 0),
	PLL_4600_RATE( 73727996, 73, 3, 3, 47710, 1),
	PLL_4600_RATE( 67737602, 90, 4, 3, 20762, 1),
	PLL_4600_RATE( 49151992, 49, 3, 3,  9961, 0),
	PLL_4600_RATE( 45158401, 45, 3, 3, 10381, 0),
	{ /* sentinel */ }
};

static const struct samsung_pll_rate_table exynos4210_vpll_rates[] __initconst = {
	PLL_4650_RATE(360000000, 44, 3, 0, 1024, 0, 14, 0),
	PLL_4650_RATE(324000000, 53, 2, 1, 1024, 1,  1, 1),
	PLL_4650_RATE(259617187, 63, 3, 1, 1950, 0, 20, 1),
	PLL_4650_RATE(110000000, 53, 3, 2, 2048, 0, 17, 0),
	PLL_4650_RATE( 55360351, 53, 3, 3, 2417, 0, 17, 0),
	{ /* sentinel */ }
};

static const struct samsung_pll_rate_table exynos4x12_apll_rates[] __initconst = {
	PLL_35XX_RATE(1500000000, 250, 4, 0),
	PLL_35XX_RATE(1400000000, 175, 3, 0),
	PLL_35XX_RATE(1300000000, 325, 6, 0),
	PLL_35XX_RATE(1200000000, 200, 4, 0),
	PLL_35XX_RATE(1100000000, 275, 6, 0),
	PLL_35XX_RATE(1000000000, 125, 3, 0),
	PLL_35XX_RATE( 900000000, 150, 4, 0),
	PLL_35XX_RATE( 800000000, 100, 3, 0),
	PLL_35XX_RATE( 700000000, 175, 3, 1),
	PLL_35XX_RATE( 600000000, 200, 4, 1),
	PLL_35XX_RATE( 500000000, 125, 3, 1),
	PLL_35XX_RATE( 400000000, 100, 3, 1),
	PLL_35XX_RATE( 300000000, 200, 4, 2),
	PLL_35XX_RATE( 200000000, 100, 3, 2),
	{ /* sentinel */ }
};

static const struct samsung_pll_rate_table exynos4x12_epll_rates[] __initconst = {
	PLL_36XX_RATE(192000000, 48, 3, 1,     0),
	PLL_36XX_RATE(180633605, 45, 3, 1, 10381),
	PLL_36XX_RATE(180000000, 45, 3, 1,     0),
	PLL_36XX_RATE( 73727996, 73, 3, 3, 47710),
	PLL_36XX_RATE( 67737602, 90, 4, 3, 20762),
	PLL_36XX_RATE( 49151992, 49, 3, 3,  9961),
	PLL_36XX_RATE( 45158401, 45, 3, 3, 10381),
	{ /* sentinel */ }
};

static const struct samsung_pll_rate_table exynos4x12_vpll_rates[] __initconst = {
	PLL_36XX_RATE(533000000, 133, 3, 1, 16384),
	PLL_36XX_RATE(440000000, 110, 3, 1,     0),
	PLL_36XX_RATE(350000000, 175, 3, 2,     0),
	PLL_36XX_RATE(266000000, 133, 3, 2,     0),
	PLL_36XX_RATE(160000000, 160, 3, 3,     0),
	PLL_36XX_RATE(106031250,  53, 3, 2,  1024),
	PLL_36XX_RATE( 53015625,  53, 3, 3,  1024),
	{ /* sentinel */ }
};

static struct samsung_pll_clock exynos4210_plls[nr_plls] __initdata = {
	[apll] = PLL_A(pll_4508, CLK_FOUT_APLL, "fout_apll", "fin_pll",
		APLL_LOCK, APLL_CON0, "fout_apll", NULL),
	[mpll] = PLL_A(pll_4508, CLK_FOUT_MPLL, "fout_mpll", "fin_pll",
		E4210_MPLL_LOCK, E4210_MPLL_CON0, "fout_mpll", NULL),
	[epll] = PLL_A(pll_4600, CLK_FOUT_EPLL, "fout_epll", "fin_pll",
		EPLL_LOCK, EPLL_CON0, "fout_epll", NULL),
	[vpll] = PLL_A(pll_4650c, CLK_FOUT_VPLL, "fout_vpll", "mout_vpllsrc",
		VPLL_LOCK, VPLL_CON0, "fout_vpll", NULL),
};

static struct samsung_pll_clock exynos4x12_plls[nr_plls] __initdata = {
	[apll] = PLL(pll_35xx, CLK_FOUT_APLL, "fout_apll", "fin_pll",
			APLL_LOCK, APLL_CON0, NULL),
	[mpll] = PLL(pll_35xx, CLK_FOUT_MPLL, "fout_mpll", "fin_pll",
			E4X12_MPLL_LOCK, E4X12_MPLL_CON0, NULL),
	[epll] = PLL(pll_36xx, CLK_FOUT_EPLL, "fout_epll", "fin_pll",
			EPLL_LOCK, EPLL_CON0, NULL),
	[vpll] = PLL(pll_36xx, CLK_FOUT_VPLL, "fout_vpll", "fin_pll",
			VPLL_LOCK, VPLL_CON0, NULL),
};

static void __init exynos4x12_core_down_clock(void)
{
	unsigned int tmp;

	/*
	 * Enable arm clock down (in idle) and set arm divider
	 * ratios in WFI/WFE state.
	 */
	tmp = (PWR_CTRL1_CORE2_DOWN_RATIO(7) | PWR_CTRL1_CORE1_DOWN_RATIO(7) |
		PWR_CTRL1_DIV2_DOWN_EN | PWR_CTRL1_DIV1_DOWN_EN |
		PWR_CTRL1_USE_CORE1_WFE | PWR_CTRL1_USE_CORE0_WFE |
		PWR_CTRL1_USE_CORE1_WFI | PWR_CTRL1_USE_CORE0_WFI);
	/* On Exynos4412 enable it also on core 2 and 3 */
	if (num_possible_cpus() == 4)
		tmp |= PWR_CTRL1_USE_CORE3_WFE | PWR_CTRL1_USE_CORE2_WFE |
		       PWR_CTRL1_USE_CORE3_WFI | PWR_CTRL1_USE_CORE2_WFI;
	writel_relaxed(tmp, reg_base + PWR_CTRL1);

	/*
	 * Disable the clock up feature in case it was enabled by bootloader.
	 */
	writel_relaxed(0x0, reg_base + E4X12_PWR_CTRL2);
}

#define E4210_CPU_DIV0(apll, pclk_dbg, atb, periph, corem1, corem0)	\
		(((apll) << 24) | ((pclk_dbg) << 20) | ((atb) << 16) |	\
		((periph) << 12) | ((corem1) << 8) | ((corem0) <<  4))
#define E4210_CPU_DIV1(hpm, copy)					\
		(((hpm) << 4) | ((copy) << 0))

static const struct exynos_cpuclk_cfg_data e4210_armclk_d[] __initconst = {
	{ 1200000, E4210_CPU_DIV0(7, 1, 4, 3, 7, 3), E4210_CPU_DIV1(0, 5), },
	{ 1000000, E4210_CPU_DIV0(7, 1, 4, 3, 7, 3), E4210_CPU_DIV1(0, 4), },
	{  800000, E4210_CPU_DIV0(7, 1, 3, 3, 7, 3), E4210_CPU_DIV1(0, 3), },
	{  500000, E4210_CPU_DIV0(7, 1, 3, 3, 7, 3), E4210_CPU_DIV1(0, 3), },
	{  400000, E4210_CPU_DIV0(7, 1, 3, 3, 7, 3), E4210_CPU_DIV1(0, 3), },
	{  200000, E4210_CPU_DIV0(0, 1, 1, 1, 3, 1), E4210_CPU_DIV1(0, 3), },
	{  0 },
};

static const struct exynos_cpuclk_cfg_data e4212_armclk_d[] __initconst = {
	{ 1500000, E4210_CPU_DIV0(2, 1, 6, 0, 7, 3), E4210_CPU_DIV1(2, 6), },
	{ 1400000, E4210_CPU_DIV0(2, 1, 6, 0, 7, 3), E4210_CPU_DIV1(2, 6), },
	{ 1300000, E4210_CPU_DIV0(2, 1, 5, 0, 7, 3), E4210_CPU_DIV1(2, 5), },
	{ 1200000, E4210_CPU_DIV0(2, 1, 5, 0, 7, 3), E4210_CPU_DIV1(2, 5), },
	{ 1100000, E4210_CPU_DIV0(2, 1, 4, 0, 6, 3), E4210_CPU_DIV1(2, 4), },
	{ 1000000, E4210_CPU_DIV0(1, 1, 4, 0, 5, 2), E4210_CPU_DIV1(2, 4), },
	{  900000, E4210_CPU_DIV0(1, 1, 3, 0, 5, 2), E4210_CPU_DIV1(2, 3), },
	{  800000, E4210_CPU_DIV0(1, 1, 3, 0, 5, 2), E4210_CPU_DIV1(2, 3), },
	{  700000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4210_CPU_DIV1(2, 3), },
	{  600000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4210_CPU_DIV1(2, 3), },
	{  500000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4210_CPU_DIV1(2, 3), },
	{  400000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4210_CPU_DIV1(2, 3), },
	{  300000, E4210_CPU_DIV0(1, 1, 2, 0, 4, 2), E4210_CPU_DIV1(2, 3), },
	{  200000, E4210_CPU_DIV0(1, 1, 1, 0, 3, 1), E4210_CPU_DIV1(2, 3), },
	{  0 },
};

#define E4412_CPU_DIV1(cores, hpm, copy)				\
		(((cores) << 8) | ((hpm) << 4) | ((copy) << 0))

static const struct exynos_cpuclk_cfg_data e4412_armclk_d[] __initconst = {
	{ 1500000, E4210_CPU_DIV0(2, 1, 6, 0, 7, 3), E4412_CPU_DIV1(7, 0, 6), },
	{ 1400000, E4210_CPU_DIV0(2, 1, 6, 0, 7, 3), E4412_CPU_DIV1(6, 0, 6), },
	{ 1300000, E4210_CPU_DIV0(2, 1, 5, 0, 7, 3), E4412_CPU_DIV1(6, 0, 5), },
	{ 1200000, E4210_CPU_DIV0(2, 1, 5, 0, 7, 3), E4412_CPU_DIV1(5, 0, 5), },
	{ 1100000, E4210_CPU_DIV0(2, 1, 4, 0, 6, 3), E4412_CPU_DIV1(5, 0, 4), },
	{ 1000000, E4210_CPU_DIV0(1, 1, 4, 0, 5, 2), E4412_CPU_DIV1(4, 0, 4), },
	{  900000, E4210_CPU_DIV0(1, 1, 3, 0, 5, 2), E4412_CPU_DIV1(4, 0, 3), },
	{  800000, E4210_CPU_DIV0(1, 1, 3, 0, 5, 2), E4412_CPU_DIV1(3, 0, 3), },
	{  700000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4412_CPU_DIV1(3, 0, 3), },
	{  600000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4412_CPU_DIV1(2, 0, 3), },
	{  500000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4412_CPU_DIV1(2, 0, 3), },
	{  400000, E4210_CPU_DIV0(1, 1, 3, 0, 4, 2), E4412_CPU_DIV1(1, 0, 3), },
	{  300000, E4210_CPU_DIV0(1, 1, 2, 0, 4, 2), E4412_CPU_DIV1(1, 0, 3), },
	{  200000, E4210_CPU_DIV0(1, 1, 1, 0, 3, 1), E4412_CPU_DIV1(0, 0, 3), },
	{  0 },
};

/* register exynos4 clocks */
static void __init exynos4_clk_init(struct device_node *np,
				    enum exynos4_soc soc)
{
	struct samsung_clk_provider *ctx;
	exynos4_soc = soc;

	reg_base = of_iomap(np, 0);
	if (!reg_base)
		panic("%s: failed to map registers\n", __func__);

	ctx = samsung_clk_init(np, reg_base, CLK_NR_CLKS);

	samsung_clk_of_register_fixed_ext(ctx, exynos4_fixed_rate_ext_clks,
			ARRAY_SIZE(exynos4_fixed_rate_ext_clks),
			ext_clk_match);

	exynos4_clk_register_finpll(ctx);

	if (exynos4_soc == EXYNOS4210) {
		samsung_clk_register_mux(ctx, exynos4210_mux_early,
					ARRAY_SIZE(exynos4210_mux_early));

		if (_get_rate("fin_pll") == 24000000) {
			exynos4210_plls[apll].rate_table =
							exynos4210_apll_rates;
			exynos4210_plls[epll].rate_table =
							exynos4210_epll_rates;
		}

		if (_get_rate("mout_vpllsrc") == 24000000)
			exynos4210_plls[vpll].rate_table =
							exynos4210_vpll_rates;

		samsung_clk_register_pll(ctx, exynos4210_plls,
					ARRAY_SIZE(exynos4210_plls), reg_base);
	} else {
		if (_get_rate("fin_pll") == 24000000) {
			exynos4x12_plls[apll].rate_table =
							exynos4x12_apll_rates;
			exynos4x12_plls[epll].rate_table =
							exynos4x12_epll_rates;
			exynos4x12_plls[vpll].rate_table =
							exynos4x12_vpll_rates;
		}

		samsung_clk_register_pll(ctx, exynos4x12_plls,
					ARRAY_SIZE(exynos4x12_plls), reg_base);
	}

	samsung_clk_register_fixed_rate(ctx, exynos4_fixed_rate_clks,
			ARRAY_SIZE(exynos4_fixed_rate_clks));
	samsung_clk_register_mux(ctx, exynos4_mux_clks,
			ARRAY_SIZE(exynos4_mux_clks));
	samsung_clk_register_div(ctx, exynos4_div_clks,
			ARRAY_SIZE(exynos4_div_clks));
	samsung_clk_register_gate(ctx, exynos4_gate_clks,
			ARRAY_SIZE(exynos4_gate_clks));
	samsung_clk_register_fixed_factor(ctx, exynos4_fixed_factor_clks,
			ARRAY_SIZE(exynos4_fixed_factor_clks));

	if (exynos4_soc == EXYNOS4210) {
		samsung_clk_register_fixed_rate(ctx, exynos4210_fixed_rate_clks,
			ARRAY_SIZE(exynos4210_fixed_rate_clks));
		samsung_clk_register_mux(ctx, exynos4210_mux_clks,
			ARRAY_SIZE(exynos4210_mux_clks));
		samsung_clk_register_div(ctx, exynos4210_div_clks,
			ARRAY_SIZE(exynos4210_div_clks));
		samsung_clk_register_gate(ctx, exynos4210_gate_clks,
			ARRAY_SIZE(exynos4210_gate_clks));
		samsung_clk_register_alias(ctx, exynos4210_aliases,
			ARRAY_SIZE(exynos4210_aliases));
		samsung_clk_register_fixed_factor(ctx,
			exynos4210_fixed_factor_clks,
			ARRAY_SIZE(exynos4210_fixed_factor_clks));
		exynos_register_cpu_clock(ctx, CLK_ARM_CLK, "armclk",
			mout_core_p4210[0], mout_core_p4210[1], 0x14200,
			e4210_armclk_d, ARRAY_SIZE(e4210_armclk_d),
			CLK_CPU_NEEDS_DEBUG_ALT_DIV | CLK_CPU_HAS_DIV1);
	} else {
		samsung_clk_register_mux(ctx, exynos4x12_mux_clks,
			ARRAY_SIZE(exynos4x12_mux_clks));
		samsung_clk_register_div(ctx, exynos4x12_div_clks,
			ARRAY_SIZE(exynos4x12_div_clks));
		samsung_clk_register_gate(ctx, exynos4x12_gate_clks,
			ARRAY_SIZE(exynos4x12_gate_clks));
		samsung_clk_register_alias(ctx, exynos4x12_aliases,
			ARRAY_SIZE(exynos4x12_aliases));
		samsung_clk_register_fixed_factor(ctx,
			exynos4x12_fixed_factor_clks,
			ARRAY_SIZE(exynos4x12_fixed_factor_clks));
		if (of_machine_is_compatible("samsung,exynos4412")) {
			exynos_register_cpu_clock(ctx, CLK_ARM_CLK, "armclk",
				mout_core_p4x12[0], mout_core_p4x12[1], 0x14200,
				e4412_armclk_d, ARRAY_SIZE(e4412_armclk_d),
				CLK_CPU_NEEDS_DEBUG_ALT_DIV | CLK_CPU_HAS_DIV1);
		} else {
			exynos_register_cpu_clock(ctx, CLK_ARM_CLK, "armclk",
				mout_core_p4x12[0], mout_core_p4x12[1], 0x14200,
				e4212_armclk_d, ARRAY_SIZE(e4212_armclk_d),
				CLK_CPU_NEEDS_DEBUG_ALT_DIV | CLK_CPU_HAS_DIV1);
		}
	}

	samsung_clk_register_alias(ctx, exynos4_aliases,
			ARRAY_SIZE(exynos4_aliases));

	if (soc == EXYNOS4X12)
		exynos4x12_core_down_clock();
	exynos4_clk_sleep_init();

	samsung_clk_of_add_provider(np, ctx);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pr_info("%s clocks: sclk_apll = %ld, sclk_mpll = %ld\n"
		"\tsclk_epll = %ld, sclk_vpll = %ld, arm_clk = %ld\n",
		exynos4_soc == EXYNOS4210 ? "Exynos4210" : "Exynos4x12",
		_get_rate("sclk_apll"),	_get_rate("sclk_mpll"),
		_get_rate("sclk_epll"), _get_rate("sclk_vpll"),
<<<<<<< HEAD
		_get_rate("arm_clk"));
=======
		_get_rate("div_core2"));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}


static void __init exynos4210_clk_init(struct device_node *np)
{
<<<<<<< HEAD
	exynos4_clk_init(np, EXYNOS4210, NULL, exynos4_get_xom());
=======
	exynos4_clk_init(np, EXYNOS4210);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
CLK_OF_DECLARE(exynos4210_clk, "samsung,exynos4210-clock", exynos4210_clk_init);

static void __init exynos4412_clk_init(struct device_node *np)
{
<<<<<<< HEAD
	exynos4_clk_init(np, EXYNOS4X12, NULL, exynos4_get_xom());
=======
	exynos4_clk_init(np, EXYNOS4X12);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
CLK_OF_DECLARE(exynos4412_clk, "samsung,exynos4412-clock", exynos4412_clk_init);
