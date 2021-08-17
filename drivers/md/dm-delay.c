/*
 * Copyright (C) 2005-2007 Red Hat GmbH
 *
 * A target that delays reads and/or writes and can send
 * them to different devices.
 *
 * This file is released under the GPL.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/slab.h>

#include <linux/device-mapper.h>

#define DM_MSG_PREFIX "delay"

struct delay_c {
	struct timer_list delay_timer;
	struct mutex timer_lock;
	struct workqueue_struct *kdelayd_wq;
	struct work_struct flush_expired_bios;
	struct list_head delayed_bios;
	atomic_t may_delay;
<<<<<<< HEAD
	mempool_t *delayed_pool;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	struct dm_dev *dev_read;
	sector_t start_read;
	unsigned read_delay;
	unsigned reads;

	struct dm_dev *dev_write;
	sector_t start_write;
	unsigned write_delay;
	unsigned writes;
};

struct dm_delay_info {
	struct delay_c *context;
	struct list_head list;
<<<<<<< HEAD
	struct bio *bio;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long expires;
};

static DEFINE_MUTEX(delayed_bios_lock);

<<<<<<< HEAD
static struct kmem_cache *delayed_cache;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void handle_delayed_timer(unsigned long data)
{
	struct delay_c *dc = (struct delay_c *)data;

	queue_work(dc->kdelayd_wq, &dc->flush_expired_bios);
}

static void queue_timeout(struct delay_c *dc, unsigned long expires)
{
	mutex_lock(&dc->timer_lock);

	if (!timer_pending(&dc->delay_timer) || expires < dc->delay_timer.expires)
		mod_timer(&dc->delay_timer, expires);

	mutex_unlock(&dc->timer_lock);
}

static void flush_bios(struct bio *bio)
{
	struct bio *n;

	while (bio) {
		n = bio->bi_next;
		bio->bi_next = NULL;
		generic_make_request(bio);
		bio = n;
	}
}

static struct bio *flush_delayed_bios(struct delay_c *dc, int flush_all)
{
	struct dm_delay_info *delayed, *next;
	unsigned long next_expires = 0;
	int start_timer = 0;
	struct bio_list flush_bios = { };

	mutex_lock(&delayed_bios_lock);
	list_for_each_entry_safe(delayed, next, &dc->delayed_bios, list) {
		if (flush_all || time_after_eq(jiffies, delayed->expires)) {
<<<<<<< HEAD
			list_del(&delayed->list);
			bio_list_add(&flush_bios, delayed->bio);
			if ((bio_data_dir(delayed->bio) == WRITE))
				delayed->context->writes--;
			else
				delayed->context->reads--;
			mempool_free(delayed, dc->delayed_pool);
=======
			struct bio *bio = dm_bio_from_per_bio_data(delayed,
						sizeof(struct dm_delay_info));
			list_del(&delayed->list);
			bio_list_add(&flush_bios, bio);
			if ((bio_data_dir(bio) == WRITE))
				delayed->context->writes--;
			else
				delayed->context->reads--;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		}

		if (!start_timer) {
			start_timer = 1;
			next_expires = delayed->expires;
		} else
			next_expires = min(next_expires, delayed->expires);
	}

	mutex_unlock(&delayed_bios_lock);

	if (start_timer)
		queue_timeout(dc, next_expires);

	return bio_list_get(&flush_bios);
}

static void flush_expired_bios(struct work_struct *work)
{
	struct delay_c *dc;

	dc = container_of(work, struct delay_c, flush_expired_bios);
	flush_bios(flush_delayed_bios(dc, 0));
}

/*
 * Mapping parameters:
 *    <device> <offset> <delay> [<write_device> <write_offset> <write_delay>]
 *
 * With separate write parameters, the first set is only used for reads.
<<<<<<< HEAD
=======
 * Offsets are specified in sectors.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Delays are specified in milliseconds.
 */
static int delay_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct delay_c *dc;
	unsigned long long tmpll;
	char dummy;
