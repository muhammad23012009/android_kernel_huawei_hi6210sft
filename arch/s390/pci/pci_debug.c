/*
<<<<<<< HEAD
 *  Copyright IBM Corp. 2012
=======
 *  Copyright IBM Corp. 2012,2015
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *  Author(s):
 *    Jan Glauber <jang@linux.vnet.ibm.com>
 */

<<<<<<< HEAD
#define COMPONENT "zPCI"
#define pr_fmt(fmt) COMPONENT ": " fmt
=======
#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/export.h>
#include <linux/pci.h>
#include <asm/debug.h>

#include <asm/pci_dma.h>

static struct dentry *debugfs_root;
debug_info_t *pci_debug_msg_id;
EXPORT_SYMBOL_GPL(pci_debug_msg_id);
debug_info_t *pci_debug_err_id;
EXPORT_SYMBOL_GPL(pci_debug_err_id);

<<<<<<< HEAD
static char *pci_perf_names[] = {
	/* hardware counters */
=======
static char *pci_common_names[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"Load operations",
	"Store operations",
	"Store block operations",
	"Refresh operations",
<<<<<<< HEAD
	"DMA read bytes",
	"DMA write bytes",
	/* software counters */
=======
};

static char *pci_fmt0_names[] = {
	"DMA read bytes",
	"DMA write bytes",
};

static char *pci_fmt1_names[] = {
	"Received bytes",
	"Received packets",
	"Transmitted bytes",
	"Transmitted packets",
};

static char *pci_fmt2_names[] = {
	"Consumed work units",
	"Maximum work units",
};

static char *pci_sw_names[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"Allocated pages",
	"Mapped pages",
	"Unmapped pages",
};

<<<<<<< HEAD
static int pci_perf_show(struct seq_file *m, void *v)
{
	struct zpci_dev *zdev = m->private;
	u64 *stat;
	int i;

	if (!zdev)
		return 0;
	if (!zdev->fmb)
		return seq_printf(m, "FMB statistics disabled\n");
=======
static void pci_fmb_show(struct seq_file *m, char *name[], int length,
			 u64 *data)
{
	int i;

	for (i = 0; i < length; i++, data++)
		seq_printf(m, "%26s:\t%llu\n", name[i], *data);
}

static void pci_sw_counter_show(struct seq_file *m)
{
	struct zpci_dev *zdev = m->private;
	atomic64_t *counter = &zdev->allocated_pages;
	int i;

	for (i = 0; i < ARRAY_SIZE(pci_sw_names); i++, counter++)
		seq_printf(m, "%26s:\t%llu\n", pci_sw_names[i],
			   atomic64_read(counter));
}

static int pci_perf_show(struct seq_file *m, void *v)
{
	struct zpci_dev *zdev = m->private;

	if (!zdev)
		return 0;

	mutex_lock(&zdev->lock);
	if (!zdev->fmb) {
		mutex_unlock(&zdev->lock);
		seq_puts(m, "FMB statistics disabled\n");
		return 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* header */
	seq_printf(m, "FMB @ %p\n", zdev->fmb);
	seq_printf(m, "Update interval: %u ms\n", zdev->fmb_update);
	seq_printf(m, "Samples: %u\n", zdev->fmb->samples);
	seq_printf(m, "Last update TOD: %Lx\n", zdev->fmb->last_update);

<<<<<<< HEAD
	/* hardware counters */
	stat = (u64 *) &zdev->fmb->ld_ops;
	for (i = 0; i < 4; i++)
		seq_printf(m, "%26s:\t%llu\n",
			   pci_perf_names[i], *(stat + i));
	if (zdev->fmb->dma_valid)
		for (i = 4; i < 6; i++)
			seq_printf(m, "%26s:\t%llu\n",
				   pci_perf_names[i], *(stat + i));
	/* software counters */
	for (i = 6; i < ARRAY_SIZE(pci_perf_names); i++)
		seq_printf(m, "%26s:\t%llu\n",
			   pci_perf_names[i],
			   atomic64_read((atomic64_t *) (stat + i)));

=======
	pci_fmb_show(m, pci_common_names, ARRAY_SIZE(pci_common_names),
		     &zdev->fmb->ld_ops);

	switch (zdev->fmb->format) {
	case 0:
		if (!(zdev->fmb->fmt_ind & ZPCI_FMB_DMA_COUNTER_VALID))
			break;
		pci_fmb_show(m, pci_fmt0_names, ARRAY_SIZE(pci_fmt0_names),
			     &zdev->fmb->fmt0.dma_rbytes);
		break;
	case 1:
		pci_fmb_show(m, pci_fmt1_names, ARRAY_SIZE(pci_fmt1_names),
			     &zdev->fmb->fmt1.rx_bytes);
		break;
	case 2:
		pci_fmb_show(m, pci_fmt2_names, ARRAY_SIZE(pci_fmt2_names),
			     &zdev->fmb->fmt2.consumed_work_units);
		break;
	default:
		seq_puts(m, "Unknown format\n");
	}

	pci_sw_counter_show(m);
	mutex_unlock(&zdev->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static ssize_t pci_perf_seq_write(struct file *file, const char __user *ubuf,
				  size_t count, loff_t *off)
{
	struct zpci_dev *zdev = ((struct seq_file *) file->private_data)->private;
	unsigned long val;
	int rc;

	if (!zdev)
		return 0;

	rc = kstrtoul_from_user(ubuf, count, 10, &val);
	if (rc)
		return rc;

<<<<<<< HEAD
	switch (val) {
	case 0:
		rc = zpci_fmb_disable_device(zdev);
		if (rc)
			return rc;
		break;
	case 1:
		rc = zpci_fmb_enable_device(zdev);
		if (rc)
			return rc;
		break;
	}
	return count;
=======
	mutex_lock(&zdev->lock);
	switch (val) {
	case 0:
		rc = zpci_fmb_disable_device(zdev);
		break;
	case 1:
		rc = zpci_fmb_enable_device(zdev);
		break;
	}
	mutex_unlock(&zdev->lock);
	return rc ? rc : count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int pci_perf_seq_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, pci_perf_show,
			   file_inode(filp)->i_private);
}

static const struct file_operations debugfs_pci_perf_fops = {
	.open	 = pci_perf_seq_open,
	.read	 = seq_read,
	.write	 = pci_perf_seq_write,
	.llseek  = seq_lseek,
	.release = single_release,
};

<<<<<<< HEAD
static int pci_debug_show(struct seq_file *m, void *v)
{
	struct zpci_dev *zdev = m->private;

	zpci_debug_info(zdev, m);
	return 0;
}

static int pci_debug_seq_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, pci_debug_show,
			   file_inode(filp)->i_private);
}

static const struct file_operations debugfs_pci_debug_fops = {
	.open	 = pci_debug_seq_open,
	.read	 = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

void zpci_debug_init_device(struct zpci_dev *zdev)
{
	zdev->debugfs_dev = debugfs_create_dir(dev_name(&zdev->pdev->dev),
					       debugfs_root);
=======
void zpci_debug_init_device(struct zpci_dev *zdev, const char *name)
{
	zdev->debugfs_dev = debugfs_create_dir(name, debugfs_root);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (IS_ERR(zdev->debugfs_dev))
		zdev->debugfs_dev = NULL;

	zdev->debugfs_perf = debugfs_create_file("statistics",
				S_IFREG | S_IRUGO | S_IWUSR,
				zdev->debugfs_dev, zdev,
				&debugfs_pci_perf_fops);
	if (IS_ERR(zdev->debugfs_perf))
		zdev->debugfs_perf = NULL;
<<<<<<< HEAD

	zdev->debugfs_debug = debugfs_create_file("debug",
				S_IFREG | S_IRUGO | S_IWUSR,
				zdev->debugfs_dev, zdev,
				&debugfs_pci_debug_fops);
	if (IS_ERR(zdev->debugfs_debug))
		zdev->debugfs_debug = NULL;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void zpci_debug_exit_device(struct zpci_dev *zdev)
{
	debugfs_remove(zdev->debugfs_perf);
<<<<<<< HEAD
	debugfs_remove(zdev->debugfs_debug);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	debugfs_remove(zdev->debugfs_dev);
}

int __init zpci_debug_init(void)
{
	/* event trace buffer */
<<<<<<< HEAD
	pci_debug_msg_id = debug_register("pci_msg", 16, 1, 16 * sizeof(long));
=======
	pci_debug_msg_id = debug_register("pci_msg", 8, 1, 8 * sizeof(long));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!pci_debug_msg_id)
		return -EINVAL;
	debug_register_view(pci_debug_msg_id, &debug_sprintf_view);
	debug_set_level(pci_debug_msg_id, 3);

	/* error log */
	pci_debug_err_id = debug_register("pci_error", 2, 1, 16);
	if (!pci_debug_err_id)
		return -EINVAL;
	debug_register_view(pci_debug_err_id, &debug_hex_ascii_view);
	debug_set_level(pci_debug_err_id, 6);

	debugfs_root = debugfs_create_dir("pci", NULL);
	return 0;
}

void zpci_debug_exit(void)
{
<<<<<<< HEAD
	if (pci_debug_msg_id)
		debug_unregister(pci_debug_msg_id);
	if (pci_debug_err_id)
		debug_unregister(pci_debug_err_id);

=======
	debug_unregister(pci_debug_msg_id);
	debug_unregister(pci_debug_err_id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	debugfs_remove(debugfs_root);
}
