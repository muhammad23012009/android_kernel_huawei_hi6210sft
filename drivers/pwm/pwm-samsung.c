<<<<<<< HEAD
/* drivers/pwm/pwm-samsung.c
 *
 * Copyright (c) 2007 Ben Dooks
 * Copyright (c) 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>, <ben-linux@fluff.org>
 *
 * S3C series PWM device core
=======
/*
 * Copyright (c) 2007 Ben Dooks
 * Copyright (c) 2008 Simtec Electronics
 *     Ben Dooks <ben@simtec.co.uk>, <ben-linux@fluff.org>
 * Copyright (c) 2013 Tomasz Figa <tomasz.figa@gmail.com>
 *
 * PWM driver for Samsung SoCs
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
<<<<<<< HEAD
*/

#define pr_fmt(fmt) "pwm-samsung: " fmt

#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/pwm.h>

#include <mach/map.h>

#include <plat/regs-timer.h>

struct s3c_chip {
	struct platform_device	*pdev;

	struct clk		*clk_div;
	struct clk		*clk;
	const char		*label;

	unsigned int		 period_ns;
	unsigned int		 duty_ns;

	unsigned char		 tcon_base;
	unsigned char		 pwm_id;
	struct pwm_chip		 chip;
};

#define to_s3c_chip(chip)	container_of(chip, struct s3c_chip, chip)

#define pwm_dbg(_pwm, msg...) dev_dbg(&(_pwm)->pdev->dev, msg)

static struct clk *clk_scaler[2];

static inline int pwm_is_tdiv(struct s3c_chip *chip)
{
	return clk_get_parent(chip->clk) == chip->clk_div;
}

#define pwm_tcon_start(pwm) (1 << (pwm->tcon_base + 0))
#define pwm_tcon_invert(pwm) (1 << (pwm->tcon_base + 2))
#define pwm_tcon_autoreload(pwm) (1 << (pwm->tcon_base + 3))
#define pwm_tcon_manulupdate(pwm) (1 << (pwm->tcon_base + 1))

static int s3c_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct s3c_chip *s3c = to_s3c_chip(chip);
	unsigned long flags;
	unsigned long tcon;

	local_irq_save(flags);

	tcon = __raw_readl(S3C2410_TCON);
	tcon |= pwm_tcon_start(s3c);
	__raw_writel(tcon, S3C2410_TCON);

	local_irq_restore(flags);

	return 0;
}

static void s3c_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct s3c_chip *s3c = to_s3c_chip(chip);
	unsigned long flags;
	unsigned long tcon;

	local_irq_save(flags);

	tcon = __raw_readl(S3C2410_TCON);
	tcon &= ~pwm_tcon_start(s3c);
	__raw_writel(tcon, S3C2410_TCON);

	local_irq_restore(flags);
}

static unsigned long pwm_calc_tin(struct s3c_chip *s3c, unsigned long freq)
{
	unsigned long tin_parent_rate;
	unsigned int div;

	tin_parent_rate = clk_get_rate(clk_get_parent(s3c->clk_div));
	pwm_dbg(s3c, "tin parent at %lu\n", tin_parent_rate);

	for (div = 2; div <= 16; div *= 2) {
		if ((tin_parent_rate / (div << 16)) < freq)
			return tin_parent_rate / div;
	}

	return tin_parent_rate / 16;
}

#define NS_IN_HZ (1000000000UL)

