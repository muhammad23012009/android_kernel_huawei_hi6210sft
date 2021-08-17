/*
	STB6100 Silicon Tuner
	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

	Copyright (C) ST Microelectronics

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

<<<<<<< HEAD
static int stb6100_get_frequency(struct dvb_frontend *fe, u32 *frequency)
{
	struct dvb_frontend_ops	*frontend_ops = NULL;
	struct dvb_tuner_ops	*tuner_ops = NULL;
	struct tuner_state	t_state;
	int err = 0;

	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->get_state) {
		if ((err = tuner_ops->get_state(fe, DVBFE_TUNER_FREQUENCY, &t_state)) < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
		*frequency = t_state.frequency;
=======
#include <linux/dvb/frontend.h>
#include "dvb_frontend.h"

static int stb6100_get_frequency(struct dvb_frontend *fe, u32 *frequency)
{
	struct dvb_frontend_ops	*frontend_ops = &fe->ops;
	struct dvb_tuner_ops	*tuner_ops = &frontend_ops->tuner_ops;
	int err = 0;

	if (tuner_ops->get_frequency) {
		err = tuner_ops->get_frequency(fe, frequency);
		if (err < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}

static int stb6100_set_frequency(struct dvb_frontend *fe, u32 frequency)
{
<<<<<<< HEAD
	struct dvb_frontend_ops	*frontend_ops = NULL;
	struct dvb_tuner_ops	*tuner_ops = NULL;
	struct tuner_state	t_state;
	int err = 0;

	t_state.frequency = frequency;
	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->set_state) {
		if ((err = tuner_ops->set_state(fe, DVBFE_TUNER_FREQUENCY, &t_state)) < 0) {
=======
	struct dvb_frontend_ops	*frontend_ops = &fe->ops;
	struct dvb_tuner_ops	*tuner_ops = &frontend_ops->tuner_ops;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	u32 bw = c->bandwidth_hz;
	int err = 0;

	c->frequency = frequency;
	c->bandwidth_hz = 0;		/* Don't adjust the bandwidth */

	if (tuner_ops->set_params) {
		err = tuner_ops->set_params(fe);
		c->bandwidth_hz = bw;
		if (err < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
	}
	return 0;
}

static int stb6100_get_bandwidth(struct dvb_frontend *fe, u32 *bandwidth)
{
	struct dvb_frontend_ops	*frontend_ops = &fe->ops;
	struct dvb_tuner_ops	*tuner_ops = &frontend_ops->tuner_ops;
<<<<<<< HEAD
	struct tuner_state	t_state;
	int err = 0;

	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->get_state) {
		if ((err = tuner_ops->get_state(fe, DVBFE_TUNER_BANDWIDTH, &t_state)) < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
		*bandwidth = t_state.bandwidth;
=======
	int err = 0;

	if (tuner_ops->get_bandwidth) {
		err = tuner_ops->get_bandwidth(fe, bandwidth);
		if (err < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}

static int stb6100_set_bandwidth(struct dvb_frontend *fe, u32 bandwidth)
{
<<<<<<< HEAD
	struct dvb_frontend_ops	*frontend_ops = NULL;
	struct dvb_tuner_ops	*tuner_ops = NULL;
	struct tuner_state	t_state;
	int err = 0;

	t_state.bandwidth = bandwidth;
	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->set_state) {
		if ((err = tuner_ops->set_state(fe, DVBFE_TUNER_BANDWIDTH, &t_state)) < 0) {
=======
	struct dvb_frontend_ops	*frontend_ops = &fe->ops;
	struct dvb_tuner_ops	*tuner_ops = &frontend_ops->tuner_ops;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	u32 freq = c->frequency;
	int err = 0;

	c->bandwidth_hz = bandwidth;
	c->frequency = 0;		/* Don't adjust the frequency */

	if (tuner_ops->set_params) {
		err = tuner_ops->set_params(fe);
		c->frequency = freq;
		if (err < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
	}
	return 0;
}
