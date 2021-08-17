/*
 * RTC subsystem, sysfs interface
 *
 * Copyright (C) 2005 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/rtc.h>

#include "rtc-core.h"


/* device attributes */

/*
 * NOTE:  RTC times displayed in sysfs use the RTC's timezone.  That's
 * ideally UTC.  However, PCs that also boot to MS-Windows normally use
 * the local time and change to match daylight savings time.  That affects
 * attributes including date, time, since_epoch, and wakealarm.
 */

static ssize_t
<<<<<<< HEAD
rtc_sysfs_show_name(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return sprintf(buf, "%s\n", to_rtc_device(dev)->name);
}

static ssize_t
rtc_sysfs_show_date(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
name_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", to_rtc_device(dev)->name);
}
static DEVICE_ATTR_RO(name);

static ssize_t
date_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		retval = sprintf(buf, "%04d-%02d-%02d\n",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	}

	return retval;
}
<<<<<<< HEAD

static ssize_t
rtc_sysfs_show_time(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
static DEVICE_ATTR_RO(date);

static ssize_t
time_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		retval = sprintf(buf, "%02d:%02d:%02d\n",
			tm.tm_hour, tm.tm_min, tm.tm_sec);
	}

	return retval;
}
<<<<<<< HEAD

static ssize_t
rtc_sysfs_show_since_epoch(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
static DEVICE_ATTR_RO(time);

static ssize_t
since_epoch_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ssize_t retval;
	struct rtc_time tm;

	retval = rtc_read_time(to_rtc_device(dev), &tm);
	if (retval == 0) {
		unsigned long time;
		rtc_tm_to_time(&tm, &time);
		retval = sprintf(buf, "%lu\n", time);
	}

	return retval;
}
<<<<<<< HEAD

static ssize_t
rtc_sysfs_show_max_user_freq(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
static DEVICE_ATTR_RO(since_epoch);

static ssize_t
max_user_freq_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return sprintf(buf, "%d\n", to_rtc_device(dev)->max_user_freq);
}

