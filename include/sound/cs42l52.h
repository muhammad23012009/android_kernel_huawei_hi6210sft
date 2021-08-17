/*
 * linux/sound/cs42l52.h -- Platform data for CS42L52
 *
 * Copyright (c) 2012 Cirrus Logic Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __CS42L52_H
#define __CS42L52_H

struct cs42l52_platform_data {

	/* MICBIAS Level. Check datasheet Pg48 */
	unsigned int micbias_lvl;

<<<<<<< HEAD
	/* MICA mode selection 0=Single 1=Differential */
	unsigned int mica_cfg;

	/* MICB mode selection 0=Single 1=Differential */
	unsigned int micb_cfg;

	/* MICA Select 0=MIC1A 1=MIC2A */
	unsigned int mica_sel;

	/* MICB Select 0=MIC2A 1=MIC2B */
	unsigned int micb_sel;
=======
	/* MICA mode selection Differential or Single-ended */
	bool mica_diff_cfg;

	/* MICB mode selection Differential or Single-ended */
	bool micb_diff_cfg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Charge Pump Freq. Check datasheet Pg73 */
	unsigned int chgfreq;

<<<<<<< HEAD
=======
	/* Reset GPIO */
	unsigned int reset_gpio;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* __CS42L52_H */
