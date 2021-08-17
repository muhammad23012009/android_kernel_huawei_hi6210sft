/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
<<<<<<< HEAD

#include <linux/module.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include <linux/mfd/pm8xxx/core.h>
#include <linux/mfd/pm8xxx/rtc.h>


=======
#include <linux/of.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* RTC Register offsets from RTC CTRL REG */
#define PM8XXX_ALARM_CTRL_OFFSET	0x01
#define PM8XXX_RTC_WRITE_OFFSET		0x02
#define PM8XXX_RTC_READ_OFFSET		0x06
#define PM8XXX_ALARM_RW_OFFSET		0x0A

/* RTC_CTRL register bit fields */
#define PM8xxx_RTC_ENABLE		BIT(7)
<<<<<<< HEAD
#define PM8xxx_RTC_ALARM_ENABLE		BIT(1)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define PM8xxx_RTC_ALARM_CLEAR		BIT(0)

#define NUM_8_BIT_RTC_REGS		0x4

/**
<<<<<<< HEAD
 * struct pm8xxx_rtc -  rtc driver internal structure
 * @rtc:		rtc device for this driver.
 * @rtc_alarm_irq:	rtc alarm irq number.
 * @rtc_base:		address of rtc control register.
 * @rtc_read_base:	base address of read registers.
 * @rtc_write_base:	base address of write registers.
 * @alarm_rw_base:	base address of alarm registers.
=======
 * struct pm8xxx_rtc_regs - describe RTC registers per PMIC versions
 * @ctrl: base address of control register
 * @write: base address of write register
 * @read: base address of read register
 * @alarm_ctrl: base address of alarm control register
 * @alarm_ctrl2: base address of alarm control2 register
 * @alarm_rw: base address of alarm read-write register
 * @alarm_en: alarm enable mask
 */
struct pm8xxx_rtc_regs {
	unsigned int ctrl;
	unsigned int write;
	unsigned int read;
	unsigned int alarm_ctrl;
	unsigned int alarm_ctrl2;
	unsigned int alarm_rw;
	unsigned int alarm_en;
};

/**
 * struct pm8xxx_rtc -  rtc driver internal structure
 * @rtc:		rtc device for this driver.
 * @regmap:		regmap used to access RTC registers
 * @allow_set_time:	indicates whether writing to the RTC is allowed
 * @rtc_alarm_irq:	rtc alarm irq number.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @ctrl_reg:		rtc control register.
 * @rtc_dev:		device structure.
 * @ctrl_reg_lock:	spinlock protecting access to ctrl_reg.
 */
struct pm8xxx_rtc {
	struct rtc_device *rtc;
<<<<<<< HEAD
	int rtc_alarm_irq;
	int rtc_base;
	int rtc_read_base;
	int rtc_write_base;
	int alarm_rw_base;
	u8  ctrl_reg;
=======
	struct regmap *regmap;
	bool allow_set_time;
	int rtc_alarm_irq;
	const struct pm8xxx_rtc_regs *regs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct device *rtc_dev;
	spinlock_t ctrl_reg_lock;
};

/*
<<<<<<< HEAD
 * The RTC registers need to be read/written one byte at a time. This is a
 * hardware limitation.
 */
static int pm8xxx_read_wrapper(struct pm8xxx_rtc *rtc_dd, u8 *rtc_val,
		int base, int count)
{
	int i, rc;
	struct device *parent = rtc_dd->rtc_dev->parent;

	for (i = 0; i < count; i++) {
		rc = pm8xxx_readb(parent, base + i, &rtc_val[i]);
		if (rc < 0) {
			dev_err(rtc_dd->rtc_dev, "PMIC read failed\n");
			return rc;
		}
	}

	return 0;
}

static int pm8xxx_write_wrapper(struct pm8xxx_rtc *rtc_dd, u8 *rtc_val,
		int base, int count)
{
	int i, rc;
	struct device *parent = rtc_dd->rtc_dev->parent;

	for (i = 0; i < count; i++) {
		rc = pm8xxx_writeb(parent, base + i, rtc_val[i]);
		if (rc < 0) {
			dev_err(rtc_dd->rtc_dev, "PMIC write failed\n");
			return rc;
		}
	}

	return 0;
}

