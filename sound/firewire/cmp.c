/*
 * Connection Management Procedures (IEC 61883-1) helper functions
 *
 * Copyright (c) Clemens Ladisch <clemens@ladisch.de>
 * Licensed under the terms of the GNU General Public License, version 2.
 */

#include <linux/device.h>
#include <linux/firewire.h>
#include <linux/firewire-constants.h>
#include <linux/module.h>
#include <linux/sched.h>
#include "lib.h"
#include "iso-resources.h"
#include "cmp.h"

<<<<<<< HEAD
#define IMPR_SPEED_MASK		0xc0000000
#define IMPR_SPEED_SHIFT	30
#define IMPR_XSPEED_MASK	0x00000060
#define IMPR_XSPEED_SHIFT	5
#define IMPR_PLUGS_MASK		0x0000001f

#define IPCR_ONLINE		0x80000000
#define IPCR_BCAST_CONN		0x40000000
#define IPCR_P2P_CONN_MASK	0x3f000000
#define IPCR_P2P_CONN_SHIFT	24
#define IPCR_CHANNEL_MASK	0x003f0000
#define IPCR_CHANNEL_SHIFT	16
=======
/* MPR common fields */
#define MPR_SPEED_MASK		0xc0000000
#define MPR_SPEED_SHIFT		30
#define MPR_XSPEED_MASK		0x00000060
#define MPR_XSPEED_SHIFT	5
#define MPR_PLUGS_MASK		0x0000001f

/* PCR common fields */
#define PCR_ONLINE		0x80000000
#define PCR_BCAST_CONN		0x40000000
#define PCR_P2P_CONN_MASK	0x3f000000
#define PCR_P2P_CONN_SHIFT	24
#define PCR_CHANNEL_MASK	0x003f0000
#define PCR_CHANNEL_SHIFT	16

/* oPCR specific fields */
#define OPCR_XSPEED_MASK	0x00C00000
#define OPCR_XSPEED_SHIFT	22
#define OPCR_SPEED_MASK		0x0000C000
#define OPCR_SPEED_SHIFT	14
#define OPCR_OVERHEAD_ID_MASK	0x00003C00
#define OPCR_OVERHEAD_ID_SHIFT	10
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

enum bus_reset_handling {
	ABORT_ON_BUS_RESET,
	SUCCEED_ON_BUS_RESET,
};

static __printf(2, 3)
void cmp_error(struct cmp_connection *c, const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	dev_err(&c->resources.unit->device, "%cPCR%u: %pV",
<<<<<<< HEAD
		'i', c->pcr_index, &(struct va_format){ fmt, &va });
	va_end(va);
}

=======
		(c->direction == CMP_INPUT) ? 'i' : 'o',
		c->pcr_index, &(struct va_format){ fmt, &va });
	va_end(va);
}

static u64 mpr_address(struct cmp_connection *c)
{
	if (c->direction == CMP_INPUT)
		return CSR_REGISTER_BASE + CSR_IMPR;
	else
		return CSR_REGISTER_BASE + CSR_OMPR;
}