static int s3c_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
		int duty_ns, int period_ns)
{
	struct s3c_chip *s3c = to_s3c_chip(chip);
	unsigned long tin_rate;
	unsigned long tin_ns;
	unsigned long period;
	unsigned long flags;
	unsigned long tcon;
	unsigned long tcnt;
	long tcmp;

	/* We currently avoid using 64bit arithmetic by using the
	 * fact that anything faster than 1Hz is easily representable
	 * by 32bits. */

	if (period_ns > NS_IN_HZ || duty_ns > NS_IN_HZ)
		return -ERANGE;

	if (period_ns == s3c->period_ns &&
	    duty_ns == s3c->duty_ns)
		return 0;

	/* The TCMP and TCNT can be read without a lock, they're not
	 * shared between the timers. */

	tcmp = __raw_readl(S3C2410_TCMPB(s3c->pwm_id));
	tcnt = __raw_readl(S3C2410_TCNTB(s3c->pwm_id));

	period = NS_IN_HZ / period_ns;

	pwm_dbg(s3c, "duty_ns=%d, period_ns=%d (%lu)\n",
		duty_ns, period_ns, period);

	/* Check to see if we are changing the clock rate of the PWM */

	if (s3c->period_ns != period_ns) {
		if (pwm_is_tdiv(s3c)) {
			tin_rate = pwm_calc_tin(s3c, period);
			clk_set_rate(s3c->clk_div, tin_rate);
		} else
			tin_rate = clk_get_rate(s3c->clk);

		s3c->period_ns = period_ns;

		pwm_dbg(s3c, "tin_rate=%lu\n", tin_rate);

		tin_ns = NS_IN_HZ / tin_rate;
		tcnt = period_ns / tin_ns;
	} else
		tin_ns = NS_IN_HZ / clk_get_rate(s3c->clk);

	/* Note, counters count down */

	tcmp = duty_ns / tin_ns;
	tcmp = tcnt - tcmp;
	/* the pwm hw only checks the compare register after a decrement,
	   so the pin never toggles if tcmp = tcnt */
	if (tcmp == tcnt)
		tcmp--;

	pwm_dbg(s3c, "tin_ns=%lu, tcmp=%ld/%lu\n", tin_ns, tcmp, tcnt);

	if (tcmp < 0)
		tcmp = 0;

	/* Update the PWM register block. */

	local_irq_save(flags);

	__raw_writel(tcmp, S3C2410_TCMPB(s3c->pwm_id));
	__raw_writel(tcnt, S3C2410_TCNTB(s3c->pwm_id));

	tcon = __raw_readl(S3C2410_TCON);
	tcon |= pwm_tcon_manulupdate(s3c);
	tcon |= pwm_tcon_autoreload(s3c);
	__raw_writel(tcon, S3C2410_TCON);

	tcon &= ~pwm_tcon_manulupdate(s3c);
	__raw_writel(tcon, S3C2410_TCON);

	local_irq_restore(flags);
=======
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/time.h>

/* For struct samsung_timer_variant and samsung_pwm_lock. */
#include <clocksource/samsung_pwm.h>

#define REG_TCFG0			0x00
#define REG_TCFG1			0x04
#define REG_TCON			0x08

#define REG_TCNTB(chan)			(0x0c + ((chan) * 0xc))
#define REG_TCMPB(chan)			(0x10 + ((chan) * 0xc))

#define TCFG0_PRESCALER_MASK		0xff
#define TCFG0_PRESCALER1_SHIFT		8

#define TCFG1_MUX_MASK			0xf
#define TCFG1_SHIFT(chan)		(4 * (chan))

/*
 * Each channel occupies 4 bits in TCON register, but there is a gap of 4
 * bits (one channel) after channel 0, so channels have different numbering
 * when accessing TCON register. See to_tcon_channel() function.
 *
 * In addition, the location of autoreload bit for channel 4 (TCON channel 5)
 * in its set of bits is 2 as opposed to 3 for other channels.
 */
#define TCON_START(chan)		BIT(4 * (chan) + 0)
#define TCON_MANUALUPDATE(chan)		BIT(4 * (chan) + 1)
#define TCON_INVERT(chan)		BIT(4 * (chan) + 2)
#define _TCON_AUTORELOAD(chan)		BIT(4 * (chan) + 3)
#define _TCON_AUTORELOAD4(chan)		BIT(4 * (chan) + 2)
#define TCON_AUTORELOAD(chan)		\
	((chan < 5) ? _TCON_AUTORELOAD(chan) : _TCON_AUTORELOAD4(chan))

/**
 * struct samsung_pwm_channel - private data of PWM channel
 * @period_ns:	current period in nanoseconds programmed to the hardware
 * @duty_ns:	current duty time in nanoseconds programmed to the hardware
 * @tin_ns:	time of one timer tick in nanoseconds with current timer rate
 */
struct samsung_pwm_channel {
	u32 period_ns;
	u32 duty_ns;
	u32 tin_ns;
};

/**
 * struct samsung_pwm_chip - private data of PWM chip
 * @chip:		generic PWM chip
 * @variant:		local copy of hardware variant data
 * @inverter_mask:	inverter status for all channels - one bit per channel
 * @base:		base address of mapped PWM registers
 * @base_clk:		base clock used to drive the timers
 * @tclk0:		external clock 0 (can be ERR_PTR if not present)
 * @tclk1:		external clock 1 (can be ERR_PTR if not present)
 */
struct samsung_pwm_chip {
	struct pwm_chip chip;
	struct samsung_pwm_variant variant;
	u8 inverter_mask;

	void __iomem *base;
	struct clk *base_clk;
	struct clk *tclk0;
	struct clk *tclk1;
};

#ifndef CONFIG_CLKSRC_SAMSUNG_PWM
/*
 * PWM block is shared between pwm-samsung and samsung_pwm_timer drivers
 * and some registers need access synchronization. If both drivers are
 * compiled in, the spinlock is defined in the clocksource driver,
 * otherwise following definition is used.
 *
 * Currently we do not need any more complex synchronization method
 * because all the supported SoCs contain only one instance of the PWM
 * IP. Should this change, both drivers will need to be modified to
 * properly synchronize accesses to particular instances.
 */
static DEFINE_SPINLOCK(samsung_pwm_lock);
#endif

static inline
struct samsung_pwm_chip *to_samsung_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct samsung_pwm_chip, chip);
}