/*
 * Steps to write the RTC registers.
 * 1. Disable alarm if enabled.
 * 2. Write 0x00 to LSB.
 * 3. Write Byte[1], Byte[2], Byte[3] then Byte[0].
 * 4. Enable alarm if disabled in step 1.
=======
 * Steps to write the RTC registers.
 * 1. Disable alarm if enabled.
 * 2. Disable rtc if enabled.
 * 3. Write 0x00 to LSB.
 * 4. Write Byte[1], Byte[2], Byte[3] then Byte[0].
 * 5. Enable rtc if disabled in step 2.
 * 6. Enable alarm if disabled in step 1.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int pm8xxx_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	int rc, i;
	unsigned long secs, irq_flags;
<<<<<<< HEAD
	u8 value[NUM_8_BIT_RTC_REGS], reg = 0, alarm_enabled = 0, ctrl_reg;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rtc_tm_to_time(tm, &secs);

=======
	u8 value[NUM_8_BIT_RTC_REGS], alarm_enabled = 0, rtc_disabled = 0;
	unsigned int ctrl_reg, rtc_ctrl_reg;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;

	if (!rtc_dd->allow_set_time)
		return -EACCES;

	rtc_tm_to_time(tm, &secs);

	dev_dbg(dev, "Seconds value to be written to RTC = %lu\n", secs);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	for (i = 0; i < NUM_8_BIT_RTC_REGS; i++) {
		value[i] = secs & 0xFF;
		secs >>= 8;
	}

<<<<<<< HEAD
	dev_dbg(dev, "Seconds value to be written to RTC = %lu\n", secs);

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);
	ctrl_reg = rtc_dd->ctrl_reg;

	if (ctrl_reg & PM8xxx_RTC_ALARM_ENABLE) {
		alarm_enabled = 1;
		ctrl_reg &= ~PM8xxx_RTC_ALARM_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
				1);
		if (rc < 0) {
			dev_err(dev, "Write to RTC control register "
								"failed\n");
			goto rtc_rw_fail;
		}
		rtc_dd->ctrl_reg = ctrl_reg;
	} else
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Write 0 to Byte[0] */
	reg = 0;
	rc = pm8xxx_write_wrapper(rtc_dd, &reg, rtc_dd->rtc_write_base, 1);
	if (rc < 0) {
=======
	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl, &ctrl_reg);
	if (rc)
		goto rtc_rw_fail;

	if (ctrl_reg & regs->alarm_en) {
		alarm_enabled = 1;
		ctrl_reg &= ~regs->alarm_en;
		rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl, ctrl_reg);
		if (rc) {
			dev_err(dev, "Write to RTC Alarm control register failed\n");
			goto rtc_rw_fail;
		}
	}

	/* Disable RTC H/w before writing on RTC register */
	rc = regmap_read(rtc_dd->regmap, regs->ctrl, &rtc_ctrl_reg);
	if (rc)
		goto rtc_rw_fail;

	if (rtc_ctrl_reg & PM8xxx_RTC_ENABLE) {
		rtc_disabled = 1;
		rtc_ctrl_reg &= ~PM8xxx_RTC_ENABLE;
		rc = regmap_write(rtc_dd->regmap, regs->ctrl, rtc_ctrl_reg);
		if (rc) {
			dev_err(dev, "Write to RTC control register failed\n");
			goto rtc_rw_fail;
		}
	}

	/* Write 0 to Byte[0] */
	rc = regmap_write(rtc_dd->regmap, regs->write, 0);
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

	/* Write Byte[1], Byte[2], Byte[3] */
<<<<<<< HEAD
	rc = pm8xxx_write_wrapper(rtc_dd, value + 1,
					rtc_dd->rtc_write_base + 1, 3);
	if (rc < 0) {
=======
	rc = regmap_bulk_write(rtc_dd->regmap, regs->write + 1,
			       &value[1], sizeof(value) - 1);
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

	/* Write Byte[0] */
<<<<<<< HEAD
	rc = pm8xxx_write_wrapper(rtc_dd, value, rtc_dd->rtc_write_base, 1);
	if (rc < 0) {
=======
	rc = regmap_write(rtc_dd->regmap, regs->write, value[0]);
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "Write to RTC write data register failed\n");
		goto rtc_rw_fail;
	}

<<<<<<< HEAD
	if (alarm_enabled) {
		ctrl_reg |= PM8xxx_RTC_ALARM_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
									1);
		if (rc < 0) {
			dev_err(dev, "Write to RTC control register "
								"failed\n");
			goto rtc_rw_fail;
		}
		rtc_dd->ctrl_reg = ctrl_reg;
	}

