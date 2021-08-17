/*
 * USB Attached SCSI
 * Note that this is not the same as the USB Mass Storage driver
 *
<<<<<<< HEAD
=======
 * Copyright Hans de Goede <hdegoede@redhat.com> for Red Hat, Inc. 2013 - 2016
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright Matthew Wilcox for Intel Corp, 2010
 * Copyright Sarah Sharp for Intel Corp, 2010
 *
 * Distributed under the terms of the GNU GPL, version two.
 */

#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/usb.h>
<<<<<<< HEAD
=======
#include <linux/usb_usual.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/usb/hcd.h>
#include <linux/usb/storage.h>
#include <linux/usb/uas.h>

#include <scsi/scsi.h>
<<<<<<< HEAD
=======
#include <scsi/scsi_eh.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <scsi/scsi_dbg.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_tcq.h>

<<<<<<< HEAD
/*
 * The r00-r01c specs define this version of the SENSE IU data structure.
 * It's still in use by several different firmware releases.
 */
struct sense_iu_old {
	__u8 iu_id;
	__u8 rsvd1;
	__be16 tag;
	__be16 len;
	__u8 status;
	__u8 service_response;
	__u8 sense[SCSI_SENSE_BUFFERSIZE];
};
=======
#include "uas-detect.h"
#include "scsiglue.h"

#define MAX_CMNDS 256
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct uas_dev_info {
	struct usb_interface *intf;
	struct usb_device *udev;
	struct usb_anchor cmd_urbs;
	struct usb_anchor sense_urbs;
	struct usb_anchor data_urbs;
<<<<<<< HEAD
	int qdepth, resetting;
	struct response_ui response;
	unsigned cmd_pipe, status_pipe, data_in_pipe, data_out_pipe;
	unsigned use_streams:1;
	unsigned uas_sense_old:1;
	struct scsi_cmnd *cmnd;
	spinlock_t lock;
};

enum {
	SUBMIT_STATUS_URB	= (1 << 1),
	ALLOC_DATA_IN_URB	= (1 << 2),
	SUBMIT_DATA_IN_URB	= (1 << 3),
	ALLOC_DATA_OUT_URB	= (1 << 4),
	SUBMIT_DATA_OUT_URB	= (1 << 5),
	ALLOC_CMD_URB		= (1 << 6),
	SUBMIT_CMD_URB		= (1 << 7),
	COMMAND_INFLIGHT        = (1 << 8),
	DATA_IN_URB_INFLIGHT    = (1 << 9),
	DATA_OUT_URB_INFLIGHT   = (1 << 10),
	COMMAND_COMPLETED       = (1 << 11),
	COMMAND_ABORTED         = (1 << 12),
	UNLINK_DATA_URBS        = (1 << 13),
	IS_IN_WORK_LIST         = (1 << 14),
=======
	unsigned long flags;
	int qdepth, resetting;
	unsigned cmd_pipe, status_pipe, data_in_pipe, data_out_pipe;
	unsigned use_streams:1;
	unsigned shutdown:1;
	struct scsi_cmnd *cmnd[MAX_CMNDS];
	spinlock_t lock;
	struct work_struct work;
	struct work_struct scan_work;      /* for async scanning */
};

enum {
	SUBMIT_STATUS_URB	= BIT(1),
	ALLOC_DATA_IN_URB	= BIT(2),
	SUBMIT_DATA_IN_URB	= BIT(3),
	ALLOC_DATA_OUT_URB	= BIT(4),
	SUBMIT_DATA_OUT_URB	= BIT(5),
	ALLOC_CMD_URB		= BIT(6),
	SUBMIT_CMD_URB		= BIT(7),
	COMMAND_INFLIGHT        = BIT(8),
	DATA_IN_URB_INFLIGHT    = BIT(9),
	DATA_OUT_URB_INFLIGHT   = BIT(10),
	COMMAND_ABORTED         = BIT(11),
	IS_IN_WORK_LIST         = BIT(12),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* Overrides scsi_pointer */
struct uas_cmd_info {
	unsigned int state;
<<<<<<< HEAD
	unsigned int stream;
	struct urb *cmd_urb;
	struct urb *data_in_urb;
	struct urb *data_out_urb;
	struct list_head list;
=======
	unsigned int uas_tag;
	struct urb *cmd_urb;
	struct urb *data_in_urb;
	struct urb *data_out_urb;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* I hate forward declarations, but I actually have a loop */
static int uas_submit_urbs(struct scsi_cmnd *cmnd,
<<<<<<< HEAD
				struct uas_dev_info *devinfo, gfp_t gfp);
static void uas_do_work(struct work_struct *work);
static int uas_try_complete(struct scsi_cmnd *cmnd, const char *caller);

static DECLARE_WORK(uas_work, uas_do_work);
static DEFINE_SPINLOCK(uas_work_lock);
static LIST_HEAD(uas_work_list);

static void uas_unlink_data_urbs(struct uas_dev_info *devinfo,
				 struct uas_cmd_info *cmdinfo)
{
	unsigned long flags;

	/*
	 * The UNLINK_DATA_URBS flag makes sure uas_try_complete
	 * (called by urb completion) doesn't release cmdinfo
	 * underneath us.
	 */
	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state |= UNLINK_DATA_URBS;
	spin_unlock_irqrestore(&devinfo->lock, flags);

