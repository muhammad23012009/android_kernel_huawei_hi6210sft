/*
 *  fair_share.c - A simple weight based Thermal governor
 *
 *  Copyright (C) 2012 Intel Corp
 *  Copyright (C) 2012 Durgadoss R <durgadoss.r@intel.com>
 *
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <linux/thermal.h>
<<<<<<< HEAD
=======
#include <trace/events/thermal.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "thermal_core.h"

/**
 * get_trip_level: - obtains the current trip level for a zone
 * @tz:		thermal zone device
 */
static int get_trip_level(struct thermal_zone_device *tz)
{
	int count = 0;
<<<<<<< HEAD
	unsigned long trip_temp;
=======
	int trip_temp;
	enum thermal_trip_type trip_type;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tz->trips == 0 || !tz->ops->get_trip_temp)
		return 0;

	for (count = 0; count < tz->trips; count++) {
		tz->ops->get_trip_temp(tz, count, &trip_temp);
		if (tz->temperature < trip_temp)
			break;
	}
<<<<<<< HEAD
=======

	/*
	 * count > 0 only if temperature is greater than first trip
	 * point, in which case, trip_point = count - 1
	 */
	if (count > 0) {
		tz->ops->get_trip_type(tz, count - 1, &trip_type);
		trace_thermal_zone_trip(tz, count - 1, trip_type);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return count;
}

static long get_target_state(struct thermal_zone_device *tz,
<<<<<<< HEAD
		struct thermal_cooling_device *cdev, int weight, int level)
=======
		struct thermal_cooling_device *cdev, int percentage, int level)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long max_state;

	cdev->ops->get_max_state(cdev, &max_state);

<<<<<<< HEAD
	return (long)(weight * level * max_state) / (100 * tz->trips);
}

/**
 * fair_share_throttle - throttles devices asscciated with the given zone
=======
	return (long)(percentage * level * max_state) / (100 * tz->trips);
}

/**
 * fair_share_throttle - throttles devices associated with the given zone
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @tz - thermal_zone_device
 *
 * Throttling Logic: This uses three parameters to calculate the new
 * throttle state of the cooling devices associated with the given zone.
 *
 * Parameters used for Throttling:
 * P1. max_state: Maximum throttle state exposed by the cooling device.
<<<<<<< HEAD
 * P2. weight[i]/100:
=======
 * P2. percentage[i]/100:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *	How 'effective' the 'i'th device is, in cooling the given zone.
 * P3. cur_trip_level/max_no_of_trips:
 *	This describes the extent to which the devices should be throttled.
 *	We do not want to throttle too much when we trip a lower temperature,
 *	whereas the throttling is at full swing if we trip critical levels.
 *	(Heavily assumes the trip points are in ascending order)
 * new_state of cooling device = P3 * P2 * P1
 */
static int fair_share_throttle(struct thermal_zone_device *tz, int trip)
{
<<<<<<< HEAD
	const struct thermal_zone_params *tzp;
	struct thermal_cooling_device *cdev;
	struct thermal_instance *instance;
	int i;
	int cur_trip_level = get_trip_level(tz);

	if (!tz->tzp || !tz->tzp->tbp)
		return -EINVAL;

	tzp = tz->tzp;

	for (i = 0; i < tzp->num_tbps; i++) {
		if (!tzp->tbp[i].cdev)
			continue;

		cdev = tzp->tbp[i].cdev;
		instance = get_thermal_instance(tz, cdev, trip);
		if (!instance)
			continue;

		instance->target = get_target_state(tz, cdev,
					tzp->tbp[i].weight, cur_trip_level);

		instance->cdev->updated = false;
		thermal_cdev_update(cdev);
	}
=======
	struct thermal_instance *instance;
	int total_weight = 0;
	int total_instance = 0;
	int cur_trip_level = get_trip_level(tz);

	mutex_lock(&tz->lock);

	list_for_each_entry(instance, &tz->thermal_instances, tz_node) {
		if (instance->trip != trip)
			continue;

		total_weight += instance->weight;
		total_instance++;
	}

	list_for_each_entry(instance, &tz->thermal_instances, tz_node) {
		int percentage;
		struct thermal_cooling_device *cdev = instance->cdev;

		if (instance->trip != trip)
			continue;

		if (!total_weight)
			percentage = 100 / total_instance;
		else
			percentage = (instance->weight * 100) / total_weight;

		instance->target = get_target_state(tz, cdev, percentage,
						    cur_trip_level);

		mutex_lock(&instance->cdev->lock);
		instance->cdev->updated = false;
		mutex_unlock(&instance->cdev->lock);
		thermal_cdev_update(cdev);
	}

	mutex_unlock(&tz->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static struct thermal_governor thermal_gov_fair_share = {
	.name		= "fair_share",
	.throttle	= fair_share_throttle,
};

int thermal_gov_fair_share_register(void)
{
	return thermal_register_governor(&thermal_gov_fair_share);
}

void thermal_gov_fair_share_unregister(void)
{
	thermal_unregister_governor(&thermal_gov_fair_share);
}

