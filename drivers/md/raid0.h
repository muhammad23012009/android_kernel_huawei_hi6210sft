#ifndef _RAID0_H
#define _RAID0_H

struct strip_zone {
	sector_t zone_end;	/* Start of the next zone (in sectors) */
	sector_t dev_start;	/* Zone offset in real dev (in sectors) */
	int	 nb_dev;	/* # of devices attached to the zone */
};

struct r0conf {
	struct strip_zone	*strip_zone;
	struct md_rdev		**devlist; /* lists of rdevs, pointed to
					    * by strip_zone->dev */
	int			nr_strip_zones;
<<<<<<< HEAD
	int			has_merge_bvec;	/* at least one member has
						 * a merge_bvec_fn */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif
