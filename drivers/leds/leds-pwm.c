/*
 * linux/drivers/leds-pwm.c
 *
 * simple PWM based LED control
 *
 * Copyright 2009 Luotao Fu @ Pengutronix (l.fu@pengutronix.de)
 *
 * based on leds-gpio.c by Raphael Assenat <raph@8d.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/fb.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/workqueue.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct led_pwm_data {
	struct led_classdev	cdev;
	struct pwm_device	*pwm;
<<<<<<< HEAD
	struct work_struct	work;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int		active_low;
	unsigned int		period;
	int			duty;
	bool			can_sleep;
};

struct led_pwm_priv {
	int num_leds;
	struct led_pwm_data leds[0];
};

static void __led_pwm_set(struct led_pwm_data *led_dat)
{
	int new_duty = led_dat->duty;

	pwm_config(led_dat->pwm, new_duty, led_dat->period);

	if (new_duty == 0)
		pwm_disable(led_dat->pwm);
	else
		pwm_enable(led_dat->pwm);
}

<<<<<<< HEAD
static void led_pwm_work(struct work_struct *work)
{
	struct led_pwm_data *led_dat =
		container_of(work, struct led_pwm_data, work);

	__led_pwm_set(led_dat);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void led_pwm_set(struct led_classdev *led_cdev,
	enum led_brightness brightness)
{
	struct led_pwm_data *led_dat =
		container_of(led_cdev, struct led_pwm_data, cdev);
	unsigned int max = led_dat->cdev.max_brightness;
<<<<<<< HEAD
	unsigned int period =  led_dat->period;

	led_dat->duty = brightness * period / max;

	if (led_dat->can_sleep)
		schedule_work(&led_dat->work);
	else
		__led_pwm_set(led_dat);
=======
	unsigned long long duty =  led_dat->period;

	duty *= brightness;
	do_div(duty, max);

	if (led_dat->active_low)
		duty = led_dat->period - duty;

	led_dat->duty = duty;

	__led_pwm_set(led_dat);
}

static int led_pwm_set_blocking(struct led_classdev *led_cdev,
	enum led_brightness brightness)
{
	led_pwm_set(led_cdev, brightness);
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline size_t sizeof_pwm_leds_priv(int num_leds)
{
	return sizeof(struct led_pwm_priv) +
		      (sizeof(struct led_pwm_data) * num_leds);
}

static void led_pwm_cleanup(struct led_pwm_priv *priv)
{
<<<<<<< HEAD
	while (priv->num_leds--) {
		led_classdev_unregister(&priv->leds[priv->num_leds].cdev);
		if (priv->leds[priv->num_leds].can_sleep)
			cancel_work_sync(&priv->leds[priv->num_leds].work);
	}
}

static struct led_pwm_priv *led_pwm_create_of(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct device_node *child;
	struct led_pwm_priv *priv;
	int count, ret;

	/* count LEDs in this device, so we know how much to allocate */
	count = of_get_child_count(node);
	if (!count)
		return NULL;

	priv = devm_kzalloc(&pdev->dev, sizeof_pwm_leds_priv(count),
			    GFP_KERNEL);
	if (!priv)
		return NULL;

	for_each_child_of_node(node, child) {
		struct led_pwm_data *led_dat = &priv->leds[priv->num_leds];

		led_dat->cdev.name = of_get_property(child, "label",
						     NULL) ? : child->name;

		led_dat->pwm = devm_of_pwm_get(&pdev->dev, child, NULL);
		if (IS_ERR(led_dat->pwm)) {
			dev_err(&pdev->dev, "unable to request PWM for %s\n",
				led_dat->cdev.name);
			goto err;
		}
		/* Get the period from PWM core when n*/
		led_dat->period = pwm_get_period(led_dat->pwm);

		led_dat->cdev.default_trigger = of_get_property(child,
						"linux,default-trigger", NULL);
		of_property_read_u32(child, "max-brightness",
				     &led_dat->cdev.max_brightness);

		led_dat->cdev.brightness_set = led_pwm_set;
		led_dat->cdev.brightness = LED_OFF;
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

		led_dat->can_sleep = pwm_can_sleep(led_dat->pwm);
		if (led_dat->can_sleep)
			INIT_WORK(&led_dat->work, led_pwm_work);

		ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
		if (ret < 0) {
			dev_err(&pdev->dev, "failed to register for %s\n",
				led_dat->cdev.name);
			of_node_put(child);
			goto err;
		}
		priv->num_leds++;
	}

	return priv;
err:
	led_pwm_cleanup(priv);

	return NULL;
=======
	while (priv->num_leds--)
		led_classdev_unregister(&priv->leds[priv->num_leds].cdev);
}

static int led_pwm_add(struct device *dev, struct led_pwm_priv *priv,
		       struct led_pwm *led, struct device_node *child)
{
	struct led_pwm_data *led_data = &priv->leds[priv->num_leds];
	struct pwm_args pargs;
	int ret;

	led_data->active_low = led->active_low;
	led_data->cdev.name = led->name;
	led_data->cdev.default_trigger = led->default_trigger;
	led_data->cdev.brightness = LED_OFF;
	led_data->cdev.max_brightness = led->max_brightness;
	led_data->cdev.flags = LED_CORE_SUSPENDRESUME;