static inline unsigned int to_tcon_channel(unsigned int channel)
{
	/* TCON register has a gap of 4 bits (1 channel) after channel 0 */
	return (channel == 0) ? 0 : (channel + 1);
}

static void pwm_samsung_set_divisor(struct samsung_pwm_chip *pwm,
				    unsigned int channel, u8 divisor)
{
	u8 shift = TCFG1_SHIFT(channel);
	unsigned long flags;
	u32 reg;
	u8 bits;

	bits = (fls(divisor) - 1) - pwm->variant.div_base;

	spin_lock_irqsave(&samsung_pwm_lock, flags);

	reg = readl(pwm->base + REG_TCFG1);
	reg &= ~(TCFG1_MUX_MASK << shift);
	reg |= bits << shift;
	writel(reg, pwm->base + REG_TCFG1);

	spin_unlock_irqrestore(&samsung_pwm_lock, flags);
}

static int pwm_samsung_is_tdiv(struct samsung_pwm_chip *chip, unsigned int chan)
{
	struct samsung_pwm_variant *variant = &chip->variant;
	u32 reg;

	reg = readl(chip->base + REG_TCFG1);
	reg >>= TCFG1_SHIFT(chan);
	reg &= TCFG1_MUX_MASK;

	return (BIT(reg) & variant->tclk_mask) == 0;
}

static unsigned long pwm_samsung_get_tin_rate(struct samsung_pwm_chip *chip,
					      unsigned int chan)
{
	unsigned long rate;
	u32 reg;

	rate = clk_get_rate(chip->base_clk);

	reg = readl(chip->base + REG_TCFG0);
	if (chan >= 2)
		reg >>= TCFG0_PRESCALER1_SHIFT;
	reg &= TCFG0_PRESCALER_MASK;

	return rate / (reg + 1);
}