<<<<<<< HEAD

	if (argc != 3 && argc != 6) {
		ti->error = "requires exactly 3 or 6 arguments";
=======
	int ret;

	if (argc != 3 && argc != 6) {
		ti->error = "Requires exactly 3 or 6 arguments";
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;
	}

	dc = kmalloc(sizeof(*dc), GFP_KERNEL);
	if (!dc) {
		ti->error = "Cannot allocate context";
		return -ENOMEM;
	}

	dc->reads = dc->writes = 0;

<<<<<<< HEAD
=======
	ret = -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (sscanf(argv[1], "%llu%c", &tmpll, &dummy) != 1) {
		ti->error = "Invalid device sector";
		goto bad;
	}
	dc->start_read = tmpll;

	if (sscanf(argv[2], "%u%c", &dc->read_delay, &dummy) != 1) {
		ti->error = "Invalid delay";
		goto bad;
	}

<<<<<<< HEAD
	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table),
			  &dc->dev_read)) {
=======
	ret = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table),
			    &dc->dev_read);
	if (ret) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ti->error = "Device lookup failed";
		goto bad;
	}

<<<<<<< HEAD
=======
	ret = -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dc->dev_write = NULL;
	if (argc == 3)
		goto out;

	if (sscanf(argv[4], "%llu%c", &tmpll, &dummy) != 1) {
		ti->error = "Invalid write device sector";
		goto bad_dev_read;
	}
	dc->start_write = tmpll;

	if (sscanf(argv[5], "%u%c", &dc->write_delay, &dummy) != 1) {
		ti->error = "Invalid write delay";
		goto bad_dev_read;
	}

<<<<<<< HEAD
	if (dm_get_device(ti, argv[3], dm_table_get_mode(ti->table),
			  &dc->dev_write)) {
=======
	ret = dm_get_device(ti, argv[3], dm_table_get_mode(ti->table),
			    &dc->dev_write);
	if (ret) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ti->error = "Write device lookup failed";
		goto bad_dev_read;
	}

out:
<<<<<<< HEAD
	dc->delayed_pool = mempool_create_slab_pool(128, delayed_cache);
	if (!dc->delayed_pool) {
		DMERR("Couldn't create delayed bio pool.");
		goto bad_dev_write;
	}