static ssize_t
<<<<<<< HEAD
rtc_sysfs_set_max_user_freq(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct rtc_device *rtc = to_rtc_device(dev);
	unsigned long val = simple_strtoul(buf, NULL, 0);
=======
max_user_freq_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct rtc_device *rtc = to_rtc_device(dev);
	unsigned long val;
	int err;

	err = kstrtoul(buf, 0, &val);
	if (err)
		return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (val >= 4096 || val == 0)
		return -EINVAL;

	rtc->max_user_freq = (int)val;

	return n;
}
<<<<<<< HEAD
=======
static DEVICE_ATTR_RW(max_user_freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * rtc_sysfs_show_hctosys - indicate if the given RTC set the system time
 *
 * Returns 1 if the system clock was set by this RTC at the last
 * boot or resume event.
 */
static ssize_t
<<<<<<< HEAD
rtc_sysfs_show_hctosys(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
hctosys_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
#ifdef CONFIG_RTC_HCTOSYS_DEVICE
	if (rtc_hctosys_ret == 0 &&
			strcmp(dev_name(&to_rtc_device(dev)->dev),
				CONFIG_RTC_HCTOSYS_DEVICE) == 0)
		return sprintf(buf, "1\n");
	else
#endif
		return sprintf(buf, "0\n");
}
<<<<<<< HEAD

static struct device_attribute rtc_attrs[] = {
	__ATTR(name, S_IRUGO, rtc_sysfs_show_name, NULL),
	__ATTR(date, S_IRUGO, rtc_sysfs_show_date, NULL),
	__ATTR(time, S_IRUGO, rtc_sysfs_show_time, NULL),
	__ATTR(since_epoch, S_IRUGO, rtc_sysfs_show_since_epoch, NULL),
	__ATTR(max_user_freq, S_IRUGO | S_IWUSR, rtc_sysfs_show_max_user_freq,
			rtc_sysfs_set_max_user_freq),
	__ATTR(hctosys, S_IRUGO, rtc_sysfs_show_hctosys, NULL),
	{ },
};

static ssize_t
rtc_sysfs_show_wakealarm(struct device *dev, struct device_attribute *attr,
		char *buf)
=======
static DEVICE_ATTR_RO(hctosys);

static ssize_t
wakealarm_show(struct device *dev, struct device_attribute *attr, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ssize_t retval;
	unsigned long alarm;
	struct rtc_wkalrm alm;

	/* Don't show disabled alarms.  For uniformity, RTC alarms are
	 * conceptually one-shot, even though some common RTCs (on PCs)
	 * don't actually work that way.
	 *
	 * NOTE: RTC implementations where the alarm doesn't match an
	 * exact YYYY-MM-DD HH:MM[:SS] date *must* disable their RTC
	 * alarms after they trigger, to ensure one-shot semantics.
	 */
	retval = rtc_read_alarm(to_rtc_device(dev), &alm);
	if (retval == 0 && alm.enabled) {
		rtc_tm_to_time(&alm.time, &alarm);
		retval = sprintf(buf, "%lu\n", alarm);
	}

	return retval;
}

static ssize_t
<<<<<<< HEAD
rtc_sysfs_set_wakealarm(struct device *dev, struct device_attribute *attr,
=======
wakealarm_store(struct device *dev, struct device_attribute *attr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		const char *buf, size_t n)
{
	ssize_t retval;
	unsigned long now, alarm;
<<<<<<< HEAD
	struct rtc_wkalrm alm;
	struct rtc_device *rtc = to_rtc_device(dev);
	char *buf_ptr;
=======
	unsigned long push = 0;
	struct rtc_wkalrm alm;
	struct rtc_device *rtc = to_rtc_device(dev);
	const char *buf_ptr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int adjust = 0;

	/* Only request alarms that trigger in the future.  Disable them
	 * by writing another time, e.g. 0 meaning Jan 1 1970 UTC.
	 */
	retval = rtc_read_time(rtc, &alm.time);
	if (retval < 0)
		return retval;
	rtc_tm_to_time(&alm.time, &now);

<<<<<<< HEAD
	buf_ptr = (char *)buf;
	if (*buf_ptr == '+') {
		buf_ptr++;
		adjust = 1;
	}
	alarm = simple_strtoul(buf_ptr, NULL, 0);
	if (adjust) {
		alarm += now;
	}
	if (alarm > now) {
=======
	buf_ptr = buf;
	if (*buf_ptr == '+') {
		buf_ptr++;
		if (*buf_ptr == '=') {
			buf_ptr++;
			push = 1;
		} else
			adjust = 1;
	}
	retval = kstrtoul(buf_ptr, 0, &alarm);
	if (retval)
		return retval;
	if (adjust) {
		alarm += now;
	}
	if (alarm > now || push) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Avoid accidentally clobbering active alarms; we can't
		 * entirely prevent that here, without even the minimal
		 * locking from the /dev/rtcN api.
		 */
		retval = rtc_read_alarm(rtc, &alm);
		if (retval < 0)
			return retval;
<<<<<<< HEAD
		if (alm.enabled)
			return -EBUSY;

=======
		if (alm.enabled) {
			if (push) {
				rtc_tm_to_time(&alm.time, &push);
				alarm += push;
			} else
				return -EBUSY;
		} else if (push)
			return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		alm.enabled = 1;
	} else {
		alm.enabled = 0;

		/* Provide a valid future alarm time.  Linux isn't EFI,
		 * this time won't be ignored when disabling the alarm.
		 */
		alarm = now + 300;
	}
	rtc_time_to_tm(alarm, &alm.time);

	retval = rtc_set_alarm(rtc, &alm);
	return (retval < 0) ? retval : n;
}
<<<<<<< HEAD
static DEVICE_ATTR(wakealarm, S_IRUGO | S_IWUSR,
		rtc_sysfs_show_wakealarm, rtc_sysfs_set_wakealarm);

=======
static DEVICE_ATTR_RW(wakealarm);

static ssize_t
offset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	long offset;

	retval = rtc_read_offset(to_rtc_device(dev), &offset);
	if (retval == 0)
		retval = sprintf(buf, "%ld\n", offset);

	return retval;
}

static ssize_t
offset_store(struct device *dev, struct device_attribute *attr,
	     const char *buf, size_t n)
{
	ssize_t retval;
	long offset;

	retval = kstrtol(buf, 10, &offset);
	if (retval == 0)
		retval = rtc_set_offset(to_rtc_device(dev), offset);

	return (retval < 0) ? retval : n;
}
static DEVICE_ATTR_RW(offset);

static struct attribute *rtc_attrs[] = {
	&dev_attr_name.attr,
	&dev_attr_date.attr,
	&dev_attr_time.attr,
	&dev_attr_since_epoch.attr,
	&dev_attr_max_user_freq.attr,
	&dev_attr_hctosys.attr,
	&dev_attr_wakealarm.attr,
	&dev_attr_offset.attr,
	NULL,
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* The reason to trigger an alarm with no process watching it (via sysfs)
 * is its side effect:  waking from a system state like suspend-to-RAM or
 * suspend-to-disk.  So: no attribute unless that side effect is possible.
 * (Userspace may disable that mechanism later.)
 */
<<<<<<< HEAD
static inline int rtc_does_wakealarm(struct rtc_device *rtc)
{
	if (!device_can_wakeup(rtc->dev.parent))
		return 0;
	return rtc->ops->set_alarm != NULL;
}


void rtc_sysfs_add_device(struct rtc_device *rtc)
{
	int err;

	/* not all RTCs support both alarms and wakeup */
	if (!rtc_does_wakealarm(rtc))
		return;

	err = device_create_file(&rtc->dev, &dev_attr_wakealarm);
	if (err)
		dev_err(rtc->dev.parent,
			"failed to create alarm attribute, %d\n", err);
}

void rtc_sysfs_del_device(struct rtc_device *rtc)
{
	/* REVISIT did we add it successfully? */
	if (rtc_does_wakealarm(rtc))
		device_remove_file(&rtc->dev, &dev_attr_wakealarm);
}

void __init rtc_sysfs_init(struct class *rtc_class)
{
	rtc_class->dev_attrs = rtc_attrs;
=======
static bool rtc_does_wakealarm(struct rtc_device *rtc)
{
	if (!device_can_wakeup(rtc->dev.parent))
		return false;

	return rtc->ops->set_alarm != NULL;
}

static umode_t rtc_attr_is_visible(struct kobject *kobj,
				   struct attribute *attr, int n)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct rtc_device *rtc = to_rtc_device(dev);
	umode_t mode = attr->mode;

	if (attr == &dev_attr_wakealarm.attr) {
		if (!rtc_does_wakealarm(rtc))
			mode = 0;
	} else if (attr == &dev_attr_offset.attr) {
		if (!rtc->ops->set_offset)
			mode = 0;
	}

	return mode;
}

static struct attribute_group rtc_attr_group = {
	.is_visible	= rtc_attr_is_visible,
	.attrs		= rtc_attrs,
};

static const struct attribute_group *rtc_attr_groups[] = {
	&rtc_attr_group,
	NULL
};

const struct attribute_group **rtc_get_dev_attribute_groups(void)
{
	return rtc_attr_groups;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