rtc_rw_fail:
	if (alarm_enabled)
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
=======
	/* Enable RTC H/w after writing on RTC register */
	if (rtc_disabled) {
		rtc_ctrl_reg |= PM8xxx_RTC_ENABLE;
		rc = regmap_write(rtc_dd->regmap, regs->ctrl, rtc_ctrl_reg);
		if (rc) {
			dev_err(dev, "Write to RTC control register failed\n");
			goto rtc_rw_fail;
		}
	}

	if (alarm_enabled) {
		ctrl_reg |= regs->alarm_en;
		rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl, ctrl_reg);
		if (rc) {
			dev_err(dev, "Write to RTC Alarm control register failed\n");
			goto rtc_rw_fail;
		}
	}

rtc_rw_fail:
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rc;
}

static int pm8xxx_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	int rc;
<<<<<<< HEAD
	u8 value[NUM_8_BIT_RTC_REGS], reg;
	unsigned long secs;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	rc = pm8xxx_read_wrapper(rtc_dd, value, rtc_dd->rtc_read_base,
							NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
=======
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned long secs;
	unsigned int reg;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;

	rc = regmap_bulk_read(rtc_dd->regmap, regs->read, value, sizeof(value));
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "RTC read data register failed\n");
		return rc;
	}

	/*
	 * Read the LSB again and check if there has been a carry over.
	 * If there is, redo the read operation.
	 */
<<<<<<< HEAD
	rc = pm8xxx_read_wrapper(rtc_dd, &reg, rtc_dd->rtc_read_base, 1);
=======
	rc = regmap_read(rtc_dd->regmap, regs->read, &reg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rc < 0) {
		dev_err(dev, "RTC read data register failed\n");
		return rc;
	}

	if (unlikely(reg < value[0])) {
<<<<<<< HEAD
		rc = pm8xxx_read_wrapper(rtc_dd, value,
				rtc_dd->rtc_read_base, NUM_8_BIT_RTC_REGS);
		if (rc < 0) {
=======
		rc = regmap_bulk_read(rtc_dd->regmap, regs->read,
				      value, sizeof(value));
		if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			dev_err(dev, "RTC read data register failed\n");
			return rc;
		}
	}

<<<<<<< HEAD
	secs = value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24);
=======
	secs = value[0] | (value[1] << 8) | (value[2] << 16) |
	       ((unsigned long)value[3] << 24);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rtc_time_to_tm(secs, tm);

	rc = rtc_valid_tm(tm);
	if (rc < 0) {
		dev_err(dev, "Invalid time read from RTC\n");
		return rc;
	}

	dev_dbg(dev, "secs = %lu, h:m:s == %d:%d:%d, d/m/y = %d/%d/%d\n",
<<<<<<< HEAD
				secs, tm->tm_hour, tm->tm_min, tm->tm_sec,
				tm->tm_mday, tm->tm_mon, tm->tm_year);
=======
		secs, tm->tm_hour, tm->tm_min, tm->tm_sec,
		tm->tm_mday, tm->tm_mon, tm->tm_year);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int pm8xxx_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	int rc, i;
<<<<<<< HEAD
	u8 value[NUM_8_BIT_RTC_REGS], ctrl_reg;
	unsigned long secs, irq_flags;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
=======
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned int ctrl_reg;
	unsigned long secs, irq_flags;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rtc_tm_to_time(&alarm->time, &secs);

	for (i = 0; i < NUM_8_BIT_RTC_REGS; i++) {
		value[i] = secs & 0xFF;
		secs >>= 8;
	}

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

<<<<<<< HEAD
	rc = pm8xxx_write_wrapper(rtc_dd, value, rtc_dd->alarm_rw_base,
							NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
=======
	rc = regmap_bulk_write(rtc_dd->regmap, regs->alarm_rw, value,
			       sizeof(value));
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "Write to RTC ALARM register failed\n");
		goto rtc_rw_fail;
	}

<<<<<<< HEAD
	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg = alarm->enabled ? (ctrl_reg | PM8xxx_RTC_ALARM_ENABLE) :
					(ctrl_reg & ~PM8xxx_RTC_ALARM_ENABLE);

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(dev, "Write to RTC control register failed\n");
		goto rtc_rw_fail;
	}

	rtc_dd->ctrl_reg = ctrl_reg;

	dev_dbg(dev, "Alarm Set for h:r:s=%d:%d:%d, d/m/y=%d/%d/%d\n",
				alarm->time.tm_hour, alarm->time.tm_min,
				alarm->time.tm_sec, alarm->time.tm_mday,
				alarm->time.tm_mon, alarm->time.tm_year);
