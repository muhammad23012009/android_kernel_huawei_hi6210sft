/*
 * ST SPEAr ADC driver
 *
 * Copyright 2012 Stefan Roese <sr@denx.de>
 *
 * Licensed under the GPL-2.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/completion.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

<<<<<<< HEAD
/*
 * SPEAR registers definitions
 */

#define SCAN_RATE_LO(x)		((x) & 0xFFFF)
#define SCAN_RATE_HI(x)		(((x) >> 0x10) & 0xFFFF)
#define CLK_LOW(x)		(((x) & 0xf) << 0)
#define CLK_HIGH(x)		(((x) & 0xf) << 4)

/* Bit definitions for SPEAR_ADC_STATUS */
#define START_CONVERSION	(1 << 0)
#define CHANNEL_NUM(x)		((x) << 1)
#define ADC_ENABLE		(1 << 4)
#define AVG_SAMPLE(x)		((x) << 5)
#define VREF_INTERNAL		(1 << 9)

#define DATA_MASK		0x03ff
#define DATA_BITS		10

#define MOD_NAME "spear-adc"

#define ADC_CHANNEL_NUM		8

#define CLK_MIN			2500000
#define CLK_MAX			20000000
=======
/* SPEAR registers definitions */
#define SPEAR600_ADC_SCAN_RATE_LO(x)	((x) & 0xFFFF)
#define SPEAR600_ADC_SCAN_RATE_HI(x)	(((x) >> 0x10) & 0xFFFF)
#define SPEAR_ADC_CLK_LOW(x)		(((x) & 0xf) << 0)
#define SPEAR_ADC_CLK_HIGH(x)		(((x) & 0xf) << 4)

/* Bit definitions for SPEAR_ADC_STATUS */
#define SPEAR_ADC_STATUS_START_CONVERSION	BIT(0)
#define SPEAR_ADC_STATUS_CHANNEL_NUM(x)		((x) << 1)
#define SPEAR_ADC_STATUS_ADC_ENABLE		BIT(4)
#define SPEAR_ADC_STATUS_AVG_SAMPLE(x)		((x) << 5)
#define SPEAR_ADC_STATUS_VREF_INTERNAL		BIT(9)

#define SPEAR_ADC_DATA_MASK		0x03ff
#define SPEAR_ADC_DATA_BITS		10

#define SPEAR_ADC_MOD_NAME "spear-adc"

#define SPEAR_ADC_CHANNEL_NUM		8