static unsigned long pwm_samsung_calc_tin(struct samsung_pwm_chip *chip,
					  unsigned int chan, unsigned long freq)
{
	struct samsung_pwm_variant *variant = &chip->variant;
	unsigned long rate;
	struct clk *clk;
	u8 div;

	if (!pwm_samsung_is_tdiv(chip, chan)) {
		clk = (chan < 2) ? chip->tclk0 : chip->tclk1;
		if (!IS_ERR(clk)) {
			rate = clk_get_rate(clk);
			if (rate)
				return rate;
		}

		dev_warn(chip->chip.dev,
			"tclk of PWM %d is inoperational, using tdiv\n", chan);
	}

	rate = pwm_samsung_get_tin_rate(chip, chan);
	dev_dbg(chip->chip.dev, "tin parent at %lu\n", rate);

	/*
	 * Compare minimum PWM frequency that can be achieved with possible
	 * divider settings and choose the lowest divisor that can generate
	 * frequencies lower than requested.
	 */
	if (variant->bits < 32) {
		/* Only for s3c24xx */
		for (div = variant->div_base; div < 4; ++div)
			if ((rate >> (variant->bits + div)) < freq)
				break;
	} else {
		/*
		 * Other variants have enough counter bits to generate any
		 * requested rate, so no need to check higher divisors.
		 */
		div = variant->div_base;
	}

	pwm_samsung_set_divisor(chip, chan, BIT(div));

	return rate >> div;
}

static int pwm_samsung_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct samsung_pwm_chip *our_chip = to_samsung_pwm_chip(chip);
	struct samsung_pwm_channel *our_chan;

	if (!(our_chip->variant.output_mask & BIT(pwm->hwpwm))) {
		dev_warn(chip->dev,
			"tried to request PWM channel %d without output\n",
			pwm->hwpwm);
		return -EINVAL;
	}

	our_chan = devm_kzalloc(chip->dev, sizeof(*our_chan), GFP_KERNEL);
	if (!our_chan)
		return -ENOMEM;

	pwm_set_chip_data(pwm, our_chan);

	return 0;
}

static void pwm_samsung_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	devm_kfree(chip->dev, pwm_get_chip_data(pwm));
}