=======
	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl, &ctrl_reg);
	if (rc)
		goto rtc_rw_fail;

	if (alarm->enabled)
		ctrl_reg |= regs->alarm_en;
	else
		ctrl_reg &= ~regs->alarm_en;

	rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl, ctrl_reg);
	if (rc) {
		dev_err(dev, "Write to RTC alarm control register failed\n");
		goto rtc_rw_fail;
	}

	dev_dbg(dev, "Alarm Set for h:r:s=%d:%d:%d, d/m/y=%d/%d/%d\n",
		alarm->time.tm_hour, alarm->time.tm_min,
		alarm->time.tm_sec, alarm->time.tm_mday,
		alarm->time.tm_mon, alarm->time.tm_year);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
rtc_rw_fail:
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
	return rc;
}

static int pm8xxx_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	int rc;
	u8 value[NUM_8_BIT_RTC_REGS];
	unsigned long secs;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
<<<<<<< HEAD

	rc = pm8xxx_read_wrapper(rtc_dd, value, rtc_dd->alarm_rw_base,
			NUM_8_BIT_RTC_REGS);
	if (rc < 0) {
=======
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;

	rc = regmap_bulk_read(rtc_dd->regmap, regs->alarm_rw, value,
			      sizeof(value));
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "RTC alarm time read failed\n");
		return rc;
	}

<<<<<<< HEAD
	secs = value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24);
=======
	secs = value[0] | (value[1] << 8) | (value[2] << 16) |
	       ((unsigned long)value[3] << 24);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rtc_time_to_tm(secs, &alarm->time);

	rc = rtc_valid_tm(&alarm->time);
	if (rc < 0) {
		dev_err(dev, "Invalid alarm time read from RTC\n");
		return rc;
	}

	dev_dbg(dev, "Alarm set for - h:r:s=%d:%d:%d, d/m/y=%d/%d/%d\n",
<<<<<<< HEAD
				alarm->time.tm_hour, alarm->time.tm_min,
				alarm->time.tm_sec, alarm->time.tm_mday,
				alarm->time.tm_mon, alarm->time.tm_year);
=======
		alarm->time.tm_hour, alarm->time.tm_min,
		alarm->time.tm_sec, alarm->time.tm_mday,
		alarm->time.tm_mon, alarm->time.tm_year);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int pm8xxx_rtc_alarm_irq_enable(struct device *dev, unsigned int enable)
{
	int rc;
	unsigned long irq_flags;
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);
<<<<<<< HEAD
	u8 ctrl_reg;

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);
	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg = (enable) ? (ctrl_reg | PM8xxx_RTC_ALARM_ENABLE) :
				(ctrl_reg & ~PM8xxx_RTC_ALARM_ENABLE);

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
=======
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	unsigned int ctrl_reg;

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl, &ctrl_reg);
	if (rc)
		goto rtc_rw_fail;

	if (enable)
		ctrl_reg |= regs->alarm_en;
	else
		ctrl_reg &= ~regs->alarm_en;

	rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl, ctrl_reg);
	if (rc) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_err(dev, "Write to RTC control register failed\n");
		goto rtc_rw_fail;
	}

<<<<<<< HEAD
	rtc_dd->ctrl_reg = ctrl_reg;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
rtc_rw_fail:
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
	return rc;
}

<<<<<<< HEAD
static struct rtc_class_ops pm8xxx_rtc_ops = {
	.read_time	= pm8xxx_rtc_read_time,
=======
static const struct rtc_class_ops pm8xxx_rtc_ops = {
	.read_time	= pm8xxx_rtc_read_time,
	.set_time	= pm8xxx_rtc_set_time,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.set_alarm	= pm8xxx_rtc_set_alarm,
	.read_alarm	= pm8xxx_rtc_read_alarm,
	.alarm_irq_enable = pm8xxx_rtc_alarm_irq_enable,
};

static irqreturn_t pm8xxx_alarm_trigger(int irq, void *dev_id)
{
	struct pm8xxx_rtc *rtc_dd = dev_id;
<<<<<<< HEAD
	u8 ctrl_reg;
=======
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	unsigned int ctrl_reg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int rc;
	unsigned long irq_flags;

	rtc_update_irq(rtc_dd->rtc, 1, RTC_IRQF | RTC_AF);

	spin_lock_irqsave(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Clear the alarm enable bit */
<<<<<<< HEAD
	ctrl_reg = rtc_dd->ctrl_reg;
	ctrl_reg &= ~PM8xxx_RTC_ALARM_ENABLE;

	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
		dev_err(rtc_dd->rtc_dev, "Write to RTC control register "
								"failed\n");
		goto rtc_alarm_handled;
	}

	rtc_dd->ctrl_reg = ctrl_reg;
	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Clear RTC alarm register */
	rc = pm8xxx_read_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base +
						PM8XXX_ALARM_CTRL_OFFSET, 1);
	if (rc < 0) {
		dev_err(rtc_dd->rtc_dev, "RTC Alarm control register read "
								"failed\n");
		goto rtc_alarm_handled;
	}