	if (child)
		led_data->pwm = devm_of_pwm_get(dev, child, NULL);
	else
		led_data->pwm = devm_pwm_get(dev, led->name);
	if (IS_ERR(led_data->pwm)) {
		ret = PTR_ERR(led_data->pwm);
		dev_err(dev, "unable to request PWM for %s: %d\n",
			led->name, ret);
		return ret;
	}

	led_data->can_sleep = pwm_can_sleep(led_data->pwm);
	if (!led_data->can_sleep)
		led_data->cdev.brightness_set = led_pwm_set;
	else
		led_data->cdev.brightness_set_blocking = led_pwm_set_blocking;

	/*
	 * FIXME: pwm_apply_args() should be removed when switching to the
	 * atomic PWM API.
	 */
	pwm_apply_args(led_data->pwm);

	pwm_get_args(led_data->pwm, &pargs);

	led_data->period = pargs.period;
	if (!led_data->period && (led->pwm_period_ns > 0))
		led_data->period = led->pwm_period_ns;

	ret = led_classdev_register(dev, &led_data->cdev);
	if (ret == 0) {
		priv->num_leds++;
		led_pwm_set(&led_data->cdev, led_data->cdev.brightness);
	} else {
		dev_err(dev, "failed to register PWM led for %s: %d\n",
			led->name, ret);
	}

	return ret;
}

static int led_pwm_create_of(struct device *dev, struct led_pwm_priv *priv)
{
	struct device_node *child;
	struct led_pwm led;
	int ret = 0;

	memset(&led, 0, sizeof(led));

	for_each_child_of_node(dev->of_node, child) {
		led.name = of_get_property(child, "label", NULL) ? :
			   child->name;

		led.default_trigger = of_get_property(child,
						"linux,default-trigger", NULL);
		led.active_low = of_property_read_bool(child, "active-low");
		of_property_read_u32(child, "max-brightness",
				     &led.max_brightness);

		ret = led_pwm_add(dev, priv, &led, child);
		if (ret) {
			of_node_put(child);
			break;
		}
	}

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int led_pwm_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
	struct led_pwm_priv *priv;
	int i, ret = 0;

	if (pdata && pdata->num_leds) {
		priv = devm_kzalloc(&pdev->dev,
				    sizeof_pwm_leds_priv(pdata->num_leds),
				    GFP_KERNEL);
		if (!priv)
			return -ENOMEM;

		for (i = 0; i < pdata->num_leds; i++) {
			struct led_pwm *cur_led = &pdata->leds[i];
			struct led_pwm_data *led_dat = &priv->leds[i];

			led_dat->pwm = devm_pwm_get(&pdev->dev, cur_led->name);
			if (IS_ERR(led_dat->pwm)) {
				ret = PTR_ERR(led_dat->pwm);
				dev_err(&pdev->dev,
					"unable to request PWM for %s\n",
					cur_led->name);
				goto err;
			}

			led_dat->cdev.name = cur_led->name;
			led_dat->cdev.default_trigger = cur_led->default_trigger;
			led_dat->active_low = cur_led->active_low;
			led_dat->period = cur_led->pwm_period_ns;
			led_dat->cdev.brightness_set = led_pwm_set;
			led_dat->cdev.brightness = LED_OFF;
			led_dat->cdev.max_brightness = cur_led->max_brightness;
			led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

			led_dat->can_sleep = pwm_can_sleep(led_dat->pwm);
			if (led_dat->can_sleep)
				INIT_WORK(&led_dat->work, led_pwm_work);

			ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
			if (ret < 0)
				goto err;
		}
		priv->num_leds = pdata->num_leds;
	} else {
		priv = led_pwm_create_of(pdev);
		if (!priv)
			return -ENODEV;
=======
	struct led_pwm_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct led_pwm_priv *priv;
	int count, i;
	int ret = 0;

	if (pdata)
		count = pdata->num_leds;
	else
		count = of_get_child_count(pdev->dev.of_node);

	if (!count)
		return -EINVAL;

	priv = devm_kzalloc(&pdev->dev, sizeof_pwm_leds_priv(count),
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	if (pdata) {
		for (i = 0; i < count; i++) {
			ret = led_pwm_add(&pdev->dev, priv, &pdata->leds[i],
					  NULL);
			if (ret)
				break;
		}
	} else {
		ret = led_pwm_create_of(&pdev->dev, priv);
	}

	if (ret) {
		led_pwm_cleanup(priv);
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	platform_set_drvdata(pdev, priv);

	return 0;
<<<<<<< HEAD

err:
	priv->num_leds = i;
	led_pwm_cleanup(priv);

	return ret;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int led_pwm_remove(struct platform_device *pdev)
{
	struct led_pwm_priv *priv = platform_get_drvdata(pdev);

	led_pwm_cleanup(priv);

	return 0;
}

static const struct of_device_id of_pwm_leds_match[] = {
	{ .compatible = "pwm-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_pwm_leds_match);

static struct platform_driver led_pwm_driver = {
	.probe		= led_pwm_probe,
	.remove		= led_pwm_remove,
	.driver		= {
		.name	= "leds_pwm",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(of_pwm_leds_match),
=======
		.of_match_table = of_pwm_leds_match,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};

module_platform_driver(led_pwm_driver);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
<<<<<<< HEAD
MODULE_DESCRIPTION("PWM LED driver for PXA");
MODULE_LICENSE("GPL");
=======
MODULE_DESCRIPTION("generic PWM LED driver");
MODULE_LICENSE("GPL v2");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_ALIAS("platform:leds-pwm");