static int pwm_samsung_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct samsung_pwm_chip *our_chip = to_samsung_pwm_chip(chip);
	unsigned int tcon_chan = to_tcon_channel(pwm->hwpwm);
	unsigned long flags;
	u32 tcon;

	spin_lock_irqsave(&samsung_pwm_lock, flags);

	tcon = readl(our_chip->base + REG_TCON);

	tcon &= ~TCON_START(tcon_chan);
	tcon |= TCON_MANUALUPDATE(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	tcon &= ~TCON_MANUALUPDATE(tcon_chan);
	tcon |= TCON_START(tcon_chan) | TCON_AUTORELOAD(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	spin_unlock_irqrestore(&samsung_pwm_lock, flags);

	return 0;
}

static void pwm_samsung_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct samsung_pwm_chip *our_chip = to_samsung_pwm_chip(chip);
	unsigned int tcon_chan = to_tcon_channel(pwm->hwpwm);
	unsigned long flags;
	u32 tcon;

	spin_lock_irqsave(&samsung_pwm_lock, flags);

	tcon = readl(our_chip->base + REG_TCON);
	tcon &= ~TCON_AUTORELOAD(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	spin_unlock_irqrestore(&samsung_pwm_lock, flags);
}

static void pwm_samsung_manual_update(struct samsung_pwm_chip *chip,
				      struct pwm_device *pwm)
{
	unsigned int tcon_chan = to_tcon_channel(pwm->hwpwm);
	u32 tcon;
	unsigned long flags;

	spin_lock_irqsave(&samsung_pwm_lock, flags);

	tcon = readl(chip->base + REG_TCON);
	tcon |= TCON_MANUALUPDATE(tcon_chan);
	writel(tcon, chip->base + REG_TCON);

	tcon &= ~TCON_MANUALUPDATE(tcon_chan);
	writel(tcon, chip->base + REG_TCON);

	spin_unlock_irqrestore(&samsung_pwm_lock, flags);
}

static int pwm_samsung_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      int duty_ns, int period_ns)
{
	struct samsung_pwm_chip *our_chip = to_samsung_pwm_chip(chip);
	struct samsung_pwm_channel *chan = pwm_get_chip_data(pwm);
	u32 tin_ns = chan->tin_ns, tcnt, tcmp, oldtcmp;

	/*
	 * We currently avoid using 64bit arithmetic by using the
	 * fact that anything faster than 1Hz is easily representable
	 * by 32bits.
	 */
	if (period_ns > NSEC_PER_SEC)
		return -ERANGE;

	if (period_ns == chan->period_ns && duty_ns == chan->duty_ns)
		return 0;

	tcnt = readl(our_chip->base + REG_TCNTB(pwm->hwpwm));
	oldtcmp = readl(our_chip->base + REG_TCMPB(pwm->hwpwm));

	/* We need tick count for calculation, not last tick. */
	++tcnt;

	/* Check to see if we are changing the clock rate of the PWM. */
	if (chan->period_ns != period_ns) {
		unsigned long tin_rate;
		u32 period;

		period = NSEC_PER_SEC / period_ns;

		dev_dbg(our_chip->chip.dev, "duty_ns=%d, period_ns=%d (%u)\n",
						duty_ns, period_ns, period);

		tin_rate = pwm_samsung_calc_tin(our_chip, pwm->hwpwm, period);

		dev_dbg(our_chip->chip.dev, "tin_rate=%lu\n", tin_rate);

		tin_ns = NSEC_PER_SEC / tin_rate;
		tcnt = period_ns / tin_ns;
	}

	/* Period is too short. */
	if (tcnt <= 1)
		return -ERANGE;

	/* Note that counters count down. */
	tcmp = duty_ns / tin_ns;

	/* 0% duty is not available */
	if (!tcmp)
		++tcmp;

	tcmp = tcnt - tcmp;

	/* Decrement to get tick numbers, instead of tick counts. */
	--tcnt;
	/* -1UL will give 100% duty. */
	--tcmp;

	dev_dbg(our_chip->chip.dev,
				"tin_ns=%u, tcmp=%u/%u\n", tin_ns, tcmp, tcnt);

	/* Update PWM registers. */
	writel(tcnt, our_chip->base + REG_TCNTB(pwm->hwpwm));
	writel(tcmp, our_chip->base + REG_TCMPB(pwm->hwpwm));

	/*
	 * In case the PWM is currently at 100% duty cycle, force a manual
	 * update to prevent the signal staying high if the PWM is disabled
	 * shortly afer this update (before it autoreloaded the new values).
	 */
	if (oldtcmp == (u32) -1) {
		dev_dbg(our_chip->chip.dev, "Forcing manual update");
		pwm_samsung_manual_update(our_chip, pwm);
	}

	chan->period_ns = period_ns;
	chan->tin_ns = tin_ns;
	chan->duty_ns = duty_ns;

	return 0;
}

static void pwm_samsung_set_invert(struct samsung_pwm_chip *chip,
				   unsigned int channel, bool invert)
{
	unsigned int tcon_chan = to_tcon_channel(channel);
	unsigned long flags;
	u32 tcon;

	spin_lock_irqsave(&samsung_pwm_lock, flags);

	tcon = readl(chip->base + REG_TCON);

	if (invert) {
		chip->inverter_mask |= BIT(channel);
		tcon |= TCON_INVERT(tcon_chan);
	} else {
		chip->inverter_mask &= ~BIT(channel);
		tcon &= ~TCON_INVERT(tcon_chan);
	}

	writel(tcon, chip->base + REG_TCON);

	spin_unlock_irqrestore(&samsung_pwm_lock, flags);
}

static int pwm_samsung_set_polarity(struct pwm_chip *chip,
				    struct pwm_device *pwm,
				    enum pwm_polarity polarity)
{
	struct samsung_pwm_chip *our_chip = to_samsung_pwm_chip(chip);
	bool invert = (polarity == PWM_POLARITY_NORMAL);

	/* Inverted means normal in the hardware. */
	pwm_samsung_set_invert(our_chip, pwm->hwpwm, invert);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static struct pwm_ops s3c_pwm_ops = {
	.enable = s3c_pwm_enable,
	.disable = s3c_pwm_disable,
	.config = s3c_pwm_config,
	.owner = THIS_MODULE,
};

static int s3c_pwm_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct s3c_chip *s3c;
	unsigned long flags;
	unsigned long tcon;
	unsigned int id = pdev->id;
	int ret;