	ctrl_reg &= ~PM8xxx_RTC_ALARM_CLEAR;
	rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base +
						PM8XXX_ALARM_CTRL_OFFSET, 1);
	if (rc < 0)
		dev_err(rtc_dd->rtc_dev, "Write to RTC Alarm control register"
								" failed\n");
=======
	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl, &ctrl_reg);
	if (rc) {
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
		goto rtc_alarm_handled;
	}

	ctrl_reg &= ~regs->alarm_en;

	rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl, ctrl_reg);
	if (rc) {
		spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);
		dev_err(rtc_dd->rtc_dev,
			"Write to alarm control register failed\n");
		goto rtc_alarm_handled;
	}

	spin_unlock_irqrestore(&rtc_dd->ctrl_reg_lock, irq_flags);

	/* Clear RTC alarm register */
	rc = regmap_read(rtc_dd->regmap, regs->alarm_ctrl2, &ctrl_reg);
	if (rc) {
		dev_err(rtc_dd->rtc_dev,
			"RTC Alarm control2 register read failed\n");
		goto rtc_alarm_handled;
	}

	ctrl_reg |= PM8xxx_RTC_ALARM_CLEAR;
	rc = regmap_write(rtc_dd->regmap, regs->alarm_ctrl2, ctrl_reg);
	if (rc)
		dev_err(rtc_dd->rtc_dev,
			"Write to RTC Alarm control2 register failed\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

rtc_alarm_handled:
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int pm8xxx_rtc_probe(struct platform_device *pdev)
{
	int rc;
	u8 ctrl_reg;
	bool rtc_write_enable = false;
	struct pm8xxx_rtc *rtc_dd;
	struct resource *rtc_resource;
	const struct pm8xxx_rtc_platform_data *pdata =
						dev_get_platdata(&pdev->dev);

	if (pdata != NULL)
		rtc_write_enable = pdata->rtc_write_enable;

	rtc_dd = kzalloc(sizeof(*rtc_dd), GFP_KERNEL);
	if (rtc_dd == NULL) {
		dev_err(&pdev->dev, "Unable to allocate memory!\n");
		return -ENOMEM;
	}
=======
static int pm8xxx_rtc_enable(struct pm8xxx_rtc *rtc_dd)
{
	const struct pm8xxx_rtc_regs *regs = rtc_dd->regs;
	unsigned int ctrl_reg;
	int rc;

	/* Check if the RTC is on, else turn it on */
	rc = regmap_read(rtc_dd->regmap, regs->ctrl, &ctrl_reg);
	if (rc)
		return rc;

	if (!(ctrl_reg & PM8xxx_RTC_ENABLE)) {
		ctrl_reg |= PM8xxx_RTC_ENABLE;
		rc = regmap_write(rtc_dd->regmap, regs->ctrl, ctrl_reg);
		if (rc)
			return rc;
	}

	return 0;
}

static const struct pm8xxx_rtc_regs pm8921_regs = {
	.ctrl		= 0x11d,
	.write		= 0x11f,
	.read		= 0x123,
	.alarm_rw	= 0x127,
	.alarm_ctrl	= 0x11d,
	.alarm_ctrl2	= 0x11e,
	.alarm_en	= BIT(1),
};

static const struct pm8xxx_rtc_regs pm8058_regs = {
	.ctrl		= 0x1e8,
	.write		= 0x1ea,
	.read		= 0x1ee,
	.alarm_rw	= 0x1f2,
	.alarm_ctrl	= 0x1e8,
	.alarm_ctrl2	= 0x1e9,
	.alarm_en	= BIT(1),
};

static const struct pm8xxx_rtc_regs pm8941_regs = {
	.ctrl		= 0x6046,
	.write		= 0x6040,
	.read		= 0x6048,
	.alarm_rw	= 0x6140,
	.alarm_ctrl	= 0x6146,
	.alarm_ctrl2	= 0x6148,
	.alarm_en	= BIT(7),
};

/*
 * Hardcoded RTC bases until IORESOURCE_REG mapping is figured out
 */
static const struct of_device_id pm8xxx_id_table[] = {
	{ .compatible = "qcom,pm8921-rtc", .data = &pm8921_regs },
	{ .compatible = "qcom,pm8018-rtc", .data = &pm8921_regs },
	{ .compatible = "qcom,pm8058-rtc", .data = &pm8058_regs },
	{ .compatible = "qcom,pm8941-rtc", .data = &pm8941_regs },
	{ },
};
MODULE_DEVICE_TABLE(of, pm8xxx_id_table);

static int pm8xxx_rtc_probe(struct platform_device *pdev)
{
	int rc;
	struct pm8xxx_rtc *rtc_dd;
	const struct of_device_id *match;

	match = of_match_node(pm8xxx_id_table, pdev->dev.of_node);
	if (!match)
		return -ENXIO;

	rtc_dd = devm_kzalloc(&pdev->dev, sizeof(*rtc_dd), GFP_KERNEL);
	if (rtc_dd == NULL)
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Initialise spinlock to protect RTC control register */
	spin_lock_init(&rtc_dd->ctrl_reg_lock);

<<<<<<< HEAD
	rtc_dd->rtc_alarm_irq = platform_get_irq(pdev, 0);
	if (rtc_dd->rtc_alarm_irq < 0) {
		dev_err(&pdev->dev, "Alarm IRQ resource absent!\n");
		rc = -ENXIO;
		goto fail_rtc_enable;
	}

	rtc_resource = platform_get_resource_byname(pdev, IORESOURCE_IO,
							"pmic_rtc_base");
	if (!(rtc_resource && rtc_resource->start)) {
		dev_err(&pdev->dev, "RTC IO resource absent!\n");
		rc = -ENXIO;
		goto fail_rtc_enable;
	}

	rtc_dd->rtc_base = rtc_resource->start;

	/* Setup RTC register addresses */
	rtc_dd->rtc_write_base = rtc_dd->rtc_base + PM8XXX_RTC_WRITE_OFFSET;
	rtc_dd->rtc_read_base = rtc_dd->rtc_base + PM8XXX_RTC_READ_OFFSET;
	rtc_dd->alarm_rw_base = rtc_dd->rtc_base + PM8XXX_ALARM_RW_OFFSET;

	rtc_dd->rtc_dev = &pdev->dev;

	/* Check if the RTC is on, else turn it on */
	rc = pm8xxx_read_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base, 1);
	if (rc < 0) {
		dev_err(&pdev->dev, "RTC control register read failed!\n");
		goto fail_rtc_enable;
	}

	if (!(ctrl_reg & PM8xxx_RTC_ENABLE)) {
		ctrl_reg |= PM8xxx_RTC_ENABLE;
		rc = pm8xxx_write_wrapper(rtc_dd, &ctrl_reg, rtc_dd->rtc_base,
									1);
		if (rc < 0) {
			dev_err(&pdev->dev, "Write to RTC control register "
								"failed\n");
			goto fail_rtc_enable;
		}
	}

	rtc_dd->ctrl_reg = ctrl_reg;
	if (rtc_write_enable == true)
		pm8xxx_rtc_ops.set_time = pm8xxx_rtc_set_time;

	platform_set_drvdata(pdev, rtc_dd);

	/* Register the RTC device */
	rtc_dd->rtc = rtc_device_register("pm8xxx_rtc", &pdev->dev,
				&pm8xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc_dd->rtc)) {
		dev_err(&pdev->dev, "%s: RTC registration failed (%ld)\n",
					__func__, PTR_ERR(rtc_dd->rtc));
		rc = PTR_ERR(rtc_dd->rtc);
		goto fail_rtc_enable;
	}

	/* Request the alarm IRQ */
	rc = request_any_context_irq(rtc_dd->rtc_alarm_irq,
				 pm8xxx_alarm_trigger, IRQF_TRIGGER_RISING,
				 "pm8xxx_rtc_alarm", rtc_dd);
	if (rc < 0) {
		dev_err(&pdev->dev, "Request IRQ failed (%d)\n", rc);
		goto fail_req_irq;
	}

	device_init_wakeup(&pdev->dev, 1);

	dev_dbg(&pdev->dev, "Probe success !!\n");

	return 0;

fail_req_irq:
	rtc_device_unregister(rtc_dd->rtc);
fail_rtc_enable:
	platform_set_drvdata(pdev, NULL);
	kfree(rtc_dd);
	return rc;
}

