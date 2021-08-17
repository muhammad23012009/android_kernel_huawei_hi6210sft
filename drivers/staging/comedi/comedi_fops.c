/*
<<<<<<< HEAD
    comedi/comedi_fops.c
    comedi kernel module

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>

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

#undef DEBUG
=======
 * comedi/comedi_fops.c
 * comedi kernel module
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "comedi_compat32.h"

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/ioport.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include "comedidev.h"
#include <linux/cdev.h>
#include <linux/stat.h>

#include <linux/io.h>
#include <linux/uaccess.h>

#include "comedi_internal.h"

<<<<<<< HEAD
=======
/*
 * comedi_subdevice "runflags"
 * COMEDI_SRF_RT:		DEPRECATED: command is running real-time
 * COMEDI_SRF_ERROR:		indicates an COMEDI_CB_ERROR event has occurred
 *				since the last command was started
 * COMEDI_SRF_RUNNING:		command is running
 * COMEDI_SRF_FREE_SPRIV:	free s->private on detach
 *
 * COMEDI_SRF_BUSY_MASK:	runflags that indicate the subdevice is "busy"
 */
#define COMEDI_SRF_RT		BIT(1)
#define COMEDI_SRF_ERROR	BIT(2)
#define COMEDI_SRF_RUNNING	BIT(27)
#define COMEDI_SRF_FREE_SPRIV	BIT(31)

#define COMEDI_SRF_BUSY_MASK	(COMEDI_SRF_ERROR | COMEDI_SRF_RUNNING)

/**
 * struct comedi_file - Per-file private data for COMEDI device
 * @dev: COMEDI device.
 * @read_subdev: Current "read" subdevice.
 * @write_subdev: Current "write" subdevice.
 * @last_detach_count: Last known detach count.
 * @last_attached: Last known attached/detached state.
 */
struct comedi_file {
	struct comedi_device *dev;
	struct comedi_subdevice *read_subdev;
	struct comedi_subdevice *write_subdev;
	unsigned int last_detach_count;
	bool last_attached:1;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define COMEDI_NUM_MINORS 0x100
#define COMEDI_NUM_SUBDEVICE_MINORS	\
	(COMEDI_NUM_MINORS - COMEDI_NUM_BOARD_MINORS)

<<<<<<< HEAD
#ifdef CONFIG_COMEDI_DEBUG
int comedi_debug;
EXPORT_SYMBOL_GPL(comedi_debug);
module_param(comedi_debug, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(comedi_debug,
		 "enable comedi core and driver debugging if non-zero (default 0)"
		);
#endif

static int comedi_num_legacy_minors;
module_param(comedi_num_legacy_minors, int, S_IRUGO);
=======
static int comedi_num_legacy_minors;
module_param(comedi_num_legacy_minors, int, 0444);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(comedi_num_legacy_minors,
		 "number of comedi minor devices to reserve for non-auto-configured devices (default 0)"
		);

unsigned int comedi_default_buf_size_kb = CONFIG_COMEDI_DEFAULT_BUF_SIZE_KB;
<<<<<<< HEAD
module_param(comedi_default_buf_size_kb, uint, S_IRUGO | S_IWUSR);
=======
module_param(comedi_default_buf_size_kb, uint, 0644);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(comedi_default_buf_size_kb,
		 "default asynchronous buffer size in KiB (default "
		 __MODULE_STRING(CONFIG_COMEDI_DEFAULT_BUF_SIZE_KB) ")");

unsigned int comedi_default_buf_maxsize_kb
	= CONFIG_COMEDI_DEFAULT_BUF_MAXSIZE_KB;
<<<<<<< HEAD
module_param(comedi_default_buf_maxsize_kb, uint, S_IRUGO | S_IWUSR);
=======
module_param(comedi_default_buf_maxsize_kb, uint, 0644);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(comedi_default_buf_maxsize_kb,
		 "default maximum size of asynchronous buffer in KiB (default "
		 __MODULE_STRING(CONFIG_COMEDI_DEFAULT_BUF_MAXSIZE_KB) ")");

static DEFINE_MUTEX(comedi_board_minor_table_lock);
static struct comedi_device
*comedi_board_minor_table[COMEDI_NUM_BOARD_MINORS];

static DEFINE_MUTEX(comedi_subdevice_minor_table_lock);
/* Note: indexed by minor - COMEDI_NUM_BOARD_MINORS. */
static struct comedi_subdevice
*comedi_subdevice_minor_table[COMEDI_NUM_SUBDEVICE_MINORS];

static struct class *comedi_class;
static struct cdev comedi_cdev;

static void comedi_device_init(struct comedi_device *dev)
{
<<<<<<< HEAD
	spin_lock_init(&dev->spinlock);
	mutex_init(&dev->mutex);
	dev->minor = -1;
}

=======
	kref_init(&dev->refcount);
	spin_lock_init(&dev->spinlock);
	mutex_init(&dev->mutex);
	init_rwsem(&dev->attach_lock);
	dev->minor = -1;
}

static void comedi_dev_kref_release(struct kref *kref)
{
	struct comedi_device *dev =
		container_of(kref, struct comedi_device, refcount);

	mutex_destroy(&dev->mutex);
	put_device(dev->class_dev);
	kfree(dev);
}

/**
 * comedi_dev_put() - Release a use of a COMEDI device
 * @dev: COMEDI device.
 *
 * Must be called when a user of a COMEDI device is finished with it.
 * When the last user of the COMEDI device calls this function, the
 * COMEDI device is destroyed.
 *
 * Return: 1 if the COMEDI device is destroyed by this call or @dev is
 * NULL, otherwise return 0.  Callers must not assume the COMEDI
 * device is still valid if this function returns 0.
 */
int comedi_dev_put(struct comedi_device *dev)
{
	if (dev)
		return kref_put(&dev->refcount, comedi_dev_kref_release);
	return 1;
}
EXPORT_SYMBOL_GPL(comedi_dev_put);

static struct comedi_device *comedi_dev_get(struct comedi_device *dev)
{
	if (dev)
		kref_get(&dev->refcount);
	return dev;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void comedi_device_cleanup(struct comedi_device *dev)
{
	struct module *driver_module = NULL;

<<<<<<< HEAD
	if (dev == NULL)
=======
	if (!dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	mutex_lock(&dev->mutex);
	if (dev->attached)
		driver_module = dev->driver->module;
	comedi_device_detach(dev);
<<<<<<< HEAD
	while (dev->use_count > 0) {
		if (driver_module)
			module_put(driver_module);
		module_put(THIS_MODULE);
		dev->use_count--;
	}
	mutex_unlock(&dev->mutex);
	mutex_destroy(&dev->mutex);
=======
	if (driver_module && dev->use_count)
		module_put(driver_module);
	mutex_unlock(&dev->mutex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool comedi_clear_board_dev(struct comedi_device *dev)
{
	unsigned int i = dev->minor;
	bool cleared = false;

	mutex_lock(&comedi_board_minor_table_lock);
	if (dev == comedi_board_minor_table[i]) {
		comedi_board_minor_table[i] = NULL;
		cleared = true;
	}
	mutex_unlock(&comedi_board_minor_table_lock);
	return cleared;
}

<<<<<<< HEAD
static struct comedi_device *comedi_clear_board_minor(unsigned minor)
=======
static struct comedi_device *comedi_clear_board_minor(unsigned int minor)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct comedi_device *dev;

	mutex_lock(&comedi_board_minor_table_lock);
	dev = comedi_board_minor_table[minor];
	comedi_board_minor_table[minor] = NULL;
	mutex_unlock(&comedi_board_minor_table_lock);
	return dev;
}

static void comedi_free_board_dev(struct comedi_device *dev)
{
	if (dev) {
<<<<<<< HEAD
=======
		comedi_device_cleanup(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (dev->class_dev) {
			device_destroy(comedi_class,
				       MKDEV(COMEDI_MAJOR, dev->minor));
		}
<<<<<<< HEAD
		comedi_device_cleanup(dev);
		kfree(dev);
	}
}

static struct comedi_subdevice
*comedi_subdevice_from_minor(unsigned minor)
=======
		comedi_dev_put(dev);
	}
}

static struct comedi_subdevice *
comedi_subdevice_from_minor(const struct comedi_device *dev, unsigned int minor)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct comedi_subdevice *s;
	unsigned int i = minor - COMEDI_NUM_BOARD_MINORS;

<<<<<<< HEAD
	BUG_ON(i >= COMEDI_NUM_SUBDEVICE_MINORS);
	mutex_lock(&comedi_subdevice_minor_table_lock);
	s = comedi_subdevice_minor_table[i];
=======
	mutex_lock(&comedi_subdevice_minor_table_lock);
	s = comedi_subdevice_minor_table[i];
	if (s && s->device != dev)
		s = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	return s;
}

<<<<<<< HEAD
static struct comedi_device *comedi_dev_from_board_minor(unsigned minor)
{
	struct comedi_device *dev;

	BUG_ON(minor >= COMEDI_NUM_BOARD_MINORS);
	mutex_lock(&comedi_board_minor_table_lock);
	dev = comedi_board_minor_table[minor];
=======
static struct comedi_device *comedi_dev_get_from_board_minor(unsigned int minor)
{
	struct comedi_device *dev;

	mutex_lock(&comedi_board_minor_table_lock);
	dev = comedi_dev_get(comedi_board_minor_table[minor]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&comedi_board_minor_table_lock);
	return dev;
}

<<<<<<< HEAD
static struct comedi_device *comedi_dev_from_subdevice_minor(unsigned minor)
{
	struct comedi_subdevice *s;

	s = comedi_subdevice_from_minor(minor);
	return s ? s->device : NULL;
}

struct comedi_device *comedi_dev_from_minor(unsigned minor)
{
	if (minor < COMEDI_NUM_BOARD_MINORS)
		return comedi_dev_from_board_minor(minor);
	else
		return comedi_dev_from_subdevice_minor(minor);
}
EXPORT_SYMBOL_GPL(comedi_dev_from_minor);
=======
static struct comedi_device *
comedi_dev_get_from_subdevice_minor(unsigned int minor)
{
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int i = minor - COMEDI_NUM_BOARD_MINORS;

	mutex_lock(&comedi_subdevice_minor_table_lock);
	s = comedi_subdevice_minor_table[i];
	dev = comedi_dev_get(s ? s->device : NULL);
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	return dev;
}

/**
 * comedi_dev_get_from_minor() - Get COMEDI device by minor device number
 * @minor: Minor device number.
 *
 * Finds the COMEDI device associated with the minor device number, if any,
 * and increments its reference count.  The COMEDI device is prevented from
 * being freed until a matching call is made to comedi_dev_put().
 *
 * Return: A pointer to the COMEDI device if it exists, with its usage
 * reference incremented.  Return NULL if no COMEDI device exists with the
 * specified minor device number.
 */
struct comedi_device *comedi_dev_get_from_minor(unsigned int minor)
{
	if (minor < COMEDI_NUM_BOARD_MINORS)
		return comedi_dev_get_from_board_minor(minor);

