/*
 * V4L2 OF binding parsing library
 *
 * Copyright (C) 2012 - 2013 Samsung Electronics Co., Ltd.
 * Author: Sylwester Nawrocki <s.nawrocki@samsung.com>
 *
 * Copyright (C) 2012 Renesas Electronics Corp.
 * Author: Guennadi Liakhovetski <g.liakhovetski@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 */
#ifndef _V4L2_OF_H
#define _V4L2_OF_H

#include <linux/list.h>
#include <linux/types.h>
#include <linux/errno.h>
<<<<<<< HEAD
=======
#include <linux/of_graph.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <media/v4l2-mediabus.h>

struct device_node;

/**
 * struct v4l2_of_bus_mipi_csi2 - MIPI CSI-2 bus data structure
 * @flags: media bus (V4L2_MBUS_*) flags
 * @data_lanes: an array of physical data lane indexes
 * @clock_lane: physical lane index of the clock lane
 * @num_data_lanes: number of data lanes
<<<<<<< HEAD
=======
 * @lane_polarities: polarity of the lanes. The order is the same of
 *		   the physical lanes.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct v4l2_of_bus_mipi_csi2 {
	unsigned int flags;
	unsigned char data_lanes[4];
	unsigned char clock_lane;
	unsigned short num_data_lanes;
<<<<<<< HEAD
=======
	bool lane_polarities[5];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/**
 * struct v4l2_of_bus_parallel - parallel data bus data structure
 * @flags: media bus (V4L2_MBUS_*) flags
 * @bus_width: bus width in bits
 * @data_shift: data shift in bits
 */
struct v4l2_of_bus_parallel {
	unsigned int flags;
	unsigned char bus_width;
	unsigned char data_shift;
};

/**
 * struct v4l2_of_endpoint - the endpoint data structure
<<<<<<< HEAD
 * @port: identifier (value of reg property) of a port this endpoint belongs to
 * @id: identifier (value of reg property) of this endpoint
 * @local_node: pointer to device_node of this endpoint
 * @remote: phandle to remote endpoint node
 * @bus_type: bus type
 * @bus: bus configuration data structure
 * @head: list head for this structure
 */
struct v4l2_of_endpoint {
	unsigned int port;
	unsigned int id;
	const struct device_node *local_node;
	const __be32 *remote;
=======
 * @base: struct of_endpoint containing port, id, and local of_node
 * @bus_type: bus type
 * @bus: bus configuration data structure
 * @link_frequencies: array of supported link frequencies
 * @nr_of_link_frequencies: number of elements in link_frequenccies array
 */
struct v4l2_of_endpoint {
	struct of_endpoint base;
	/* Fields below this line will be zeroed by v4l2_of_parse_endpoint() */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	enum v4l2_mbus_type bus_type;
	union {
		struct v4l2_of_bus_parallel parallel;
		struct v4l2_of_bus_mipi_csi2 mipi_csi2;
	} bus;
<<<<<<< HEAD
	struct list_head head;
};

#ifdef CONFIG_OF
void v4l2_of_parse_endpoint(const struct device_node *node,
				struct v4l2_of_endpoint *link);
struct device_node *v4l2_of_get_next_endpoint(const struct device_node *parent,
					struct device_node *previous);
struct device_node *v4l2_of_get_remote_port_parent(
					const struct device_node *node);
struct device_node *v4l2_of_get_remote_port(const struct device_node *node);
=======
	u64 *link_frequencies;
	unsigned int nr_of_link_frequencies;
};

/**
 * struct v4l2_of_link - a link between two endpoints
 * @local_node: pointer to device_node of this endpoint
 * @local_port: identifier of the port this endpoint belongs to
 * @remote_node: pointer to device_node of the remote endpoint
 * @remote_port: identifier of the port the remote endpoint belongs to
 */
struct v4l2_of_link {
	struct device_node *local_node;
	unsigned int local_port;
	struct device_node *remote_node;
	unsigned int remote_port;
};

#ifdef CONFIG_OF
int v4l2_of_parse_endpoint(const struct device_node *node,
			   struct v4l2_of_endpoint *endpoint);
struct v4l2_of_endpoint *v4l2_of_alloc_parse_endpoint(
	const struct device_node *node);
void v4l2_of_free_endpoint(struct v4l2_of_endpoint *endpoint);
int v4l2_of_parse_link(const struct device_node *node,
		       struct v4l2_of_link *link);
void v4l2_of_put_link(struct v4l2_of_link *link);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else /* CONFIG_OF */

static inline int v4l2_of_parse_endpoint(const struct device_node *node,
					struct v4l2_of_endpoint *link)
{
	return -ENOSYS;
}

<<<<<<< HEAD
static inline struct device_node *v4l2_of_get_next_endpoint(
					const struct device_node *parent,
					struct device_node *previous)
=======
static inline struct v4l2_of_endpoint *v4l2_of_alloc_parse_endpoint(
	const struct device_node *node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return NULL;
}

<<<<<<< HEAD
static inline struct device_node *v4l2_of_get_remote_port_parent(
					const struct device_node *node)
{
	return NULL;
}

static inline struct device_node *v4l2_of_get_remote_port(
					const struct device_node *node)
{
	return NULL;
=======
static inline void v4l2_of_free_endpoint(struct v4l2_of_endpoint *endpoint)
{
}

static inline int v4l2_of_parse_link(const struct device_node *node,
				     struct v4l2_of_link *link)
{
	return -ENOSYS;
}

static inline void v4l2_of_put_link(struct v4l2_of_link *link)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* CONFIG_OF */

#endif /* _V4L2_OF_H */
