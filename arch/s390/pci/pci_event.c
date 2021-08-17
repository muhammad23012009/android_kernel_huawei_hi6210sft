/*
 *  Copyright IBM Corp. 2012
 *
 *  Author(s):
 *    Jan Glauber <jang@linux.vnet.ibm.com>
 */

<<<<<<< HEAD
#define COMPONENT "zPCI"
#define pr_fmt(fmt) COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/pci.h>
=======
#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/pci.h>
#include <asm/pci_debug.h>
#include <asm/sclp.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Content Code Description for PCI Function Error */
struct zpci_ccdf_err {
	u32 reserved1;
	u32 fh;				/* function handle */
	u32 fid;			/* function id */
	u32 ett		:  4;		/* expected table type */
	u32 mvn		: 12;		/* MSI vector number */
	u32 dmaas	:  8;		/* DMA address space */
	u32		:  6;
	u32 q		:  1;		/* event qualifier */
	u32 rw		:  1;		/* read/write */
	u64 faddr;			/* failing address */
	u32 reserved3;
	u16 reserved4;
	u16 pec;			/* PCI event code */
} __packed;

/* Content Code Description for PCI Function Availability */
struct zpci_ccdf_avail {
	u32 reserved1;
	u32 fh;				/* function handle */
	u32 fid;			/* function id */
	u32 reserved2;
	u32 reserved3;
	u32 reserved4;
	u32 reserved5;
	u16 reserved6;
	u16 pec;			/* PCI event code */
} __packed;

<<<<<<< HEAD
static void zpci_event_log_err(struct zpci_ccdf_err *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);

	zpci_err("SEI error CCD:\n");
	zpci_err_hex(ccdf, sizeof(*ccdf));
	dev_err(&zdev->pdev->dev, "event code: 0x%x\n", ccdf->pec);
}

static void zpci_event_log_avail(struct zpci_ccdf_avail *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);

	pr_err("%s%s: availability event: fh: 0x%x  fid: 0x%x  event code: 0x%x  reason:",
		(zdev) ? dev_driver_string(&zdev->pdev->dev) : "?",
		(zdev) ? dev_name(&zdev->pdev->dev) : "?",
		ccdf->fh, ccdf->fid, ccdf->pec);
	print_hex_dump(KERN_CONT, "ccdf", DUMP_PREFIX_OFFSET,
		       16, 1, ccdf, sizeof(*ccdf), false);

	switch (ccdf->pec) {
	case 0x0301:
		zpci_enable_device(zdev);
		break;
	case 0x0302:
		clp_add_pci_device(ccdf->fid, ccdf->fh, 0);
		break;
	case 0x0306:
		clp_find_pci_devices();
=======
static void __zpci_event_error(struct zpci_ccdf_err *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);
	struct pci_dev *pdev = NULL;

	zpci_err("error CCDF:\n");
	zpci_err_hex(ccdf, sizeof(*ccdf));

	if (zdev)
		pdev = pci_get_slot(zdev->bus, ZPCI_DEVFN);

	pr_err("%s: Event 0x%x reports an error for PCI function 0x%x\n",
	       pdev ? pci_name(pdev) : "n/a", ccdf->pec, ccdf->fid);

	if (!pdev)
		return;

	pdev->error_state = pci_channel_io_perm_failure;
	pci_dev_put(pdev);
}

void zpci_event_error(void *data)
{
	if (zpci_is_enabled())
		__zpci_event_error(data);
}

static void __zpci_event_availability(struct zpci_ccdf_avail *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);
	struct pci_dev *pdev = NULL;
	int ret;

	if (zdev)
		pdev = pci_get_slot(zdev->bus, ZPCI_DEVFN);

	pr_info("%s: Event 0x%x reconfigured PCI function 0x%x\n",
		pdev ? pci_name(pdev) : "n/a", ccdf->pec, ccdf->fid);
	zpci_err("avail CCDF:\n");
	zpci_err_hex(ccdf, sizeof(*ccdf));

	switch (ccdf->pec) {
	case 0x0301: /* Reserved|Standby -> Configured */
		if (!zdev) {
			ret = clp_add_pci_device(ccdf->fid, ccdf->fh, 0);
			if (ret)
				break;
			zdev = get_zdev_by_fid(ccdf->fid);
		}
		if (!zdev || zdev->state != ZPCI_FN_STATE_STANDBY)
			break;
		zdev->state = ZPCI_FN_STATE_CONFIGURED;
		zdev->fh = ccdf->fh;
		ret = zpci_enable_device(zdev);
		if (ret)
			break;
		pci_lock_rescan_remove();
		pci_rescan_bus(zdev->bus);
		pci_unlock_rescan_remove();
		break;
	case 0x0302: /* Reserved -> Standby */
		if (!zdev)
			clp_add_pci_device(ccdf->fid, ccdf->fh, 0);
		break;
	case 0x0303: /* Deconfiguration requested */
		if (pdev)
			pci_stop_and_remove_bus_device_locked(pdev);

		ret = zpci_disable_device(zdev);
		if (ret)
			break;

		ret = sclp_pci_deconfigure(zdev->fid);
		zpci_dbg(3, "deconf fid:%x, rc:%d\n", zdev->fid, ret);
		if (!ret)
			zdev->state = ZPCI_FN_STATE_STANDBY;

		break;
	case 0x0304: /* Configured -> Standby */
		if (pdev) {
			/* Give the driver a hint that the function is
			 * already unusable. */
			pdev->error_state = pci_channel_io_perm_failure;
			pci_stop_and_remove_bus_device_locked(pdev);
		}

		zdev->fh = ccdf->fh;
		zpci_disable_device(zdev);
		zdev->state = ZPCI_FN_STATE_STANDBY;
		break;
	case 0x0306: /* 0x308 or 0x302 for multiple devices */
		clp_rescan_pci_devices();
		break;
	case 0x0308: /* Standby -> Reserved */
		if (!zdev)
			break;
		pci_stop_root_bus(zdev->bus);
		pci_remove_root_bus(zdev->bus);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	default:
		break;
	}
<<<<<<< HEAD
}

void zpci_event_error(void *data)
{
	struct zpci_ccdf_err *ccdf = data;
	struct zpci_dev *zdev;

	zpci_event_log_err(ccdf);
	zdev = get_zdev_by_fid(ccdf->fid);
	if (!zdev) {
		pr_err("Error event for unknown fid: %x", ccdf->fid);
		return;
	}
=======
	pci_dev_put(pdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void zpci_event_availability(void *data)
{
<<<<<<< HEAD
	zpci_event_log_avail(data);
=======
	if (zpci_is_enabled())
		__zpci_event_availability(data);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
