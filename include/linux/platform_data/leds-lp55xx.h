/*
 * LP55XX Platform Data Header
 *
 * Copyright (C) 2012 Texas Instruments
 *
 * Author: Milo(Woogyom) Kim <milo.kim@ti.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Derived from leds-lp5521.h, leds-lp5523.h
 */

#ifndef _LEDS_LP55XX_H
#define _LEDS_LP55XX_H

/* Clock configuration */
#define LP55XX_CLOCK_AUTO	0
#define LP55XX_CLOCK_INT	1
#define LP55XX_CLOCK_EXT	2

struct lp55xx_led_config {
	const char *name;
<<<<<<< HEAD
=======
	const char *default_trigger;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u8 chan_nr;
	u8 led_current; /* mA x10, 0 if led is not connected */
	u8 max_current;
};

struct lp55xx_predef_pattern {
	const u8 *r;
	const u8 *g;
	const u8 *b;
	u8 size_r;
	u8 size_g;
	u8 size_b;
};

<<<<<<< HEAD
=======
enum lp8501_pwr_sel {
	LP8501_ALL_VDD,		/* D1~9 are connected to VDD */
	LP8501_6VDD_3VOUT,	/* D1~6 with VDD, D7~9 with VOUT */
	LP8501_3VDD_6VOUT,	/* D1~6 with VOUT, D7~9 with VDD */
	LP8501_ALL_VOUT,	/* D1~9 are connected to VOUT */
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * struct lp55xx_platform_data
 * @led_config        : Configurable led class device
 * @num_channels      : Number of LED channels
 * @label             : Used for naming LEDs
 * @clock_mode        : Input clock mode. LP55XX_CLOCK_AUTO or _INT or _EXT
 * @setup_resources   : Platform specific function before enabling the chip
 * @release_resources : Platform specific function after  disabling the chip
 * @enable            : EN pin control by platform side
 * @patterns          : Predefined pattern data for RGB channels
 * @num_patterns      : Number of patterns
 * @update_config     : Value of CONFIG register
 */
struct lp55xx_platform_data {

	/* LED channel configuration */
	struct lp55xx_led_config *led_config;
	u8 num_channels;
	const char *label;

	/* Clock configuration */
	u8 clock_mode;

<<<<<<< HEAD
	/* Platform specific functions */
	int (*setup_resources)(void);
	void (*release_resources)(void);
	void (*enable)(bool state);
=======
	/* optional enable GPIO */
	int enable_gpio;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Predefined pattern data */
	struct lp55xx_predef_pattern *patterns;
	unsigned int num_patterns;
<<<<<<< HEAD
=======

	/* LP8501 specific */
	enum lp8501_pwr_sel pwr_sel;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* _LEDS_LP55XX_H */
