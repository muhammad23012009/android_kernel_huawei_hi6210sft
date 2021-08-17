/*
 *  exynos_adc.c - Support for ADC in EXYNOS SoCs
 *
 *  8 ~ 10 channel, 10/12-bit ADC
 *
 *  Copyright (C) 2013 Naveen Krishna Chatradhi <ch.naveen@samsung.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
<<<<<<< HEAD
=======
#include <linux/errno.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/regulator/consumer.h>
#include <linux/of_platform.h>
<<<<<<< HEAD
=======
#include <linux/err.h>
#include <linux/input.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/iio/iio.h>
#include <linux/iio/machine.h>
#include <linux/iio/driver.h>
<<<<<<< HEAD

enum adc_version {
	ADC_V1,
	ADC_V2
};

/* EXYNOS4412/5250 ADC_V1 registers definitions */
#define ADC_V1_CON(x)		((x) + 0x00)
#define ADC_V1_DLY(x)		((x) + 0x08)
#define ADC_V1_DATX(x)		((x) + 0x0C)
#define ADC_V1_INTCLR(x)	((x) + 0x18)
#define ADC_V1_MUX(x)		((x) + 0x1c)
=======
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include <linux/platform_data/touchscreen-s3c2410.h>

/* S3C/EXYNOS4412/5250 ADC_V1 registers definitions */
#define ADC_V1_CON(x)		((x) + 0x00)
#define ADC_V1_TSC(x)		((x) + 0x04)
#define ADC_V1_DLY(x)		((x) + 0x08)
#define ADC_V1_DATX(x)		((x) + 0x0C)
#define ADC_V1_DATY(x)		((x) + 0x10)
#define ADC_V1_UPDN(x)		((x) + 0x14)
#define ADC_V1_INTCLR(x)	((x) + 0x18)
#define ADC_V1_MUX(x)		((x) + 0x1c)
#define ADC_V1_CLRINTPNDNUP(x)	((x) + 0x20)

/* S3C2410 ADC registers definitions */
#define ADC_S3C2410_MUX(x)	((x) + 0x18)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Future ADC_V2 registers definitions */
#define ADC_V2_CON1(x)		((x) + 0x00)
#define ADC_V2_CON2(x)		((x) + 0x04)
#define ADC_V2_STAT(x)		((x) + 0x08)
#define ADC_V2_INT_EN(x)	((x) + 0x10)
#define ADC_V2_INT_ST(x)	((x) + 0x14)
#define ADC_V2_VER(x)		((x) + 0x20)

/* Bit definitions for ADC_V1 */
#define ADC_V1_CON_RES		(1u << 16)
#define ADC_V1_CON_PRSCEN	(1u << 14)
#define ADC_V1_CON_PRSCLV(x)	(((x) & 0xFF) << 6)
#define ADC_V1_CON_STANDBY	(1u << 2)

<<<<<<< HEAD
=======
/* Bit definitions for S3C2410 ADC */
#define ADC_S3C2410_CON_SELMUX(x) (((x) & 7) << 3)
#define ADC_S3C2410_DATX_MASK	0x3FF
#define ADC_S3C2416_CON_RES_SEL	(1u << 3)

/* touch screen always uses channel 0 */
#define ADC_S3C2410_MUX_TS	0

/* ADCTSC Register Bits */
#define ADC_S3C2443_TSC_UD_SEN		(1u << 8)
#define ADC_S3C2410_TSC_YM_SEN		(1u << 7)
#define ADC_S3C2410_TSC_YP_SEN		(1u << 6)
#define ADC_S3C2410_TSC_XM_SEN		(1u << 5)
#define ADC_S3C2410_TSC_XP_SEN		(1u << 4)
#define ADC_S3C2410_TSC_PULL_UP_DISABLE	(1u << 3)
#define ADC_S3C2410_TSC_AUTO_PST	(1u << 2)
#define ADC_S3C2410_TSC_XY_PST(x)	(((x) & 0x3) << 0)

#define ADC_TSC_WAIT4INT (ADC_S3C2410_TSC_YM_SEN | \
			 ADC_S3C2410_TSC_YP_SEN | \
			 ADC_S3C2410_TSC_XP_SEN | \
			 ADC_S3C2410_TSC_XY_PST(3))

#define ADC_TSC_AUTOPST	(ADC_S3C2410_TSC_YM_SEN | \
			 ADC_S3C2410_TSC_YP_SEN | \
			 ADC_S3C2410_TSC_XP_SEN | \
			 ADC_S3C2410_TSC_AUTO_PST | \
			 ADC_S3C2410_TSC_XY_PST(0))

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Bit definitions for ADC_V2 */
#define ADC_V2_CON1_SOFT_RESET	(1u << 2)

