/*
 * Load firmware files from Analog Devices SigmaStudio
 *
 * Copyright 2009-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __SIGMA_FIRMWARE_H__
#define __SIGMA_FIRMWARE_H__

#include <linux/device.h>
#include <linux/regmap.h>
<<<<<<< HEAD

struct i2c_client;

extern int process_sigma_firmware(struct i2c_client *client, const char *name);
extern int process_sigma_firmware_regmap(struct device *dev,
		struct regmap *regmap, const char *name);
=======
#include <linux/list.h>

#include <sound/pcm.h>

struct sigmadsp;
struct snd_soc_component;
struct snd_pcm_substream;

struct sigmadsp_ops {
	int (*safeload)(struct sigmadsp *sigmadsp, unsigned int addr,
			const uint8_t *data, size_t len);
};

struct sigmadsp {
	const struct sigmadsp_ops *ops;

	struct list_head ctrl_list;
	struct list_head data_list;

	struct snd_pcm_hw_constraint_list rate_constraints;

	unsigned int current_samplerate;
	struct snd_soc_component *component;
	struct device *dev;

	struct mutex lock;

	void *control_data;
	int (*write)(void *, unsigned int, const uint8_t *, size_t);
	int (*read)(void *, unsigned int, uint8_t *, size_t);
};

struct sigmadsp *devm_sigmadsp_init(struct device *dev,
	const struct sigmadsp_ops *ops, const char *firmware_name);
void sigmadsp_reset(struct sigmadsp *sigmadsp);

int sigmadsp_restrict_params(struct sigmadsp *sigmadsp,
	struct snd_pcm_substream *substream);

struct i2c_client;

struct sigmadsp *devm_sigmadsp_init_regmap(struct device *dev,
	struct regmap *regmap, const struct sigmadsp_ops *ops,
	const char *firmware_name);
struct sigmadsp *devm_sigmadsp_init_i2c(struct i2c_client *client,
	const struct sigmadsp_ops *ops,	const char *firmware_name);

int sigmadsp_attach(struct sigmadsp *sigmadsp,
	struct snd_soc_component *component);
int sigmadsp_setup(struct sigmadsp *sigmadsp, unsigned int rate);
void sigmadsp_reset(struct sigmadsp *sigmadsp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