	return comedi_dev_get_from_subdevice_minor(minor);
}
EXPORT_SYMBOL_GPL(comedi_dev_get_from_minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct comedi_subdevice *
comedi_read_subdevice(const struct comedi_device *dev, unsigned int minor)
{
	struct comedi_subdevice *s;

	if (minor >= COMEDI_NUM_BOARD_MINORS) {
<<<<<<< HEAD
		s = comedi_subdevice_from_minor(minor);
		if (!s || s->device != dev)
			return NULL;
		if (s->subdev_flags & SDF_CMD_READ)
=======
		s = comedi_subdevice_from_minor(dev, minor);
		if (!s || (s->subdev_flags & SDF_CMD_READ))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return s;
	}
	return dev->read_subdev;
}

static struct comedi_subdevice *
comedi_write_subdevice(const struct comedi_device *dev, unsigned int minor)
{
	struct comedi_subdevice *s;

	if (minor >= COMEDI_NUM_BOARD_MINORS) {
<<<<<<< HEAD
		s = comedi_subdevice_from_minor(minor);
		if (!s || s->device != dev)
			return NULL;
		if (s->subdev_flags & SDF_CMD_WRITE)
=======
		s = comedi_subdevice_from_minor(dev, minor);
		if (!s || (s->subdev_flags & SDF_CMD_WRITE))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return s;
	}
	return dev->write_subdev;
}

<<<<<<< HEAD
static int resize_async_buffer(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_async *async, unsigned new_size)
{
=======
static void comedi_file_reset(struct file *file)
{
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	struct comedi_subdevice *s, *read_s, *write_s;
	unsigned int minor = iminor(file_inode(file));

	read_s = dev->read_subdev;
	write_s = dev->write_subdev;
	if (minor >= COMEDI_NUM_BOARD_MINORS) {
		s = comedi_subdevice_from_minor(dev, minor);
		if (!s || s->subdev_flags & SDF_CMD_READ)
			read_s = s;
		if (!s || s->subdev_flags & SDF_CMD_WRITE)
			write_s = s;
	}
	cfp->last_attached = dev->attached;
	cfp->last_detach_count = dev->detach_count;
	WRITE_ONCE(cfp->read_subdev, read_s);
	WRITE_ONCE(cfp->write_subdev, write_s);
}

static void comedi_file_check(struct file *file)
{
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;

	if (cfp->last_attached != dev->attached ||
	    cfp->last_detach_count != dev->detach_count)
		comedi_file_reset(file);
}

static struct comedi_subdevice *comedi_file_read_subdevice(struct file *file)
{
	struct comedi_file *cfp = file->private_data;

	comedi_file_check(file);
	return READ_ONCE(cfp->read_subdev);
}

static struct comedi_subdevice *comedi_file_write_subdevice(struct file *file)
{
	struct comedi_file *cfp = file->private_data;

	comedi_file_check(file);
	return READ_ONCE(cfp->write_subdev);
}

static int resize_async_buffer(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       unsigned int new_size)
{
	struct comedi_async *async = s->async;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int retval;

	if (new_size > async->max_bufsize)
		return -EPERM;

	if (s->busy) {
<<<<<<< HEAD
		DPRINTK("subdevice is busy, cannot resize buffer\n");
		return -EBUSY;
	}
	if (async->mmap_count) {
		DPRINTK("subdevice is mmapped, cannot resize buffer\n");
		return -EBUSY;
	}

	/* make sure buffer is an integral number of pages
	 * (we round up) */
=======
		dev_dbg(dev->class_dev,
			"subdevice is busy, cannot resize buffer\n");
		return -EBUSY;
	}
	if (comedi_buf_is_mmapped(s)) {
		dev_dbg(dev->class_dev,
			"subdevice is mmapped, cannot resize buffer\n");
		return -EBUSY;
	}

	/* make sure buffer is an integral number of pages (we round up) */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	new_size = (new_size + PAGE_SIZE - 1) & PAGE_MASK;

	retval = comedi_buf_alloc(dev, s, new_size);
	if (retval < 0)
		return retval;

	if (s->buf_change) {
<<<<<<< HEAD
		retval = s->buf_change(dev, s, new_size);
=======
		retval = s->buf_change(dev, s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (retval < 0)
			return retval;
	}

<<<<<<< HEAD
	DPRINTK("comedi%i subd %d buffer resized to %i bytes\n",
		dev->minor, s->index, async->prealloc_bufsz);
=======
	dev_dbg(dev->class_dev, "subd %d buffer resized to %i bytes\n",
		s->index, async->prealloc_bufsz);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/* sysfs attribute files */

<<<<<<< HEAD
static ssize_t show_max_read_buffer_kb(struct device *csdev,
=======
static ssize_t max_read_buffer_kb_show(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				       struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		size = s->async->max_bufsize / 1024;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t store_max_read_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%u\n", size);
}

static ssize_t max_read_buffer_kb_store(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		s->async->max_bufsize = size;
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return err ? err : count;
}

static ssize_t show_read_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(max_read_buffer_kb);

static ssize_t read_buffer_kb_show(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				   struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		size = s->async->prealloc_bufsz / 1024;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t store_read_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%u\n", size);
}

static ssize_t read_buffer_kb_store(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
<<<<<<< HEAD
		err = resize_async_buffer(dev, s, s->async, size);
=======
		err = resize_async_buffer(dev, s, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return err ? err : count;
}

static ssize_t show_max_write_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(read_buffer_kb);

static ssize_t max_write_buffer_kb_show(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					struct device_attribute *attr,
					char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		size = s->async->max_bufsize / 1024;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t store_max_write_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%u\n", size);
}

static ssize_t max_write_buffer_kb_store(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					 struct device_attribute *attr,
					 const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		s->async->max_bufsize = size;
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return err ? err : count;
}

static ssize_t show_write_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(max_write_buffer_kb);

static ssize_t write_buffer_kb_show(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				    struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		size = s->async->prealloc_bufsz / 1024;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t store_write_buffer_kb(struct device *csdev,
=======
	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%u\n", size);
}

static ssize_t write_buffer_kb_store(struct device *csdev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

<<<<<<< HEAD
	dev = comedi_dev_from_minor(minor);
=======
	dev = comedi_dev_get_from_minor(minor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
<<<<<<< HEAD
		err = resize_async_buffer(dev, s, s->async, size);
=======
		err = resize_async_buffer(dev, s, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

<<<<<<< HEAD
	return err ? err : count;
}

static struct device_attribute comedi_dev_attrs[] = {
	__ATTR(max_read_buffer_kb, S_IRUGO | S_IWUSR,
		show_max_read_buffer_kb, store_max_read_buffer_kb),
	__ATTR(read_buffer_kb, S_IRUGO | S_IWUSR | S_IWGRP,
		show_read_buffer_kb, store_read_buffer_kb),
	__ATTR(max_write_buffer_kb, S_IRUGO | S_IWUSR,
		show_max_write_buffer_kb, store_max_write_buffer_kb),
	__ATTR(write_buffer_kb, S_IRUGO | S_IWUSR | S_IWGRP,
		show_write_buffer_kb, store_write_buffer_kb),
	__ATTR_NULL
};

static void comedi_set_subdevice_runflags(struct comedi_subdevice *s,
					  unsigned mask, unsigned bits)
=======
	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(write_buffer_kb);

static struct attribute *comedi_dev_attrs[] = {
	&dev_attr_max_read_buffer_kb.attr,
	&dev_attr_read_buffer_kb.attr,
	&dev_attr_max_write_buffer_kb.attr,
	&dev_attr_write_buffer_kb.attr,
	NULL,
};
ATTRIBUTE_GROUPS(comedi_dev);

static void __comedi_clear_subdevice_runflags(struct comedi_subdevice *s,
					      unsigned int bits)
{
	s->runflags &= ~bits;
}

static void __comedi_set_subdevice_runflags(struct comedi_subdevice *s,
					    unsigned int bits)
{
	s->runflags |= bits;
}

static void comedi_update_subdevice_runflags(struct comedi_subdevice *s,
					     unsigned int mask,
					     unsigned int bits)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long flags;

	spin_lock_irqsave(&s->spin_lock, flags);
<<<<<<< HEAD
	s->runflags &= ~mask;
	s->runflags |= (bits & mask);
	spin_unlock_irqrestore(&s->spin_lock, flags);
}

static unsigned comedi_get_subdevice_runflags(struct comedi_subdevice *s)
{
	unsigned long flags;
	unsigned runflags;

	spin_lock_irqsave(&s->spin_lock, flags);
	runflags = s->runflags;
=======
	__comedi_clear_subdevice_runflags(s, mask);
	__comedi_set_subdevice_runflags(s, bits & mask);
	spin_unlock_irqrestore(&s->spin_lock, flags);
}

static unsigned int __comedi_get_subdevice_runflags(struct comedi_subdevice *s)
{
	return s->runflags;
}

static unsigned int comedi_get_subdevice_runflags(struct comedi_subdevice *s)
{
	unsigned long flags;
	unsigned int runflags;

	spin_lock_irqsave(&s->spin_lock, flags);
	runflags = __comedi_get_subdevice_runflags(s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_irqrestore(&s->spin_lock, flags);
	return runflags;
}

<<<<<<< HEAD
bool comedi_is_subdevice_running(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & SRF_RUNNING) ? true : false;
}
EXPORT_SYMBOL_GPL(comedi_is_subdevice_running);

static bool comedi_is_subdevice_in_error(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & SRF_ERROR) ? true : false;
}

static bool comedi_is_subdevice_idle(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & (SRF_ERROR | SRF_RUNNING)) ? false : true;
}

/*
   This function restores a subdevice to an idle state.
=======
static bool comedi_is_runflags_running(unsigned int runflags)
{
	return runflags & COMEDI_SRF_RUNNING;
}

static bool comedi_is_runflags_in_error(unsigned int runflags)
{
	return runflags & COMEDI_SRF_ERROR;
}

/**
 * comedi_is_subdevice_running() - Check if async command running on subdevice
 * @s: COMEDI subdevice.
 *
 * Return: %true if an asynchronous COMEDI command is active on the
 * subdevice, else %false.
 */
bool comedi_is_subdevice_running(struct comedi_subdevice *s)
{
	unsigned int runflags = comedi_get_subdevice_runflags(s);

	return comedi_is_runflags_running(runflags);
}
EXPORT_SYMBOL_GPL(comedi_is_subdevice_running);

static bool __comedi_is_subdevice_running(struct comedi_subdevice *s)
{
	unsigned int runflags = __comedi_get_subdevice_runflags(s);

	return comedi_is_runflags_running(runflags);
}

bool comedi_can_auto_free_spriv(struct comedi_subdevice *s)
{
	unsigned int runflags = __comedi_get_subdevice_runflags(s);

	return runflags & COMEDI_SRF_FREE_SPRIV;
}

/**
 * comedi_set_spriv_auto_free() - Mark subdevice private data as freeable
 * @s: COMEDI subdevice.
 *
 * Mark the subdevice as having a pointer to private data that can be
 * automatically freed when the COMEDI device is detached from the low-level
 * driver.
 */
void comedi_set_spriv_auto_free(struct comedi_subdevice *s)
{
	__comedi_set_subdevice_runflags(s, COMEDI_SRF_FREE_SPRIV);
}
EXPORT_SYMBOL_GPL(comedi_set_spriv_auto_free);

/**
 * comedi_alloc_spriv - Allocate memory for the subdevice private data
 * @s: COMEDI subdevice.
 * @size: Size of the memory to allocate.
 *
 * Allocate memory for the subdevice private data and point @s->private
 * to it.  The memory will be freed automatically when the COMEDI device
 * is detached from the low-level driver.
 *
 * Return: A pointer to the allocated memory @s->private on success.
 * Return NULL on failure.
 */
void *comedi_alloc_spriv(struct comedi_subdevice *s, size_t size)
{
	s->private = kzalloc(size, GFP_KERNEL);
	if (s->private)
		comedi_set_spriv_auto_free(s);
	return s->private;
}
EXPORT_SYMBOL_GPL(comedi_alloc_spriv);

/*
 * This function restores a subdevice to an idle state.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static void do_become_nonbusy(struct comedi_device *dev,
			      struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;

<<<<<<< HEAD
	comedi_set_subdevice_runflags(s, SRF_RUNNING, 0);
	if (async) {
		comedi_buf_reset(async);
		async->inttrig = NULL;
		kfree(async->cmd.chanlist);
		async->cmd.chanlist = NULL;
	} else {
		dev_err(dev->class_dev,
			"BUG: (?) do_become_nonbusy called with async=NULL\n");
	}

	s->busy = NULL;
=======
	comedi_update_subdevice_runflags(s, COMEDI_SRF_RUNNING, 0);
	if (async) {
		comedi_buf_reset(s);
		async->inttrig = NULL;
		kfree(async->cmd.chanlist);
		async->cmd.chanlist = NULL;
		s->busy = NULL;
		wake_up_interruptible_all(&async->wait_head);
	} else {
		dev_err(dev->class_dev,
			"BUG: (?) do_become_nonbusy called with async=NULL\n");
		s->busy = NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int do_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
	int ret = 0;

	if (comedi_is_subdevice_running(s) && s->cancel)
		ret = s->cancel(dev, s);

	do_become_nonbusy(dev, s);

	return ret;
}

<<<<<<< HEAD
=======
void comedi_device_cancel_all(struct comedi_device *dev)
{
	struct comedi_subdevice *s;
	int i;

	if (!dev->attached)
		return;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		if (s->async)
			do_cancel(dev, s);
	}
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int is_device_busy(struct comedi_device *dev)
{
	struct comedi_subdevice *s;
	int i;

	if (!dev->attached)
		return 0;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		if (s->busy)
			return 1;
<<<<<<< HEAD
		if (s->async && s->async->mmap_count)
=======
		if (s->async && comedi_buf_is_mmapped(s))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return 1;
	}

	return 0;
}

/*
<<<<<<< HEAD
	COMEDI_DEVCONFIG
	device config ioctl

	arg:
		pointer to devconfig structure

	reads:
		devconfig structure at arg

	writes:
		none
*/
=======
 * COMEDI_DEVCONFIG ioctl
 * attaches (and configures) or detaches a legacy device
 *
 * arg:
 *	pointer to comedi_devconfig structure (NULL if detaching)
 *
 * reads:
 *	comedi_devconfig structure (if attaching)
 *
 * writes:
 *	nothing
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_devconfig_ioctl(struct comedi_device *dev,
			      struct comedi_devconfig __user *arg)
{
	struct comedi_devconfig it;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

<<<<<<< HEAD
	if (arg == NULL) {
=======
	if (!arg) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (is_device_busy(dev))
			return -EBUSY;
		if (dev->attached) {
			struct module *driver_module = dev->driver->module;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			comedi_device_detach(dev);
			module_put(driver_module);
		}
		return 0;
	}

	if (copy_from_user(&it, arg, sizeof(it)))
		return -EFAULT;

	it.board_name[COMEDI_NAMELEN - 1] = 0;

	if (it.options[COMEDI_DEVCONF_AUX_DATA_LENGTH]) {
		dev_warn(dev->class_dev,
			 "comedi_config --init_data is deprecated\n");
		return -EINVAL;
	}

	if (dev->minor >= comedi_num_legacy_minors)
		/* don't re-use dynamically allocated comedi devices */
		return -EBUSY;

	/* This increments the driver module count on success. */
	return comedi_device_attach(dev, &it);
}

/*
<<<<<<< HEAD
	COMEDI_BUFCONFIG
	buffer configuration ioctl

	arg:
		pointer to bufconfig structure

	reads:
		bufconfig at arg

	writes:
		modified bufconfig at arg

*/
=======
 * COMEDI_BUFCONFIG ioctl
 * buffer configuration
 *
 * arg:
 *	pointer to comedi_bufconfig structure
 *
 * reads:
 *	comedi_bufconfig structure
 *
 * writes:
 *	modified comedi_bufconfig structure
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_bufconfig_ioctl(struct comedi_device *dev,
			      struct comedi_bufconfig __user *arg)
{
	struct comedi_bufconfig bc;
	struct comedi_async *async;
	struct comedi_subdevice *s;
	int retval = 0;

	if (copy_from_user(&bc, arg, sizeof(bc)))
		return -EFAULT;

<<<<<<< HEAD
	if (bc.subdevice >= dev->n_subdevices || bc.subdevice < 0)
=======
	if (bc.subdevice >= dev->n_subdevices)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	s = &dev->subdevices[bc.subdevice];
	async = s->async;

	if (!async) {
<<<<<<< HEAD
		DPRINTK("subdevice does not have async capability\n");
=======
		dev_dbg(dev->class_dev,
			"subdevice does not have async capability\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bc.size = 0;
		bc.maximum_size = 0;
		goto copyback;
	}

	if (bc.maximum_size) {
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

		async->max_bufsize = bc.maximum_size;
	}

	if (bc.size) {
<<<<<<< HEAD
		retval = resize_async_buffer(dev, s, async, bc.size);
=======
		retval = resize_async_buffer(dev, s, bc.size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (retval < 0)
			return retval;
	}

	bc.size = async->prealloc_bufsz;
	bc.maximum_size = async->max_bufsize;

copyback:
	if (copy_to_user(arg, &bc, sizeof(bc)))
		return -EFAULT;

	return 0;
}

/*
<<<<<<< HEAD
	COMEDI_DEVINFO
	device info ioctl

	arg:
		pointer to devinfo structure

	reads:
		none

	writes:
		devinfo structure

*/
=======
 * COMEDI_DEVINFO ioctl
 * device info
 *
 * arg:
 *	pointer to comedi_devinfo structure
 *
 * reads:
 *	nothing
 *
 * writes:
 *	comedi_devinfo structure
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_devinfo_ioctl(struct comedi_device *dev,
			    struct comedi_devinfo __user *arg,
			    struct file *file)
{
<<<<<<< HEAD
	const unsigned minor = iminor(file_inode(file));
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct comedi_subdevice *s;
	struct comedi_devinfo devinfo;

	memset(&devinfo, 0, sizeof(devinfo));

	/* fill devinfo structure */
	devinfo.version_code = COMEDI_VERSION_CODE;
	devinfo.n_subdevs = dev->n_subdevices;
	strlcpy(devinfo.driver_name, dev->driver->driver_name, COMEDI_NAMELEN);
	strlcpy(devinfo.board_name, dev->board_name, COMEDI_NAMELEN);

<<<<<<< HEAD
	s = comedi_read_subdevice(dev, minor);
=======
	s = comedi_file_read_subdevice(file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (s)
		devinfo.read_subdevice = s->index;
	else
		devinfo.read_subdevice = -1;

<<<<<<< HEAD
	s = comedi_write_subdevice(dev, minor);
=======
	s = comedi_file_write_subdevice(file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (s)
		devinfo.write_subdevice = s->index;
	else
		devinfo.write_subdevice = -1;

	if (copy_to_user(arg, &devinfo, sizeof(devinfo)))
		return -EFAULT;

	return 0;
}

/*
<<<<<<< HEAD
	COMEDI_SUBDINFO
	subdevice info ioctl

	arg:
		pointer to array of subdevice info structures

	reads:
		none

	writes:
		array of subdevice info structures at arg

*/
=======
 * COMEDI_SUBDINFO ioctl
 * subdevices info
 *
 * arg:
 *	pointer to array of comedi_subdinfo structures
 *
 * reads:
 *	nothing
 *
 * writes:
 *	array of comedi_subdinfo structures
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_subdinfo_ioctl(struct comedi_device *dev,
			     struct comedi_subdinfo __user *arg, void *file)
{
	int ret, i;
	struct comedi_subdinfo *tmp, *us;
	struct comedi_subdevice *s;

	tmp = kcalloc(dev->n_subdevices, sizeof(*tmp), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	/* fill subdinfo structs */
	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		us = tmp + i;

		us->type = s->type;
		us->n_chan = s->n_chan;
		us->subd_flags = s->subdev_flags;
		if (comedi_is_subdevice_running(s))
			us->subd_flags |= SDF_RUNNING;
#define TIMER_nanosec 5		/* backwards compatibility */
		us->timer_type = TIMER_nanosec;
		us->len_chanlist = s->len_chanlist;
		us->maxdata = s->maxdata;
		if (s->range_table) {
			us->range_type =
			    (i << 24) | (0 << 16) | (s->range_table->length);
		} else {
			us->range_type = 0;	/* XXX */
		}
<<<<<<< HEAD
		us->flags = s->flags;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (s->busy)
			us->subd_flags |= SDF_BUSY;
		if (s->busy == file)
			us->subd_flags |= SDF_BUSY_OWNER;
		if (s->lock)
			us->subd_flags |= SDF_LOCKED;
		if (s->lock == file)
			us->subd_flags |= SDF_LOCK_OWNER;
		if (!s->maxdata && s->maxdata_list)
			us->subd_flags |= SDF_MAXDATA;
<<<<<<< HEAD
		if (s->flaglist)
			us->subd_flags |= SDF_FLAGS;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (s->range_table_list)
			us->subd_flags |= SDF_RANGETYPE;
		if (s->do_cmd)
			us->subd_flags |= SDF_CMD;

		if (s->insn_bits != &insn_inval)
			us->insn_bits_support = COMEDI_SUPPORTED;
		else
			us->insn_bits_support = COMEDI_UNSUPPORTED;
<<<<<<< HEAD

		us->settling_time_0 = s->settling_time_0;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = copy_to_user(arg, tmp, dev->n_subdevices * sizeof(*tmp));

	kfree(tmp);

	return ret ? -EFAULT : 0;
}

/*
<<<<<<< HEAD
	COMEDI_CHANINFO
	subdevice info ioctl

	arg:
		pointer to chaninfo structure

	reads:
		chaninfo structure at arg

	writes:
		arrays at elements of chaninfo structure

*/
=======
 * COMEDI_CHANINFO ioctl
 * subdevice channel info
 *
 * arg:
 *	pointer to comedi_chaninfo structure
 *
 * reads:
 *	comedi_chaninfo structure
 *
 * writes:
 *	array of maxdata values to chaninfo->maxdata_list if requested
 *	array of range table lengths to chaninfo->range_table_list if requested
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_chaninfo_ioctl(struct comedi_device *dev,
			     struct comedi_chaninfo __user *arg)
{
	struct comedi_subdevice *s;
	struct comedi_chaninfo it;

	if (copy_from_user(&it, arg, sizeof(it)))
		return -EFAULT;

	if (it.subdev >= dev->n_subdevices)
		return -EINVAL;
	s = &dev->subdevices[it.subdev];

	if (it.maxdata_list) {
		if (s->maxdata || !s->maxdata_list)
			return -EINVAL;
		if (copy_to_user(it.maxdata_list, s->maxdata_list,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
	}

<<<<<<< HEAD
	if (it.flaglist) {
		if (!s->flaglist)
			return -EINVAL;
		if (copy_to_user(it.flaglist, s->flaglist,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
	}
=======
	if (it.flaglist)
		return -EINVAL;	/* flaglist not supported */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (it.rangelist) {
		int i;

		if (!s->range_table_list)
			return -EINVAL;
		for (i = 0; i < s->n_chan; i++) {
			int x;

			x = (dev->minor << 28) | (it.subdev << 24) | (i << 16) |
			    (s->range_table_list[i]->length);
			if (put_user(x, it.rangelist + i))
				return -EFAULT;
		}
<<<<<<< HEAD
#if 0
		if (copy_to_user(it.rangelist, s->range_type_list,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;
}

<<<<<<< HEAD
 /*
    COMEDI_BUFINFO
    buffer information ioctl

    arg:
    pointer to bufinfo structure

    reads:
    bufinfo at arg

    writes:
    modified bufinfo at arg

  */
=======
/*
 * COMEDI_BUFINFO ioctl
 * buffer information
 *
 * arg:
 *	pointer to comedi_bufinfo structure
 *
 * reads:
 *	comedi_bufinfo structure
 *
 * writes:
 *	modified comedi_bufinfo structure
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_bufinfo_ioctl(struct comedi_device *dev,
			    struct comedi_bufinfo __user *arg, void *file)
{
	struct comedi_bufinfo bi;
	struct comedi_subdevice *s;
	struct comedi_async *async;
<<<<<<< HEAD
=======
	unsigned int runflags;
	int retval = 0;
	bool become_nonbusy = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (copy_from_user(&bi, arg, sizeof(bi)))
		return -EFAULT;

<<<<<<< HEAD
	if (bi.subdevice >= dev->n_subdevices || bi.subdevice < 0)
=======
	if (bi.subdevice >= dev->n_subdevices)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	s = &dev->subdevices[bi.subdevice];

<<<<<<< HEAD
	if (s->lock && s->lock != file)
		return -EACCES;

	async = s->async;

	if (!async) {
		DPRINTK("subdevice does not have async capability\n");
		bi.buf_write_ptr = 0;
		bi.buf_read_ptr = 0;
		bi.buf_write_count = 0;
		bi.buf_read_count = 0;
		bi.bytes_read = 0;
		bi.bytes_written = 0;
		goto copyback;
	}
	if (!s->busy) {
		bi.bytes_read = 0;
		bi.bytes_written = 0;
		goto copyback_position;
	}
	if (s->busy != file)
		return -EACCES;

	if (bi.bytes_read && (s->subdev_flags & SDF_CMD_READ)) {
		bi.bytes_read = comedi_buf_read_alloc(async, bi.bytes_read);
		comedi_buf_read_free(async, bi.bytes_read);

		if (comedi_is_subdevice_idle(s) &&
		    async->buf_write_count == async->buf_read_count) {
			do_become_nonbusy(dev, s);
		}
	}

	if (bi.bytes_written && (s->subdev_flags & SDF_CMD_WRITE)) {
		bi.bytes_written =
		    comedi_buf_write_alloc(async, bi.bytes_written);
		comedi_buf_write_free(async, bi.bytes_written);
	}

copyback_position:
=======
	async = s->async;

	if (!async || s->busy != file)
		return -EINVAL;

	runflags = comedi_get_subdevice_runflags(s);
	if (!(async->cmd.flags & CMDF_WRITE)) {
		/* command was set up in "read" direction */
		if (bi.bytes_read) {
			comedi_buf_read_alloc(s, bi.bytes_read);
			bi.bytes_read = comedi_buf_read_free(s, bi.bytes_read);
		}
		/*
		 * If nothing left to read, and command has stopped, and
		 * {"read" position not updated or command stopped normally},
		 * then become non-busy.
		 */
		if (comedi_buf_read_n_available(s) == 0 &&
		    !comedi_is_runflags_running(runflags) &&
		    (bi.bytes_read == 0 ||
		     !comedi_is_runflags_in_error(runflags))) {
			become_nonbusy = true;
			if (comedi_is_runflags_in_error(runflags))
				retval = -EPIPE;
		}
		bi.bytes_written = 0;
	} else {
		/* command was set up in "write" direction */
		if (!comedi_is_runflags_running(runflags)) {
			bi.bytes_written = 0;
			become_nonbusy = true;
			if (comedi_is_runflags_in_error(runflags))
				retval = -EPIPE;
		} else if (bi.bytes_written) {
			comedi_buf_write_alloc(s, bi.bytes_written);
			bi.bytes_written =
			    comedi_buf_write_free(s, bi.bytes_written);
		}
		bi.bytes_read = 0;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bi.buf_write_count = async->buf_write_count;
	bi.buf_write_ptr = async->buf_write_ptr;
	bi.buf_read_count = async->buf_read_count;
	bi.buf_read_ptr = async->buf_read_ptr;

<<<<<<< HEAD
copyback:
=======
	if (become_nonbusy)
		do_become_nonbusy(dev, s);

	if (retval)
		return retval;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (copy_to_user(arg, &bi, sizeof(bi)))
		return -EFAULT;

	return 0;
}

static int check_insn_config_length(struct comedi_insn *insn,
				    unsigned int *data)
{
	if (insn->n < 1)
		return -EINVAL;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
	case INSN_CONFIG_DIO_INPUT:
	case INSN_CONFIG_DISARM:
	case INSN_CONFIG_RESET:
		if (insn->n == 1)
			return 0;
		break;
	case INSN_CONFIG_ARM:
	case INSN_CONFIG_DIO_QUERY:
	case INSN_CONFIG_BLOCK_SIZE:
	case INSN_CONFIG_FILTER:
	case INSN_CONFIG_SERIAL_CLOCK:
	case INSN_CONFIG_BIDIRECTIONAL_DATA:
	case INSN_CONFIG_ALT_SOURCE:
	case INSN_CONFIG_SET_COUNTER_MODE:
	case INSN_CONFIG_8254_READ_STATUS:
	case INSN_CONFIG_SET_ROUTING:
	case INSN_CONFIG_GET_ROUTING:
	case INSN_CONFIG_GET_PWM_STATUS:
	case INSN_CONFIG_PWM_SET_PERIOD:
	case INSN_CONFIG_PWM_GET_PERIOD:
		if (insn->n == 2)
			return 0;
		break;
	case INSN_CONFIG_SET_GATE_SRC:
	case INSN_CONFIG_GET_GATE_SRC:
	case INSN_CONFIG_SET_CLOCK_SRC:
	case INSN_CONFIG_GET_CLOCK_SRC:
	case INSN_CONFIG_SET_OTHER_SRC:
	case INSN_CONFIG_GET_COUNTER_STATUS:
	case INSN_CONFIG_PWM_SET_H_BRIDGE:
	case INSN_CONFIG_PWM_GET_H_BRIDGE:
	case INSN_CONFIG_GET_HARDWARE_BUFFER_SIZE:
		if (insn->n == 3)
			return 0;
		break;
	case INSN_CONFIG_PWM_OUTPUT:
	case INSN_CONFIG_ANALOG_TRIG:
		if (insn->n == 5)
			return 0;
		break;
	case INSN_CONFIG_DIGITAL_TRIG:
		if (insn->n == 6)
			return 0;
		break;
<<<<<<< HEAD
		/* by default we allow the insn since we don't have checks for
		 * all possible cases yet */
	default:
		pr_warn("comedi: No check for data length of config insn id %i is implemented.\n",
			data[0]);
		pr_warn("comedi: Add a check to %s in %s.\n",
			__func__, __FILE__);
		pr_warn("comedi: Assuming n=%i is correct.\n", insn->n);
=======
		/*
		 * by default we allow the insn since we don't have checks for
		 * all possible cases yet
		 */
	default:
		pr_warn("No check for data length of config insn id %i is implemented\n",
			data[0]);
		pr_warn("Add a check to %s in %s\n", __func__, __FILE__);
		pr_warn("Assuming n=%i is correct\n", insn->n);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;
	}
	return -EINVAL;
}

static int parse_insn(struct comedi_device *dev, struct comedi_insn *insn,
		      unsigned int *data, void *file)
{
	struct comedi_subdevice *s;
	int ret = 0;
	int i;

	if (insn->insn & INSN_MASK_SPECIAL) {
		/* a non-subdevice instruction */

		switch (insn->insn) {
		case INSN_GTOD:
			{
<<<<<<< HEAD
				struct timeval tv;
=======
				struct timespec64 tv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

				if (insn->n != 2) {
					ret = -EINVAL;
					break;
				}

<<<<<<< HEAD
				do_gettimeofday(&tv);
				data[0] = tv.tv_sec;
				data[1] = tv.tv_usec;
=======
				ktime_get_real_ts64(&tv);
				/* unsigned data safe until 2106 */
				data[0] = (unsigned int)tv.tv_sec;
				data[1] = tv.tv_nsec / NSEC_PER_USEC;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = 2;

				break;
			}
		case INSN_WAIT:
			if (insn->n != 1 || data[0] >= 100000) {
				ret = -EINVAL;
				break;
			}
			udelay(data[0] / 1000);
			ret = 1;
			break;
		case INSN_INTTRIG:
			if (insn->n != 1) {
				ret = -EINVAL;
				break;
			}
			if (insn->subdev >= dev->n_subdevices) {
<<<<<<< HEAD
				DPRINTK("%d not usable subdevice\n",
=======
				dev_dbg(dev->class_dev,
					"%d not usable subdevice\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					insn->subdev);
				ret = -EINVAL;
				break;
			}
			s = &dev->subdevices[insn->subdev];
			if (!s->async) {
<<<<<<< HEAD
				DPRINTK("no async\n");
=======
				dev_dbg(dev->class_dev, "no async\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = -EINVAL;
				break;
			}
			if (!s->async->inttrig) {
<<<<<<< HEAD
				DPRINTK("no inttrig\n");
=======
				dev_dbg(dev->class_dev, "no inttrig\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = -EAGAIN;
				break;
			}
			ret = s->async->inttrig(dev, s, data[0]);
			if (ret >= 0)
				ret = 1;
			break;
		default:
<<<<<<< HEAD
			DPRINTK("invalid insn\n");
=======
			dev_dbg(dev->class_dev, "invalid insn\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EINVAL;
			break;
		}
	} else {
		/* a subdevice instruction */
		unsigned int maxdata;

		if (insn->subdev >= dev->n_subdevices) {
<<<<<<< HEAD
			DPRINTK("subdevice %d out of range\n", insn->subdev);
=======
			dev_dbg(dev->class_dev, "subdevice %d out of range\n",
				insn->subdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EINVAL;
			goto out;
		}
		s = &dev->subdevices[insn->subdev];

		if (s->type == COMEDI_SUBD_UNUSED) {
<<<<<<< HEAD
			DPRINTK("%d not usable subdevice\n", insn->subdev);
=======
			dev_dbg(dev->class_dev, "%d not usable subdevice\n",
				insn->subdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EIO;
			goto out;
		}

		/* are we locked? (ioctl lock) */
		if (s->lock && s->lock != file) {
<<<<<<< HEAD
			DPRINTK("device locked\n");
=======
			dev_dbg(dev->class_dev, "device locked\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EACCES;
			goto out;
		}

		ret = comedi_check_chanlist(s, 1, &insn->chanspec);
		if (ret < 0) {
			ret = -EINVAL;
<<<<<<< HEAD
			DPRINTK("bad chanspec\n");
=======
			dev_dbg(dev->class_dev, "bad chanspec\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		}

		if (s->busy) {
			ret = -EBUSY;
			goto out;
		}
		/* This looks arbitrary.  It is. */
<<<<<<< HEAD
		s->busy = &parse_insn;
		switch (insn->insn) {
		case INSN_READ:
			ret = s->insn_read(dev, s, insn, data);
=======
		s->busy = parse_insn;
		switch (insn->insn) {
		case INSN_READ:
			ret = s->insn_read(dev, s, insn, data);
			if (ret == -ETIMEDOUT) {
				dev_dbg(dev->class_dev,
					"subdevice %d read instruction timed out\n",
					s->index);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		case INSN_WRITE:
			maxdata = s->maxdata_list
			    ? s->maxdata_list[CR_CHAN(insn->chanspec)]
			    : s->maxdata;
			for (i = 0; i < insn->n; ++i) {
				if (data[i] > maxdata) {
					ret = -EINVAL;
<<<<<<< HEAD
					DPRINTK("bad data value(s)\n");
					break;
				}
			}
			if (ret == 0)
				ret = s->insn_write(dev, s, insn, data);
=======
					dev_dbg(dev->class_dev,
						"bad data value(s)\n");
					break;
				}
			}
			if (ret == 0) {
				ret = s->insn_write(dev, s, insn, data);
				if (ret == -ETIMEDOUT) {
					dev_dbg(dev->class_dev,
						"subdevice %d write instruction timed out\n",
						s->index);
				}
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		case INSN_BITS:
			if (insn->n != 2) {
				ret = -EINVAL;
			} else {
<<<<<<< HEAD
				/* Most drivers ignore the base channel in
				 * insn->chanspec.  Fix this here if
				 * the subdevice has <= 32 channels.  */
				unsigned int shift;
				unsigned int orig_mask;

				orig_mask = data[0];
=======
				/*
				 * Most drivers ignore the base channel in
				 * insn->chanspec.  Fix this here if
				 * the subdevice has <= 32 channels.
				 */
				unsigned int orig_mask = data[0];
				unsigned int shift = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (s->n_chan <= 32) {
					shift = CR_CHAN(insn->chanspec);
					if (shift > 0) {
						insn->chanspec = 0;
						data[0] <<= shift;
						data[1] <<= shift;
					}
<<<<<<< HEAD
				} else
					shift = 0;
=======
				}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = s->insn_bits(dev, s, insn, data);
				data[0] = orig_mask;
				if (shift > 0)
					data[1] >>= shift;
			}
			break;
		case INSN_CONFIG:
			ret = check_insn_config_length(insn, data);
			if (ret)
				break;
			ret = s->insn_config(dev, s, insn, data);
			break;
		default:
			ret = -EINVAL;
			break;
		}

		s->busy = NULL;
	}

out:
	return ret;
}

/*
<<<<<<< HEAD
 *	COMEDI_INSNLIST
 *	synchronous instructions
 *
 *	arg:
 *		pointer to sync cmd structure
 *
 *	reads:
 *		sync cmd struct at arg
 *		instruction list
 *		data (for writes)
 *
 *	writes:
 *		data (for reads)
=======
 * COMEDI_INSNLIST ioctl
 * synchronous instruction list
 *
 * arg:
 *	pointer to comedi_insnlist structure
 *
 * reads:
 *	comedi_insnlist structure
 *	array of comedi_insn structures from insnlist->insns pointer
 *	data (for writes) from insns[].data pointers
 *
 * writes:
 *	data (for reads) to insns[].data pointers
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
/* arbitrary limits */
#define MAX_SAMPLES 256
static int do_insnlist_ioctl(struct comedi_device *dev,
			     struct comedi_insnlist __user *arg, void *file)
{
	struct comedi_insnlist insnlist;
	struct comedi_insn *insns = NULL;
	unsigned int *data = NULL;
	int i = 0;
	int ret = 0;

	if (copy_from_user(&insnlist, arg, sizeof(insnlist)))
		return -EFAULT;

<<<<<<< HEAD
	data = kmalloc(sizeof(unsigned int) * MAX_SAMPLES, GFP_KERNEL);
	if (!data) {
		DPRINTK("kmalloc failed\n");
=======
	data = kmalloc_array(MAX_SAMPLES, sizeof(unsigned int), GFP_KERNEL);
	if (!data) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = -ENOMEM;
		goto error;
	}

	insns = kcalloc(insnlist.n_insns, sizeof(*insns), GFP_KERNEL);
	if (!insns) {
<<<<<<< HEAD
		DPRINTK("kmalloc failed\n");
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = -ENOMEM;
		goto error;
	}

	if (copy_from_user(insns, insnlist.insns,
			   sizeof(*insns) * insnlist.n_insns)) {
<<<<<<< HEAD
		DPRINTK("copy_from_user failed\n");
=======
		dev_dbg(dev->class_dev, "copy_from_user failed\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = -EFAULT;
		goto error;
	}

	for (i = 0; i < insnlist.n_insns; i++) {
		if (insns[i].n > MAX_SAMPLES) {
<<<<<<< HEAD
			DPRINTK("number of samples too large\n");
=======
			dev_dbg(dev->class_dev,
				"number of samples too large\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EINVAL;
			goto error;
		}
		if (insns[i].insn & INSN_MASK_WRITE) {
			if (copy_from_user(data, insns[i].data,
					   insns[i].n * sizeof(unsigned int))) {
<<<<<<< HEAD
				DPRINTK("copy_from_user failed\n");
=======
				dev_dbg(dev->class_dev,
					"copy_from_user failed\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = -EFAULT;
				goto error;
			}
		}
		ret = parse_insn(dev, insns + i, data, file);
		if (ret < 0)
			goto error;
		if (insns[i].insn & INSN_MASK_READ) {
			if (copy_to_user(insns[i].data, data,
					 insns[i].n * sizeof(unsigned int))) {
<<<<<<< HEAD
				DPRINTK("copy_to_user failed\n");
=======
				dev_dbg(dev->class_dev,
					"copy_to_user failed\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ret = -EFAULT;
				goto error;
			}
		}
		if (need_resched())
			schedule();
	}

error:
	kfree(insns);
	kfree(data);

	if (ret < 0)
		return ret;
	return i;
}

/*
<<<<<<< HEAD
 *	COMEDI_INSN
 *	synchronous instructions
 *
 *	arg:
 *		pointer to insn
 *
 *	reads:
 *		struct comedi_insn struct at arg
 *		data (for writes)
 *
 *	writes:
 *		data (for reads)
=======
 * COMEDI_INSN ioctl
 * synchronous instruction
 *
 * arg:
 *	pointer to comedi_insn structure
 *
 * reads:
 *	comedi_insn structure
 *	data (for writes) from insn->data pointer
 *
 * writes:
 *	data (for reads) to insn->data pointer
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int do_insn_ioctl(struct comedi_device *dev,
			 struct comedi_insn __user *arg, void *file)
{
	struct comedi_insn insn;
	unsigned int *data = NULL;
	int ret = 0;

<<<<<<< HEAD
	data = kmalloc(sizeof(unsigned int) * MAX_SAMPLES, GFP_KERNEL);
=======
	data = kmalloc_array(MAX_SAMPLES, sizeof(unsigned int), GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!data) {
		ret = -ENOMEM;
		goto error;
	}

	if (copy_from_user(&insn, arg, sizeof(insn))) {
		ret = -EFAULT;
		goto error;
	}

	/* This is where the behavior of insn and insnlist deviate. */
	if (insn.n > MAX_SAMPLES)
		insn.n = MAX_SAMPLES;
	if (insn.insn & INSN_MASK_WRITE) {
		if (copy_from_user(data,
				   insn.data,
				   insn.n * sizeof(unsigned int))) {
			ret = -EFAULT;
			goto error;
		}
	}
	ret = parse_insn(dev, &insn, data, file);
	if (ret < 0)
		goto error;
	if (insn.insn & INSN_MASK_READ) {
		if (copy_to_user(insn.data,
				 data,
				 insn.n * sizeof(unsigned int))) {
			ret = -EFAULT;
			goto error;
		}
	}
	ret = insn.n;

error:
	kfree(data);

	return ret;
}

<<<<<<< HEAD
static int do_cmd_ioctl(struct comedi_device *dev,
			struct comedi_cmd __user *arg, void *file)
{
	struct comedi_cmd cmd;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int ret = 0;
	unsigned int __user *user_chanlist;

	if (copy_from_user(&cmd, arg, sizeof(cmd))) {
		DPRINTK("bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	if (cmd.subdev >= dev->n_subdevices) {
		DPRINTK("%d no such subdevice\n", cmd.subdev);
		return -ENODEV;
	}

	s = &dev->subdevices[cmd.subdev];
	async = s->async;

	if (s->type == COMEDI_SUBD_UNUSED) {
		DPRINTK("%d not valid subdevice\n", cmd.subdev);
=======
static int __comedi_get_user_cmd(struct comedi_device *dev,
				 struct comedi_cmd __user *arg,
				 struct comedi_cmd *cmd)
{
	struct comedi_subdevice *s;

	if (copy_from_user(cmd, arg, sizeof(*cmd))) {
		dev_dbg(dev->class_dev, "bad cmd address\n");
		return -EFAULT;
	}

	if (cmd->subdev >= dev->n_subdevices) {
		dev_dbg(dev->class_dev, "%d no such subdevice\n", cmd->subdev);
		return -ENODEV;
	}

	s = &dev->subdevices[cmd->subdev];

	if (s->type == COMEDI_SUBD_UNUSED) {
		dev_dbg(dev->class_dev, "%d not valid subdevice\n",
			cmd->subdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EIO;
	}

	if (!s->do_cmd || !s->do_cmdtest || !s->async) {
<<<<<<< HEAD
		DPRINTK("subdevice %i does not support commands\n",
			cmd.subdev);
		return -EIO;
	}

	/* are we locked? (ioctl lock) */
	if (s->lock && s->lock != file) {
		DPRINTK("subdevice locked\n");
		return -EACCES;
	}

	/* are we busy? */
	if (s->busy) {
		DPRINTK("subdevice busy\n");
		return -EBUSY;
	}

	/* make sure channel/gain list isn't too long */
	if (cmd.chanlist_len > s->len_chanlist) {
		DPRINTK("channel/gain list too long %u > %d\n",
			cmd.chanlist_len, s->len_chanlist);
		return -EINVAL;
	}

	/* make sure channel/gain list isn't too short */
	if (cmd.chanlist_len < 1) {
		DPRINTK("channel/gain list too short %u < 1\n",
			cmd.chanlist_len);
		return -EINVAL;
	}

	async->cmd = cmd;
	async->cmd.data = NULL;
	/* load channel/gain list */
	async->cmd.chanlist =
	    kmalloc(async->cmd.chanlist_len * sizeof(int), GFP_KERNEL);
	if (!async->cmd.chanlist) {
		DPRINTK("allocation failed\n");
		return -ENOMEM;
	}

	if (copy_from_user(async->cmd.chanlist, user_chanlist,
			   async->cmd.chanlist_len * sizeof(int))) {
		DPRINTK("fault reading chanlist\n");
		ret = -EFAULT;
		goto cleanup;
	}

	/* make sure each element in channel/gain list is valid */
	ret = comedi_check_chanlist(s,
				    async->cmd.chanlist_len,
				    async->cmd.chanlist);
	if (ret < 0) {
		DPRINTK("bad chanlist\n");
		goto cleanup;
	}

	ret = s->do_cmdtest(dev, s, &async->cmd);

	if (async->cmd.flags & TRIG_BOGUS || ret) {
		DPRINTK("test returned %d\n", ret);
		cmd = async->cmd;
		/* restore chanlist pointer before copying back */
		cmd.chanlist = (unsigned int __force *)user_chanlist;
		cmd.data = NULL;
		if (copy_to_user(arg, &cmd, sizeof(cmd))) {
			DPRINTK("fault writing cmd\n");
=======
		dev_dbg(dev->class_dev,
			"subdevice %d does not support commands\n",
			cmd->subdev);
		return -EIO;
	}

	/* make sure channel/gain list isn't too long */
	if (cmd->chanlist_len > s->len_chanlist) {
		dev_dbg(dev->class_dev, "channel/gain list too long %d > %d\n",
			cmd->chanlist_len, s->len_chanlist);
		return -EINVAL;
	}

	/*
	 * Set the CMDF_WRITE flag to the correct state if the subdevice
	 * supports only "read" commands or only "write" commands.
	 */
	switch (s->subdev_flags & (SDF_CMD_READ | SDF_CMD_WRITE)) {
	case SDF_CMD_READ:
		cmd->flags &= ~CMDF_WRITE;
		break;
	case SDF_CMD_WRITE:
		cmd->flags |= CMDF_WRITE;
		break;
	default:
		break;
	}

	return 0;
}

static int __comedi_get_user_chanlist(struct comedi_device *dev,
				      struct comedi_subdevice *s,
				      unsigned int __user *user_chanlist,
				      struct comedi_cmd *cmd)
{
	unsigned int *chanlist;
	int ret;

	cmd->chanlist = NULL;
	chanlist = memdup_user(user_chanlist,
			       cmd->chanlist_len * sizeof(unsigned int));
	if (IS_ERR(chanlist))
		return PTR_ERR(chanlist);

	/* make sure each element in channel/gain list is valid */
	ret = comedi_check_chanlist(s, cmd->chanlist_len, chanlist);
	if (ret < 0) {
		kfree(chanlist);
		return ret;
	}

	cmd->chanlist = chanlist;

	return 0;
}

/*
 * COMEDI_CMD ioctl
 * asynchronous acquisition command set-up
 *
 * arg:
 *	pointer to comedi_cmd structure
 *
 * reads:
 *	comedi_cmd structure
 *	channel/range list from cmd->chanlist pointer
 *
 * writes:
 *	possibly modified comedi_cmd structure (when -EAGAIN returned)
 */
static int do_cmd_ioctl(struct comedi_device *dev,
			struct comedi_cmd __user *arg, void *file)
{
	struct comedi_cmd cmd;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	unsigned int __user *user_chanlist;
	int ret;

	/* get the user's cmd and do some simple validation */
	ret = __comedi_get_user_cmd(dev, arg, &cmd);
	if (ret)
		return ret;

	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	s = &dev->subdevices[cmd.subdev];
	async = s->async;

	/* are we locked? (ioctl lock) */
	if (s->lock && s->lock != file) {
		dev_dbg(dev->class_dev, "subdevice locked\n");
		return -EACCES;
	}

	/* are we busy? */
	if (s->busy) {
		dev_dbg(dev->class_dev, "subdevice busy\n");
		return -EBUSY;
	}

	/* make sure channel/gain list isn't too short */
	if (cmd.chanlist_len < 1) {
		dev_dbg(dev->class_dev, "channel/gain list too short %u < 1\n",
			cmd.chanlist_len);
		return -EINVAL;
	}

	async->cmd = cmd;
	async->cmd.data = NULL;

	/* load channel/gain list */
	ret = __comedi_get_user_chanlist(dev, s, user_chanlist, &async->cmd);
	if (ret)
		goto cleanup;

	ret = s->do_cmdtest(dev, s, &async->cmd);

	if (async->cmd.flags & CMDF_BOGUS || ret) {
		dev_dbg(dev->class_dev, "test returned %d\n", ret);
		cmd = async->cmd;
		/* restore chanlist pointer before copying back */
		cmd.chanlist = (unsigned int __force *)user_chanlist;
		cmd.data = NULL;
		if (copy_to_user(arg, &cmd, sizeof(cmd))) {
			dev_dbg(dev->class_dev, "fault writing cmd\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EFAULT;
			goto cleanup;
		}
		ret = -EAGAIN;
		goto cleanup;
	}

	if (!async->prealloc_bufsz) {
		ret = -ENOMEM;
<<<<<<< HEAD
		DPRINTK("no buffer (?)\n");
		goto cleanup;
	}

	comedi_buf_reset(async);

	async->cb_mask =
	    COMEDI_CB_EOA | COMEDI_CB_BLOCK | COMEDI_CB_ERROR |
	    COMEDI_CB_OVERFLOW;
	if (async->cmd.flags & TRIG_WAKE_EOS)
		async->cb_mask |= COMEDI_CB_EOS;

	comedi_set_subdevice_runflags(s, ~0, SRF_USER | SRF_RUNNING);

	/* set s->busy _after_ setting SRF_RUNNING flag to avoid race with
	 * comedi_read() or comedi_write() */
=======
		dev_dbg(dev->class_dev, "no buffer (?)\n");
		goto cleanup;
	}

	comedi_buf_reset(s);

	async->cb_mask = COMEDI_CB_BLOCK | COMEDI_CB_CANCEL_MASK;
	if (async->cmd.flags & CMDF_WAKE_EOS)
		async->cb_mask |= COMEDI_CB_EOS;

	comedi_update_subdevice_runflags(s, COMEDI_SRF_BUSY_MASK,
					 COMEDI_SRF_RUNNING);

	/*
	 * Set s->busy _after_ setting COMEDI_SRF_RUNNING flag to avoid
	 * race with comedi_read() or comedi_write().
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	s->busy = file;
	ret = s->do_cmd(dev, s);
	if (ret == 0)
		return 0;

cleanup:
	do_become_nonbusy(dev, s);

	return ret;
}

/*
<<<<<<< HEAD
	COMEDI_CMDTEST
	command testing ioctl

	arg:
		pointer to cmd structure

	reads:
		cmd structure at arg
		channel/range list

	writes:
		modified cmd structure at arg

*/
=======
 * COMEDI_CMDTEST ioctl
 * asynchronous acquisition command testing
 *
 * arg:
 *	pointer to comedi_cmd structure
 *
 * reads:
 *	comedi_cmd structure
 *	channel/range list from cmd->chanlist pointer
 *
 * writes:
 *	possibly modified comedi_cmd structure
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int do_cmdtest_ioctl(struct comedi_device *dev,
			    struct comedi_cmd __user *arg, void *file)
{
	struct comedi_cmd cmd;
	struct comedi_subdevice *s;
<<<<<<< HEAD
	int ret = 0;
	unsigned int *chanlist = NULL;
	unsigned int __user *user_chanlist;

	if (copy_from_user(&cmd, arg, sizeof(cmd))) {
		DPRINTK("bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	if (cmd.subdev >= dev->n_subdevices) {
		DPRINTK("%d no such subdevice\n", cmd.subdev);
		return -ENODEV;
	}

	s = &dev->subdevices[cmd.subdev];
	if (s->type == COMEDI_SUBD_UNUSED) {
		DPRINTK("%d not valid subdevice\n", cmd.subdev);
		return -EIO;
	}

	if (!s->do_cmd || !s->do_cmdtest) {
		DPRINTK("subdevice %i does not support commands\n",
			cmd.subdev);
		return -EIO;
	}

	/* make sure channel/gain list isn't too long */
	if (cmd.chanlist_len > s->len_chanlist) {
		DPRINTK("channel/gain list too long %d > %d\n",
			cmd.chanlist_len, s->len_chanlist);
		ret = -EINVAL;
		goto cleanup;
	}

	/* load channel/gain list */
	if (cmd.chanlist) {
		chanlist =
		    kmalloc(cmd.chanlist_len * sizeof(int), GFP_KERNEL);
		if (!chanlist) {
			DPRINTK("allocation failed\n");
			ret = -ENOMEM;
			goto cleanup;
		}

		if (copy_from_user(chanlist, user_chanlist,
				   cmd.chanlist_len * sizeof(int))) {
			DPRINTK("fault reading chanlist\n");
			ret = -EFAULT;
			goto cleanup;
		}

		/* make sure each element in channel/gain list is valid */
		ret = comedi_check_chanlist(s, cmd.chanlist_len, chanlist);
		if (ret < 0) {
			DPRINTK("bad chanlist\n");
			goto cleanup;
		}

		cmd.chanlist = chanlist;
=======
	unsigned int __user *user_chanlist;
	int ret;

	/* get the user's cmd and do some simple validation */
	ret = __comedi_get_user_cmd(dev, arg, &cmd);
	if (ret)
		return ret;

	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	s = &dev->subdevices[cmd.subdev];

	/* user_chanlist can be NULL for COMEDI_CMDTEST ioctl */
	if (user_chanlist) {
		/* load channel/gain list */
		ret = __comedi_get_user_chanlist(dev, s, user_chanlist, &cmd);
		if (ret)
			return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = s->do_cmdtest(dev, s, &cmd);

<<<<<<< HEAD
=======
	kfree(cmd.chanlist);	/* free kernel copy of user chanlist */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* restore chanlist pointer before copying back */
	cmd.chanlist = (unsigned int __force *)user_chanlist;

	if (copy_to_user(arg, &cmd, sizeof(cmd))) {
<<<<<<< HEAD
		DPRINTK("bad cmd address\n");
		ret = -EFAULT;
		goto cleanup;
	}
cleanup:
	kfree(chanlist);
=======
		dev_dbg(dev->class_dev, "bad cmd address\n");
		ret = -EFAULT;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

/*
<<<<<<< HEAD
	COMEDI_LOCK
	lock subdevice

	arg:
		subdevice number

	reads:
		none

	writes:
		none

*/

static int do_lock_ioctl(struct comedi_device *dev, unsigned int arg,
=======
 * COMEDI_LOCK ioctl
 * lock subdevice
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_lock_ioctl(struct comedi_device *dev, unsigned long arg,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			 void *file)
{
	int ret = 0;
	unsigned long flags;
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
	s = &dev->subdevices[arg];

	spin_lock_irqsave(&s->spin_lock, flags);
	if (s->busy || s->lock)
		ret = -EBUSY;
	else
		s->lock = file;
	spin_unlock_irqrestore(&s->spin_lock, flags);

<<<<<<< HEAD
#if 0
	if (ret < 0)
		return ret;

	if (s->lock_f)
		ret = s->lock_f(dev, s);
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

/*
<<<<<<< HEAD
	COMEDI_UNLOCK
	unlock subdevice

	arg:
		subdevice number

	reads:
		none

	writes:
		none

	This function isn't protected by the semaphore, since
	we already own the lock.
*/
static int do_unlock_ioctl(struct comedi_device *dev, unsigned int arg,
=======
 * COMEDI_UNLOCK ioctl
 * unlock subdevice
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_unlock_ioctl(struct comedi_device *dev, unsigned long arg,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			   void *file)
{
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
	s = &dev->subdevices[arg];

	if (s->busy)
		return -EBUSY;

	if (s->lock && s->lock != file)
		return -EACCES;

<<<<<<< HEAD
	if (s->lock == file) {
#if 0
		if (s->unlock)
			s->unlock(dev, s);
#endif

		s->lock = NULL;
	}
=======
	if (s->lock == file)
		s->lock = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

/*
<<<<<<< HEAD
	COMEDI_CANCEL
	cancel acquisition ioctl

	arg:
		subdevice number

	reads:
		nothing

	writes:
		nothing

*/
static int do_cancel_ioctl(struct comedi_device *dev, unsigned int arg,
			   void *file)
{
	struct comedi_subdevice *s;
	int ret;
=======
 * COMEDI_CANCEL ioctl
 * cancel asynchronous acquisition
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_cancel_ioctl(struct comedi_device *dev, unsigned long arg,
			   void *file)
{
	struct comedi_subdevice *s;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (arg >= dev->n_subdevices)
		return -EINVAL;
	s = &dev->subdevices[arg];
<<<<<<< HEAD
	if (s->async == NULL)
		return -EINVAL;

	if (s->lock && s->lock != file)
		return -EACCES;

=======
	if (!s->async)
		return -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!s->busy)
		return 0;

	if (s->busy != file)
		return -EBUSY;

<<<<<<< HEAD
	ret = do_cancel(dev, s);
	if (comedi_get_subdevice_runflags(s) & SRF_USER)
		wake_up_interruptible(&s->async->wait_head);

	return ret;
}

/*
	COMEDI_POLL ioctl
	instructs driver to synchronize buffers

	arg:
		subdevice number

	reads:
		nothing

	writes:
		nothing

*/
static int do_poll_ioctl(struct comedi_device *dev, unsigned int arg,
=======
	return do_cancel(dev, s);
}

/*
 * COMEDI_POLL ioctl
 * instructs driver to synchronize buffers
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_poll_ioctl(struct comedi_device *dev, unsigned long arg,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			 void *file)
{
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
	s = &dev->subdevices[arg];

<<<<<<< HEAD
	if (s->lock && s->lock != file)
		return -EACCES;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!s->busy)
		return 0;

	if (s->busy != file)
		return -EBUSY;

	if (s->poll)
		return s->poll(dev, s);

	return -EINVAL;
}

<<<<<<< HEAD
static long comedi_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);
	int rc;

	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);

	/* Device config is special, because it must work on
	 * an unconfigured device. */
=======
/*
 * COMEDI_SETRSUBD ioctl
 * sets the current "read" subdevice on a per-file basis
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_setrsubd_ioctl(struct comedi_device *dev, unsigned long arg,
			     struct file *file)
{
	struct comedi_file *cfp = file->private_data;
	struct comedi_subdevice *s_old, *s_new;

	if (arg >= dev->n_subdevices)
		return -EINVAL;

	s_new = &dev->subdevices[arg];
	s_old = comedi_file_read_subdevice(file);
	if (s_old == s_new)
		return 0;	/* no change */

	if (!(s_new->subdev_flags & SDF_CMD_READ))
		return -EINVAL;

	/*
	 * Check the file isn't still busy handling a "read" command on the
	 * old subdevice (if any).
	 */
	if (s_old && s_old->busy == file && s_old->async &&
	    !(s_old->async->cmd.flags & CMDF_WRITE))
		return -EBUSY;

	WRITE_ONCE(cfp->read_subdev, s_new);
	return 0;
}

/*
 * COMEDI_SETWSUBD ioctl
 * sets the current "write" subdevice on a per-file basis
 *
 * arg:
 *	subdevice number
 *
 * reads:
 *	nothing
 *
 * writes:
 *	nothing
 */
static int do_setwsubd_ioctl(struct comedi_device *dev, unsigned long arg,
			     struct file *file)
{
	struct comedi_file *cfp = file->private_data;
	struct comedi_subdevice *s_old, *s_new;

	if (arg >= dev->n_subdevices)
		return -EINVAL;

	s_new = &dev->subdevices[arg];
	s_old = comedi_file_write_subdevice(file);
	if (s_old == s_new)
		return 0;	/* no change */

	if (!(s_new->subdev_flags & SDF_CMD_WRITE))
		return -EINVAL;

	/*
	 * Check the file isn't still busy handling a "write" command on the
	 * old subdevice (if any).
	 */
	if (s_old && s_old->busy == file && s_old->async &&
	    (s_old->async->cmd.flags & CMDF_WRITE))
		return -EBUSY;

	WRITE_ONCE(cfp->write_subdev, s_new);
	return 0;
}

static long comedi_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	unsigned int minor = iminor(file_inode(file));
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	int rc;

	mutex_lock(&dev->mutex);

	/*
	 * Device config is special, because it must work on
	 * an unconfigured device.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (cmd == COMEDI_DEVCONFIG) {
		if (minor >= COMEDI_NUM_BOARD_MINORS) {
			/* Device config not appropriate on non-board minors. */
			rc = -ENOTTY;
			goto done;
		}
		rc = do_devconfig_ioctl(dev,
					(struct comedi_devconfig __user *)arg);
		if (rc == 0) {
			if (arg == 0 &&
			    dev->minor >= comedi_num_legacy_minors) {
<<<<<<< HEAD
				/* Successfully unconfigured a dynamically
				 * allocated device.  Try and remove it. */
=======
				/*
				 * Successfully unconfigured a dynamically
				 * allocated device.  Try and remove it.
				 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (comedi_clear_board_dev(dev)) {
					mutex_unlock(&dev->mutex);
					comedi_free_board_dev(dev);
					return rc;
				}
			}
		}
		goto done;
	}

	if (!dev->attached) {
<<<<<<< HEAD
		DPRINTK("no driver configured on /dev/comedi%i\n", dev->minor);
=======
		dev_dbg(dev->class_dev, "no driver attached\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rc = -ENODEV;
		goto done;
	}

	switch (cmd) {
	case COMEDI_BUFCONFIG:
		rc = do_bufconfig_ioctl(dev,
					(struct comedi_bufconfig __user *)arg);
		break;
	case COMEDI_DEVINFO:
		rc = do_devinfo_ioctl(dev, (struct comedi_devinfo __user *)arg,
				      file);
		break;
	case COMEDI_SUBDINFO:
		rc = do_subdinfo_ioctl(dev,
				       (struct comedi_subdinfo __user *)arg,
				       file);
		break;
	case COMEDI_CHANINFO:
		rc = do_chaninfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_RANGEINFO:
		rc = do_rangeinfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_BUFINFO:
		rc = do_bufinfo_ioctl(dev,
				      (struct comedi_bufinfo __user *)arg,
				      file);
		break;
	case COMEDI_LOCK:
		rc = do_lock_ioctl(dev, arg, file);
		break;
	case COMEDI_UNLOCK:
		rc = do_unlock_ioctl(dev, arg, file);
		break;
	case COMEDI_CANCEL:
		rc = do_cancel_ioctl(dev, arg, file);
		break;
	case COMEDI_CMD:
		rc = do_cmd_ioctl(dev, (struct comedi_cmd __user *)arg, file);
		break;
	case COMEDI_CMDTEST:
		rc = do_cmdtest_ioctl(dev, (struct comedi_cmd __user *)arg,
				      file);
		break;
	case COMEDI_INSNLIST:
		rc = do_insnlist_ioctl(dev,
				       (struct comedi_insnlist __user *)arg,
				       file);
		break;
	case COMEDI_INSN:
		rc = do_insn_ioctl(dev, (struct comedi_insn __user *)arg,
				   file);
		break;
	case COMEDI_POLL:
		rc = do_poll_ioctl(dev, arg, file);
		break;
<<<<<<< HEAD
=======
	case COMEDI_SETRSUBD:
		rc = do_setrsubd_ioctl(dev, arg, file);
		break;
	case COMEDI_SETWSUBD:
		rc = do_setwsubd_ioctl(dev, arg, file);
		break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		rc = -ENOTTY;
		break;
	}

done:
	mutex_unlock(&dev->mutex);
	return rc;
}

static void comedi_vm_open(struct vm_area_struct *area)
{
<<<<<<< HEAD
	struct comedi_async *async;
	struct comedi_device *dev;

	async = area->vm_private_data;
	dev = async->subdevice->device;

	mutex_lock(&dev->mutex);
	async->mmap_count++;
	mutex_unlock(&dev->mutex);
=======
	struct comedi_buf_map *bm;

	bm = area->vm_private_data;
	comedi_buf_map_get(bm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void comedi_vm_close(struct vm_area_struct *area)
{
<<<<<<< HEAD
	struct comedi_async *async;
	struct comedi_device *dev;

	async = area->vm_private_data;
	dev = async->subdevice->device;

	mutex_lock(&dev->mutex);
	async->mmap_count--;
	mutex_unlock(&dev->mutex);
=======
	struct comedi_buf_map *bm;

	bm = area->vm_private_data;
	comedi_buf_map_put(bm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct vm_operations_struct comedi_vm_ops = {
	.open = comedi_vm_open,
	.close = comedi_vm_close,
};

static int comedi_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);
	struct comedi_subdevice *s;
	struct comedi_async *async;
=======
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	struct comedi_buf_map *bm = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long start = vma->vm_start;
	unsigned long size;
	int n_pages;
	int i;
	int retval;

<<<<<<< HEAD
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
=======
	/*
	 * 'trylock' avoids circular dependency with current->mm->mmap_sem
	 * and down-reading &dev->attach_lock should normally succeed without
	 * contention unless the device is in the process of being attached
	 * or detached.
	 */
	if (!down_read_trylock(&dev->attach_lock))
		return -EAGAIN;

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		retval = -ENODEV;
		goto done;
	}

	if (vma->vm_flags & VM_WRITE)
<<<<<<< HEAD
		s = comedi_write_subdevice(dev, minor);
	else
		s = comedi_read_subdevice(dev, minor);
=======
		s = comedi_file_write_subdevice(file);
	else
		s = comedi_file_read_subdevice(file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!s) {
		retval = -EINVAL;
		goto done;
	}

	async = s->async;
	if (!async) {
		retval = -EINVAL;
		goto done;
	}

	if (vma->vm_pgoff != 0) {
<<<<<<< HEAD
		DPRINTK("comedi: mmap() offset must be 0.\n");
=======
		dev_dbg(dev->class_dev, "mmap() offset must be 0.\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		retval = -EINVAL;
		goto done;
	}

	size = vma->vm_end - vma->vm_start;
	if (size > async->prealloc_bufsz) {
		retval = -EFAULT;
		goto done;
	}
<<<<<<< HEAD
	if (size & (~PAGE_MASK)) {
=======
	if (offset_in_page(size)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		retval = -EFAULT;
		goto done;
	}

<<<<<<< HEAD
	n_pages = size >> PAGE_SHIFT;
	for (i = 0; i < n_pages; ++i) {
		struct comedi_buf_page *buf = &async->buf_page_list[i];
=======
	n_pages = vma_pages(vma);

	/* get reference to current buf map (if any) */
	bm = comedi_buf_map_from_subdev_get(s);
	if (!bm || n_pages > bm->n_pages) {
		retval = -EINVAL;
		goto done;
	}
	for (i = 0; i < n_pages; ++i) {
		struct comedi_buf_page *buf = &bm->page_list[i];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (remap_pfn_range(vma, start,
				    page_to_pfn(virt_to_page(buf->virt_addr)),
				    PAGE_SIZE, PAGE_SHARED)) {
			retval = -EAGAIN;
			goto done;
		}
		start += PAGE_SIZE;
	}

	vma->vm_ops = &comedi_vm_ops;
<<<<<<< HEAD
	vma->vm_private_data = async;

	async->mmap_count++;

	retval = 0;
done:
	mutex_unlock(&dev->mutex);
=======
	vma->vm_private_data = bm;

	vma->vm_ops->open(vma);

	retval = 0;
done:
	up_read(&dev->attach_lock);
	comedi_buf_map_put(bm);	/* put reference to buf map - okay if NULL */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

static unsigned int comedi_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
<<<<<<< HEAD
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);
	struct comedi_subdevice *s;

	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		goto done;
	}

	s = comedi_read_subdevice(dev, minor);
	if (s && s->async) {
		poll_wait(file, &s->async->wait_head, wait);
		if (!s->busy || !comedi_is_subdevice_running(s) ||
		    comedi_buf_read_n_available(s->async) > 0)
			mask |= POLLIN | POLLRDNORM;
	}

	s = comedi_write_subdevice(dev, minor);
	if (s && s->async) {
		unsigned int bps = bytes_per_sample(s->async->subdevice);

		poll_wait(file, &s->async->wait_head, wait);
		comedi_buf_write_alloc(s->async, s->async->prealloc_bufsz);
		if (!s->busy || !comedi_is_subdevice_running(s) ||
		    comedi_buf_write_n_allocated(s->async) >= bps)
=======
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	struct comedi_subdevice *s, *s_read;

	down_read(&dev->attach_lock);

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		goto done;
	}

	s = comedi_file_read_subdevice(file);
	s_read = s;
	if (s && s->async) {
		poll_wait(file, &s->async->wait_head, wait);
		if (s->busy != file || !comedi_is_subdevice_running(s) ||
		    (s->async->cmd.flags & CMDF_WRITE) ||
		    comedi_buf_read_n_available(s) > 0)
			mask |= POLLIN | POLLRDNORM;
	}

	s = comedi_file_write_subdevice(file);
	if (s && s->async) {
		unsigned int bps = comedi_bytes_per_sample(s);

		if (s != s_read)
			poll_wait(file, &s->async->wait_head, wait);
		if (s->busy != file || !comedi_is_subdevice_running(s) ||
		    !(s->async->cmd.flags & CMDF_WRITE) ||
		    comedi_buf_write_n_available(s) >= bps)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			mask |= POLLOUT | POLLWRNORM;
	}

done:
<<<<<<< HEAD
	mutex_unlock(&dev->mutex);
=======
	up_read(&dev->attach_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return mask;
}

static ssize_t comedi_write(struct file *file, const char __user *buf,
			    size_t nbytes, loff_t *offset)
{
	struct comedi_subdevice *s;
	struct comedi_async *async;
<<<<<<< HEAD
	int n, m, count = 0, retval = 0;
	DECLARE_WAITQUEUE(wait, current);
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);

	if (!dev)
		return -ENODEV;

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		return -ENODEV;
	}

	s = comedi_write_subdevice(dev, minor);
	if (!s || !s->async)
		return -EIO;

	async = s->async;

	if (!s->busy || !nbytes)
		return 0;
	if (s->busy != file)
		return -EACCES;

	add_wait_queue(&async->wait_head, &wait);
	while (nbytes > 0 && !retval) {
		set_current_state(TASK_INTERRUPTIBLE);

		if (!comedi_is_subdevice_running(s)) {
			if (count == 0) {
				mutex_lock(&dev->mutex);
				if (comedi_is_subdevice_in_error(s))
					retval = -EPIPE;
				else
					retval = 0;
				do_become_nonbusy(dev, s);
				mutex_unlock(&dev->mutex);
			}
			break;
		}

		n = nbytes;

		m = n;
		if (async->buf_write_ptr + m > async->prealloc_bufsz)
			m = async->prealloc_bufsz - async->buf_write_ptr;
		comedi_buf_write_alloc(async, async->prealloc_bufsz);
		if (m > comedi_buf_write_n_allocated(async))
			m = comedi_buf_write_n_allocated(async);
		if (m < n)
			n = m;
=======
	unsigned int n, m;
	ssize_t count = 0;
	int retval = 0;
	DECLARE_WAITQUEUE(wait, current);
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	bool become_nonbusy = false;
	bool attach_locked;
	unsigned int old_detach_count;

	/* Protect against device detachment during operation. */
	down_read(&dev->attach_lock);
	attach_locked = true;
	old_detach_count = dev->detach_count;

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		retval = -ENODEV;
		goto out;
	}

	s = comedi_file_write_subdevice(file);
	if (!s || !s->async) {
		retval = -EIO;
		goto out;
	}

	async = s->async;
	if (s->busy != file || !(async->cmd.flags & CMDF_WRITE)) {
		retval = -EINVAL;
		goto out;
	}

	add_wait_queue(&async->wait_head, &wait);
	while (count == 0 && !retval) {
		unsigned int runflags;
		unsigned int wp, n1, n2;

		set_current_state(TASK_INTERRUPTIBLE);

		runflags = comedi_get_subdevice_runflags(s);
		if (!comedi_is_runflags_running(runflags)) {
			if (comedi_is_runflags_in_error(runflags))
				retval = -EPIPE;
			if (retval || nbytes)
				become_nonbusy = true;
			break;
		}
		if (nbytes == 0)
			break;

		/* Allocate all free buffer space. */
		comedi_buf_write_alloc(s, async->prealloc_bufsz);
		m = comedi_buf_write_n_allocated(s);
		n = min_t(size_t, m, nbytes);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (n == 0) {
			if (file->f_flags & O_NONBLOCK) {
				retval = -EAGAIN;
				break;
			}
			schedule();
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
<<<<<<< HEAD
			if (!s->busy)
				break;
			if (s->busy != file) {
				retval = -EACCES;
=======
			if (s->busy != file ||
			    !(async->cmd.flags & CMDF_WRITE)) {
				retval = -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				break;
			}
			continue;
		}

<<<<<<< HEAD
		m = copy_from_user(async->prealloc_buf + async->buf_write_ptr,
				   buf, n);
=======
		set_current_state(TASK_RUNNING);
		wp = async->buf_write_ptr;
		n1 = min(n, async->prealloc_bufsz - wp);
		n2 = n - n1;
		m = copy_from_user(async->prealloc_buf + wp, buf, n1);
		if (m)
			m += n2;
		else if (n2)
			m = copy_from_user(async->prealloc_buf, buf + n1, n2);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (m) {
			n -= m;
			retval = -EFAULT;
		}
<<<<<<< HEAD
		comedi_buf_write_free(async, n);
=======
		comedi_buf_write_free(s, n);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		count += n;
		nbytes -= n;

		buf += n;
<<<<<<< HEAD
		break;		/* makes device work like a pipe */
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&async->wait_head, &wait);
=======
	}
	remove_wait_queue(&async->wait_head, &wait);
	set_current_state(TASK_RUNNING);
	if (become_nonbusy && count == 0) {
		struct comedi_subdevice *new_s;

		/*
		 * To avoid deadlock, cannot acquire dev->mutex
		 * while dev->attach_lock is held.
		 */
		up_read(&dev->attach_lock);
		attach_locked = false;
		mutex_lock(&dev->mutex);
		/*
		 * Check device hasn't become detached behind our back.
		 * Checking dev->detach_count is unchanged ought to be
		 * sufficient (unless there have been 2**32 detaches in the
		 * meantime!), but check the subdevice pointer as well just in
		 * case.
		 *
		 * Also check the subdevice is still in a suitable state to
		 * become non-busy in case it changed behind our back.
		 */
		new_s = comedi_file_write_subdevice(file);
		if (dev->attached && old_detach_count == dev->detach_count &&
		    s == new_s && new_s->async == async && s->busy == file &&
		    (async->cmd.flags & CMDF_WRITE) &&
		    !comedi_is_subdevice_running(s))
			do_become_nonbusy(dev, s);
		mutex_unlock(&dev->mutex);
	}
out:
	if (attach_locked)
		up_read(&dev->attach_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return count ? count : retval;
}

static ssize_t comedi_read(struct file *file, char __user *buf, size_t nbytes,
<<<<<<< HEAD
				loff_t *offset)
{
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int n, m, count = 0, retval = 0;
	DECLARE_WAITQUEUE(wait, current);
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);

	if (!dev)
		return -ENODEV;

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		return -ENODEV;
	}

	s = comedi_read_subdevice(dev, minor);
	if (!s || !s->async)
		return -EIO;

	async = s->async;
	if (!s->busy || !nbytes)
		return 0;
	if (s->busy != file)
		return -EACCES;

	add_wait_queue(&async->wait_head, &wait);
	while (nbytes > 0 && !retval) {
		set_current_state(TASK_INTERRUPTIBLE);

		n = nbytes;

		m = comedi_buf_read_n_available(async);
		/* printk("%d available\n",m); */
		if (async->buf_read_ptr + m > async->prealloc_bufsz)
			m = async->prealloc_bufsz - async->buf_read_ptr;
		/* printk("%d contiguous\n",m); */
		if (m < n)
			n = m;

		if (n == 0) {
			if (!comedi_is_subdevice_running(s)) {
				mutex_lock(&dev->mutex);
				do_become_nonbusy(dev, s);
				if (comedi_is_subdevice_in_error(s))
					retval = -EPIPE;
				else
					retval = 0;
				mutex_unlock(&dev->mutex);
				break;
			}
=======
			   loff_t *offset)
{
	struct comedi_subdevice *s;
	struct comedi_async *async;
	unsigned int n, m;
	ssize_t count = 0;
	int retval = 0;
	DECLARE_WAITQUEUE(wait, current);
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	unsigned int old_detach_count;
	bool become_nonbusy = false;
	bool attach_locked;

	/* Protect against device detachment during operation. */
	down_read(&dev->attach_lock);
	attach_locked = true;
	old_detach_count = dev->detach_count;

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		retval = -ENODEV;
		goto out;
	}

	s = comedi_file_read_subdevice(file);
	if (!s || !s->async) {
		retval = -EIO;
		goto out;
	}

	async = s->async;
	if (s->busy != file || (async->cmd.flags & CMDF_WRITE)) {
		retval = -EINVAL;
		goto out;
	}

	add_wait_queue(&async->wait_head, &wait);
	while (count == 0 && !retval) {
		unsigned int rp, n1, n2;

		set_current_state(TASK_INTERRUPTIBLE);

		m = comedi_buf_read_n_available(s);
		n = min_t(size_t, m, nbytes);

		if (n == 0) {
			unsigned int runflags =
				     comedi_get_subdevice_runflags(s);

			if (!comedi_is_runflags_running(runflags)) {
				if (comedi_is_runflags_in_error(runflags))
					retval = -EPIPE;
				if (retval || nbytes)
					become_nonbusy = true;
				break;
			}
			if (nbytes == 0)
				break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (file->f_flags & O_NONBLOCK) {
				retval = -EAGAIN;
				break;
			}
			schedule();
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
<<<<<<< HEAD
			if (!s->busy) {
				retval = 0;
				break;
			}
			if (s->busy != file) {
				retval = -EACCES;
=======
			if (s->busy != file ||
			    (async->cmd.flags & CMDF_WRITE)) {
				retval = -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				break;
			}
			continue;
		}
<<<<<<< HEAD
		m = copy_to_user(buf, async->prealloc_buf +
				 async->buf_read_ptr, n);
=======

		set_current_state(TASK_RUNNING);
		rp = async->buf_read_ptr;
		n1 = min(n, async->prealloc_bufsz - rp);
		n2 = n - n1;
		m = copy_to_user(buf, async->prealloc_buf + rp, n1);
		if (m)
			m += n2;
		else if (n2)
			m = copy_to_user(buf + n1, async->prealloc_buf, n2);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (m) {
			n -= m;
			retval = -EFAULT;
		}

<<<<<<< HEAD
		comedi_buf_read_alloc(async, n);
		comedi_buf_read_free(async, n);
=======
		comedi_buf_read_alloc(s, n);
		comedi_buf_read_free(s, n);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		count += n;
		nbytes -= n;

		buf += n;
<<<<<<< HEAD
		break;		/* makes device work like a pipe */
	}
	if (comedi_is_subdevice_idle(s)) {
		mutex_lock(&dev->mutex);
		if (async->buf_read_count - async->buf_write_count == 0)
			do_become_nonbusy(dev, s);
		mutex_unlock(&dev->mutex);
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&async->wait_head, &wait);
=======
	}
	remove_wait_queue(&async->wait_head, &wait);
	set_current_state(TASK_RUNNING);
	if (become_nonbusy && count == 0) {
		struct comedi_subdevice *new_s;

		/*
		 * To avoid deadlock, cannot acquire dev->mutex
		 * while dev->attach_lock is held.
		 */
		up_read(&dev->attach_lock);
		attach_locked = false;
		mutex_lock(&dev->mutex);
		/*
		 * Check device hasn't become detached behind our back.
		 * Checking dev->detach_count is unchanged ought to be
		 * sufficient (unless there have been 2**32 detaches in the
		 * meantime!), but check the subdevice pointer as well just in
		 * case.
		 *
		 * Also check the subdevice is still in a suitable state to
		 * become non-busy in case it changed behind our back.
		 */
		new_s = comedi_file_read_subdevice(file);
		if (dev->attached && old_detach_count == dev->detach_count &&
		    s == new_s && new_s->async == async && s->busy == file &&
		    !(async->cmd.flags & CMDF_WRITE) &&
		    !comedi_is_subdevice_running(s) &&
		    comedi_buf_read_n_available(s) == 0)
			do_become_nonbusy(dev, s);
		mutex_unlock(&dev->mutex);
	}
out:
	if (attach_locked)
		up_read(&dev->attach_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return count ? count : retval;
}

static int comedi_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	const unsigned minor = iminor(inode);
	struct comedi_device *dev = comedi_dev_from_minor(minor);

	if (!dev) {
		DPRINTK("invalid minor number\n");
		return -ENODEV;
	}

	/* This is slightly hacky, but we want module autoloading
	 * to work for root.
	 * case: user opens device, attached -> ok
	 * case: user opens device, unattached, !in_request_module -> autoload
	 * case: user opens device, unattached, in_request_module -> fail
	 * case: root opens device, attached -> ok
	 * case: root opens device, unattached, in_request_module -> ok
	 *   (typically called from modprobe)
	 * case: root opens device, unattached, !in_request_module -> autoload
	 *
	 * The last could be changed to "-> ok", which would deny root
	 * autoloading.
	 */
	mutex_lock(&dev->mutex);
	if (dev->attached)
		goto ok;
	if (!capable(CAP_NET_ADMIN) && dev->in_request_module) {
		DPRINTK("in request module\n");
		mutex_unlock(&dev->mutex);
		return -ENODEV;
	}
	if (capable(CAP_NET_ADMIN) && dev->in_request_module)
		goto ok;

	dev->in_request_module = true;

#ifdef CONFIG_KMOD
	mutex_unlock(&dev->mutex);
	request_module("char-major-%i-%i", COMEDI_MAJOR, dev->minor);
	mutex_lock(&dev->mutex);
#endif

	dev->in_request_module = false;

	if (!dev->attached && !capable(CAP_NET_ADMIN)) {
		DPRINTK("not attached and not CAP_NET_ADMIN\n");
		mutex_unlock(&dev->mutex);
		return -ENODEV;
	}
ok:
	__module_get(THIS_MODULE);

	if (dev->attached) {
		if (!try_module_get(dev->driver->module)) {
			module_put(THIS_MODULE);
			mutex_unlock(&dev->mutex);
			return -ENOSYS;
		}
	}

	if (dev->attached && dev->use_count == 0 && dev->open) {
		int rc = dev->open(dev);
		if (rc < 0) {
			module_put(dev->driver->module);
			module_put(THIS_MODULE);
			mutex_unlock(&dev->mutex);
			return rc;
=======
	const unsigned int minor = iminor(inode);
	struct comedi_file *cfp;
	struct comedi_device *dev = comedi_dev_get_from_minor(minor);
	int rc;

	if (!dev) {
		pr_debug("invalid minor number\n");
		return -ENODEV;
	}

	cfp = kzalloc(sizeof(*cfp), GFP_KERNEL);
	if (!cfp) {
		comedi_dev_put(dev);
		return -ENOMEM;
	}

	cfp->dev = dev;

	mutex_lock(&dev->mutex);
	if (!dev->attached && !capable(CAP_SYS_ADMIN)) {
		dev_dbg(dev->class_dev, "not attached and not CAP_SYS_ADMIN\n");
		rc = -ENODEV;
		goto out;
	}
	if (dev->attached && dev->use_count == 0) {
		if (!try_module_get(dev->driver->module)) {
			rc = -ENXIO;
			goto out;
		}
		if (dev->open) {
			rc = dev->open(dev);
			if (rc < 0) {
				module_put(dev->driver->module);
				goto out;
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	dev->use_count++;
<<<<<<< HEAD

	mutex_unlock(&dev->mutex);

	return 0;
=======
	file->private_data = cfp;
	comedi_file_reset(file);
	rc = 0;

out:
	mutex_unlock(&dev->mutex);
	if (rc) {
		comedi_dev_put(dev);
		kfree(cfp);
	}
	return rc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int comedi_fasync(int fd, struct file *file, int on)
{
<<<<<<< HEAD
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = comedi_dev_from_minor(minor);

	if (!dev)
		return -ENODEV;
=======
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return fasync_helper(fd, file, on, &dev->async_queue);
}

static int comedi_close(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	const unsigned minor = iminor(inode);
	struct comedi_device *dev = comedi_dev_from_minor(minor);
	struct comedi_subdevice *s = NULL;
	int i;

	if (!dev)
		return -ENODEV;

=======
	struct comedi_file *cfp = file->private_data;
	struct comedi_device *dev = cfp->dev;
	struct comedi_subdevice *s = NULL;
	int i;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&dev->mutex);

	if (dev->subdevices) {
		for (i = 0; i < dev->n_subdevices; i++) {
			s = &dev->subdevices[i];

			if (s->busy == file)
				do_cancel(dev, s);
			if (s->lock == file)
				s->lock = NULL;
		}
	}
<<<<<<< HEAD
	if (dev->attached && dev->use_count == 1 && dev->close)
		dev->close(dev);

	module_put(THIS_MODULE);
	if (dev->attached)
		module_put(dev->driver->module);
=======
	if (dev->attached && dev->use_count == 1) {
		if (dev->close)
			dev->close(dev);
		module_put(dev->driver->module);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev->use_count--;

	mutex_unlock(&dev->mutex);
<<<<<<< HEAD

	if (file->f_flags & FASYNC)
		comedi_fasync(-1, file, 0);
=======
	comedi_dev_put(dev);
	kfree(cfp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static const struct file_operations comedi_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = comedi_unlocked_ioctl,
	.compat_ioctl = comedi_compat_ioctl,
	.open = comedi_open,
	.release = comedi_close,
	.read = comedi_read,
	.write = comedi_write,
	.mmap = comedi_mmap,
	.poll = comedi_poll,
	.fasync = comedi_fasync,
	.llseek = noop_llseek,
};

<<<<<<< HEAD
void comedi_error(const struct comedi_device *dev, const char *s)
{
	dev_err(dev->class_dev, "%s: %s\n", dev->driver->driver_name, s);
}
EXPORT_SYMBOL_GPL(comedi_error);

void comedi_event(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned runflags = 0;
	unsigned runflags_mask = 0;

	/* DPRINTK("comedi_event 0x%x\n",mask); */

	if (!comedi_is_subdevice_running(s))
		return;

	if (s->
	    async->events & (COMEDI_CB_EOA | COMEDI_CB_ERROR |
			     COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_RUNNING;
	}
	/* remember if an error event has occurred, so an error
	 * can be returned the next time the user does a read() */
	if (s->async->events & (COMEDI_CB_ERROR | COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_ERROR;
		runflags |= SRF_ERROR;
	}
	if (runflags_mask) {
		/*sets SRF_ERROR and SRF_RUNNING together atomically */
		comedi_set_subdevice_runflags(s, runflags_mask, runflags);
	}

	if (async->cb_mask & s->async->events) {
		if (comedi_get_subdevice_runflags(s) & SRF_USER) {
			wake_up_interruptible(&async->wait_head);
			if (s->subdev_flags & SDF_CMD_READ)
				kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
			if (s->subdev_flags & SDF_CMD_WRITE)
				kill_fasync(&dev->async_queue, SIGIO, POLL_OUT);
		} else {
			if (async->cb_func)
				async->cb_func(s->async->events, async->cb_arg);
		}
	}
	s->async->events = 0;
=======
/**
 * comedi_event() - Handle events for asynchronous COMEDI command
 * @dev: COMEDI device.
 * @s: COMEDI subdevice.
 * Context: in_interrupt() (usually), @s->spin_lock spin-lock not held.
 *
 * If an asynchronous COMEDI command is active on the subdevice, process
 * any %COMEDI_CB_... event flags that have been set, usually by an
 * interrupt handler.  These may change the run state of the asynchronous
 * command, wake a task, and/or send a %SIGIO signal.
 */
void comedi_event(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned int events;
	int si_code = 0;
	unsigned long flags;

	spin_lock_irqsave(&s->spin_lock, flags);

	events = async->events;
	async->events = 0;
	if (!__comedi_is_subdevice_running(s)) {
		spin_unlock_irqrestore(&s->spin_lock, flags);
		return;
	}

	if (events & COMEDI_CB_CANCEL_MASK)
		__comedi_clear_subdevice_runflags(s, COMEDI_SRF_RUNNING);

	/*
	 * Remember if an error event has occurred, so an error can be
	 * returned the next time the user does a read() or write().
	 */
	if (events & COMEDI_CB_ERROR_MASK)
		__comedi_set_subdevice_runflags(s, COMEDI_SRF_ERROR);

	if (async->cb_mask & events) {
		wake_up_interruptible(&async->wait_head);
		si_code = async->cmd.flags & CMDF_WRITE ? POLL_OUT : POLL_IN;
	}

	spin_unlock_irqrestore(&s->spin_lock, flags);

	if (si_code)
		kill_fasync(&dev->async_queue, SIGIO, si_code);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(comedi_event);

/* Note: the ->mutex is pre-locked on successful return */
struct comedi_device *comedi_alloc_board_minor(struct device *hardware_device)
{
	struct comedi_device *dev;
	struct device *csdev;
<<<<<<< HEAD
	unsigned i;

	dev = kzalloc(sizeof(struct comedi_device), GFP_KERNEL);
	if (dev == NULL)
=======
	unsigned int i;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return ERR_PTR(-ENOMEM);
	comedi_device_init(dev);
	comedi_set_hw_dev(dev, hardware_device);
	mutex_lock(&dev->mutex);
	mutex_lock(&comedi_board_minor_table_lock);
	for (i = hardware_device ? comedi_num_legacy_minors : 0;
	     i < COMEDI_NUM_BOARD_MINORS; ++i) {
<<<<<<< HEAD
		if (comedi_board_minor_table[i] == NULL) {
=======
		if (!comedi_board_minor_table[i]) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			comedi_board_minor_table[i] = dev;
			break;
		}
	}
	mutex_unlock(&comedi_board_minor_table_lock);
	if (i == COMEDI_NUM_BOARD_MINORS) {
		mutex_unlock(&dev->mutex);
		comedi_device_cleanup(dev);
<<<<<<< HEAD
		kfree(dev);
		pr_err("comedi: error: ran out of minor numbers for board device files.\n");
=======
		comedi_dev_put(dev);
		dev_err(hardware_device,
			"ran out of minor numbers for board device files\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return ERR_PTR(-EBUSY);
	}
	dev->minor = i;
	csdev = device_create(comedi_class, hardware_device,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i", i);
	if (!IS_ERR(csdev))
<<<<<<< HEAD
		dev->class_dev = csdev;
=======
		dev->class_dev = get_device(csdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Note: dev->mutex needs to be unlocked by the caller. */
	return dev;
}

<<<<<<< HEAD
static void comedi_free_board_minor(unsigned minor)
{
	BUG_ON(minor >= COMEDI_NUM_BOARD_MINORS);
	comedi_free_board_dev(comedi_clear_board_minor(minor));
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void comedi_release_hardware_device(struct device *hardware_device)
{
	int minor;
	struct comedi_device *dev;

	for (minor = comedi_num_legacy_minors; minor < COMEDI_NUM_BOARD_MINORS;
	     minor++) {
		mutex_lock(&comedi_board_minor_table_lock);
		dev = comedi_board_minor_table[minor];
		if (dev && dev->hw_dev == hardware_device) {
			comedi_board_minor_table[minor] = NULL;
			mutex_unlock(&comedi_board_minor_table_lock);
			comedi_free_board_dev(dev);
			break;
		}
		mutex_unlock(&comedi_board_minor_table_lock);
	}
}

int comedi_alloc_subdevice_minor(struct comedi_subdevice *s)
{
	struct comedi_device *dev = s->device;
	struct device *csdev;
<<<<<<< HEAD
	unsigned i;

	mutex_lock(&comedi_subdevice_minor_table_lock);
	for (i = 0; i < COMEDI_NUM_SUBDEVICE_MINORS; ++i) {
		if (comedi_subdevice_minor_table[i] == NULL) {
=======
	unsigned int i;

	mutex_lock(&comedi_subdevice_minor_table_lock);
	for (i = 0; i < COMEDI_NUM_SUBDEVICE_MINORS; ++i) {
		if (!comedi_subdevice_minor_table[i]) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			comedi_subdevice_minor_table[i] = s;
			break;
		}
	}
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	if (i == COMEDI_NUM_SUBDEVICE_MINORS) {
<<<<<<< HEAD
		pr_err("comedi: error: ran out of minor numbers for subdevice files.\n");
=======
		dev_err(dev->class_dev,
			"ran out of minor numbers for subdevice files\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EBUSY;
	}
	i += COMEDI_NUM_BOARD_MINORS;
	s->minor = i;
	csdev = device_create(comedi_class, dev->class_dev,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i_subd%i",
			      dev->minor, s->index);
	if (!IS_ERR(csdev))
		s->class_dev = csdev;

	return 0;
}

void comedi_free_subdevice_minor(struct comedi_subdevice *s)
{
	unsigned int i;

<<<<<<< HEAD
	if (s == NULL)
		return;
	if (s->minor < 0)
		return;

	BUG_ON(s->minor >= COMEDI_NUM_MINORS);
	BUG_ON(s->minor < COMEDI_NUM_BOARD_MINORS);

=======
	if (!s)
		return;
	if (s->minor < COMEDI_NUM_BOARD_MINORS ||
	    s->minor >= COMEDI_NUM_MINORS)
		return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	i = s->minor - COMEDI_NUM_BOARD_MINORS;
	mutex_lock(&comedi_subdevice_minor_table_lock);
	if (s == comedi_subdevice_minor_table[i])
		comedi_subdevice_minor_table[i] = NULL;
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	if (s->class_dev) {
		device_destroy(comedi_class, MKDEV(COMEDI_MAJOR, s->minor));
		s->class_dev = NULL;
	}
}

static void comedi_cleanup_board_minors(void)
{
<<<<<<< HEAD
	unsigned i;

	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; i++)
		comedi_free_board_minor(i);
=======
	struct comedi_device *dev;
	unsigned int i;

	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; i++) {
		dev = comedi_clear_board_minor(i);
		comedi_free_board_dev(dev);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int __init comedi_init(void)
{
	int i;
	int retval;

<<<<<<< HEAD
	pr_info("comedi: version " COMEDI_RELEASE " - http://www.comedi.org\n");

	if (comedi_num_legacy_minors < 0 ||
	    comedi_num_legacy_minors > COMEDI_NUM_BOARD_MINORS) {
		pr_err("comedi: error: invalid value for module parameter \"comedi_num_legacy_minors\".  Valid values are 0 through %i.\n",
=======
	pr_info("version " COMEDI_RELEASE " - http://www.comedi.org\n");

	if (comedi_num_legacy_minors < 0 ||
	    comedi_num_legacy_minors > COMEDI_NUM_BOARD_MINORS) {
		pr_err("invalid value for module parameter \"comedi_num_legacy_minors\".  Valid values are 0 through %i.\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       COMEDI_NUM_BOARD_MINORS);
		return -EINVAL;
	}

	retval = register_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					COMEDI_NUM_MINORS, "comedi");
	if (retval)
		return -EIO;
	cdev_init(&comedi_cdev, &comedi_fops);
	comedi_cdev.owner = THIS_MODULE;
<<<<<<< HEAD
	kobject_set_name(&comedi_cdev.kobj, "comedi");
=======

	retval = kobject_set_name(&comedi_cdev.kobj, "comedi");
	if (retval) {
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return retval;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (cdev_add(&comedi_cdev, MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS)) {
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return -EIO;
	}
	comedi_class = class_create(THIS_MODULE, "comedi");
	if (IS_ERR(comedi_class)) {
<<<<<<< HEAD
		pr_err("comedi: failed to create class\n");
=======
		pr_err("failed to create class\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cdev_del(&comedi_cdev);
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return PTR_ERR(comedi_class);
	}

<<<<<<< HEAD
	comedi_class->dev_attrs = comedi_dev_attrs;
=======
	comedi_class->dev_groups = comedi_dev_groups;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* create devices files for legacy/manual use */
	for (i = 0; i < comedi_num_legacy_minors; i++) {
		struct comedi_device *dev;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev = comedi_alloc_board_minor(NULL);
		if (IS_ERR(dev)) {
			comedi_cleanup_board_minors();
			class_destroy(comedi_class);
			cdev_del(&comedi_cdev);
			unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
						 COMEDI_NUM_MINORS);
			return PTR_ERR(dev);
<<<<<<< HEAD
		} else {
			/* comedi_alloc_board_minor() locked the mutex */
			mutex_unlock(&dev->mutex);
		}
=======
		}
		/* comedi_alloc_board_minor() locked the mutex */
		mutex_unlock(&dev->mutex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* XXX requires /proc interface */
	comedi_proc_init();

	return 0;
}
module_init(comedi_init);

static void __exit comedi_cleanup(void)
{
<<<<<<< HEAD
	int i;

	comedi_cleanup_board_minors();
	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; ++i)
		BUG_ON(comedi_board_minor_table[i]);
	for (i = 0; i < COMEDI_NUM_SUBDEVICE_MINORS; ++i)
		BUG_ON(comedi_subdevice_minor_table[i]);

=======
	comedi_cleanup_board_minors();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	class_destroy(comedi_class);
	cdev_del(&comedi_cdev);
	unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS);

	comedi_proc_cleanup();
}
module_exit(comedi_cleanup);

MODULE_AUTHOR("http://www.comedi.org");
MODULE_DESCRIPTION("Comedi core module");
MODULE_LICENSE("GPL");