#define ADC_V2_CON2_OSEL	(1u << 10)
#define ADC_V2_CON2_ESEL	(1u << 9)
#define ADC_V2_CON2_HIGHF	(1u << 8)
#define ADC_V2_CON2_C_TIME(x)	(((x) & 7) << 4)
#define ADC_V2_CON2_ACH_SEL(x)	(((x) & 0xF) << 0)
#define ADC_V2_CON2_ACH_MASK	0xF

<<<<<<< HEAD
#define MAX_ADC_V2_CHANNELS	10
#define MAX_ADC_V1_CHANNELS	8

/* Bit definitions common for ADC_V1 and ADC_V2 */
#define ADC_CON_EN_START	(1u << 0)
#define ADC_DATX_MASK		0xFFF

#define EXYNOS_ADC_TIMEOUT	(msecs_to_jiffies(1000))

struct exynos_adc {
	void __iomem		*regs;
	void __iomem		*enable_reg;
	struct clk		*clk;
	unsigned int		irq;
=======
#define MAX_ADC_V2_CHANNELS		10
#define MAX_ADC_V1_CHANNELS		8
#define MAX_EXYNOS3250_ADC_CHANNELS	2

/* Bit definitions common for ADC_V1 and ADC_V2 */
#define ADC_CON_EN_START	(1u << 0)
#define ADC_CON_EN_START_MASK	(0x3 << 0)
#define ADC_DATX_PRESSED	(1u << 15)
#define ADC_DATX_MASK		0xFFF
#define ADC_DATY_MASK		0xFFF

#define EXYNOS_ADC_TIMEOUT	(msecs_to_jiffies(100))

#define EXYNOS_ADCV1_PHY_OFFSET	0x0718
#define EXYNOS_ADCV2_PHY_OFFSET	0x0720

struct exynos_adc {
	struct exynos_adc_data	*data;
	struct device		*dev;
	struct input_dev	*input;
	void __iomem		*regs;
	struct regmap		*pmu_map;
	struct clk		*clk;
	struct clk		*sclk;
	unsigned int		irq;
	unsigned int		tsirq;
	unsigned int		delay;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct regulator	*vdd;

	struct completion	completion;

	u32			value;
	unsigned int            version;
<<<<<<< HEAD
};

static const struct of_device_id exynos_adc_match[] = {
	{ .compatible = "samsung,exynos-adc-v1", .data = (void *)ADC_V1 },
	{ .compatible = "samsung,exynos-adc-v2", .data = (void *)ADC_V2 },
=======

	bool			read_ts;
	u32			ts_x;
	u32			ts_y;
};

struct exynos_adc_data {
	int num_channels;
	bool needs_sclk;
	bool needs_adc_phy;
	int phy_offset;
	u32 mask;