	if (id == 4) {
		dev_err(dev, "TIMER4 is currently not supported\n");
		return -ENXIO;
	}

	s3c = devm_kzalloc(&pdev->dev, sizeof(*s3c), GFP_KERNEL);
	if (s3c == NULL) {
		dev_err(dev, "failed to allocate pwm_device\n");
		return -ENOMEM;
	}

	/* calculate base of control bits in TCON */
	s3c->tcon_base = id == 0 ? 0 : (id * 4) + 4;
	s3c->pwm_id = id;
	s3c->chip.dev = &pdev->dev;
	s3c->chip.ops = &s3c_pwm_ops;
	s3c->chip.base = -1;
	s3c->chip.npwm = 1;

	s3c->clk = devm_clk_get(dev, "pwm-tin");
	if (IS_ERR(s3c->clk)) {
		dev_err(dev, "failed to get pwm tin clk\n");
		return PTR_ERR(s3c->clk);
	}

	s3c->clk_div = devm_clk_get(dev, "pwm-tdiv");
	if (IS_ERR(s3c->clk_div)) {
		dev_err(dev, "failed to get pwm tdiv clk\n");
		return PTR_ERR(s3c->clk_div);
	}

	clk_enable(s3c->clk);
	clk_enable(s3c->clk_div);

	local_irq_save(flags);

	tcon = __raw_readl(S3C2410_TCON);
	tcon |= pwm_tcon_invert(s3c);
	__raw_writel(tcon, S3C2410_TCON);

	local_irq_restore(flags);

	ret = pwmchip_add(&s3c->chip);
	if (ret < 0) {
		dev_err(dev, "failed to register pwm\n");
		goto err_clk_tdiv;
	}

	pwm_dbg(s3c, "config bits %02x\n",
		(__raw_readl(S3C2410_TCON) >> s3c->tcon_base) & 0x0f);

	dev_info(dev, "tin at %lu, tdiv at %lu, tin=%sclk, base %d\n",
		 clk_get_rate(s3c->clk),
		 clk_get_rate(s3c->clk_div),
		 pwm_is_tdiv(s3c) ? "div" : "ext", s3c->tcon_base);

	platform_set_drvdata(pdev, s3c);
	return 0;

 err_clk_tdiv:
	clk_disable(s3c->clk_div);
	clk_disable(s3c->clk);
	return ret;
}