#define SPEAR_ADC_CLK_MIN			2500000
#define SPEAR_ADC_CLK_MAX			20000000
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct adc_regs_spear3xx {
	u32 status;
	u32 average;
	u32 scan_rate;
	u32 clk;	/* Not avail for 1340 & 1310 */
<<<<<<< HEAD
	u32 ch_ctrl[ADC_CHANNEL_NUM];
	u32 ch_data[ADC_CHANNEL_NUM];
=======
	u32 ch_ctrl[SPEAR_ADC_CHANNEL_NUM];
	u32 ch_data[SPEAR_ADC_CHANNEL_NUM];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct chan_data {
	u32 lsb;
	u32 msb;
};

struct adc_regs_spear6xx {
	u32 status;
	u32 pad[2];
	u32 clk;
<<<<<<< HEAD
	u32 ch_ctrl[ADC_CHANNEL_NUM];
	struct chan_data ch_data[ADC_CHANNEL_NUM];
=======
	u32 ch_ctrl[SPEAR_ADC_CHANNEL_NUM];
	struct chan_data ch_data[SPEAR_ADC_CHANNEL_NUM];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 scan_rate_lo;
	u32 scan_rate_hi;
	struct chan_data average;
};

<<<<<<< HEAD
struct spear_adc_info {
=======
struct spear_adc_state {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct device_node *np;
	struct adc_regs_spear3xx __iomem *adc_base_spear3xx;
	struct adc_regs_spear6xx __iomem *adc_base_spear6xx;
	struct clk *clk;
	struct completion completion;
	u32 current_clk;
	u32 sampling_freq;
	u32 avg_samples;
	u32 vref_external;
	u32 value;
};

/*
 * Functions to access some SPEAr ADC register. Abstracted into
 * static inline functions, because of different register offsets
 * on different SoC variants (SPEAr300 vs SPEAr600 etc).
 */
<<<<<<< HEAD
static void spear_adc_set_status(struct spear_adc_info *info, u32 val)
{
	__raw_writel(val, &info->adc_base_spear6xx->status);
}

static void spear_adc_set_clk(struct spear_adc_info *info, u32 val)
{
	u32 clk_high, clk_low, count;
	u32 apb_clk = clk_get_rate(info->clk);

	count = (apb_clk + val - 1) / val;
	clk_low = count / 2;
	clk_high = count - clk_low;
	info->current_clk = apb_clk / count;

	__raw_writel(CLK_LOW(clk_low) | CLK_HIGH(clk_high),
		     &info->adc_base_spear6xx->clk);
}

static void spear_adc_set_ctrl(struct spear_adc_info *info, int n,
			       u32 val)
{
	__raw_writel(val, &info->adc_base_spear6xx->ch_ctrl[n]);
}

static u32 spear_adc_get_average(struct spear_adc_info *info)
{
	if (of_device_is_compatible(info->np, "st,spear600-adc")) {
		return __raw_readl(&info->adc_base_spear6xx->average.msb) &
			DATA_MASK;
	} else {
		return __raw_readl(&info->adc_base_spear3xx->average) &
			DATA_MASK;
	}
}

static void spear_adc_set_scanrate(struct spear_adc_info *info, u32 rate)
{
	if (of_device_is_compatible(info->np, "st,spear600-adc")) {
		__raw_writel(SCAN_RATE_LO(rate),
			     &info->adc_base_spear6xx->scan_rate_lo);
		__raw_writel(SCAN_RATE_HI(rate),
			     &info->adc_base_spear6xx->scan_rate_hi);
	} else {
		__raw_writel(rate, &info->adc_base_spear3xx->scan_rate);
	}
}

static int spear_read_raw(struct iio_dev *indio_dev,
			  struct iio_chan_spec const *chan,
			  int *val,
			  int *val2,
			  long mask)
{
	struct spear_adc_info *info = iio_priv(indio_dev);
	u32 scale_mv;
=======
static void spear_adc_set_status(struct spear_adc_state *st, u32 val)
{
	__raw_writel(val, &st->adc_base_spear6xx->status);
}

static void spear_adc_set_clk(struct spear_adc_state *st, u32 val)
{
	u32 clk_high, clk_low, count;
	u32 apb_clk = clk_get_rate(st->clk);

	count = DIV_ROUND_UP(apb_clk, val);
	clk_low = count / 2;
	clk_high = count - clk_low;
	st->current_clk = apb_clk / count;

	__raw_writel(SPEAR_ADC_CLK_LOW(clk_low) | SPEAR_ADC_CLK_HIGH(clk_high),
		     &st->adc_base_spear6xx->clk);
}

static void spear_adc_set_ctrl(struct spear_adc_state *st, int n,
			       u32 val)
{
	__raw_writel(val, &st->adc_base_spear6xx->ch_ctrl[n]);
}

static u32 spear_adc_get_average(struct spear_adc_state *st)
{
	if (of_device_is_compatible(st->np, "st,spear600-adc")) {
		return __raw_readl(&st->adc_base_spear6xx->average.msb) &
			SPEAR_ADC_DATA_MASK;
	} else {
		return __raw_readl(&st->adc_base_spear3xx->average) &
			SPEAR_ADC_DATA_MASK;
	}
}

static void spear_adc_set_scanrate(struct spear_adc_state *st, u32 rate)
{
	if (of_device_is_compatible(st->np, "st,spear600-adc")) {
		__raw_writel(SPEAR600_ADC_SCAN_RATE_LO(rate),
			     &st->adc_base_spear6xx->scan_rate_lo);
		__raw_writel(SPEAR600_ADC_SCAN_RATE_HI(rate),
			     &st->adc_base_spear6xx->scan_rate_hi);
	} else {
		__raw_writel(rate, &st->adc_base_spear3xx->scan_rate);
	}
}

static int spear_adc_read_raw(struct iio_dev *indio_dev,
			      struct iio_chan_spec const *chan,
			      int *val,
			      int *val2,
			      long mask)
{
	struct spear_adc_state *st = iio_priv(indio_dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 status;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		mutex_lock(&indio_dev->mlock);

<<<<<<< HEAD
		status = CHANNEL_NUM(chan->channel) |
			AVG_SAMPLE(info->avg_samples) |
			START_CONVERSION | ADC_ENABLE;
		if (info->vref_external == 0)
			status |= VREF_INTERNAL;

		spear_adc_set_status(info, status);
		wait_for_completion(&info->completion); /* set by ISR */
		*val = info->value;
=======
		status = SPEAR_ADC_STATUS_CHANNEL_NUM(chan->channel) |
			SPEAR_ADC_STATUS_AVG_SAMPLE(st->avg_samples) |
			SPEAR_ADC_STATUS_START_CONVERSION |
			SPEAR_ADC_STATUS_ADC_ENABLE;
		if (st->vref_external == 0)
			status |= SPEAR_ADC_STATUS_VREF_INTERNAL;

		spear_adc_set_status(st, status);
		wait_for_completion(&st->completion); /* set by ISR */
		*val = st->value;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		mutex_unlock(&indio_dev->mlock);

		return IIO_VAL_INT;

	case IIO_CHAN_INFO_SCALE:
<<<<<<< HEAD
		scale_mv = (info->vref_external * 1000) >> DATA_BITS;
		*val =  scale_mv / 1000;
		*val2 = (scale_mv % 1000) * 1000;
		return IIO_VAL_INT_PLUS_MICRO;
=======
		*val = st->vref_external;
		*val2 = SPEAR_ADC_DATA_BITS;
		return IIO_VAL_FRACTIONAL_LOG2;
	case IIO_CHAN_INFO_SAMP_FREQ:
		*val = st->current_clk;
		return IIO_VAL_INT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return -EINVAL;
}

<<<<<<< HEAD
=======
static int spear_adc_write_raw(struct iio_dev *indio_dev,
			       struct iio_chan_spec const *chan,
			       int val,
			       int val2,
			       long mask)
{
	struct spear_adc_state *st = iio_priv(indio_dev);
	int ret = 0;

	if (mask != IIO_CHAN_INFO_SAMP_FREQ)
		return -EINVAL;

	mutex_lock(&indio_dev->mlock);

	if ((val < SPEAR_ADC_CLK_MIN) ||
	    (val > SPEAR_ADC_CLK_MAX) ||
	    (val2 != 0)) {
		ret = -EINVAL;
		goto out;
	}

	spear_adc_set_clk(st, val);

out:
	mutex_unlock(&indio_dev->mlock);
	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define SPEAR_ADC_CHAN(idx) {				\
	.type = IIO_VOLTAGE,				\
	.indexed = 1,					\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),	\
	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),	\
<<<<<<< HEAD
	.channel = idx,					\
	.scan_type = {					\
		.sign = 'u',				\
		.storagebits = 16,			\
	},						\
=======
	.info_mask_shared_by_all = BIT(IIO_CHAN_INFO_SAMP_FREQ),\
	.channel = idx,					\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct iio_chan_spec spear_adc_iio_channels[] = {
	SPEAR_ADC_CHAN(0),
	SPEAR_ADC_CHAN(1),
	SPEAR_ADC_CHAN(2),
	SPEAR_ADC_CHAN(3),
	SPEAR_ADC_CHAN(4),
	SPEAR_ADC_CHAN(5),
	SPEAR_ADC_CHAN(6),
	SPEAR_ADC_CHAN(7),
};

static irqreturn_t spear_adc_isr(int irq, void *dev_id)
{
<<<<<<< HEAD
	struct spear_adc_info *info = (struct spear_adc_info *)dev_id;

	/* Read value to clear IRQ */
	info->value = spear_adc_get_average(info);
	complete(&info->completion);
=======
	struct spear_adc_state *st = dev_id;

	/* Read value to clear IRQ */
	st->value = spear_adc_get_average(st);
	complete(&st->completion);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int spear_adc_configure(struct spear_adc_info *info)
=======
static int spear_adc_configure(struct spear_adc_state *st)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i;

	/* Reset ADC core */
<<<<<<< HEAD
	spear_adc_set_status(info, 0);
	__raw_writel(0, &info->adc_base_spear6xx->clk);
	for (i = 0; i < 8; i++)
		spear_adc_set_ctrl(info, i, 0);
	spear_adc_set_scanrate(info, 0);

	spear_adc_set_clk(info, info->sampling_freq);
=======
	spear_adc_set_status(st, 0);
	__raw_writel(0, &st->adc_base_spear6xx->clk);
	for (i = 0; i < 8; i++)
		spear_adc_set_ctrl(st, i, 0);
	spear_adc_set_scanrate(st, 0);

	spear_adc_set_clk(st, st->sampling_freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static ssize_t spear_adc_read_frequency(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct spear_adc_info *info = iio_priv(indio_dev);

	return sprintf(buf, "%d\n", info->current_clk);
}

static ssize_t spear_adc_write_frequency(struct device *dev,
					 struct device_attribute *attr,
					 const char *buf,
					 size_t len)
{
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct spear_adc_info *info = iio_priv(indio_dev);
	u32 clk_high, clk_low, count;
	u32 apb_clk = clk_get_rate(info->clk);
	unsigned long lval;
	int ret;

	ret = kstrtoul(buf, 10, &lval);
	if (ret)
		return ret;

	mutex_lock(&indio_dev->mlock);

	if ((lval < CLK_MIN) || (lval > CLK_MAX)) {
		ret = -EINVAL;
		goto out;
	}

	count = (apb_clk + lval - 1) / lval;
	clk_low = count / 2;
	clk_high = count - clk_low;
	info->current_clk = apb_clk / count;
	spear_adc_set_clk(info, lval);

out:
	mutex_unlock(&indio_dev->mlock);

	return ret ? ret : len;
}

static IIO_DEV_ATTR_SAMP_FREQ(S_IWUSR | S_IRUGO,
			      spear_adc_read_frequency,
			      spear_adc_write_frequency);

static struct attribute *spear_attributes[] = {
	&iio_dev_attr_sampling_frequency.dev_attr.attr,
	NULL
};

static const struct attribute_group spear_attribute_group = {
	.attrs = spear_attributes,
};

static const struct iio_info spear_adc_iio_info = {
	.read_raw = &spear_read_raw,
	.attrs = &spear_attribute_group,
=======
static const struct iio_info spear_adc_info = {
	.read_raw = &spear_adc_read_raw,
	.write_raw = &spear_adc_write_raw,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.driver_module = THIS_MODULE,
};

static int spear_adc_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
	struct spear_adc_info *info;
	struct iio_dev *iodev = NULL;
	int ret = -ENODEV;
	int irq;

	iodev = iio_device_alloc(sizeof(struct spear_adc_info));
	if (!iodev) {
		dev_err(dev, "failed allocating iio device\n");
		ret = -ENOMEM;
		goto errout1;
	}

	info = iio_priv(iodev);
	info->np = np;
=======
	struct spear_adc_state *st;
	struct resource *res;
	struct iio_dev *indio_dev = NULL;
	int ret = -ENODEV;
	int irq;

	indio_dev = devm_iio_device_alloc(dev, sizeof(struct spear_adc_state));
	if (!indio_dev) {
		dev_err(dev, "failed allocating iio device\n");
		return -ENOMEM;
	}

	st = iio_priv(indio_dev);
	st->np = np;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * SPEAr600 has a different register layout than other SPEAr SoC's
	 * (e.g. SPEAr3xx). Let's provide two register base addresses
	 * to support multi-arch kernels.
	 */
<<<<<<< HEAD
	info->adc_base_spear6xx = of_iomap(np, 0);
	if (!info->adc_base_spear6xx) {
		dev_err(dev, "failed mapping memory\n");
		ret = -ENOMEM;
		goto errout2;
	}
	info->adc_base_spear3xx =
		(struct adc_regs_spear3xx *)info->adc_base_spear6xx;

	info->clk = clk_get(dev, NULL);
	if (IS_ERR(info->clk)) {
		dev_err(dev, "failed getting clock\n");
		goto errout3;
	}

	ret = clk_prepare_enable(info->clk);
	if (ret) {
		dev_err(dev, "failed enabling clock\n");
		goto errout4;
	}

	irq = platform_get_irq(pdev, 0);
	if ((irq < 0) || (irq >= NR_IRQS)) {
		dev_err(dev, "failed getting interrupt resource\n");
		ret = -EINVAL;
		goto errout5;
	}

	ret = devm_request_irq(dev, irq, spear_adc_isr, 0, MOD_NAME, info);
	if (ret < 0) {
		dev_err(dev, "failed requesting interrupt\n");
		goto errout5;
	}

	if (of_property_read_u32(np, "sampling-frequency",
				 &info->sampling_freq)) {
		dev_err(dev, "sampling-frequency missing in DT\n");
		ret = -EINVAL;
		goto errout5;
=======
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	st->adc_base_spear6xx = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(st->adc_base_spear6xx))
		return PTR_ERR(st->adc_base_spear6xx);

	st->adc_base_spear3xx =
		(struct adc_regs_spear3xx __iomem *)st->adc_base_spear6xx;

	st->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(st->clk)) {
		dev_err(dev, "failed getting clock\n");
		return PTR_ERR(st->clk);
	}

	ret = clk_prepare_enable(st->clk);
	if (ret) {
		dev_err(dev, "failed enabling clock\n");
		return ret;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq <= 0) {
		dev_err(dev, "failed getting interrupt resource\n");
		ret = -EINVAL;
		goto errout2;
	}

	ret = devm_request_irq(dev, irq, spear_adc_isr, 0, SPEAR_ADC_MOD_NAME,
			       st);
	if (ret < 0) {
		dev_err(dev, "failed requesting interrupt\n");
		goto errout2;
	}

	if (of_property_read_u32(np, "sampling-frequency",
				 &st->sampling_freq)) {
		dev_err(dev, "sampling-frequency missing in DT\n");
		ret = -EINVAL;
		goto errout2;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/*
	 * Optional avg_samples defaults to 0, resulting in single data
	 * conversion
	 */
<<<<<<< HEAD
	of_property_read_u32(np, "average-samples", &info->avg_samples);
=======
	of_property_read_u32(np, "average-samples", &st->avg_samples);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Optional vref_external defaults to 0, resulting in internal vref
	 * selection
	 */
<<<<<<< HEAD
	of_property_read_u32(np, "vref-external", &info->vref_external);

	spear_adc_configure(info);

	platform_set_drvdata(pdev, iodev);

	init_completion(&info->completion);

	iodev->name = MOD_NAME;
	iodev->dev.parent = dev;
	iodev->info = &spear_adc_iio_info;
	iodev->modes = INDIO_DIRECT_MODE;
	iodev->channels = spear_adc_iio_channels;
	iodev->num_channels = ARRAY_SIZE(spear_adc_iio_channels);

	ret = iio_device_register(iodev);
	if (ret)
		goto errout5;
=======
	of_property_read_u32(np, "vref-external", &st->vref_external);

	spear_adc_configure(st);

	platform_set_drvdata(pdev, indio_dev);

	init_completion(&st->completion);

	indio_dev->name = SPEAR_ADC_MOD_NAME;
	indio_dev->dev.parent = dev;
	indio_dev->info = &spear_adc_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = spear_adc_iio_channels;
	indio_dev->num_channels = ARRAY_SIZE(spear_adc_iio_channels);

	ret = iio_device_register(indio_dev);
	if (ret)
		goto errout2;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev_info(dev, "SPEAR ADC driver loaded, IRQ %d\n", irq);

	return 0;

<<<<<<< HEAD
errout5:
	clk_disable_unprepare(info->clk);
errout4:
	clk_put(info->clk);
errout3:
	iounmap(info->adc_base_spear6xx);
errout2:
	iio_device_free(iodev);
errout1:
=======
errout2:
	clk_disable_unprepare(st->clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int spear_adc_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct iio_dev *iodev = platform_get_drvdata(pdev);
	struct spear_adc_info *info = iio_priv(iodev);

	iio_device_unregister(iodev);
	platform_set_drvdata(pdev, NULL);
	clk_disable_unprepare(info->clk);
	clk_put(info->clk);
	iounmap(info->adc_base_spear6xx);
	iio_device_free(iodev);
=======
	struct iio_dev *indio_dev = platform_get_drvdata(pdev);
	struct spear_adc_state *st = iio_priv(indio_dev);

	iio_device_unregister(indio_dev);
	clk_disable_unprepare(st->clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_OF
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const struct of_device_id spear_adc_dt_ids[] = {
	{ .compatible = "st,spear600-adc", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, spear_adc_dt_ids);
<<<<<<< HEAD
=======
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct platform_driver spear_adc_driver = {
	.probe		= spear_adc_probe,
	.remove		= spear_adc_remove,
	.driver		= {
<<<<<<< HEAD
		.name	= MOD_NAME,
		.owner	= THIS_MODULE,
=======
		.name	= SPEAR_ADC_MOD_NAME,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.of_match_table = of_match_ptr(spear_adc_dt_ids),
	},
};

module_platform_driver(spear_adc_driver);

MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("SPEAr ADC driver");
MODULE_LICENSE("GPL");