	if (cmdinfo->data_in_urb)
		usb_unlink_urb(cmdinfo->data_in_urb);
	if (cmdinfo->data_out_urb)
		usb_unlink_urb(cmdinfo->data_out_urb);

	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state &= ~UNLINK_DATA_URBS;
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static void uas_do_work(struct work_struct *work)
{
	struct uas_cmd_info *cmdinfo;
	struct uas_cmd_info *temp;
	struct list_head list;
	unsigned long flags;
	int err;

	spin_lock_irq(&uas_work_lock);
	list_replace_init(&uas_work_list, &list);
	spin_unlock_irq(&uas_work_lock);

	list_for_each_entry_safe(cmdinfo, temp, &list, list) {
		struct scsi_pointer *scp = (void *)cmdinfo;
		struct scsi_cmnd *cmnd = container_of(scp,
							struct scsi_cmnd, SCp);
		struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
		spin_lock_irqsave(&devinfo->lock, flags);
		err = uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_ATOMIC);
		if (!err)
			cmdinfo->state &= ~IS_IN_WORK_LIST;
		spin_unlock_irqrestore(&devinfo->lock, flags);
		if (err) {
			list_del(&cmdinfo->list);
			spin_lock_irq(&uas_work_lock);
			list_add_tail(&cmdinfo->list, &uas_work_list);
			spin_unlock_irq(&uas_work_lock);
			schedule_work(&uas_work);
		}
	}
}

static void uas_abort_work(struct uas_dev_info *devinfo)
{
	struct uas_cmd_info *cmdinfo;
	struct uas_cmd_info *temp;
	struct list_head list;
	unsigned long flags;

	spin_lock_irq(&uas_work_lock);
	list_replace_init(&uas_work_list, &list);
	spin_unlock_irq(&uas_work_lock);

	spin_lock_irqsave(&devinfo->lock, flags);
	list_for_each_entry_safe(cmdinfo, temp, &list, list) {
		struct scsi_pointer *scp = (void *)cmdinfo;
		struct scsi_cmnd *cmnd = container_of(scp,
							struct scsi_cmnd, SCp);
		struct uas_dev_info *di = (void *)cmnd->device->hostdata;

		if (di == devinfo) {
			cmdinfo->state |= COMMAND_ABORTED;
			cmdinfo->state &= ~IS_IN_WORK_LIST;
			if (devinfo->resetting) {
				/* uas_stat_cmplt() will not do that
				 * when a device reset is in
				 * progress */
				cmdinfo->state &= ~COMMAND_INFLIGHT;
			}
			uas_try_complete(cmnd, __func__);
		} else {
			/* not our uas device, relink into list */
			list_del(&cmdinfo->list);
			spin_lock_irq(&uas_work_lock);
			list_add_tail(&cmdinfo->list, &uas_work_list);
			spin_unlock_irq(&uas_work_lock);
		}
=======
				struct uas_dev_info *devinfo);
static void uas_do_work(struct work_struct *work);
static int uas_try_complete(struct scsi_cmnd *cmnd, const char *caller);
static void uas_free_streams(struct uas_dev_info *devinfo);
static void uas_log_cmd_state(struct scsi_cmnd *cmnd, const char *prefix,
				int status);

/*
 * This driver needs its own workqueue, as we need to control memory allocation.
 *
 * In the course of error handling and power management uas_wait_for_pending_cmnds()
 * needs to flush pending work items. In these contexts we cannot allocate memory
 * by doing block IO as we would deadlock. For the same reason we cannot wait
 * for anything allocating memory not heeding these constraints.
 *
 * So we have to control all work items that can be on the workqueue we flush.
 * Hence we cannot share a queue and need our own.
 */
static struct workqueue_struct *workqueue;

static void uas_do_work(struct work_struct *work)
{
	struct uas_dev_info *devinfo =
		container_of(work, struct uas_dev_info, work);
	struct uas_cmd_info *cmdinfo;
	struct scsi_cmnd *cmnd;
	unsigned long flags;
	int i, err;

	spin_lock_irqsave(&devinfo->lock, flags);

	if (devinfo->resetting)
		goto out;

	for (i = 0; i < devinfo->qdepth; i++) {
		if (!devinfo->cmnd[i])
			continue;

		cmnd = devinfo->cmnd[i];
		cmdinfo = (void *)&cmnd->SCp;

		if (!(cmdinfo->state & IS_IN_WORK_LIST))
			continue;

		err = uas_submit_urbs(cmnd, cmnd->device->hostdata);
		if (!err)
			cmdinfo->state &= ~IS_IN_WORK_LIST;
		else
			queue_work(workqueue, &devinfo->work);
	}
out:
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static void uas_scan_work(struct work_struct *work)
{
	struct uas_dev_info *devinfo =
		container_of(work, struct uas_dev_info, scan_work);
	struct Scsi_Host *shost = usb_get_intfdata(devinfo->intf);

	dev_dbg(&devinfo->intf->dev, "starting scan\n");
	scsi_scan_host(shost);
	dev_dbg(&devinfo->intf->dev, "scan complete\n");
}

static void uas_add_work(struct uas_cmd_info *cmdinfo)
{
	struct scsi_pointer *scp = (void *)cmdinfo;
	struct scsi_cmnd *cmnd = container_of(scp, struct scsi_cmnd, SCp);
	struct uas_dev_info *devinfo = cmnd->device->hostdata;

	lockdep_assert_held(&devinfo->lock);
	cmdinfo->state |= IS_IN_WORK_LIST;
	queue_work(workqueue, &devinfo->work);
}

static void uas_zap_pending(struct uas_dev_info *devinfo, int result)
{
	struct uas_cmd_info *cmdinfo;
	struct scsi_cmnd *cmnd;
	unsigned long flags;
	int i, err;

	spin_lock_irqsave(&devinfo->lock, flags);
	for (i = 0; i < devinfo->qdepth; i++) {
		if (!devinfo->cmnd[i])
			continue;

		cmnd = devinfo->cmnd[i];
		cmdinfo = (void *)&cmnd->SCp;
		uas_log_cmd_state(cmnd, __func__, 0);
		/* Sense urbs were killed, clear COMMAND_INFLIGHT manually */
		cmdinfo->state &= ~COMMAND_INFLIGHT;
		cmnd->result = result << 16;
		err = uas_try_complete(cmnd, __func__);
		WARN_ON(err != 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static void uas_sense(struct urb *urb, struct scsi_cmnd *cmnd)
{
	struct sense_iu *sense_iu = urb->transfer_buffer;
	struct scsi_device *sdev = cmnd->device;

	if (urb->actual_length > 16) {
		unsigned len = be16_to_cpup(&sense_iu->len);
		if (len + 16 != urb->actual_length) {
			int newlen = min(len + 16, urb->actual_length) - 16;
			if (newlen < 0)
				newlen = 0;
			sdev_printk(KERN_INFO, sdev, "%s: urb length %d "
				"disagrees with IU sense data length %d, "
				"using %d bytes of sense data\n", __func__,
					urb->actual_length, len, newlen);
			len = newlen;
		}
		memcpy(cmnd->sense_buffer, sense_iu->sense, len);
	}

	cmnd->result = sense_iu->status;
}

<<<<<<< HEAD
static void uas_sense_old(struct urb *urb, struct scsi_cmnd *cmnd)
{
	struct sense_iu_old *sense_iu = urb->transfer_buffer;
	struct scsi_device *sdev = cmnd->device;

	if (urb->actual_length > 8) {
		unsigned len = be16_to_cpup(&sense_iu->len) - 2;
		if (len + 8 != urb->actual_length) {
			int newlen = min(len + 8, urb->actual_length) - 8;
			if (newlen < 0)
				newlen = 0;
			sdev_printk(KERN_INFO, sdev, "%s: urb length %d "
				"disagrees with IU sense data length %d, "
				"using %d bytes of sense data\n", __func__,
					urb->actual_length, len, newlen);
			len = newlen;
		}
		memcpy(cmnd->sense_buffer, sense_iu->sense, len);
	}

	cmnd->result = sense_iu->status;
}

static void uas_log_cmd_state(struct scsi_cmnd *cmnd, const char *caller)
{
	struct uas_cmd_info *ci = (void *)&cmnd->SCp;

	scmd_printk(KERN_INFO, cmnd, "%s %p tag %d, inflight:"
		    "%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
		    caller, cmnd, cmnd->request->tag,
=======
static void uas_log_cmd_state(struct scsi_cmnd *cmnd, const char *prefix,
			      int status)
{
	struct uas_cmd_info *ci = (void *)&cmnd->SCp;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;

	if (status == -ENODEV) /* too late */
		return;

	scmd_printk(KERN_INFO, cmnd,
		    "%s %d uas-tag %d inflight:%s%s%s%s%s%s%s%s%s%s%s%s ",
		    prefix, status, cmdinfo->uas_tag,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    (ci->state & SUBMIT_STATUS_URB)     ? " s-st"  : "",
		    (ci->state & ALLOC_DATA_IN_URB)     ? " a-in"  : "",
		    (ci->state & SUBMIT_DATA_IN_URB)    ? " s-in"  : "",
		    (ci->state & ALLOC_DATA_OUT_URB)    ? " a-out" : "",
		    (ci->state & SUBMIT_DATA_OUT_URB)   ? " s-out" : "",
		    (ci->state & ALLOC_CMD_URB)         ? " a-cmd" : "",
		    (ci->state & SUBMIT_CMD_URB)        ? " s-cmd" : "",
		    (ci->state & COMMAND_INFLIGHT)      ? " CMD"   : "",
		    (ci->state & DATA_IN_URB_INFLIGHT)  ? " IN"    : "",
		    (ci->state & DATA_OUT_URB_INFLIGHT) ? " OUT"   : "",
<<<<<<< HEAD
		    (ci->state & COMMAND_COMPLETED)     ? " done"  : "",
		    (ci->state & COMMAND_ABORTED)       ? " abort" : "",
		    (ci->state & UNLINK_DATA_URBS)      ? " unlink": "",
		    (ci->state & IS_IN_WORK_LIST)       ? " work"  : "");
=======
		    (ci->state & COMMAND_ABORTED)       ? " abort" : "",
		    (ci->state & IS_IN_WORK_LIST)       ? " work"  : "");
	scsi_print_command(cmnd);
}

static void uas_free_unsubmitted_urbs(struct scsi_cmnd *cmnd)
{
	struct uas_cmd_info *cmdinfo;

	if (!cmnd)
		return;

	cmdinfo = (void *)&cmnd->SCp;

	if (cmdinfo->state & SUBMIT_CMD_URB)
		usb_free_urb(cmdinfo->cmd_urb);

	/* data urbs may have never gotten their submit flag set */
	if (!(cmdinfo->state & DATA_IN_URB_INFLIGHT))
		usb_free_urb(cmdinfo->data_in_urb);
	if (!(cmdinfo->state & DATA_OUT_URB_INFLIGHT))
		usb_free_urb(cmdinfo->data_out_urb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int uas_try_complete(struct scsi_cmnd *cmnd, const char *caller)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;

<<<<<<< HEAD
	WARN_ON(!spin_is_locked(&devinfo->lock));
	if (cmdinfo->state & (COMMAND_INFLIGHT |
			      DATA_IN_URB_INFLIGHT |
			      DATA_OUT_URB_INFLIGHT |
			      UNLINK_DATA_URBS))
		return -EBUSY;
	BUG_ON(cmdinfo->state & COMMAND_COMPLETED);
	cmdinfo->state |= COMMAND_COMPLETED;
	usb_free_urb(cmdinfo->data_in_urb);
	usb_free_urb(cmdinfo->data_out_urb);
	if (cmdinfo->state & COMMAND_ABORTED) {
		scmd_printk(KERN_INFO, cmnd, "abort completed\n");
		cmnd->result = DID_ABORT << 16;
	}
=======
	lockdep_assert_held(&devinfo->lock);
	if (cmdinfo->state & (COMMAND_INFLIGHT |
			      DATA_IN_URB_INFLIGHT |
			      DATA_OUT_URB_INFLIGHT |
			      COMMAND_ABORTED))
		return -EBUSY;
	devinfo->cmnd[cmdinfo->uas_tag - 1] = NULL;
	uas_free_unsubmitted_urbs(cmnd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cmnd->scsi_done(cmnd);
	return 0;
}

static void uas_xfer_data(struct urb *urb, struct scsi_cmnd *cmnd,
			  unsigned direction)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	int err;

	cmdinfo->state |= direction | SUBMIT_STATUS_URB;
<<<<<<< HEAD
	err = uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_ATOMIC);
	if (err) {
		spin_lock(&uas_work_lock);
		list_add_tail(&cmdinfo->list, &uas_work_list);
		cmdinfo->state |= IS_IN_WORK_LIST;
		spin_unlock(&uas_work_lock);
		schedule_work(&uas_work);
	}
=======
	err = uas_submit_urbs(cmnd, cmnd->device->hostdata);
	if (err) {
		uas_add_work(cmdinfo);
	}
}

static bool uas_evaluate_response_iu(struct response_iu *riu, struct scsi_cmnd *cmnd)
{
	u8 response_code = riu->response_code;

	switch (response_code) {
	case RC_INCORRECT_LUN:
		cmnd->result = DID_BAD_TARGET << 16;
		break;
	case RC_TMF_SUCCEEDED:
		cmnd->result = DID_OK << 16;
		break;
	case RC_TMF_NOT_SUPPORTED:
		cmnd->result = DID_TARGET_FAILURE << 16;
		break;
	default:
		uas_log_cmd_state(cmnd, "response iu", response_code);
		cmnd->result = DID_ERROR << 16;
		break;
	}

	return response_code == RC_TMF_SUCCEEDED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void uas_stat_cmplt(struct urb *urb)
{
	struct iu *iu = urb->transfer_buffer;
	struct Scsi_Host *shost = urb->context;
<<<<<<< HEAD
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	struct scsi_cmnd *cmnd;
	struct uas_cmd_info *cmdinfo;
	unsigned long flags;
	u16 tag;

	if (urb->status) {
		dev_err(&urb->dev->dev, "URB BAD STATUS %d\n", urb->status);
		usb_free_urb(urb);
		return;
	}

	if (devinfo->resetting) {
		usb_free_urb(urb);
		return;
	}

	spin_lock_irqsave(&devinfo->lock, flags);
	tag = be16_to_cpup(&iu->tag) - 1;
	if (tag == 0)
		cmnd = devinfo->cmnd;
	else
		cmnd = scsi_host_find_tag(shost, tag - 1);

	if (!cmnd) {
		if (iu->iu_id == IU_ID_RESPONSE) {
			/* store results for uas_eh_task_mgmt() */
			memcpy(&devinfo->response, iu, sizeof(devinfo->response));
		}
		usb_free_urb(urb);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return;
	}

	cmdinfo = (void *)&cmnd->SCp;
	switch (iu->iu_id) {
	case IU_ID_STATUS:
		if (devinfo->cmnd == cmnd)
			devinfo->cmnd = NULL;

		if (urb->actual_length < 16)
			devinfo->uas_sense_old = 1;
		if (devinfo->uas_sense_old)
			uas_sense_old(urb, cmnd);
		else
			uas_sense(urb, cmnd);
		if (cmnd->result != 0) {
			/* cancel data transfers on error */
			spin_unlock_irqrestore(&devinfo->lock, flags);
			uas_unlink_data_urbs(devinfo, cmdinfo);
			spin_lock_irqsave(&devinfo->lock, flags);
=======
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;
	struct urb *data_in_urb = NULL;
	struct urb *data_out_urb = NULL;
	struct scsi_cmnd *cmnd;
	struct uas_cmd_info *cmdinfo;
	unsigned long flags;
	unsigned int idx;
	int status = urb->status;
	bool success;

	spin_lock_irqsave(&devinfo->lock, flags);

	if (devinfo->resetting)
		goto out;

	if (status) {
		if (status != -ENOENT && status != -ECONNRESET && status != -ESHUTDOWN)
			dev_err(&urb->dev->dev, "stat urb: status %d\n", status);
		goto out;
	}

	idx = be16_to_cpup(&iu->tag) - 1;
	if (idx >= MAX_CMNDS || !devinfo->cmnd[idx]) {
		dev_err(&urb->dev->dev,
			"stat urb: no pending cmd for uas-tag %d\n", idx + 1);
		goto out;
	}

	cmnd = devinfo->cmnd[idx];
	cmdinfo = (void *)&cmnd->SCp;

	if (!(cmdinfo->state & COMMAND_INFLIGHT)) {
		uas_log_cmd_state(cmnd, "unexpected status cmplt", 0);
		goto out;
	}

	switch (iu->iu_id) {
	case IU_ID_STATUS:
		uas_sense(urb, cmnd);
		if (cmnd->result != 0) {
			/* cancel data transfers on error */
			data_in_urb = usb_get_urb(cmdinfo->data_in_urb);
			data_out_urb = usb_get_urb(cmdinfo->data_out_urb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		cmdinfo->state &= ~COMMAND_INFLIGHT;
		uas_try_complete(cmnd, __func__);
		break;
	case IU_ID_READ_READY:
<<<<<<< HEAD
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_IN_URB);
		break;
	case IU_ID_WRITE_READY:
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_OUT_URB);
		break;
	default:
		scmd_printk(KERN_ERR, cmnd,
			"Bogus IU (%d) received on status pipe\n", iu->iu_id);
	}
	usb_free_urb(urb);
	spin_unlock_irqrestore(&devinfo->lock, flags);
=======
		if (!cmdinfo->data_in_urb ||
				(cmdinfo->state & DATA_IN_URB_INFLIGHT)) {
			uas_log_cmd_state(cmnd, "unexpected read rdy", 0);
			break;
		}
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_IN_URB);
		break;
	case IU_ID_WRITE_READY:
		if (!cmdinfo->data_out_urb ||
				(cmdinfo->state & DATA_OUT_URB_INFLIGHT)) {
			uas_log_cmd_state(cmnd, "unexpected write rdy", 0);
			break;
		}
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_OUT_URB);
		break;
	case IU_ID_RESPONSE:
		cmdinfo->state &= ~COMMAND_INFLIGHT;
		success = uas_evaluate_response_iu((struct response_iu *)iu, cmnd);
		if (!success) {
			/* Error, cancel data transfers */
			data_in_urb = usb_get_urb(cmdinfo->data_in_urb);
			data_out_urb = usb_get_urb(cmdinfo->data_out_urb);
		}
		uas_try_complete(cmnd, __func__);
		break;
	default:
		uas_log_cmd_state(cmnd, "bogus IU", iu->iu_id);
	}
out:
	usb_free_urb(urb);
	spin_unlock_irqrestore(&devinfo->lock, flags);

	/* Unlinking of data urbs must be done without holding the lock */
	if (data_in_urb) {
		usb_unlink_urb(data_in_urb);
		usb_put_urb(data_in_urb);
	}
	if (data_out_urb) {
		usb_unlink_urb(data_out_urb);
		usb_put_urb(data_out_urb);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void uas_data_cmplt(struct urb *urb)
{
	struct scsi_cmnd *cmnd = urb->context;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
	struct scsi_data_buffer *sdb = NULL;
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&devinfo->lock, flags);
	if (cmdinfo->data_in_urb == urb) {
		sdb = scsi_in(cmnd);
		cmdinfo->state &= ~DATA_IN_URB_INFLIGHT;
	} else if (cmdinfo->data_out_urb == urb) {
		sdb = scsi_out(cmnd);
		cmdinfo->state &= ~DATA_OUT_URB_INFLIGHT;
	}
	BUG_ON(sdb == NULL);
	if (urb->status) {
=======
	int status = urb->status;

	spin_lock_irqsave(&devinfo->lock, flags);

	if (cmdinfo->data_in_urb == urb) {
		sdb = scsi_in(cmnd);
		cmdinfo->state &= ~DATA_IN_URB_INFLIGHT;
		cmdinfo->data_in_urb = NULL;
	} else if (cmdinfo->data_out_urb == urb) {
		sdb = scsi_out(cmnd);
		cmdinfo->state &= ~DATA_OUT_URB_INFLIGHT;
		cmdinfo->data_out_urb = NULL;
	}
	if (sdb == NULL) {
		WARN_ON_ONCE(1);
		goto out;
	}

	if (devinfo->resetting)
		goto out;

	/* Data urbs should not complete before the cmd urb is submitted */
	if (cmdinfo->state & SUBMIT_CMD_URB) {
		uas_log_cmd_state(cmnd, "unexpected data cmplt", 0);
		goto out;
	}

	if (status) {
		if (status != -ENOENT && status != -ECONNRESET && status != -ESHUTDOWN)
			uas_log_cmd_state(cmnd, "data cmplt err", status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* error: no data transfered */
		sdb->resid = sdb->length;
	} else {
		sdb->resid = sdb->length - urb->actual_length;
	}
	uas_try_complete(cmnd, __func__);
<<<<<<< HEAD
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static struct urb *uas_alloc_data_urb(struct uas_dev_info *devinfo, gfp_t gfp,
				      unsigned int pipe, u16 stream_id,
=======
out:
	usb_free_urb(urb);
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static void uas_cmd_cmplt(struct urb *urb)
{
	if (urb->status)
		dev_err(&urb->dev->dev, "cmd cmplt err %d\n", urb->status);

	usb_free_urb(urb);
}

static struct urb *uas_alloc_data_urb(struct uas_dev_info *devinfo, gfp_t gfp,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				      struct scsi_cmnd *cmnd,
				      enum dma_data_direction dir)
{
	struct usb_device *udev = devinfo->udev;
<<<<<<< HEAD
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct scsi_data_buffer *sdb = (dir == DMA_FROM_DEVICE)
		? scsi_in(cmnd) : scsi_out(cmnd);
=======
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct scsi_data_buffer *sdb = (dir == DMA_FROM_DEVICE)
		? scsi_in(cmnd) : scsi_out(cmnd);
	unsigned int pipe = (dir == DMA_FROM_DEVICE)
		? devinfo->data_in_pipe : devinfo->data_out_pipe;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!urb)
		goto out;
	usb_fill_bulk_urb(urb, udev, pipe, NULL, sdb->length,
			  uas_data_cmplt, cmnd);
	if (devinfo->use_streams)
<<<<<<< HEAD
		urb->stream_id = stream_id;
=======
		urb->stream_id = cmdinfo->uas_tag;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	urb->num_sgs = udev->bus->sg_tablesize ? sdb->table.nents : 0;
	urb->sg = sdb->table.sgl;
 out:
	return urb;
}

static struct urb *uas_alloc_sense_urb(struct uas_dev_info *devinfo, gfp_t gfp,
<<<<<<< HEAD
				       struct Scsi_Host *shost, u16 stream_id)
{
	struct usb_device *udev = devinfo->udev;
=======
				       struct scsi_cmnd *cmnd)
{
	struct usb_device *udev = devinfo->udev;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct sense_iu *iu;

	if (!urb)
		goto out;

	iu = kzalloc(sizeof(*iu), gfp);
	if (!iu)
		goto free;

	usb_fill_bulk_urb(urb, udev, devinfo->status_pipe, iu, sizeof(*iu),
<<<<<<< HEAD
						uas_stat_cmplt, shost);
	urb->stream_id = stream_id;
=======
			  uas_stat_cmplt, cmnd->device->host);
	if (devinfo->use_streams)
		urb->stream_id = cmdinfo->uas_tag;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	urb->transfer_flags |= URB_FREE_BUFFER;
 out:
	return urb;
 free:
	usb_free_urb(urb);
	return NULL;
}

static struct urb *uas_alloc_cmd_urb(struct uas_dev_info *devinfo, gfp_t gfp,
<<<<<<< HEAD
					struct scsi_cmnd *cmnd, u16 stream_id)
{
	struct usb_device *udev = devinfo->udev;
	struct scsi_device *sdev = cmnd->device;
=======
					struct scsi_cmnd *cmnd)
{
	struct usb_device *udev = devinfo->udev;
	struct scsi_device *sdev = cmnd->device;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct command_iu *iu;
	int len;

	if (!urb)
		goto out;

	len = cmnd->cmd_len - 16;
	if (len < 0)
		len = 0;
	len = ALIGN(len, 4);
	iu = kzalloc(sizeof(*iu) + len, gfp);
	if (!iu)
		goto free;

	iu->iu_id = IU_ID_COMMAND;
<<<<<<< HEAD
	if (blk_rq_tagged(cmnd->request))
		iu->tag = cpu_to_be16(cmnd->request->tag + 2);
	else
		iu->tag = cpu_to_be16(1);
=======
	iu->tag = cpu_to_be16(cmdinfo->uas_tag);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	iu->prio_attr = UAS_SIMPLE_TAG;
	iu->len = len;
	int_to_scsilun(sdev->lun, &iu->lun);
	memcpy(iu->cdb, cmnd->cmnd, cmnd->cmd_len);

	usb_fill_bulk_urb(urb, udev, devinfo->cmd_pipe, iu, sizeof(*iu) + len,
<<<<<<< HEAD
							usb_free_urb, NULL);
=======
							uas_cmd_cmplt, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	urb->transfer_flags |= URB_FREE_BUFFER;
 out:
	return urb;
 free:
	usb_free_urb(urb);
	return NULL;
}

<<<<<<< HEAD
static int uas_submit_task_urb(struct scsi_cmnd *cmnd, gfp_t gfp,
			       u8 function, u16 stream_id)
{
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct task_mgmt_iu *iu;
	int err = -ENOMEM;

	if (!urb)
		goto err;

	iu = kzalloc(sizeof(*iu), gfp);
	if (!iu)
		goto err;

	iu->iu_id = IU_ID_TASK_MGMT;
	iu->tag = cpu_to_be16(stream_id);
	int_to_scsilun(cmnd->device->lun, &iu->lun);

	iu->function = function;
	switch (function) {
	case TMF_ABORT_TASK:
		if (blk_rq_tagged(cmnd->request))
			iu->task_tag = cpu_to_be16(cmnd->request->tag + 2);
		else
			iu->task_tag = cpu_to_be16(1);
		break;
	}

	usb_fill_bulk_urb(urb, udev, devinfo->cmd_pipe, iu, sizeof(*iu),
			  usb_free_urb, NULL);
	urb->transfer_flags |= URB_FREE_BUFFER;

	err = usb_submit_urb(urb, gfp);
	if (err)
		goto err;
	usb_anchor_urb(urb, &devinfo->cmd_urbs);

	return 0;

err:
	usb_free_urb(urb);
	return err;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Why should I request the Status IU before sending the Command IU?  Spec
 * says to, but also says the device may receive them in any order.  Seems
 * daft to me.
 */

<<<<<<< HEAD
static int uas_submit_sense_urb(struct Scsi_Host *shost,
				gfp_t gfp, unsigned int stream)
{
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	struct urb *urb;

	urb = uas_alloc_sense_urb(devinfo, gfp, shost, stream);
	if (!urb)
		return SCSI_MLQUEUE_DEVICE_BUSY;
	if (usb_submit_urb(urb, gfp)) {
		shost_printk(KERN_INFO, shost,
			     "sense urb submission failure\n");
		usb_free_urb(urb);
		return SCSI_MLQUEUE_DEVICE_BUSY;
	}
	usb_anchor_urb(urb, &devinfo->sense_urbs);
	return 0;
}

static int uas_submit_urbs(struct scsi_cmnd *cmnd,
			   struct uas_dev_info *devinfo, gfp_t gfp)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	int err;

	WARN_ON(!spin_is_locked(&devinfo->lock));
	if (cmdinfo->state & SUBMIT_STATUS_URB) {
		err = uas_submit_sense_urb(cmnd->device->host, gfp,
					   cmdinfo->stream);
		if (err) {
			return err;
		}
=======
static struct urb *uas_submit_sense_urb(struct scsi_cmnd *cmnd, gfp_t gfp)
{
	struct uas_dev_info *devinfo = cmnd->device->hostdata;
	struct urb *urb;
	int err;

	urb = uas_alloc_sense_urb(devinfo, gfp, cmnd);
	if (!urb)
		return NULL;
	usb_anchor_urb(urb, &devinfo->sense_urbs);
	err = usb_submit_urb(urb, gfp);
	if (err) {
		usb_unanchor_urb(urb);
		uas_log_cmd_state(cmnd, "sense submit err", err);
		usb_free_urb(urb);
		return NULL;
	}
	return urb;
}

static int uas_submit_urbs(struct scsi_cmnd *cmnd,
			   struct uas_dev_info *devinfo)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct urb *urb;
	int err;

	lockdep_assert_held(&devinfo->lock);
	if (cmdinfo->state & SUBMIT_STATUS_URB) {
		urb = uas_submit_sense_urb(cmnd, GFP_ATOMIC);
		if (!urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cmdinfo->state &= ~SUBMIT_STATUS_URB;
	}

	if (cmdinfo->state & ALLOC_DATA_IN_URB) {
<<<<<<< HEAD
		cmdinfo->data_in_urb = uas_alloc_data_urb(devinfo, gfp,
					devinfo->data_in_pipe, cmdinfo->stream,
					cmnd, DMA_FROM_DEVICE);
=======
		cmdinfo->data_in_urb = uas_alloc_data_urb(devinfo, GFP_ATOMIC,
							cmnd, DMA_FROM_DEVICE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!cmdinfo->data_in_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_DATA_IN_URB;
	}

	if (cmdinfo->state & SUBMIT_DATA_IN_URB) {
<<<<<<< HEAD
		if (usb_submit_urb(cmdinfo->data_in_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"data in urb submission failure\n");
=======
		usb_anchor_urb(cmdinfo->data_in_urb, &devinfo->data_urbs);
		err = usb_submit_urb(cmdinfo->data_in_urb, GFP_ATOMIC);
		if (err) {
			usb_unanchor_urb(cmdinfo->data_in_urb);
			uas_log_cmd_state(cmnd, "data in submit err", err);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		cmdinfo->state &= ~SUBMIT_DATA_IN_URB;
		cmdinfo->state |= DATA_IN_URB_INFLIGHT;
<<<<<<< HEAD
		usb_anchor_urb(cmdinfo->data_in_urb, &devinfo->data_urbs);
	}

	if (cmdinfo->state & ALLOC_DATA_OUT_URB) {
		cmdinfo->data_out_urb = uas_alloc_data_urb(devinfo, gfp,
					devinfo->data_out_pipe, cmdinfo->stream,
					cmnd, DMA_TO_DEVICE);
=======
	}

	if (cmdinfo->state & ALLOC_DATA_OUT_URB) {
		cmdinfo->data_out_urb = uas_alloc_data_urb(devinfo, GFP_ATOMIC,
							cmnd, DMA_TO_DEVICE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!cmdinfo->data_out_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_DATA_OUT_URB;
	}

	if (cmdinfo->state & SUBMIT_DATA_OUT_URB) {
<<<<<<< HEAD
		if (usb_submit_urb(cmdinfo->data_out_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"data out urb submission failure\n");
=======
		usb_anchor_urb(cmdinfo->data_out_urb, &devinfo->data_urbs);
		err = usb_submit_urb(cmdinfo->data_out_urb, GFP_ATOMIC);
		if (err) {
			usb_unanchor_urb(cmdinfo->data_out_urb);
			uas_log_cmd_state(cmnd, "data out submit err", err);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		cmdinfo->state &= ~SUBMIT_DATA_OUT_URB;
		cmdinfo->state |= DATA_OUT_URB_INFLIGHT;
<<<<<<< HEAD
		usb_anchor_urb(cmdinfo->data_out_urb, &devinfo->data_urbs);
	}

	if (cmdinfo->state & ALLOC_CMD_URB) {
		cmdinfo->cmd_urb = uas_alloc_cmd_urb(devinfo, gfp, cmnd,
						     cmdinfo->stream);
=======
	}

	if (cmdinfo->state & ALLOC_CMD_URB) {
		cmdinfo->cmd_urb = uas_alloc_cmd_urb(devinfo, GFP_ATOMIC, cmnd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!cmdinfo->cmd_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_CMD_URB;
	}

	if (cmdinfo->state & SUBMIT_CMD_URB) {
<<<<<<< HEAD
		usb_get_urb(cmdinfo->cmd_urb);
		if (usb_submit_urb(cmdinfo->cmd_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"cmd urb submission failure\n");
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		usb_anchor_urb(cmdinfo->cmd_urb, &devinfo->cmd_urbs);
		usb_put_urb(cmdinfo->cmd_urb);
=======
		usb_anchor_urb(cmdinfo->cmd_urb, &devinfo->cmd_urbs);
		err = usb_submit_urb(cmdinfo->cmd_urb, GFP_ATOMIC);
		if (err) {
			usb_unanchor_urb(cmdinfo->cmd_urb);
			uas_log_cmd_state(cmnd, "cmd submit err", err);
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cmdinfo->cmd_urb = NULL;
		cmdinfo->state &= ~SUBMIT_CMD_URB;
		cmdinfo->state |= COMMAND_INFLIGHT;
	}

	return 0;
}

static int uas_queuecommand_lck(struct scsi_cmnd *cmnd,
					void (*done)(struct scsi_cmnd *))
{
	struct scsi_device *sdev = cmnd->device;
	struct uas_dev_info *devinfo = sdev->hostdata;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	unsigned long flags;
<<<<<<< HEAD
	int err;

	BUILD_BUG_ON(sizeof(struct uas_cmd_info) > sizeof(struct scsi_pointer));

	if (devinfo->resetting) {
		cmnd->result = DID_ERROR << 16;
=======
	int idx, err;

	BUILD_BUG_ON(sizeof(struct uas_cmd_info) > sizeof(struct scsi_pointer));

	/* Re-check scsi_block_requests now that we've the host-lock */
	if (cmnd->device->host->host_self_blocked)
		return SCSI_MLQUEUE_DEVICE_BUSY;

	if ((devinfo->flags & US_FL_NO_ATA_1X) &&
			(cmnd->cmnd[0] == ATA_12 || cmnd->cmnd[0] == ATA_16)) {
		memcpy(cmnd->sense_buffer, usb_stor_sense_invalidCDB,
		       sizeof(usb_stor_sense_invalidCDB));
		cmnd->result = SAM_STAT_CHECK_CONDITION;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cmnd->scsi_done(cmnd);
		return 0;
	}

	spin_lock_irqsave(&devinfo->lock, flags);
<<<<<<< HEAD
	if (devinfo->cmnd) {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return SCSI_MLQUEUE_DEVICE_BUSY;
	}

	if (blk_rq_tagged(cmnd->request)) {
		cmdinfo->stream = cmnd->request->tag + 2;
	} else {
		devinfo->cmnd = cmnd;
		cmdinfo->stream = 1;
=======

	if (devinfo->resetting) {
		cmnd->result = DID_ERROR << 16;
		cmnd->scsi_done(cmnd);
		goto zombie;
	}

	/* Find a free uas-tag */
	for (idx = 0; idx < devinfo->qdepth; idx++) {
		if (!devinfo->cmnd[idx])
			break;
	}
	if (idx == devinfo->qdepth) {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return SCSI_MLQUEUE_DEVICE_BUSY;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	cmnd->scsi_done = done;

<<<<<<< HEAD
	cmdinfo->state = SUBMIT_STATUS_URB |
			ALLOC_CMD_URB | SUBMIT_CMD_URB;
=======
	memset(cmdinfo, 0, sizeof(*cmdinfo));
	cmdinfo->uas_tag = idx + 1; /* uas-tag == usb-stream-id, so 1 based */
	cmdinfo->state = SUBMIT_STATUS_URB | ALLOC_CMD_URB | SUBMIT_CMD_URB;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (cmnd->sc_data_direction) {
	case DMA_FROM_DEVICE:
		cmdinfo->state |= ALLOC_DATA_IN_URB | SUBMIT_DATA_IN_URB;
		break;
	case DMA_BIDIRECTIONAL:
		cmdinfo->state |= ALLOC_DATA_IN_URB | SUBMIT_DATA_IN_URB;
	case DMA_TO_DEVICE:
		cmdinfo->state |= ALLOC_DATA_OUT_URB | SUBMIT_DATA_OUT_URB;
	case DMA_NONE:
		break;
	}

<<<<<<< HEAD
	if (!devinfo->use_streams) {
		cmdinfo->state &= ~(SUBMIT_DATA_IN_URB | SUBMIT_DATA_OUT_URB);
		cmdinfo->stream = 0;
	}

	err = uas_submit_urbs(cmnd, devinfo, GFP_ATOMIC);
=======
	if (!devinfo->use_streams)
		cmdinfo->state &= ~(SUBMIT_DATA_IN_URB | SUBMIT_DATA_OUT_URB);

	err = uas_submit_urbs(cmnd, devinfo);
	/*
	 * in case of fatal errors the SCSI layer is peculiar
	 * a command that has finished is a success for the purpose
	 * of queueing, no matter how fatal the error
	 */
	if (err == -ENODEV) {
		cmnd->result = DID_ERROR << 16;
		cmnd->scsi_done(cmnd);
		goto zombie;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err) {
		/* If we did nothing, give up now */
		if (cmdinfo->state & SUBMIT_STATUS_URB) {
			spin_unlock_irqrestore(&devinfo->lock, flags);
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
<<<<<<< HEAD
		spin_lock(&uas_work_lock);
		list_add_tail(&cmdinfo->list, &uas_work_list);
		cmdinfo->state |= IS_IN_WORK_LIST;
		spin_unlock(&uas_work_lock);
		schedule_work(&uas_work);
	}

=======
		uas_add_work(cmdinfo);
	}

	devinfo->cmnd[idx] = cmnd;
zombie:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_irqrestore(&devinfo->lock, flags);
	return 0;
}

static DEF_SCSI_QCMD(uas_queuecommand)

<<<<<<< HEAD
static int uas_eh_task_mgmt(struct scsi_cmnd *cmnd,
			    const char *fname, u8 function)
{
	struct Scsi_Host *shost = cmnd->device->host;
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	u16 tag = devinfo->qdepth - 1;
	unsigned long flags;

	spin_lock_irqsave(&devinfo->lock, flags);
	memset(&devinfo->response, 0, sizeof(devinfo->response));
	if (uas_submit_sense_urb(shost, GFP_ATOMIC, tag)) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s: submit sense urb failed\n",
			     __func__, fname);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return FAILED;
	}
	if (uas_submit_task_urb(cmnd, GFP_ATOMIC, function, tag)) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s: submit task mgmt urb failed\n",
			     __func__, fname);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return FAILED;
	}
	spin_unlock_irqrestore(&devinfo->lock, flags);

	if (usb_wait_anchor_empty_timeout(&devinfo->sense_urbs, 3000) == 0) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s timed out\n", __func__, fname);
		return FAILED;
	}
	if (be16_to_cpu(devinfo->response.tag) != tag) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s failed (wrong tag %d/%d)\n", __func__,
			     fname, be16_to_cpu(devinfo->response.tag), tag);
		return FAILED;
	}
	if (devinfo->response.response_code != RC_TMF_COMPLETE) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s failed (rc 0x%x)\n", __func__,
			     fname, devinfo->response.response_code);
		return FAILED;
	}
	return SUCCESS;
}

=======
/*
 * For now we do not support actually sending an abort to the device, so
 * this eh always fails. Still we must define it to make sure that we've
 * dropped all references to the cmnd in question once this function exits.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int uas_eh_abort_handler(struct scsi_cmnd *cmnd)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
<<<<<<< HEAD
	unsigned long flags;
	int ret;

	uas_log_cmd_state(cmnd, __func__);
	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state |= COMMAND_ABORTED;
	if (cmdinfo->state & IS_IN_WORK_LIST) {
		spin_lock(&uas_work_lock);
		list_del(&cmdinfo->list);
		cmdinfo->state &= ~IS_IN_WORK_LIST;
		spin_unlock(&uas_work_lock);
	}
	if (cmdinfo->state & COMMAND_INFLIGHT) {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		ret = uas_eh_task_mgmt(cmnd, "ABORT TASK", TMF_ABORT_TASK);
	} else {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		uas_unlink_data_urbs(devinfo, cmdinfo);
		spin_lock_irqsave(&devinfo->lock, flags);
		uas_try_complete(cmnd, __func__);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		ret = SUCCESS;
	}
	return ret;
}

static int uas_eh_device_reset_handler(struct scsi_cmnd *cmnd)
{
	sdev_printk(KERN_INFO, cmnd->device, "%s\n", __func__);
	return uas_eh_task_mgmt(cmnd, "LOGICAL UNIT RESET",
				TMF_LOGICAL_UNIT_RESET);
=======
	struct urb *data_in_urb = NULL;
	struct urb *data_out_urb = NULL;
	unsigned long flags;

	spin_lock_irqsave(&devinfo->lock, flags);

	uas_log_cmd_state(cmnd, __func__, 0);

	/* Ensure that try_complete does not call scsi_done */
	cmdinfo->state |= COMMAND_ABORTED;

	/* Drop all refs to this cmnd, kill data urbs to break their ref */
	devinfo->cmnd[cmdinfo->uas_tag - 1] = NULL;
	if (cmdinfo->state & DATA_IN_URB_INFLIGHT)
		data_in_urb = usb_get_urb(cmdinfo->data_in_urb);
	if (cmdinfo->state & DATA_OUT_URB_INFLIGHT)
		data_out_urb = usb_get_urb(cmdinfo->data_out_urb);

	uas_free_unsubmitted_urbs(cmnd);

	spin_unlock_irqrestore(&devinfo->lock, flags);

	if (data_in_urb) {
		usb_kill_urb(data_in_urb);
		usb_put_urb(data_in_urb);
	}
	if (data_out_urb) {
		usb_kill_urb(data_out_urb);
		usb_put_urb(data_out_urb);
	}

	return FAILED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int uas_eh_bus_reset_handler(struct scsi_cmnd *cmnd)
{
	struct scsi_device *sdev = cmnd->device;
	struct uas_dev_info *devinfo = sdev->hostdata;
	struct usb_device *udev = devinfo->udev;
<<<<<<< HEAD
	int err;

	devinfo->resetting = 1;
	uas_abort_work(devinfo);
	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	err = usb_reset_device(udev);
	devinfo->resetting = 0;

	if (err) {
		shost_printk(KERN_INFO, sdev->host, "%s FAILED\n", __func__);
=======
	unsigned long flags;
	int err;

	err = usb_lock_device_for_reset(udev, devinfo->intf);
	if (err) {
		shost_printk(KERN_ERR, sdev->host,
			     "%s FAILED to get lock err %d\n", __func__, err);
		return FAILED;
	}

	shost_printk(KERN_INFO, sdev->host, "%s start\n", __func__);

	spin_lock_irqsave(&devinfo->lock, flags);
	devinfo->resetting = 1;
	spin_unlock_irqrestore(&devinfo->lock, flags);

	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	uas_zap_pending(devinfo, DID_RESET);

	err = usb_reset_device(udev);

	spin_lock_irqsave(&devinfo->lock, flags);
	devinfo->resetting = 0;
	spin_unlock_irqrestore(&devinfo->lock, flags);

	usb_unlock_device(udev);

	if (err) {
		shost_printk(KERN_INFO, sdev->host, "%s FAILED err %d\n",
			     __func__, err);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return FAILED;
	}

	shost_printk(KERN_INFO, sdev->host, "%s success\n", __func__);
	return SUCCESS;
}

<<<<<<< HEAD
static int uas_slave_alloc(struct scsi_device *sdev)
{
	sdev->hostdata = (void *)sdev->host->hostdata[0];
=======
static int uas_target_alloc(struct scsi_target *starget)
{
	struct uas_dev_info *devinfo = (struct uas_dev_info *)
			dev_to_shost(starget->dev.parent)->hostdata;

	if (devinfo->flags & US_FL_NO_REPORT_LUNS)
		starget->no_report_luns = 1;

	return 0;
}

static int uas_slave_alloc(struct scsi_device *sdev)
{
	struct uas_dev_info *devinfo =
		(struct uas_dev_info *)sdev->host->hostdata;

	sdev->hostdata = devinfo;

	/*
	 * The protocol has no requirements on alignment in the strict sense.
	 * Controllers may or may not have alignment restrictions.
	 * As this is not exported, we use an extremely conservative guess.
	 */
	blk_queue_update_dma_alignment(sdev->request_queue, (512 - 1));

	if (devinfo->flags & US_FL_MAX_SECTORS_64)
		blk_queue_max_hw_sectors(sdev->request_queue, 64);
	else if (devinfo->flags & US_FL_MAX_SECTORS_240)
		blk_queue_max_hw_sectors(sdev->request_queue, 240);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int uas_slave_configure(struct scsi_device *sdev)
{
	struct uas_dev_info *devinfo = sdev->hostdata;
<<<<<<< HEAD
	scsi_set_tag_type(sdev, MSG_ORDERED_TAG);
	scsi_activate_tcq(sdev, devinfo->qdepth - 3);
=======

	if (devinfo->flags & US_FL_NO_REPORT_OPCODES)
		sdev->no_report_opcodes = 1;

	/* A few buggy USB-ATA bridges don't understand FUA */
	if (devinfo->flags & US_FL_BROKEN_FUA)
		sdev->broken_fua = 1;

	/* UAS also needs to support FL_ALWAYS_SYNC */
	if (devinfo->flags & US_FL_ALWAYS_SYNC) {
		sdev->skip_ms_page_3f = 1;
		sdev->skip_ms_page_8 = 1;
		sdev->wce_default_on = 1;
	}

	/* Some disks cannot handle READ_CAPACITY_16 */
	if (devinfo->flags & US_FL_NO_READ_CAPACITY_16)
		sdev->no_read_capacity_16 = 1;

	/*
	 * Some disks return the total number of blocks in response
	 * to READ CAPACITY rather than the highest block number.
	 * If this device makes that mistake, tell the sd driver.
	 */
	if (devinfo->flags & US_FL_FIX_CAPACITY)
		sdev->fix_capacity = 1;

	/*
	 * in some cases we have to guess
	 */
	if (devinfo->flags & US_FL_CAPACITY_HEURISTICS)
		sdev->guess_capacity = 1;

	/*
	 * Some devices don't like MODE SENSE with page=0x3f,
	 * which is the command used for checking if a device
	 * is write-protected.  Now that we tell the sd driver
	 * to do a 192-byte transfer with this command the
	 * majority of devices work fine, but a few still can't
	 * handle it.  The sd driver will simply assume those
	 * devices are write-enabled.
	 */
	if (devinfo->flags & US_FL_NO_WP_DETECT)
		sdev->skip_ms_page_3f = 1;

	scsi_change_queue_depth(sdev, devinfo->qdepth - 2);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static struct scsi_host_template uas_host_template = {
	.module = THIS_MODULE,
	.name = "uas",
	.queuecommand = uas_queuecommand,
<<<<<<< HEAD
	.slave_alloc = uas_slave_alloc,
	.slave_configure = uas_slave_configure,
	.eh_abort_handler = uas_eh_abort_handler,
	.eh_device_reset_handler = uas_eh_device_reset_handler,
	.eh_bus_reset_handler = uas_eh_bus_reset_handler,
	.can_queue = 65536,	/* Is there a limit on the _host_ ? */
	.this_id = -1,
	.sg_tablesize = SG_NONE,
	.cmd_per_lun = 1,	/* until we override it */
	.skip_settle_delay = 1,
	.ordered_tag = 1,
};

static struct usb_device_id uas_usb_ids[] = {
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_BULK) },
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_UAS) },
	/* 0xaa is a prototype device I happen to have access to */
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, 0xaa) },
=======
	.target_alloc = uas_target_alloc,
	.slave_alloc = uas_slave_alloc,
	.slave_configure = uas_slave_configure,
	.eh_abort_handler = uas_eh_abort_handler,
	.eh_bus_reset_handler = uas_eh_bus_reset_handler,
	.this_id = -1,
	.sg_tablesize = SG_NONE,
	.skip_settle_delay = 1,
};

#define UNUSUAL_DEV(id_vendor, id_product, bcdDeviceMin, bcdDeviceMax, \
		    vendorName, productName, useProtocol, useTransport, \
		    initFunction, flags) \
{ USB_DEVICE_VER(id_vendor, id_product, bcdDeviceMin, bcdDeviceMax), \
	.driver_info = (flags) }

static struct usb_device_id uas_usb_ids[] = {
#	include "unusual_uas.h"
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_BULK) },
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_UAS) },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ }
};
MODULE_DEVICE_TABLE(usb, uas_usb_ids);

<<<<<<< HEAD
static int uas_is_interface(struct usb_host_interface *intf)
{
	return (intf->desc.bInterfaceClass == USB_CLASS_MASS_STORAGE &&
		intf->desc.bInterfaceSubClass == USB_SC_SCSI &&
		intf->desc.bInterfaceProtocol == USB_PR_UAS);
}

static int uas_isnt_supported(struct usb_device *udev)
{
	struct usb_hcd *hcd = bus_to_hcd(udev->bus);

	dev_warn(&udev->dev, "The driver for the USB controller %s does not "
			"support scatter-gather which is\n",
			hcd->driver->description);
	dev_warn(&udev->dev, "required by the UAS driver. Please try an"
			"alternative USB controller if you wish to use UAS.\n");
	return -ENODEV;
}

static int uas_switch_interface(struct usb_device *udev,
						struct usb_interface *intf)
{
	int i;
	int sg_supported = udev->bus->sg_tablesize != 0;

	for (i = 0; i < intf->num_altsetting; i++) {
		struct usb_host_interface *alt = &intf->altsetting[i];

		if (uas_is_interface(alt)) {
			if (!sg_supported)
				return uas_isnt_supported(udev);
			return usb_set_interface(udev,
						alt->desc.bInterfaceNumber,
						alt->desc.bAlternateSetting);
		}
	}

	return -ENODEV;
}

static void uas_configure_endpoints(struct uas_dev_info *devinfo)
{
	struct usb_host_endpoint *eps[4] = { };
	struct usb_interface *intf = devinfo->intf;
	struct usb_device *udev = devinfo->udev;
	struct usb_host_endpoint *endpoint = intf->cur_altsetting->endpoint;
	unsigned i, n_endpoints = intf->cur_altsetting->desc.bNumEndpoints;

	devinfo->uas_sense_old = 0;
	devinfo->cmnd = NULL;

	for (i = 0; i < n_endpoints; i++) {
		unsigned char *extra = endpoint[i].extra;
		int len = endpoint[i].extralen;
		while (len > 1) {
			if (extra[1] == USB_DT_PIPE_USAGE) {
				unsigned pipe_id = extra[2];
				if (pipe_id > 0 && pipe_id < 5)
					eps[pipe_id - 1] = &endpoint[i];
				break;
			}
			len -= extra[0];
			extra += extra[0];
		}
	}

	/*
	 * Assume that if we didn't find a control pipe descriptor, we're
	 * using a device with old firmware that happens to be set up like
	 * this.
	 */
	if (!eps[0]) {
		devinfo->cmd_pipe = usb_sndbulkpipe(udev, 1);
		devinfo->status_pipe = usb_rcvbulkpipe(udev, 1);
		devinfo->data_in_pipe = usb_rcvbulkpipe(udev, 2);
		devinfo->data_out_pipe = usb_sndbulkpipe(udev, 2);

		eps[1] = usb_pipe_endpoint(udev, devinfo->status_pipe);
		eps[2] = usb_pipe_endpoint(udev, devinfo->data_in_pipe);
		eps[3] = usb_pipe_endpoint(udev, devinfo->data_out_pipe);
	} else {
		devinfo->cmd_pipe = usb_sndbulkpipe(udev,
						eps[0]->desc.bEndpointAddress);
		devinfo->status_pipe = usb_rcvbulkpipe(udev,
						eps[1]->desc.bEndpointAddress);
		devinfo->data_in_pipe = usb_rcvbulkpipe(udev,
						eps[2]->desc.bEndpointAddress);
		devinfo->data_out_pipe = usb_sndbulkpipe(udev,
						eps[3]->desc.bEndpointAddress);
	}

	devinfo->qdepth = usb_alloc_streams(devinfo->intf, eps + 1, 3, 256,
								GFP_KERNEL);
	if (devinfo->qdepth < 0) {
		devinfo->qdepth = 256;
		devinfo->use_streams = 0;
	} else {
		devinfo->use_streams = 1;
	}
=======
#undef UNUSUAL_DEV

static int uas_switch_interface(struct usb_device *udev,
				struct usb_interface *intf)
{
	struct usb_host_interface *alt;

	alt = uas_find_uas_alt_setting(intf);
	if (!alt)
		return -ENODEV;

	return usb_set_interface(udev, alt->desc.bInterfaceNumber,
			alt->desc.bAlternateSetting);
}

static int uas_configure_endpoints(struct uas_dev_info *devinfo)
{
	struct usb_host_endpoint *eps[4] = { };
	struct usb_device *udev = devinfo->udev;
	int r;

	r = uas_find_endpoints(devinfo->intf->cur_altsetting, eps);
	if (r)
		return r;

	devinfo->cmd_pipe = usb_sndbulkpipe(udev,
					    usb_endpoint_num(&eps[0]->desc));
	devinfo->status_pipe = usb_rcvbulkpipe(udev,
					    usb_endpoint_num(&eps[1]->desc));
	devinfo->data_in_pipe = usb_rcvbulkpipe(udev,
					    usb_endpoint_num(&eps[2]->desc));
	devinfo->data_out_pipe = usb_sndbulkpipe(udev,
					    usb_endpoint_num(&eps[3]->desc));

	if (udev->speed < USB_SPEED_SUPER) {
		devinfo->qdepth = 32;
		devinfo->use_streams = 0;
	} else {
		devinfo->qdepth = usb_alloc_streams(devinfo->intf, eps + 1,
						    3, MAX_CMNDS, GFP_NOIO);
		if (devinfo->qdepth < 0)
			return devinfo->qdepth;
		devinfo->use_streams = 1;
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void uas_free_streams(struct uas_dev_info *devinfo)
{
	struct usb_device *udev = devinfo->udev;
	struct usb_host_endpoint *eps[3];

	eps[0] = usb_pipe_endpoint(udev, devinfo->status_pipe);
	eps[1] = usb_pipe_endpoint(udev, devinfo->data_in_pipe);
	eps[2] = usb_pipe_endpoint(udev, devinfo->data_out_pipe);
<<<<<<< HEAD
	usb_free_streams(devinfo->intf, eps, 3, GFP_KERNEL);
}

/*
 * XXX: What I'd like to do here is register a SCSI host for each USB host in
 * the system.  Follow usb-storage's design of registering a SCSI host for
 * each USB device for the moment.  Can implement this by walking up the
 * USB hierarchy until we find a USB host.
 */
static int uas_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	int result;
	struct Scsi_Host *shost;
	struct uas_dev_info *devinfo;
	struct usb_device *udev = interface_to_usbdev(intf);

	if (uas_switch_interface(udev, intf))
		return -ENODEV;

	devinfo = kmalloc(sizeof(struct uas_dev_info), GFP_KERNEL);
	if (!devinfo)
		return -ENOMEM;

	result = -ENOMEM;
	shost = scsi_host_alloc(&uas_host_template, sizeof(void *));
	if (!shost)
		goto free;
=======
	usb_free_streams(devinfo->intf, eps, 3, GFP_NOIO);
}

static int uas_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	int result = -ENOMEM;
	struct Scsi_Host *shost = NULL;
	struct uas_dev_info *devinfo;
	struct usb_device *udev = interface_to_usbdev(intf);
	unsigned long dev_flags;

	if (!uas_use_uas_driver(intf, id, &dev_flags))
		return -ENODEV;

	if (uas_switch_interface(udev, intf))
		return -ENODEV;

	shost = scsi_host_alloc(&uas_host_template,
				sizeof(struct uas_dev_info));
	if (!shost)
		goto set_alt0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	shost->max_cmd_len = 16 + 252;
	shost->max_id = 1;
	shost->max_lun = 256;
	shost->max_channel = 0;
	shost->sg_tablesize = udev->bus->sg_tablesize;

<<<<<<< HEAD
	devinfo->intf = intf;
	devinfo->udev = udev;
	devinfo->resetting = 0;
=======
	devinfo = (struct uas_dev_info *)shost->hostdata;
	devinfo->intf = intf;
	devinfo->udev = udev;
	devinfo->resetting = 0;
	devinfo->shutdown = 0;
	devinfo->flags = dev_flags;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	init_usb_anchor(&devinfo->cmd_urbs);
	init_usb_anchor(&devinfo->sense_urbs);
	init_usb_anchor(&devinfo->data_urbs);
	spin_lock_init(&devinfo->lock);
<<<<<<< HEAD
	uas_configure_endpoints(devinfo);

	result = scsi_init_shared_tag_map(shost, devinfo->qdepth - 3);
	if (result)
		goto free;

	result = scsi_add_host(shost, &intf->dev);
	if (result)
		goto deconfig_eps;

	shost->hostdata[0] = (unsigned long)devinfo;

	scsi_scan_host(shost);
	usb_set_intfdata(intf, shost);
	return result;

deconfig_eps:
	uas_free_streams(devinfo);
 free:
	kfree(devinfo);
=======
	INIT_WORK(&devinfo->work, uas_do_work);
	INIT_WORK(&devinfo->scan_work, uas_scan_work);

	result = uas_configure_endpoints(devinfo);
	if (result)
		goto set_alt0;

	/*
	 * 1 tag is reserved for untagged commands +
	 * 1 tag to avoid off by one errors in some bridge firmwares
	 */
	shost->can_queue = devinfo->qdepth - 2;

	usb_set_intfdata(intf, shost);
	result = scsi_add_host(shost, &intf->dev);
	if (result)
		goto free_streams;

	/* Submit the delayed_work for SCSI-device scanning */
	schedule_work(&devinfo->scan_work);

	return result;

free_streams:
	uas_free_streams(devinfo);
	usb_set_intfdata(intf, NULL);
set_alt0:
	usb_set_interface(udev, intf->altsetting[0].desc.bInterfaceNumber, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (shost)
		scsi_host_put(shost);
	return result;
}

<<<<<<< HEAD
static int uas_pre_reset(struct usb_interface *intf)
{
/* XXX: Need to return 1 if it's not our device in error handling */
=======
static int uas_cmnd_list_empty(struct uas_dev_info *devinfo)
{
	unsigned long flags;
	int i, r = 1;

	spin_lock_irqsave(&devinfo->lock, flags);

	for (i = 0; i < devinfo->qdepth; i++) {
		if (devinfo->cmnd[i]) {
			r = 0; /* Not empty */
			break;
		}
	}

	spin_unlock_irqrestore(&devinfo->lock, flags);

	return r;
}

/*
 * Wait for any pending cmnds to complete, on usb-2 sense_urbs may temporarily
 * get empty while there still is more work to do due to sense-urbs completing
 * with a READ/WRITE_READY iu code, so keep waiting until the list gets empty.
 */
static int uas_wait_for_pending_cmnds(struct uas_dev_info *devinfo)
{
	unsigned long start_time;
	int r;

	start_time = jiffies;
	do {
		flush_work(&devinfo->work);

		r = usb_wait_anchor_empty_timeout(&devinfo->sense_urbs, 5000);
		if (r == 0)
			return -ETIME;

		r = usb_wait_anchor_empty_timeout(&devinfo->data_urbs, 500);
		if (r == 0)
			return -ETIME;

		if (time_after(jiffies, start_time + 5 * HZ))
			return -ETIME;
	} while (!uas_cmnd_list_empty(devinfo));

	return 0;
}

static int uas_pre_reset(struct usb_interface *intf)
{
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;
	unsigned long flags;

	if (devinfo->shutdown)
		return 0;

	/* Block new requests */
	spin_lock_irqsave(shost->host_lock, flags);
	scsi_block_requests(shost);
	spin_unlock_irqrestore(shost->host_lock, flags);

	if (uas_wait_for_pending_cmnds(devinfo) != 0) {
		shost_printk(KERN_ERR, shost, "%s: timed out\n", __func__);
		scsi_unblock_requests(shost);
		return 1;
	}

	uas_free_streams(devinfo);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int uas_post_reset(struct usb_interface *intf)
{
<<<<<<< HEAD
/* XXX: Need to return 1 if it's not our device in error handling */
=======
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;
	unsigned long flags;
	int err;

	if (devinfo->shutdown)
		return 0;

	err = uas_configure_endpoints(devinfo);
	if (err && err != -ENODEV)
		shost_printk(KERN_ERR, shost,
			     "%s: alloc streams error %d after reset",
			     __func__, err);

	/* we must unblock the host in every case lest we deadlock */
	spin_lock_irqsave(shost->host_lock, flags);
	scsi_report_bus_reset(shost, 0);
	spin_unlock_irqrestore(shost->host_lock, flags);

	scsi_unblock_requests(shost);

	return err ? 1 : 0;
}

static int uas_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;

	if (uas_wait_for_pending_cmnds(devinfo) != 0) {
		shost_printk(KERN_ERR, shost, "%s: timed out\n", __func__);
		return -ETIME;
	}

	return 0;
}

static int uas_resume(struct usb_interface *intf)
{
	return 0;
}

static int uas_reset_resume(struct usb_interface *intf)
{
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;
	unsigned long flags;
	int err;

	err = uas_configure_endpoints(devinfo);
	if (err) {
		shost_printk(KERN_ERR, shost,
			     "%s: alloc streams error %d after reset",
			     __func__, err);
		return -EIO;
	}

	spin_lock_irqsave(shost->host_lock, flags);
	scsi_report_bus_reset(shost, 0);
	spin_unlock_irqrestore(shost->host_lock, flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void uas_disconnect(struct usb_interface *intf)
{
	struct Scsi_Host *shost = usb_get_intfdata(intf);
<<<<<<< HEAD
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];

	devinfo->resetting = 1;
	uas_abort_work(devinfo);
	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	scsi_remove_host(shost);
	uas_free_streams(devinfo);
	kfree(devinfo);
}

/*
 * XXX: Should this plug into libusual so we can auto-upgrade devices from
 * Bulk-Only to UAS?
 */
=======
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;
	unsigned long flags;

	spin_lock_irqsave(&devinfo->lock, flags);
	devinfo->resetting = 1;
	spin_unlock_irqrestore(&devinfo->lock, flags);

	cancel_work_sync(&devinfo->work);
	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	uas_zap_pending(devinfo, DID_NO_CONNECT);

	/*
	 * Prevent SCSI scanning (if it hasn't started yet)
	 * or wait for the SCSI-scanning routine to stop.
	 */
	cancel_work_sync(&devinfo->scan_work);

	scsi_remove_host(shost);
	uas_free_streams(devinfo);
	scsi_host_put(shost);
}

/*
 * Put the device back in usb-storage mode on shutdown, as some BIOS-es
 * hang on reboot when the device is still in uas mode. Note the reset is
 * necessary as some devices won't revert to usb-storage mode without it.
 */
static void uas_shutdown(struct device *dev)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct usb_device *udev = interface_to_usbdev(intf);
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (struct uas_dev_info *)shost->hostdata;

	if (system_state != SYSTEM_RESTART)
		return;

	devinfo->shutdown = 1;
	uas_free_streams(devinfo);
	usb_set_interface(udev, intf->altsetting[0].desc.bInterfaceNumber, 0);
	usb_reset_device(udev);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct usb_driver uas_driver = {
	.name = "uas",
	.probe = uas_probe,
	.disconnect = uas_disconnect,
	.pre_reset = uas_pre_reset,
	.post_reset = uas_post_reset,
<<<<<<< HEAD
	.id_table = uas_usb_ids,
};

module_usb_driver(uas_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Wilcox and Sarah Sharp");
=======
	.suspend = uas_suspend,
	.resume = uas_resume,
	.reset_resume = uas_reset_resume,
	.drvwrap.driver.shutdown = uas_shutdown,
	.id_table = uas_usb_ids,
};

static int __init uas_init(void)
{
	int rv;

	workqueue = alloc_workqueue("uas", WQ_MEM_RECLAIM, 0);
	if (!workqueue)
		return -ENOMEM;

	rv = usb_register(&uas_driver);
	if (rv) {
		destroy_workqueue(workqueue);
		return -ENOMEM;
	}

	return 0;
}

static void __exit uas_exit(void)
{
	usb_deregister(&uas_driver);
	destroy_workqueue(workqueue);
}

module_init(uas_init);
module_exit(uas_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(
	"Hans de Goede <hdegoede@redhat.com>, Matthew Wilcox and Sarah Sharp");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