static int s3c_pwm_remove(struct platform_device *pdev)
{
	struct s3c_chip *s3c = platform_get_drvdata(pdev);
	int err;

	err = pwmchip_remove(&s3c->chip);
	if (err < 0)
		return err;

	clk_disable(s3c->clk_div);
	clk_disable(s3c->clk);
=======
static const struct pwm_ops pwm_samsung_ops = {
	.request	= pwm_samsung_request,
	.free		= pwm_samsung_free,
	.enable		= pwm_samsung_enable,
	.disable	= pwm_samsung_disable,
	.config		= pwm_samsung_config,
	.set_polarity	= pwm_samsung_set_polarity,
	.owner		= THIS_MODULE,
};

#ifdef CONFIG_OF
static const struct samsung_pwm_variant s3c24xx_variant = {
	.bits		= 16,
	.div_base	= 1,
	.has_tint_cstat	= false,
	.tclk_mask	= BIT(4),
};

static const struct samsung_pwm_variant s3c64xx_variant = {
	.bits		= 32,
	.div_base	= 0,
	.has_tint_cstat	= true,
	.tclk_mask	= BIT(7) | BIT(6) | BIT(5),
};

static const struct samsung_pwm_variant s5p64x0_variant = {
	.bits		= 32,
	.div_base	= 0,
	.has_tint_cstat	= true,
	.tclk_mask	= 0,
};

static const struct samsung_pwm_variant s5pc100_variant = {
	.bits		= 32,
	.div_base	= 0,
	.has_tint_cstat	= true,
	.tclk_mask	= BIT(5),
};

static const struct of_device_id samsung_pwm_matches[] = {
	{ .compatible = "samsung,s3c2410-pwm", .data = &s3c24xx_variant },
	{ .compatible = "samsung,s3c6400-pwm", .data = &s3c64xx_variant },
	{ .compatible = "samsung,s5p6440-pwm", .data = &s5p64x0_variant },
	{ .compatible = "samsung,s5pc100-pwm", .data = &s5pc100_variant },
	{ .compatible = "samsung,exynos4210-pwm", .data = &s5p64x0_variant },
	{},
};
MODULE_DEVICE_TABLE(of, samsung_pwm_matches);

static int pwm_samsung_parse_dt(struct samsung_pwm_chip *chip)
{
	struct device_node *np = chip->chip.dev->of_node;
	const struct of_device_id *match;
	struct property *prop;
	const __be32 *cur;
	u32 val;

	match = of_match_node(samsung_pwm_matches, np);
	if (!match)
		return -ENODEV;

	memcpy(&chip->variant, match->data, sizeof(chip->variant));

	of_property_for_each_u32(np, "samsung,pwm-outputs", prop, cur, val) {
		if (val >= SAMSUNG_PWM_NUM) {
			dev_err(chip->chip.dev,
				"%s: invalid channel index in samsung,pwm-outputs property\n",
								__func__);
			continue;
		}
		chip->variant.output_mask |= BIT(val);
	}

	return 0;
}
#else
static int pwm_samsung_parse_dt(struct samsung_pwm_chip *chip)
{
	return -ENODEV;
}
#endif

static int pwm_samsung_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct samsung_pwm_chip *chip;
	struct resource *res;
	unsigned int chan;
	int ret;

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	chip->chip.dev = &pdev->dev;
	chip->chip.ops = &pwm_samsung_ops;
	chip->chip.base = -1;
	chip->chip.npwm = SAMSUNG_PWM_NUM;
	chip->inverter_mask = BIT(SAMSUNG_PWM_NUM) - 1;

	if (IS_ENABLED(CONFIG_OF) && pdev->dev.of_node) {
		ret = pwm_samsung_parse_dt(chip);
		if (ret)
			return ret;

		chip->chip.of_xlate = of_pwm_xlate_with_flags;
		chip->chip.of_pwm_n_cells = 3;
	} else {
		if (!pdev->dev.platform_data) {
			dev_err(&pdev->dev, "no platform data specified\n");
			return -EINVAL;
		}

		memcpy(&chip->variant, pdev->dev.platform_data,
							sizeof(chip->variant));
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	chip->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(chip->base))
		return PTR_ERR(chip->base);

	chip->base_clk = devm_clk_get(&pdev->dev, "timers");
	if (IS_ERR(chip->base_clk)) {
		dev_err(dev, "failed to get timer base clk\n");
		return PTR_ERR(chip->base_clk);
	}

	ret = clk_prepare_enable(chip->base_clk);
	if (ret < 0) {
		dev_err(dev, "failed to enable base clock\n");
		return ret;
	}

	for (chan = 0; chan < SAMSUNG_PWM_NUM; ++chan)
		if (chip->variant.output_mask & BIT(chan))
			pwm_samsung_set_invert(chip, chan, true);

	/* Following clocks are optional. */
	chip->tclk0 = devm_clk_get(&pdev->dev, "pwm-tclk0");
	chip->tclk1 = devm_clk_get(&pdev->dev, "pwm-tclk1");

	platform_set_drvdata(pdev, chip);

	ret = pwmchip_add(&chip->chip);
	if (ret < 0) {
		dev_err(dev, "failed to register PWM chip\n");
		clk_disable_unprepare(chip->base_clk);
		return ret;
	}

	dev_dbg(dev, "base_clk at %lu, tclk0 at %lu, tclk1 at %lu\n",
		clk_get_rate(chip->base_clk),
		!IS_ERR(chip->tclk0) ? clk_get_rate(chip->tclk0) : 0,
		!IS_ERR(chip->tclk1) ? clk_get_rate(chip->tclk1) : 0);

	return 0;
}