	void (*init_hw)(struct exynos_adc *info);
	void (*exit_hw)(struct exynos_adc *info);
	void (*clear_irq)(struct exynos_adc *info);
	void (*start_conv)(struct exynos_adc *info, unsigned long addr);
};

static void exynos_adc_unprepare_clk(struct exynos_adc *info)
{
	if (info->data->needs_sclk)
		clk_unprepare(info->sclk);
	clk_unprepare(info->clk);
}

static int exynos_adc_prepare_clk(struct exynos_adc *info)
{
	int ret;

	ret = clk_prepare(info->clk);
	if (ret) {
		dev_err(info->dev, "failed preparing adc clock: %d\n", ret);
		return ret;
	}

	if (info->data->needs_sclk) {
		ret = clk_prepare(info->sclk);
		if (ret) {
			clk_unprepare(info->clk);
			dev_err(info->dev,
				"failed preparing sclk_adc clock: %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static void exynos_adc_disable_clk(struct exynos_adc *info)
{
	if (info->data->needs_sclk)
		clk_disable(info->sclk);
	clk_disable(info->clk);
}

static int exynos_adc_enable_clk(struct exynos_adc *info)
{
	int ret;

	ret = clk_enable(info->clk);
	if (ret) {
		dev_err(info->dev, "failed enabling adc clock: %d\n", ret);
		return ret;
	}

	if (info->data->needs_sclk) {
		ret = clk_enable(info->sclk);
		if (ret) {
			clk_disable(info->clk);
			dev_err(info->dev,
				"failed enabling sclk_adc clock: %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static void exynos_adc_v1_init_hw(struct exynos_adc *info)
{
	u32 con1;

	if (info->data->needs_adc_phy)
		regmap_write(info->pmu_map, info->data->phy_offset, 1);

	/* set default prescaler values and Enable prescaler */
	con1 =  ADC_V1_CON_PRSCLV(49) | ADC_V1_CON_PRSCEN;

	/* Enable 12-bit ADC resolution */
	con1 |= ADC_V1_CON_RES;
	writel(con1, ADC_V1_CON(info->regs));

	/* set touchscreen delay */
	writel(info->delay, ADC_V1_DLY(info->regs));
}

static void exynos_adc_v1_exit_hw(struct exynos_adc *info)
{
	u32 con;

	if (info->data->needs_adc_phy)
		regmap_write(info->pmu_map, info->data->phy_offset, 0);

	con = readl(ADC_V1_CON(info->regs));
	con |= ADC_V1_CON_STANDBY;
	writel(con, ADC_V1_CON(info->regs));
}

static void exynos_adc_v1_clear_irq(struct exynos_adc *info)
{
	writel(1, ADC_V1_INTCLR(info->regs));
}

static void exynos_adc_v1_start_conv(struct exynos_adc *info,
				     unsigned long addr)
{
	u32 con1;

	writel(addr, ADC_V1_MUX(info->regs));

	con1 = readl(ADC_V1_CON(info->regs));
	writel(con1 | ADC_CON_EN_START, ADC_V1_CON(info->regs));
}

static const struct exynos_adc_data exynos_adc_v1_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_DATX_MASK,	/* 12 bit ADC resolution */
	.needs_adc_phy	= true,
	.phy_offset	= EXYNOS_ADCV1_PHY_OFFSET,

	.init_hw	= exynos_adc_v1_init_hw,
	.exit_hw	= exynos_adc_v1_exit_hw,
	.clear_irq	= exynos_adc_v1_clear_irq,
	.start_conv	= exynos_adc_v1_start_conv,
};

static void exynos_adc_s3c2416_start_conv(struct exynos_adc *info,
					  unsigned long addr)
{
	u32 con1;

	/* Enable 12 bit ADC resolution */
	con1 = readl(ADC_V1_CON(info->regs));
	con1 |= ADC_S3C2416_CON_RES_SEL;
	writel(con1, ADC_V1_CON(info->regs));

	/* Select channel for S3C2416 */
	writel(addr, ADC_S3C2410_MUX(info->regs));

	con1 = readl(ADC_V1_CON(info->regs));
	writel(con1 | ADC_CON_EN_START, ADC_V1_CON(info->regs));
}

static struct exynos_adc_data const exynos_adc_s3c2416_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_DATX_MASK,	/* 12 bit ADC resolution */

	.init_hw	= exynos_adc_v1_init_hw,
	.exit_hw	= exynos_adc_v1_exit_hw,
	.start_conv	= exynos_adc_s3c2416_start_conv,
};

static void exynos_adc_s3c2443_start_conv(struct exynos_adc *info,
					  unsigned long addr)
{
	u32 con1;

	/* Select channel for S3C2433 */
	writel(addr, ADC_S3C2410_MUX(info->regs));

	con1 = readl(ADC_V1_CON(info->regs));
	writel(con1 | ADC_CON_EN_START, ADC_V1_CON(info->regs));
}

static struct exynos_adc_data const exynos_adc_s3c2443_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_S3C2410_DATX_MASK, /* 10 bit ADC resolution */

	.init_hw	= exynos_adc_v1_init_hw,
	.exit_hw	= exynos_adc_v1_exit_hw,
	.start_conv	= exynos_adc_s3c2443_start_conv,
};

static void exynos_adc_s3c64xx_start_conv(struct exynos_adc *info,
					  unsigned long addr)
{
	u32 con1;

	con1 = readl(ADC_V1_CON(info->regs));
	con1 &= ~ADC_S3C2410_CON_SELMUX(0x7);
	con1 |= ADC_S3C2410_CON_SELMUX(addr);
	writel(con1 | ADC_CON_EN_START, ADC_V1_CON(info->regs));
}

static struct exynos_adc_data const exynos_adc_s3c24xx_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_S3C2410_DATX_MASK, /* 10 bit ADC resolution */

	.init_hw	= exynos_adc_v1_init_hw,
	.exit_hw	= exynos_adc_v1_exit_hw,
	.start_conv	= exynos_adc_s3c64xx_start_conv,
};

static struct exynos_adc_data const exynos_adc_s3c64xx_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_DATX_MASK,	/* 12 bit ADC resolution */

	.init_hw	= exynos_adc_v1_init_hw,
	.exit_hw	= exynos_adc_v1_exit_hw,
	.clear_irq	= exynos_adc_v1_clear_irq,
	.start_conv	= exynos_adc_s3c64xx_start_conv,
};

static void exynos_adc_v2_init_hw(struct exynos_adc *info)
{
	u32 con1, con2;

	if (info->data->needs_adc_phy)
		regmap_write(info->pmu_map, info->data->phy_offset, 1);

	con1 = ADC_V2_CON1_SOFT_RESET;
	writel(con1, ADC_V2_CON1(info->regs));

	con2 = ADC_V2_CON2_OSEL | ADC_V2_CON2_ESEL |
		ADC_V2_CON2_HIGHF | ADC_V2_CON2_C_TIME(0);
	writel(con2, ADC_V2_CON2(info->regs));

	/* Enable interrupts */
	writel(1, ADC_V2_INT_EN(info->regs));
}

static void exynos_adc_v2_exit_hw(struct exynos_adc *info)
{
	u32 con;

	if (info->data->needs_adc_phy)
		regmap_write(info->pmu_map, info->data->phy_offset, 0);

	con = readl(ADC_V2_CON1(info->regs));
	con &= ~ADC_CON_EN_START;
	writel(con, ADC_V2_CON1(info->regs));
}

static void exynos_adc_v2_clear_irq(struct exynos_adc *info)
{
	writel(1, ADC_V2_INT_ST(info->regs));
}

static void exynos_adc_v2_start_conv(struct exynos_adc *info,
				     unsigned long addr)
{
	u32 con1, con2;

	con2 = readl(ADC_V2_CON2(info->regs));
	con2 &= ~ADC_V2_CON2_ACH_MASK;
	con2 |= ADC_V2_CON2_ACH_SEL(addr);
	writel(con2, ADC_V2_CON2(info->regs));

	con1 = readl(ADC_V2_CON1(info->regs));
	writel(con1 | ADC_CON_EN_START, ADC_V2_CON1(info->regs));
}

static const struct exynos_adc_data exynos_adc_v2_data = {
	.num_channels	= MAX_ADC_V2_CHANNELS,
	.mask		= ADC_DATX_MASK, /* 12 bit ADC resolution */
	.needs_adc_phy	= true,
	.phy_offset	= EXYNOS_ADCV2_PHY_OFFSET,

	.init_hw	= exynos_adc_v2_init_hw,
	.exit_hw	= exynos_adc_v2_exit_hw,
	.clear_irq	= exynos_adc_v2_clear_irq,
	.start_conv	= exynos_adc_v2_start_conv,
};

static const struct exynos_adc_data exynos3250_adc_data = {
	.num_channels	= MAX_EXYNOS3250_ADC_CHANNELS,
	.mask		= ADC_DATX_MASK, /* 12 bit ADC resolution */
	.needs_sclk	= true,
	.needs_adc_phy	= true,
	.phy_offset	= EXYNOS_ADCV1_PHY_OFFSET,

	.init_hw	= exynos_adc_v2_init_hw,
	.exit_hw	= exynos_adc_v2_exit_hw,
	.clear_irq	= exynos_adc_v2_clear_irq,
	.start_conv	= exynos_adc_v2_start_conv,
};

static void exynos_adc_exynos7_init_hw(struct exynos_adc *info)
{
	u32 con1, con2;

	if (info->data->needs_adc_phy)
		regmap_write(info->pmu_map, info->data->phy_offset, 1);

	con1 = ADC_V2_CON1_SOFT_RESET;
	writel(con1, ADC_V2_CON1(info->regs));

	con2 = readl(ADC_V2_CON2(info->regs));
	con2 &= ~ADC_V2_CON2_C_TIME(7);
	con2 |= ADC_V2_CON2_C_TIME(0);
	writel(con2, ADC_V2_CON2(info->regs));

	/* Enable interrupts */
	writel(1, ADC_V2_INT_EN(info->regs));
}

static const struct exynos_adc_data exynos7_adc_data = {
	.num_channels	= MAX_ADC_V1_CHANNELS,
	.mask		= ADC_DATX_MASK, /* 12 bit ADC resolution */

	.init_hw	= exynos_adc_exynos7_init_hw,
	.exit_hw	= exynos_adc_v2_exit_hw,
	.clear_irq	= exynos_adc_v2_clear_irq,
	.start_conv	= exynos_adc_v2_start_conv,
};

static const struct of_device_id exynos_adc_match[] = {
	{
		.compatible = "samsung,s3c2410-adc",
		.data = &exynos_adc_s3c24xx_data,
	}, {
		.compatible = "samsung,s3c2416-adc",
		.data = &exynos_adc_s3c2416_data,
	}, {
		.compatible = "samsung,s3c2440-adc",
		.data = &exynos_adc_s3c24xx_data,
	}, {
		.compatible = "samsung,s3c2443-adc",
		.data = &exynos_adc_s3c2443_data,
	}, {
		.compatible = "samsung,s3c6410-adc",
		.data = &exynos_adc_s3c64xx_data,
	}, {
		.compatible = "samsung,exynos-adc-v1",
		.data = &exynos_adc_v1_data,
	}, {
		.compatible = "samsung,exynos-adc-v2",
		.data = &exynos_adc_v2_data,
	}, {
		.compatible = "samsung,exynos3250-adc",
		.data = &exynos3250_adc_data,
	}, {
		.compatible = "samsung,exynos7-adc",
		.data = &exynos7_adc_data,
	},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{},
};
MODULE_DEVICE_TABLE(of, exynos_adc_match);

<<<<<<< HEAD
static inline unsigned int exynos_adc_get_version(struct platform_device *pdev)
=======
static struct exynos_adc_data *exynos_adc_get_data(struct platform_device *pdev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	const struct of_device_id *match;

	match = of_match_node(exynos_adc_match, pdev->dev.of_node);
<<<<<<< HEAD
	return (unsigned int)match->data;
=======
	return (struct exynos_adc_data *)match->data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int exynos_read_raw(struct iio_dev *indio_dev,
				struct iio_chan_spec const *chan,
				int *val,
				int *val2,
				long mask)
{
	struct exynos_adc *info = iio_priv(indio_dev);
	unsigned long timeout;
<<<<<<< HEAD
	u32 con1, con2;
=======
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (mask != IIO_CHAN_INFO_RAW)
		return -EINVAL;

	mutex_lock(&indio_dev->mlock);
<<<<<<< HEAD

	/* Select the channel to be used and Trigger conversion */
	if (info->version == ADC_V2) {
		con2 = readl(ADC_V2_CON2(info->regs));
		con2 &= ~ADC_V2_CON2_ACH_MASK;
		con2 |= ADC_V2_CON2_ACH_SEL(chan->address);
		writel(con2, ADC_V2_CON2(info->regs));

		con1 = readl(ADC_V2_CON1(info->regs));
		writel(con1 | ADC_CON_EN_START,
				ADC_V2_CON1(info->regs));
	} else {
		writel(chan->address, ADC_V1_MUX(info->regs));

		con1 = readl(ADC_V1_CON(info->regs));
		writel(con1 | ADC_CON_EN_START,
				ADC_V1_CON(info->regs));
	}

	timeout = wait_for_completion_interruptible_timeout
			(&info->completion, EXYNOS_ADC_TIMEOUT);
	*val = info->value;

	mutex_unlock(&indio_dev->mlock);

	if (timeout == 0)
		return -ETIMEDOUT;

	return IIO_VAL_INT;
=======
	reinit_completion(&info->completion);

	/* Select the channel to be used and Trigger conversion */
	if (info->data->start_conv)
		info->data->start_conv(info, chan->address);

	timeout = wait_for_completion_timeout(&info->completion,
					      EXYNOS_ADC_TIMEOUT);
	if (timeout == 0) {
		dev_warn(&indio_dev->dev, "Conversion timed out! Resetting\n");
		if (info->data->init_hw)
			info->data->init_hw(info);
		ret = -ETIMEDOUT;
	} else {
		*val = info->value;
		*val2 = 0;
		ret = IIO_VAL_INT;
	}

	mutex_unlock(&indio_dev->mlock);

	return ret;
}

static int exynos_read_s3c64xx_ts(struct iio_dev *indio_dev, int *x, int *y)
{
	struct exynos_adc *info = iio_priv(indio_dev);
	unsigned long timeout;
	int ret;

	mutex_lock(&indio_dev->mlock);
	info->read_ts = true;

	reinit_completion(&info->completion);

	writel(ADC_S3C2410_TSC_PULL_UP_DISABLE | ADC_TSC_AUTOPST,
	       ADC_V1_TSC(info->regs));

	/* Select the ts channel to be used and Trigger conversion */
	info->data->start_conv(info, ADC_S3C2410_MUX_TS);

	timeout = wait_for_completion_timeout(&info->completion,
					      EXYNOS_ADC_TIMEOUT);
	if (timeout == 0) {
		dev_warn(&indio_dev->dev, "Conversion timed out! Resetting\n");
		if (info->data->init_hw)
			info->data->init_hw(info);
		ret = -ETIMEDOUT;
	} else {
		*x = info->ts_x;
		*y = info->ts_y;
		ret = 0;
	}

	info->read_ts = false;
	mutex_unlock(&indio_dev->mlock);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static irqreturn_t exynos_adc_isr(int irq, void *dev_id)
{
	struct exynos_adc *info = (struct exynos_adc *)dev_id;
<<<<<<< HEAD

	/* Read value */
	info->value = readl(ADC_V1_DATX(info->regs)) &
						ADC_DATX_MASK;
	/* clear irq */
	if (info->version == ADC_V2)
		writel(1, ADC_V2_INT_ST(info->regs));
	else
		writel(1, ADC_V1_INTCLR(info->regs));
=======
	u32 mask = info->data->mask;

	/* Read value */
	if (info->read_ts) {
		info->ts_x = readl(ADC_V1_DATX(info->regs));
		info->ts_y = readl(ADC_V1_DATY(info->regs));
		writel(ADC_TSC_WAIT4INT | ADC_S3C2443_TSC_UD_SEN, ADC_V1_TSC(info->regs));
	} else {
		info->value = readl(ADC_V1_DATX(info->regs)) & mask;
	}

	/* clear irq */
	if (info->data->clear_irq)
		info->data->clear_irq(info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	complete(&info->completion);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
=======
/*
 * Here we (ab)use a threaded interrupt handler to stay running
 * for as long as the touchscreen remains pressed, we report
 * a new event with the latest data and then sleep until the
 * next timer tick. This mirrors the behavior of the old
 * driver, with much less code.
 */
static irqreturn_t exynos_ts_isr(int irq, void *dev_id)
{
	struct exynos_adc *info = dev_id;
	struct iio_dev *dev = dev_get_drvdata(info->dev);
	u32 x, y;
	bool pressed;
	int ret;

	while (info->input->users) {
		ret = exynos_read_s3c64xx_ts(dev, &x, &y);
		if (ret == -ETIMEDOUT)
			break;

		pressed = x & y & ADC_DATX_PRESSED;
		if (!pressed) {
			input_report_key(info->input, BTN_TOUCH, 0);
			input_sync(info->input);
			break;
		}

		input_report_abs(info->input, ABS_X, x & ADC_DATX_MASK);
		input_report_abs(info->input, ABS_Y, y & ADC_DATY_MASK);
		input_report_key(info->input, BTN_TOUCH, 1);
		input_sync(info->input);

		msleep(1);
	};

	writel(0, ADC_V1_CLRINTPNDNUP(info->regs));

	return IRQ_HANDLED;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int exynos_adc_reg_access(struct iio_dev *indio_dev,
			      unsigned reg, unsigned writeval,
			      unsigned *readval)
{
	struct exynos_adc *info = iio_priv(indio_dev);

	if (readval == NULL)
		return -EINVAL;

	*readval = readl(info->regs + reg);

	return 0;
}

static const struct iio_info exynos_adc_iio_info = {
	.read_raw = &exynos_read_raw,
	.debugfs_reg_access = &exynos_adc_reg_access,
	.driver_module = THIS_MODULE,
};

#define ADC_CHANNEL(_index, _id) {			\
	.type = IIO_VOLTAGE,				\
	.indexed = 1,					\
	.channel = _index,				\
	.address = _index,				\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),	\
	.datasheet_name = _id,				\
}

static const struct iio_chan_spec exynos_adc_iio_channels[] = {
	ADC_CHANNEL(0, "adc0"),
	ADC_CHANNEL(1, "adc1"),
	ADC_CHANNEL(2, "adc2"),
	ADC_CHANNEL(3, "adc3"),
	ADC_CHANNEL(4, "adc4"),
	ADC_CHANNEL(5, "adc5"),
	ADC_CHANNEL(6, "adc6"),
	ADC_CHANNEL(7, "adc7"),
	ADC_CHANNEL(8, "adc8"),
	ADC_CHANNEL(9, "adc9"),
};

static int exynos_adc_remove_devices(struct device *dev, void *c)
{
	struct platform_device *pdev = to_platform_device(dev);

	platform_device_unregister(pdev);

	return 0;
}

<<<<<<< HEAD
static void exynos_adc_hw_init(struct exynos_adc *info)
{
	u32 con1, con2;

	if (info->version == ADC_V2) {
		con1 = ADC_V2_CON1_SOFT_RESET;
		writel(con1, ADC_V2_CON1(info->regs));

		con2 = ADC_V2_CON2_OSEL | ADC_V2_CON2_ESEL |
			ADC_V2_CON2_HIGHF | ADC_V2_CON2_C_TIME(0);
		writel(con2, ADC_V2_CON2(info->regs));

		/* Enable interrupts */
		writel(1, ADC_V2_INT_EN(info->regs));
	} else {
		/* set default prescaler values and Enable prescaler */
		con1 =  ADC_V1_CON_PRSCLV(49) | ADC_V1_CON_PRSCEN;

		/* Enable 12-bit ADC resolution */
		con1 |= ADC_V1_CON_RES;
		writel(con1, ADC_V1_CON(info->regs));
	}
=======
static int exynos_adc_ts_open(struct input_dev *dev)
{
	struct exynos_adc *info = input_get_drvdata(dev);

	enable_irq(info->tsirq);

	return 0;
}

static void exynos_adc_ts_close(struct input_dev *dev)
{
	struct exynos_adc *info = input_get_drvdata(dev);

	disable_irq(info->tsirq);
}

static int exynos_adc_ts_init(struct exynos_adc *info)
{
	int ret;

	if (info->tsirq <= 0)
		return -ENODEV;

	info->input = input_allocate_device();
	if (!info->input)
		return -ENOMEM;

	info->input->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	info->input->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

	input_set_abs_params(info->input, ABS_X, 0, 0x3FF, 0, 0);
	input_set_abs_params(info->input, ABS_Y, 0, 0x3FF, 0, 0);

	info->input->name = "S3C24xx TouchScreen";
	info->input->id.bustype = BUS_HOST;
	info->input->open = exynos_adc_ts_open;
	info->input->close = exynos_adc_ts_close;

	input_set_drvdata(info->input, info);

	ret = input_register_device(info->input);
	if (ret) {
		input_free_device(info->input);
		return ret;
	}

	disable_irq(info->tsirq);
	ret = request_threaded_irq(info->tsirq, NULL, exynos_ts_isr,
				   IRQF_ONESHOT, "touchscreen", info);
	if (ret)
		input_unregister_device(info->input);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int exynos_adc_probe(struct platform_device *pdev)
{
	struct exynos_adc *info = NULL;
	struct device_node *np = pdev->dev.of_node;
<<<<<<< HEAD
	struct iio_dev *indio_dev = NULL;
	struct resource	*mem;
	int ret = -ENODEV;
	int irq;

	if (!np)
		return ret;

	indio_dev = iio_device_alloc(sizeof(struct exynos_adc));
=======
	struct s3c2410_ts_mach_info *pdata = dev_get_platdata(&pdev->dev);
	struct iio_dev *indio_dev = NULL;
	struct resource	*mem;
	bool has_ts = false;
	int ret = -ENODEV;
	int irq;

	indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(struct exynos_adc));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!indio_dev) {
		dev_err(&pdev->dev, "failed allocating iio device\n");
		return -ENOMEM;
	}

	info = iio_priv(indio_dev);

<<<<<<< HEAD
	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	info->regs = devm_request_and_ioremap(&pdev->dev, mem);
	if (!info->regs) {
		ret = -ENOMEM;
		goto err_iio;
	}

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	info->enable_reg = devm_request_and_ioremap(&pdev->dev, mem);
	if (!info->enable_reg) {
		ret = -ENOMEM;
		goto err_iio;
=======
	info->data = exynos_adc_get_data(pdev);
	if (!info->data) {
		dev_err(&pdev->dev, "failed getting exynos_adc_data\n");
		return -EINVAL;
	}

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	info->regs = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(info->regs))
		return PTR_ERR(info->regs);


	if (info->data->needs_adc_phy) {
		info->pmu_map = syscon_regmap_lookup_by_phandle(
					pdev->dev.of_node,
					"samsung,syscon-phandle");
		if (IS_ERR(info->pmu_map)) {
			dev_err(&pdev->dev, "syscon regmap lookup failed.\n");
			return PTR_ERR(info->pmu_map);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "no irq resource?\n");
<<<<<<< HEAD
		ret = irq;
		goto err_iio;
	}

	info->irq = irq;

	init_completion(&info->completion);

	ret = request_irq(info->irq, exynos_adc_isr,
					0, dev_name(&pdev->dev), info);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed requesting irq, irq = %d\n",
							info->irq);
		goto err_iio;
	}

	writel(1, info->enable_reg);
=======
		return irq;
	}
	info->irq = irq;

	irq = platform_get_irq(pdev, 1);
	if (irq == -EPROBE_DEFER)
		return irq;

	info->tsirq = irq;

	info->dev = &pdev->dev;

	init_completion(&info->completion);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	info->clk = devm_clk_get(&pdev->dev, "adc");
	if (IS_ERR(info->clk)) {
		dev_err(&pdev->dev, "failed getting clock, err = %ld\n",
							PTR_ERR(info->clk));
<<<<<<< HEAD
		ret = PTR_ERR(info->clk);
		goto err_irq;
=======
		return PTR_ERR(info->clk);
	}

	if (info->data->needs_sclk) {
		info->sclk = devm_clk_get(&pdev->dev, "sclk");
		if (IS_ERR(info->sclk)) {
			dev_err(&pdev->dev,
				"failed getting sclk clock, err = %ld\n",
				PTR_ERR(info->sclk));
			return PTR_ERR(info->sclk);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	info->vdd = devm_regulator_get(&pdev->dev, "vdd");
	if (IS_ERR(info->vdd)) {
		dev_err(&pdev->dev, "failed getting regulator, err = %ld\n",
							PTR_ERR(info->vdd));
<<<<<<< HEAD
		ret = PTR_ERR(info->vdd);
		goto err_irq;
	}

	info->version = exynos_adc_get_version(pdev);
=======
		return PTR_ERR(info->vdd);
	}

	ret = regulator_enable(info->vdd);
	if (ret)
		return ret;

	ret = exynos_adc_prepare_clk(info);
	if (ret)
		goto err_disable_reg;

	ret = exynos_adc_enable_clk(info);
	if (ret)
		goto err_unprepare_clk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	platform_set_drvdata(pdev, indio_dev);

	indio_dev->name = dev_name(&pdev->dev);
	indio_dev->dev.parent = &pdev->dev;
	indio_dev->dev.of_node = pdev->dev.of_node;
	indio_dev->info = &exynos_adc_iio_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = exynos_adc_iio_channels;
<<<<<<< HEAD

	if (info->version == ADC_V1)
		indio_dev->num_channels = MAX_ADC_V1_CHANNELS;
	else
		indio_dev->num_channels = MAX_ADC_V2_CHANNELS;
=======
	indio_dev->num_channels = info->data->num_channels;

	ret = request_irq(info->irq, exynos_adc_isr,
					0, dev_name(&pdev->dev), info);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed requesting irq, irq = %d\n",
							info->irq);
		goto err_disable_clk;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = iio_device_register(indio_dev);
	if (ret)
		goto err_irq;

<<<<<<< HEAD
	ret = regulator_enable(info->vdd);
	if (ret)
		goto err_iio_dev;

	clk_prepare_enable(info->clk);

	exynos_adc_hw_init(info);

	ret = of_platform_populate(np, exynos_adc_match, NULL, &pdev->dev);
=======
	if (info->data->init_hw)
		info->data->init_hw(info);

	/* leave out any TS related code if unreachable */
	if (IS_REACHABLE(CONFIG_INPUT)) {
		has_ts = of_property_read_bool(pdev->dev.of_node,
					       "has-touchscreen") || pdata;
	}

	if (pdata)
		info->delay = pdata->delay;
	else
		info->delay = 10000;

	if (has_ts)
		ret = exynos_adc_ts_init(info);
	if (ret)
		goto err_iio;

	ret = of_platform_populate(np, exynos_adc_match, NULL, &indio_dev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret < 0) {
		dev_err(&pdev->dev, "failed adding child nodes\n");
		goto err_of_populate;
	}

	return 0;

err_of_populate:
<<<<<<< HEAD
	device_for_each_child(&pdev->dev, NULL,
				exynos_adc_remove_devices);
	regulator_disable(info->vdd);
	clk_disable_unprepare(info->clk);
err_iio_dev:
	iio_device_unregister(indio_dev);
err_irq:
	free_irq(info->irq, info);
err_iio:
	iio_device_free(indio_dev);
=======
	device_for_each_child(&indio_dev->dev, NULL,
				exynos_adc_remove_devices);
	if (has_ts) {
		input_unregister_device(info->input);
		free_irq(info->tsirq, info);
	}
err_iio:
	iio_device_unregister(indio_dev);
err_irq:
	free_irq(info->irq, info);
err_disable_clk:
	if (info->data->exit_hw)
		info->data->exit_hw(info);
	exynos_adc_disable_clk(info);
err_unprepare_clk:
	exynos_adc_unprepare_clk(info);
err_disable_reg:
	regulator_disable(info->vdd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int exynos_adc_remove(struct platform_device *pdev)
{
	struct iio_dev *indio_dev = platform_get_drvdata(pdev);
	struct exynos_adc *info = iio_priv(indio_dev);

<<<<<<< HEAD
	device_for_each_child(&pdev->dev, NULL,
				exynos_adc_remove_devices);
	regulator_disable(info->vdd);
	clk_disable_unprepare(info->clk);
	writel(0, info->enable_reg);
	iio_device_unregister(indio_dev);
	free_irq(info->irq, info);
	iio_device_free(indio_dev);
=======
	if (IS_REACHABLE(CONFIG_INPUT) && info->input) {
		free_irq(info->tsirq, info);
		input_unregister_device(info->input);
	}
	device_for_each_child(&indio_dev->dev, NULL,
				exynos_adc_remove_devices);
	iio_device_unregister(indio_dev);
	free_irq(info->irq, info);
	if (info->data->exit_hw)
		info->data->exit_hw(info);
	exynos_adc_disable_clk(info);
	exynos_adc_unprepare_clk(info);
	regulator_disable(info->vdd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int exynos_adc_suspend(struct device *dev)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct exynos_adc *info = iio_priv(indio_dev);
<<<<<<< HEAD
	u32 con;

	if (info->version == ADC_V2) {
		con = readl(ADC_V2_CON1(info->regs));
		con &= ~ADC_CON_EN_START;
		writel(con, ADC_V2_CON1(info->regs));
	} else {
		con = readl(ADC_V1_CON(info->regs));
		con |= ADC_V1_CON_STANDBY;
		writel(con, ADC_V1_CON(info->regs));
	}

	clk_disable_unprepare(info->clk);
	writel(0, info->enable_reg);
=======

	if (info->data->exit_hw)
		info->data->exit_hw(info);
	exynos_adc_disable_clk(info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	regulator_disable(info->vdd);

	return 0;
}

static int exynos_adc_resume(struct device *dev)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct exynos_adc *info = iio_priv(indio_dev);
	int ret;

	ret = regulator_enable(info->vdd);
	if (ret)
		return ret;

<<<<<<< HEAD
	writel(1, info->enable_reg);
	clk_prepare_enable(info->clk);

	exynos_adc_hw_init(info);
=======
	ret = exynos_adc_enable_clk(info);
	if (ret)
		return ret;

	if (info->data->init_hw)
		info->data->init_hw(info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(exynos_adc_pm_ops,
			exynos_adc_suspend,
			exynos_adc_resume);

static struct platform_driver exynos_adc_driver = {
	.probe		= exynos_adc_probe,
	.remove		= exynos_adc_remove,
	.driver		= {
		.name	= "exynos-adc",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.of_match_table = exynos_adc_match,
		.pm	= &exynos_adc_pm_ops,
	},
};

module_platform_driver(exynos_adc_driver);

MODULE_AUTHOR("Naveen Krishna Chatradhi <ch.naveen@samsung.com>");
MODULE_DESCRIPTION("Samsung EXYNOS5 ADC driver");
MODULE_LICENSE("GPL v2");