static u64 pcr_address(struct cmp_connection *c)
{
	if (c->direction == CMP_INPUT)
		return CSR_REGISTER_BASE + CSR_IPCR(c->pcr_index);
	else
		return CSR_REGISTER_BASE + CSR_OPCR(c->pcr_index);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int pcr_modify(struct cmp_connection *c,
		      __be32 (*modify)(struct cmp_connection *c, __be32 old),
		      int (*check)(struct cmp_connection *c, __be32 pcr),
		      enum bus_reset_handling bus_reset_handling)
{
<<<<<<< HEAD
	struct fw_device *device = fw_parent_device(c->resources.unit);
	int generation = c->resources.generation;
	int rcode, errors = 0;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__be32 old_arg, buffer[2];
	int err;

	buffer[0] = c->last_pcr_value;
	for (;;) {
		old_arg = buffer[0];
		buffer[1] = modify(c, buffer[0]);

<<<<<<< HEAD
		rcode = fw_run_transaction(
				device->card, TCODE_LOCK_COMPARE_SWAP,
				device->node_id, generation, device->max_speed,
				CSR_REGISTER_BASE + CSR_IPCR(c->pcr_index),
				buffer, 8);

		if (rcode == RCODE_COMPLETE) {
			if (buffer[0] == old_arg) /* success? */
				break;

			if (check) {
				err = check(c, buffer[0]);
				if (err < 0)
					return err;
			}
		} else if (rcode == RCODE_GENERATION)
			goto bus_reset;
		else if (rcode_is_permanent_error(rcode) || ++errors >= 3)
			goto io_error;
=======
		err = snd_fw_transaction(
				c->resources.unit, TCODE_LOCK_COMPARE_SWAP,
				pcr_address(c), buffer, 8,
				FW_FIXED_GENERATION | c->resources.generation);

		if (err < 0) {
			if (err == -EAGAIN &&
			    bus_reset_handling == SUCCEED_ON_BUS_RESET)
				err = 0;
			return err;
		}

		if (buffer[0] == old_arg) /* success? */
			break;

		if (check) {
			err = check(c, buffer[0]);
			if (err < 0)
				return err;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	c->last_pcr_value = buffer[1];

	return 0;
<<<<<<< HEAD

io_error:
	cmp_error(c, "transaction failed: %s\n", fw_rcode_string(rcode));
	return -EIO;

bus_reset:
	return bus_reset_handling == ABORT_ON_BUS_RESET ? -EAGAIN : 0;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}


/**
 * cmp_connection_init - initializes a connection manager
 * @c: the connection manager to initialize
 * @unit: a unit of the target device
<<<<<<< HEAD
 * @ipcr_index: the index of the iPCR on the target device
 */
int cmp_connection_init(struct cmp_connection *c,
			struct fw_unit *unit,
			unsigned int ipcr_index)
{
	__be32 impr_be;
	u32 impr;
	int err;

	err = snd_fw_transaction(unit, TCODE_READ_QUADLET_REQUEST,
				 CSR_REGISTER_BASE + CSR_IMPR,
				 &impr_be, 4);
	if (err < 0)
		return err;
	impr = be32_to_cpu(impr_be);

	if (ipcr_index >= (impr & IMPR_PLUGS_MASK))
=======
 * @direction: input or output
 * @pcr_index: the index of the iPCR/oPCR on the target device
 */
int cmp_connection_init(struct cmp_connection *c,
			struct fw_unit *unit,
			enum cmp_direction direction,
			unsigned int pcr_index)
{
	__be32 mpr_be;
	u32 mpr;
	int err;

	c->direction = direction;
	err = snd_fw_transaction(unit, TCODE_READ_QUADLET_REQUEST,
				 mpr_address(c), &mpr_be, 4, 0);
	if (err < 0)
		return err;
	mpr = be32_to_cpu(mpr_be);

	if (pcr_index >= (mpr & MPR_PLUGS_MASK))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	err = fw_iso_resources_init(&c->resources, unit);
	if (err < 0)
		return err;

	c->connected = false;
	mutex_init(&c->mutex);
	c->last_pcr_value = cpu_to_be32(0x80000000);
<<<<<<< HEAD
	c->pcr_index = ipcr_index;
	c->max_speed = (impr & IMPR_SPEED_MASK) >> IMPR_SPEED_SHIFT;
	if (c->max_speed == SCODE_BETA)
		c->max_speed += (impr & IMPR_XSPEED_MASK) >> IMPR_XSPEED_SHIFT;
=======
	c->pcr_index = pcr_index;
	c->max_speed = (mpr & MPR_SPEED_MASK) >> MPR_SPEED_SHIFT;
	if (c->max_speed == SCODE_BETA)
		c->max_speed += (mpr & MPR_XSPEED_MASK) >> MPR_XSPEED_SHIFT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
EXPORT_SYMBOL(cmp_connection_init);

/**
<<<<<<< HEAD
=======
 * cmp_connection_check_used - check connection is already esablished or not
 * @c: the connection manager to be checked
 * @used: the pointer to store the result of checking the connection
 */
int cmp_connection_check_used(struct cmp_connection *c, bool *used)
{
	__be32 pcr;
	int err;

	err = snd_fw_transaction(
			c->resources.unit, TCODE_READ_QUADLET_REQUEST,
			pcr_address(c), &pcr, 4, 0);
	if (err >= 0)
		*used = !!(pcr & cpu_to_be32(PCR_BCAST_CONN |
					     PCR_P2P_CONN_MASK));

	return err;
}
EXPORT_SYMBOL(cmp_connection_check_used);

/**
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * cmp_connection_destroy - free connection manager resources
 * @c: the connection manager
 */
void cmp_connection_destroy(struct cmp_connection *c)
{
	WARN_ON(c->connected);
	mutex_destroy(&c->mutex);
	fw_iso_resources_destroy(&c->resources);
}
EXPORT_SYMBOL(cmp_connection_destroy);


static __be32 ipcr_set_modify(struct cmp_connection *c, __be32 ipcr)
{
<<<<<<< HEAD
	ipcr &= ~cpu_to_be32(IPCR_BCAST_CONN |
			     IPCR_P2P_CONN_MASK |
			     IPCR_CHANNEL_MASK);
	ipcr |= cpu_to_be32(1 << IPCR_P2P_CONN_SHIFT);
	ipcr |= cpu_to_be32(c->resources.channel << IPCR_CHANNEL_SHIFT);
=======
	ipcr &= ~cpu_to_be32(PCR_BCAST_CONN |
			     PCR_P2P_CONN_MASK |
			     PCR_CHANNEL_MASK);
	ipcr |= cpu_to_be32(1 << PCR_P2P_CONN_SHIFT);
	ipcr |= cpu_to_be32(c->resources.channel << PCR_CHANNEL_SHIFT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ipcr;
}

<<<<<<< HEAD
static int ipcr_set_check(struct cmp_connection *c, __be32 ipcr)
{
	if (ipcr & cpu_to_be32(IPCR_BCAST_CONN |
			       IPCR_P2P_CONN_MASK)) {
		cmp_error(c, "plug is already in use\n");
		return -EBUSY;
	}
	if (!(ipcr & cpu_to_be32(IPCR_ONLINE))) {
=======
static int get_overhead_id(struct cmp_connection *c)
{
	int id;

	/*
	 * apply "oPCR overhead ID encoding"
	 * the encoding table can convert up to 512.
	 * here the value over 512 is converted as the same way as 512.
	 */
	for (id = 1; id < 16; id++) {
		if (c->resources.bandwidth_overhead < (id << 5))
			break;
	}
	if (id == 16)
		id = 0;

	return id;
}

static __be32 opcr_set_modify(struct cmp_connection *c, __be32 opcr)
{
	unsigned int spd, xspd;

	/* generate speed and extended speed field value */
	if (c->speed > SCODE_400) {
		spd  = SCODE_800;
		xspd = c->speed - SCODE_800;
	} else {
		spd = c->speed;
		xspd = 0;
	}

	opcr &= ~cpu_to_be32(PCR_BCAST_CONN |
			     PCR_P2P_CONN_MASK |
			     OPCR_XSPEED_MASK |
			     PCR_CHANNEL_MASK |
			     OPCR_SPEED_MASK |
			     OPCR_OVERHEAD_ID_MASK);
	opcr |= cpu_to_be32(1 << PCR_P2P_CONN_SHIFT);
	opcr |= cpu_to_be32(xspd << OPCR_XSPEED_SHIFT);
	opcr |= cpu_to_be32(c->resources.channel << PCR_CHANNEL_SHIFT);
	opcr |= cpu_to_be32(spd << OPCR_SPEED_SHIFT);
	opcr |= cpu_to_be32(get_overhead_id(c) << OPCR_OVERHEAD_ID_SHIFT);

	return opcr;
}

static int pcr_set_check(struct cmp_connection *c, __be32 pcr)
{
	if (pcr & cpu_to_be32(PCR_BCAST_CONN |
			      PCR_P2P_CONN_MASK)) {
		cmp_error(c, "plug is already in use\n");
		return -EBUSY;
	}
	if (!(pcr & cpu_to_be32(PCR_ONLINE))) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cmp_error(c, "plug is not on-line\n");
		return -ECONNREFUSED;
	}

	return 0;
}

/**
 * cmp_connection_establish - establish a connection to the target
 * @c: the connection manager
 * @max_payload_bytes: the amount of data (including CIP headers) per packet
 *
 * This function establishes a point-to-point connection from the local
 * computer to the target by allocating isochronous resources (channel and
<<<<<<< HEAD
 * bandwidth) and setting the target's input plug control register.  When this
 * function succeeds, the caller is responsible for starting transmitting
 * packets.
=======
 * bandwidth) and setting the target's input/output plug control register.
 * When this function succeeds, the caller is responsible for starting
 * transmitting packets.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
int cmp_connection_establish(struct cmp_connection *c,
			     unsigned int max_payload_bytes)
{
	int err;

	if (WARN_ON(c->connected))
		return -EISCONN;

	c->speed = min(c->max_speed,
		       fw_parent_device(c->resources.unit)->max_speed);

	mutex_lock(&c->mutex);

retry_after_bus_reset:
	err = fw_iso_resources_allocate(&c->resources,
					max_payload_bytes, c->speed);
	if (err < 0)
		goto err_mutex;

<<<<<<< HEAD
	err = pcr_modify(c, ipcr_set_modify, ipcr_set_check,
			 ABORT_ON_BUS_RESET);
=======
	if (c->direction == CMP_OUTPUT)
		err = pcr_modify(c, opcr_set_modify, pcr_set_check,
				 ABORT_ON_BUS_RESET);
	else
		err = pcr_modify(c, ipcr_set_modify, pcr_set_check,
				 ABORT_ON_BUS_RESET);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err == -EAGAIN) {
		fw_iso_resources_free(&c->resources);
		goto retry_after_bus_reset;
	}
	if (err < 0)
		goto err_resources;

	c->connected = true;

	mutex_unlock(&c->mutex);

	return 0;

err_resources:
	fw_iso_resources_free(&c->resources);
err_mutex:
	mutex_unlock(&c->mutex);

	return err;
}
EXPORT_SYMBOL(cmp_connection_establish);

/**
 * cmp_connection_update - update the connection after a bus reset
 * @c: the connection manager
 *
<<<<<<< HEAD
 * This function must be called from the driver's .update handler to reestablish
 * any connection that might have been active.
=======
 * This function must be called from the driver's .update handler to
 * reestablish any connection that might have been active.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Returns zero on success, or a negative error code.  On an error, the
 * connection is broken and the caller must stop transmitting iso packets.
 */
int cmp_connection_update(struct cmp_connection *c)
{
	int err;

	mutex_lock(&c->mutex);

	if (!c->connected) {
		mutex_unlock(&c->mutex);
		return 0;
	}

	err = fw_iso_resources_update(&c->resources);
	if (err < 0)
		goto err_unconnect;

<<<<<<< HEAD
	err = pcr_modify(c, ipcr_set_modify, ipcr_set_check,
			 SUCCEED_ON_BUS_RESET);
=======
	if (c->direction == CMP_OUTPUT)
		err = pcr_modify(c, opcr_set_modify, pcr_set_check,
				 SUCCEED_ON_BUS_RESET);
	else
		err = pcr_modify(c, ipcr_set_modify, pcr_set_check,
				 SUCCEED_ON_BUS_RESET);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err < 0)
		goto err_resources;

	mutex_unlock(&c->mutex);

	return 0;

err_resources:
	fw_iso_resources_free(&c->resources);
err_unconnect:
	c->connected = false;
	mutex_unlock(&c->mutex);

	return err;
}
EXPORT_SYMBOL(cmp_connection_update);

<<<<<<< HEAD

static __be32 ipcr_break_modify(struct cmp_connection *c, __be32 ipcr)
{
	return ipcr & ~cpu_to_be32(IPCR_BCAST_CONN | IPCR_P2P_CONN_MASK);
=======
static __be32 pcr_break_modify(struct cmp_connection *c, __be32 pcr)
{
	return pcr & ~cpu_to_be32(PCR_BCAST_CONN | PCR_P2P_CONN_MASK);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * cmp_connection_break - break the connection to the target
 * @c: the connection manager
 *
<<<<<<< HEAD
 * This function deactives the connection in the target's input plug control
 * register, and frees the isochronous resources of the connection.  Before
 * calling this function, the caller should cease transmitting packets.
=======
 * This function deactives the connection in the target's input/output plug
 * control register, and frees the isochronous resources of the connection.
 * Before calling this function, the caller should cease transmitting packets.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
void cmp_connection_break(struct cmp_connection *c)
{
	int err;

	mutex_lock(&c->mutex);

	if (!c->connected) {
		mutex_unlock(&c->mutex);
		return;
	}

<<<<<<< HEAD
	err = pcr_modify(c, ipcr_break_modify, NULL, SUCCEED_ON_BUS_RESET);
=======
	err = pcr_modify(c, pcr_break_modify, NULL, SUCCEED_ON_BUS_RESET);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err < 0)
		cmp_error(c, "plug is still connected\n");

	fw_iso_resources_free(&c->resources);

	c->connected = false;

	mutex_unlock(&c->mutex);
}
EXPORT_SYMBOL(cmp_connection_break);
