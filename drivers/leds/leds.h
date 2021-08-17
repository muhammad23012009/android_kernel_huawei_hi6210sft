/*
 * LED Core
 *
 * Copyright 2005 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef __LEDS_H_INCLUDED
#define __LEDS_H_INCLUDED

<<<<<<< HEAD
#include <linux/device.h>
#include <linux/rwsem.h>
#include <linux/leds.h>

static inline void __led_set_brightness(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	if (value > led_cdev->max_brightness)
		value = led_cdev->max_brightness;
	led_cdev->brightness = value;
	if (!(led_cdev->flags & LED_SUSPENDED))
		led_cdev->brightness_set(led_cdev, value);
}

=======
#include <linux/rwsem.h>
#include <linux/leds.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int led_get_brightness(struct led_classdev *led_cdev)
{
	return led_cdev->brightness;
}

<<<<<<< HEAD
void led_stop_software_blink(struct led_classdev *led_cdev);

extern struct rw_semaphore leds_list_lock;
extern struct list_head leds_list;

#ifdef CONFIG_LEDS_TRIGGERS
void led_trigger_set_default(struct led_classdev *led_cdev);
void led_trigger_set(struct led_classdev *led_cdev,
			struct led_trigger *trigger);
void led_trigger_remove(struct led_classdev *led_cdev);

static inline void *led_get_trigger_data(struct led_classdev *led_cdev)
{
	return led_cdev->trigger_data;
}

#else
#define led_trigger_set_default(x) do {} while (0)
#define led_trigger_set(x, y) do {} while (0)
#define led_trigger_remove(x) do {} while (0)
#define led_get_trigger_data(x) (NULL)
#endif

ssize_t led_trigger_store(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t count);
ssize_t led_trigger_show(struct device *dev, struct device_attribute *attr,
			char *buf);
=======
void led_init_core(struct led_classdev *led_cdev);
void led_stop_software_blink(struct led_classdev *led_cdev);
void led_set_brightness_nopm(struct led_classdev *led_cdev,
				enum led_brightness value);
void led_set_brightness_nosleep(struct led_classdev *led_cdev,
				enum led_brightness value);

extern struct rw_semaphore leds_list_lock;
extern struct list_head leds_list;
extern struct list_head trigger_list;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif	/* __LEDS_H_INCLUDED */