static int pm8xxx_rtc_remove(struct platform_device *pdev)
{
	struct pm8xxx_rtc *rtc_dd = platform_get_drvdata(pdev);

	device_init_wakeup(&pdev->dev, 0);
	free_irq(rtc_dd->rtc_alarm_irq, rtc_dd);
	rtc_device_unregister(rtc_dd->rtc);
	platform_set_drvdata(pdev, NULL);
	kfree(rtc_dd);

	return 0;
=======
	rtc_dd->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!rtc_dd->regmap) {
		dev_err(&pdev->dev, "Parent regmap unavailable.\n");
		return -ENXIO;
	}

	rtc_dd->rtc_alarm_irq = platform_get_irq(pdev, 0);
	if (rtc_dd->rtc_alarm_irq < 0) {
		dev_err(&pdev->dev, "Alarm IRQ resource absent!\n");
		return -ENXIO;
	}

	rtc_dd->allow_set_time = of_property_read_bool(pdev->dev.of_node,
						      "allow-set-time");

	rtc_dd->regs = match->data;
	rtc_dd->rtc_dev = &pdev->dev;

	rc = pm8xxx_rtc_enable(rtc_dd);
	if (rc)
		return rc;

	platform_set_drvdata(pdev, rtc_dd);

	device_init_wakeup(&pdev->dev, 1);

	/* Register the RTC device */
	rtc_dd->rtc = devm_rtc_device_register(&pdev->dev, "pm8xxx_rtc",
					       &pm8xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc_dd->rtc)) {
		dev_err(&pdev->dev, "%s: RTC registration failed (%ld)\n",
			__func__, PTR_ERR(rtc_dd->rtc));
		return PTR_ERR(rtc_dd->rtc);
	}

	/* Request the alarm IRQ */
	rc = devm_request_any_context_irq(&pdev->dev, rtc_dd->rtc_alarm_irq,
					  pm8xxx_alarm_trigger,
					  IRQF_TRIGGER_RISING,
					  "pm8xxx_rtc_alarm", rtc_dd);
	if (rc < 0) {
		dev_err(&pdev->dev, "Request IRQ failed (%d)\n", rc);
		return rc;
	}

	dev_dbg(&pdev->dev, "Probe success !!\n");

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_PM_SLEEP
static int pm8xxx_rtc_resume(struct device *dev)
{
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(rtc_dd->rtc_alarm_irq);

	return 0;
}