=======
	ret = -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dc->kdelayd_wq = alloc_workqueue("kdelayd", WQ_MEM_RECLAIM, 0);
	if (!dc->kdelayd_wq) {
		DMERR("Couldn't start kdelayd");
		goto bad_queue;
	}

	setup_timer(&dc->delay_timer, handle_delayed_timer, (unsigned long)dc);

	INIT_WORK(&dc->flush_expired_bios, flush_expired_bios);
	INIT_LIST_HEAD(&dc->delayed_bios);
	mutex_init(&dc->timer_lock);
	atomic_set(&dc->may_delay, 1);

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
<<<<<<< HEAD
=======
	ti->per_io_data_size = sizeof(struct dm_delay_info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ti->private = dc;
	return 0;

bad_queue:
<<<<<<< HEAD
	mempool_destroy(dc->delayed_pool);
bad_dev_write:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (dc->dev_write)
		dm_put_device(ti, dc->dev_write);
bad_dev_read:
	dm_put_device(ti, dc->dev_read);
bad:
	kfree(dc);
<<<<<<< HEAD
	return -EINVAL;
=======
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void delay_dtr(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

<<<<<<< HEAD
	destroy_workqueue(dc->kdelayd_wq);
=======
	if (dc->kdelayd_wq)
		destroy_workqueue(dc->kdelayd_wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dm_put_device(ti, dc->dev_read);

	if (dc->dev_write)
		dm_put_device(ti, dc->dev_write);

<<<<<<< HEAD
	mempool_destroy(dc->delayed_pool);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(dc);
}

static int delay_bio(struct delay_c *dc, int delay, struct bio *bio)
{
	struct dm_delay_info *delayed;
	unsigned long expires = 0;

	if (!delay || !atomic_read(&dc->may_delay))
<<<<<<< HEAD
		return 1;

	delayed = mempool_alloc(dc->delayed_pool, GFP_NOIO);

	delayed->context = dc;
	delayed->bio = bio;
	delayed->expires = expires = jiffies + (delay * HZ / 1000);
=======
		return DM_MAPIO_REMAPPED;

	delayed = dm_per_bio_data(bio, sizeof(struct dm_delay_info));

	delayed->context = dc;
	delayed->expires = expires = jiffies + msecs_to_jiffies(delay);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&delayed_bios_lock);

	if (bio_data_dir(bio) == WRITE)
		dc->writes++;
	else
		dc->reads++;

	list_add_tail(&delayed->list, &dc->delayed_bios);

	mutex_unlock(&delayed_bios_lock);

	queue_timeout(dc, expires);

<<<<<<< HEAD
	return 0;
=======
	return DM_MAPIO_SUBMITTED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void delay_presuspend(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

	atomic_set(&dc->may_delay, 0);
	del_timer_sync(&dc->delay_timer);
	flush_bios(flush_delayed_bios(dc, 1));
}

static void delay_resume(struct dm_target *ti)
{
	struct delay_c *dc = ti->private;

	atomic_set(&dc->may_delay, 1);
}

static int delay_map(struct dm_target *ti, struct bio *bio)
{
	struct delay_c *dc = ti->private;

	if ((bio_data_dir(bio) == WRITE) && (dc->dev_write)) {
		bio->bi_bdev = dc->dev_write->bdev;
		if (bio_sectors(bio))
<<<<<<< HEAD
			bio->bi_sector = dc->start_write +
					 dm_target_offset(ti, bio->bi_sector);
=======
			bio->bi_iter.bi_sector = dc->start_write +
				dm_target_offset(ti, bio->bi_iter.bi_sector);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		return delay_bio(dc, dc->write_delay, bio);
	}

	bio->bi_bdev = dc->dev_read->bdev;
<<<<<<< HEAD
	bio->bi_sector = dc->start_read + dm_target_offset(ti, bio->bi_sector);
=======
	bio->bi_iter.bi_sector = dc->start_read +
		dm_target_offset(ti, bio->bi_iter.bi_sector);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return delay_bio(dc, dc->read_delay, bio);
}

static void delay_status(struct dm_target *ti, status_type_t type,
			 unsigned status_flags, char *result, unsigned maxlen)
{
	struct delay_c *dc = ti->private;
	int sz = 0;

	switch (type) {
	case STATUSTYPE_INFO:
		DMEMIT("%u %u", dc->reads, dc->writes);
		break;

	case STATUSTYPE_TABLE:
		DMEMIT("%s %llu %u", dc->dev_read->name,
		       (unsigned long long) dc->start_read,
		       dc->read_delay);
		if (dc->dev_write)
			DMEMIT(" %s %llu %u", dc->dev_write->name,
			       (unsigned long long) dc->start_write,
			       dc->write_delay);
		break;
	}
}

static int delay_iterate_devices(struct dm_target *ti,
				 iterate_devices_callout_fn fn, void *data)
{
	struct delay_c *dc = ti->private;
	int ret = 0;

	ret = fn(ti, dc->dev_read, dc->start_read, ti->len, data);
	if (ret)
		goto out;

	if (dc->dev_write)
		ret = fn(ti, dc->dev_write, dc->start_write, ti->len, data);

out:
	return ret;
}

static struct target_type delay_target = {
	.name	     = "delay",
	.version     = {1, 2, 1},
	.module      = THIS_MODULE,
	.ctr	     = delay_ctr,
	.dtr	     = delay_dtr,
	.map	     = delay_map,
	.presuspend  = delay_presuspend,
	.resume	     = delay_resume,
	.status	     = delay_status,
	.iterate_devices = delay_iterate_devices,
};

static int __init dm_delay_init(void)
{
<<<<<<< HEAD
	int r = -ENOMEM;

	delayed_cache = KMEM_CACHE(dm_delay_info, 0);
	if (!delayed_cache) {
		DMERR("Couldn't create delayed bio cache.");
		goto bad_memcache;
	}
=======
	int r;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	r = dm_register_target(&delay_target);
	if (r < 0) {
		DMERR("register failed %d", r);
		goto bad_register;
	}

	return 0;

bad_register:
<<<<<<< HEAD
	kmem_cache_destroy(delayed_cache);
bad_memcache:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return r;
}

static void __exit dm_delay_exit(void)
{
	dm_unregister_target(&delay_target);
<<<<<<< HEAD
	kmem_cache_destroy(delayed_cache);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Module hooks */
module_init(dm_delay_init);
module_exit(dm_delay_exit);

MODULE_DESCRIPTION(DM_NAME " delay target");
MODULE_AUTHOR("Heinz Mauelshagen <mauelshagen@redhat.com>");
MODULE_LICENSE("GPL");