static int pwm_samsung_remove(struct platform_device *pdev)
{
	struct samsung_pwm_chip *chip = platform_get_drvdata(pdev);
	int ret;

	ret = pwmchip_remove(&chip->chip);
	if (ret < 0)
		return ret;

	clk_disable_unprepare(chip->base_clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

#ifdef CONFIG_PM_SLEEP
<<<<<<< HEAD
static int s3c_pwm_suspend(struct device *dev)
{
	struct s3c_chip *s3c = dev_get_drvdata(dev);

	/* No one preserve these values during suspend so reset them
	 * Otherwise driver leaves PWM unconfigured if same values
	 * passed to pwm_config
	 */
	s3c->period_ns = 0;
	s3c->duty_ns = 0;
=======
static int pwm_samsung_suspend(struct device *dev)
{
	struct samsung_pwm_chip *chip = dev_get_drvdata(dev);
	unsigned int i;

	/*
	 * No one preserves these values during suspend so reset them.
	 * Otherwise driver leaves PWM unconfigured if same values are
	 * passed to pwm_config() next time.
	 */
	for (i = 0; i < SAMSUNG_PWM_NUM; ++i) {
		struct pwm_device *pwm = &chip->chip.pwms[i];
		struct samsung_pwm_channel *chan = pwm_get_chip_data(pwm);

		if (!chan)
			continue;

		chan->period_ns = 0;
		chan->duty_ns = 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int s3c_pwm_resume(struct device *dev)
{
	struct s3c_chip *s3c = dev_get_drvdata(dev);
	unsigned long tcon;

	/* Restore invertion */
	tcon = __raw_readl(S3C2410_TCON);
	tcon |= pwm_tcon_invert(s3c);
	__raw_writel(tcon, S3C2410_TCON);
=======
static int pwm_samsung_resume(struct device *dev)
{
	struct samsung_pwm_chip *chip = dev_get_drvdata(dev);
	unsigned int chan;

	/*
	 * Inverter setting must be preserved across suspend/resume
	 * as nobody really seems to configure it more than once.
	 */
	for (chan = 0; chan < SAMSUNG_PWM_NUM; ++chan) {
		if (chip->variant.output_mask & BIT(chan))
			pwm_samsung_set_invert(chip, chan,
					chip->inverter_mask & BIT(chan));
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
#endif

<<<<<<< HEAD
static SIMPLE_DEV_PM_OPS(s3c_pwm_pm_ops, s3c_pwm_suspend,
			s3c_pwm_resume);

static struct platform_driver s3c_pwm_driver = {
	.driver		= {
		.name	= "s3c24xx-pwm",
		.owner	= THIS_MODULE,
		.pm	= &s3c_pwm_pm_ops,
	},
	.probe		= s3c_pwm_probe,
	.remove		= s3c_pwm_remove,
};

static int __init pwm_init(void)
{
	int ret;

	clk_scaler[0] = clk_get(NULL, "pwm-scaler0");
	clk_scaler[1] = clk_get(NULL, "pwm-scaler1");

	if (IS_ERR(clk_scaler[0]) || IS_ERR(clk_scaler[1])) {
		pr_err("failed to get scaler clocks\n");
		return -EINVAL;
	}

	ret = platform_driver_register(&s3c_pwm_driver);
	if (ret)
		pr_err("failed to add pwm driver\n");

	return ret;
}

arch_initcall(pwm_init);
=======
static SIMPLE_DEV_PM_OPS(pwm_samsung_pm_ops, pwm_samsung_suspend,
			 pwm_samsung_resume);

static struct platform_driver pwm_samsung_driver = {
	.driver		= {
		.name	= "samsung-pwm",
		.pm	= &pwm_samsung_pm_ops,
		.of_match_table = of_match_ptr(samsung_pwm_matches),
	},
	.probe		= pwm_samsung_probe,
	.remove		= pwm_samsung_remove,
};
module_platform_driver(pwm_samsung_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomasz Figa <tomasz.figa@gmail.com>");
MODULE_ALIAS("platform:samsung-pwm");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