static int pm8xxx_rtc_suspend(struct device *dev)
{
	struct pm8xxx_rtc *rtc_dd = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(rtc_dd->rtc_alarm_irq);

	return 0;
}
#endif

<<<<<<< HEAD
static SIMPLE_DEV_PM_OPS(pm8xxx_rtc_pm_ops, pm8xxx_rtc_suspend, pm8xxx_rtc_resume);

static struct platform_driver pm8xxx_rtc_driver = {
	.probe		= pm8xxx_rtc_probe,
	.remove		= pm8xxx_rtc_remove,
	.driver	= {
		.name	= PM8XXX_RTC_DEV_NAME,
		.owner	= THIS_MODULE,
		.pm	= &pm8xxx_rtc_pm_ops,
=======
static SIMPLE_DEV_PM_OPS(pm8xxx_rtc_pm_ops,
			 pm8xxx_rtc_suspend,
			 pm8xxx_rtc_resume);

static struct platform_driver pm8xxx_rtc_driver = {
	.probe		= pm8xxx_rtc_probe,
	.driver	= {
		.name		= "rtc-pm8xxx",
		.pm		= &pm8xxx_rtc_pm_ops,
		.of_match_table	= pm8xxx_id_table,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};

module_platform_driver(pm8xxx_rtc_driver);

MODULE_ALIAS("platform:rtc-pm8xxx");
MODULE_DESCRIPTION("PMIC8xxx RTC driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Anirudh Ghayal <aghayal@codeaurora.org>");
