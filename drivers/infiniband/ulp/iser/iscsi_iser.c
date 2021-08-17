/*
 * iSCSI Initiator over iSER Data-Path
 *
 * Copyright (C) 2004 Dmitry Yusupov
 * Copyright (C) 2004 Alex Aizman
 * Copyright (C) 2005 Mike Christie
 * Copyright (c) 2005, 2006 Voltaire, Inc. All rights reserved.
<<<<<<< HEAD
 * Copyright (c) 2013 Mellanox Technologies. All rights reserved.
=======
 * Copyright (c) 2013-2014 Mellanox Technologies. All rights reserved.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * maintained by openib-general@openib.org
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Credits:
 *	Christoph Hellwig
 *	FUJITA Tomonori
 *	Arne Redlich
 *	Zhenyu Wang
 * Modified by:
 *      Erez Zilber
 */

#include <linux/types.h>
#include <linux/list.h>
#include <linux/hardirq.h>
#include <linux/kfifo.h>
#include <linux/blkdev.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/in.h>
#include <linux/net.h>
#include <linux/scatterlist.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/module.h>

#include <net/sock.h>

#include <asm/uaccess.h>

#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_tcq.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi.h>
#include <scsi/scsi_transport_iscsi.h>

#include "iscsi_iser.h"

<<<<<<< HEAD
static struct scsi_host_template iscsi_iser_sht;
static struct iscsi_transport iscsi_iser_transport;
static struct scsi_transport_template *iscsi_iser_scsi_transport;

static unsigned int iscsi_max_lun = 512;
module_param_named(max_lun, iscsi_max_lun, uint, S_IRUGO);

int iser_debug_level = 0;

MODULE_DESCRIPTION("iSER (iSCSI Extensions for RDMA) Datamover");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Alex Nezhinsky, Dan Bar Dov, Or Gerlitz");
MODULE_VERSION(DRV_VER);

module_param_named(debug_level, iser_debug_level, int, 0644);
MODULE_PARM_DESC(debug_level, "Enable debug tracing if > 0 (default:disabled)");

struct iser_global ig;

void
iscsi_iser_recv(struct iscsi_conn *conn,
		struct iscsi_hdr *hdr, char *rx_data, int rx_data_len)
{
	int rc = 0;
	int datalen;
	int ahslen;
=======
MODULE_DESCRIPTION("iSER (iSCSI Extensions for RDMA) Datamover");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Alex Nezhinsky, Dan Bar Dov, Or Gerlitz");
MODULE_VERSION(DRV_VER);

static struct scsi_host_template iscsi_iser_sht;
static struct iscsi_transport iscsi_iser_transport;
static struct scsi_transport_template *iscsi_iser_scsi_transport;
static struct workqueue_struct *release_wq;
static DEFINE_MUTEX(unbind_iser_conn_mutex);
struct iser_global ig;

int iser_debug_level = 0;
module_param_named(debug_level, iser_debug_level, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_level, "Enable debug tracing if > 0 (default:disabled)");

static unsigned int iscsi_max_lun = 512;
module_param_named(max_lun, iscsi_max_lun, uint, S_IRUGO);
MODULE_PARM_DESC(max_lun, "Max LUNs to allow per session (default:512");

unsigned int iser_max_sectors = ISER_DEF_MAX_SECTORS;
module_param_named(max_sectors, iser_max_sectors, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(max_sectors, "Max number of sectors in a single scsi command (default:1024");

bool iser_always_reg = true;
module_param_named(always_register, iser_always_reg, bool, S_IRUGO);
MODULE_PARM_DESC(always_register,
		 "Always register memory, even for continuous memory regions (default:true)");

bool iser_pi_enable = false;
module_param_named(pi_enable, iser_pi_enable, bool, S_IRUGO);
MODULE_PARM_DESC(pi_enable, "Enable T10-PI offload support (default:disabled)");

int iser_pi_guard;
module_param_named(pi_guard, iser_pi_guard, int, S_IRUGO);
MODULE_PARM_DESC(pi_guard, "T10-PI guard_type [deprecated]");

/*
 * iscsi_iser_recv() - Process a successful recv completion
 * @conn:         iscsi connection
 * @hdr:          iscsi header
 * @rx_data:      buffer containing receive data payload
 * @rx_data_len:  length of rx_data
 *
 * Notes: In case of data length errors or iscsi PDU completion failures
 *        this routine will signal iscsi layer of connection failure.
 */
void
iscsi_iser_recv(struct iscsi_conn *conn, struct iscsi_hdr *hdr,
		char *rx_data, int rx_data_len)
{
	int rc = 0;
	int datalen;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* verify PDU length */
	datalen = ntoh24(hdr->dlength);
	if (datalen > rx_data_len || (datalen + 4) < rx_data_len) {
		iser_err("wrong datalen %d (hdr), %d (IB)\n",
			datalen, rx_data_len);
		rc = ISCSI_ERR_DATALEN;
		goto error;
	}

	if (datalen != rx_data_len)
		iser_dbg("aligned datalen (%d) hdr, %d (IB)\n",
			datalen, rx_data_len);

<<<<<<< HEAD
	/* read AHS */
	ahslen = hdr->hlength * 4;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rc = iscsi_complete_pdu(conn, hdr, rx_data, rx_data_len);
	if (rc && rc != ISCSI_ERR_NO_SCSI_CMD)
		goto error;

	return;
error:
	iscsi_conn_failure(conn, rc);
}

<<<<<<< HEAD
static int iscsi_iser_pdu_alloc(struct iscsi_task *task, uint8_t opcode)
=======
/**
 * iscsi_iser_pdu_alloc() - allocate an iscsi-iser PDU
 * @task:     iscsi task
 * @opcode:   iscsi command opcode
 *
 * Netes: This routine can't fail, just assign iscsi task
 *        hdr and max hdr size.
 */
static int
iscsi_iser_pdu_alloc(struct iscsi_task *task, uint8_t opcode)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct iscsi_iser_task *iser_task = task->dd_data;

	task->hdr = (struct iscsi_hdr *)&iser_task->desc.iscsi_header;
	task->hdr_max = sizeof(iser_task->desc.iscsi_header);
<<<<<<< HEAD
	return 0;
}

int iser_initialize_task_headers(struct iscsi_task *task,
						struct iser_tx_desc *tx_desc)
{
	struct iscsi_iser_conn *iser_conn = task->conn->dd_data;
	struct iser_device     *device    = iser_conn->ib_conn->device;
	struct iscsi_iser_task *iser_task = task->dd_data;
	u64 dma_addr;

	dma_addr = ib_dma_map_single(device->ib_device, (void *)tx_desc,
				ISER_HEADERS_LEN, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(device->ib_device, dma_addr))
		return -ENOMEM;

	tx_desc->dma_addr = dma_addr;
	tx_desc->tx_sg[0].addr   = tx_desc->dma_addr;
	tx_desc->tx_sg[0].length = ISER_HEADERS_LEN;
	tx_desc->tx_sg[0].lkey   = device->mr->lkey;

	iser_task->iser_conn		= iser_conn;
	return 0;
}
/**
 * iscsi_iser_task_init - Initialize task
 * @task: iscsi task
 *
 * Initialize the task for the scsi command or mgmt command.
=======

	return 0;
}

/**
 * iser_initialize_task_headers() - Initialize task headers
 * @task:       iscsi task
 * @tx_desc:    iser tx descriptor
 *
 * Notes:
 * This routine may race with iser teardown flow for scsi
 * error handling TMFs. So for TMF we should acquire the
 * state mutex to avoid dereferencing the IB device which
 * may have already been terminated.
 */
int
iser_initialize_task_headers(struct iscsi_task *task,
			     struct iser_tx_desc *tx_desc)
{
	struct iser_conn *iser_conn = task->conn->dd_data;
	struct iser_device *device = iser_conn->ib_conn.device;
	struct iscsi_iser_task *iser_task = task->dd_data;
	u64 dma_addr;
	const bool mgmt_task = !task->sc && !in_interrupt();
	int ret = 0;

	if (unlikely(mgmt_task))
		mutex_lock(&iser_conn->state_mutex);

	if (unlikely(iser_conn->state != ISER_CONN_UP)) {
		ret = -ENODEV;
		goto out;
	}

	dma_addr = ib_dma_map_single(device->ib_device, (void *)tx_desc,
				ISER_HEADERS_LEN, DMA_TO_DEVICE);
	if (ib_dma_mapping_error(device->ib_device, dma_addr)) {
		ret = -ENOMEM;
		goto out;
	}

	tx_desc->wr_idx = 0;
	tx_desc->mapped = true;
	tx_desc->dma_addr = dma_addr;
	tx_desc->tx_sg[0].addr   = tx_desc->dma_addr;
	tx_desc->tx_sg[0].length = ISER_HEADERS_LEN;
	tx_desc->tx_sg[0].lkey   = device->pd->local_dma_lkey;

	iser_task->iser_conn = iser_conn;
out:
	if (unlikely(mgmt_task))
		mutex_unlock(&iser_conn->state_mutex);

	return ret;
}

/**
 * iscsi_iser_task_init() - Initialize iscsi-iser task
 * @task: iscsi task
 *
 * Initialize the task for the scsi command or mgmt command.
 *
 * Return: Returns zero on success or -ENOMEM when failing
 *         to init task headers (dma mapping error).
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int
iscsi_iser_task_init(struct iscsi_task *task)
{
	struct iscsi_iser_task *iser_task = task->dd_data;
<<<<<<< HEAD

	if (iser_initialize_task_headers(task, &iser_task->desc))
			return -ENOMEM;
=======
	int ret;

	ret = iser_initialize_task_headers(task, &iser_task->desc);
	if (ret) {
		iser_err("Failed to init task %p, err = %d\n",
			 iser_task, ret);
		return ret;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* mgmt task */
	if (!task->sc)
		return 0;

	iser_task->command_sent = 0;
	iser_task_rdma_init(iser_task);
<<<<<<< HEAD
=======
	iser_task->sc = task->sc;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/**
<<<<<<< HEAD
 * iscsi_iser_mtask_xmit - xmit management(immediate) task
=======
 * iscsi_iser_mtask_xmit() - xmit management (immediate) task
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @conn: iscsi connection
 * @task: task management task
 *
 * Notes:
 *	The function can return -EAGAIN in which case caller must
 *	call it again later, or recover. '0' return code means successful
 *	xmit.
 *
 **/
static int
iscsi_iser_mtask_xmit(struct iscsi_conn *conn, struct iscsi_task *task)
{
	int error = 0;

	iser_dbg("mtask xmit [cid %d itt 0x%x]\n", conn->id, task->itt);

	error = iser_send_control(conn, task);

	/* since iser xmits control with zero copy, tasks can not be recycled
	 * right after sending them.
	 * The recycling scheme is based on whether a response is expected
	 * - if yes, the task is recycled at iscsi_complete_pdu
	 * - if no,  the task is recycled at iser_snd_completion
	 */
	return error;
}

static int
iscsi_iser_task_xmit_unsol_data(struct iscsi_conn *conn,
				 struct iscsi_task *task)
{
	struct iscsi_r2t_info *r2t = &task->unsol_r2t;
	struct iscsi_data hdr;
	int error = 0;

	/* Send data-out PDUs while there's still unsolicited data to send */
	while (iscsi_task_has_unsol_data(task)) {
		iscsi_prep_data_out_pdu(task, r2t, &hdr);
		iser_dbg("Sending data-out: itt 0x%x, data count %d\n",
			   hdr.itt, r2t->data_count);

		/* the buffer description has been passed with the command */
		/* Send the command */
		error = iser_send_data_out(conn, task, &hdr);
		if (error) {
			r2t->datasn--;
			goto iscsi_iser_task_xmit_unsol_data_exit;
		}
		r2t->sent += r2t->data_count;
		iser_dbg("Need to send %d more as data-out PDUs\n",
			   r2t->data_length - r2t->sent);
	}

iscsi_iser_task_xmit_unsol_data_exit:
	return error;
}

<<<<<<< HEAD
=======
/**
 * iscsi_iser_task_xmit() - xmit iscsi-iser task
 * @task: iscsi task
 *
 * Return: zero on success or escalates $error on failure.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int
iscsi_iser_task_xmit(struct iscsi_task *task)
{
	struct iscsi_conn *conn = task->conn;
	struct iscsi_iser_task *iser_task = task->dd_data;
	int error = 0;

	if (!task->sc)
		return iscsi_iser_mtask_xmit(conn, task);

	if (task->sc->sc_data_direction == DMA_TO_DEVICE) {
		BUG_ON(scsi_bufflen(task->sc) == 0);

		iser_dbg("cmd [itt %x total %d imm %d unsol_data %d\n",
			   task->itt, scsi_bufflen(task->sc),
			   task->imm_count, task->unsol_r2t.data_length);
	}

	iser_dbg("ctask xmit [cid %d itt 0x%x]\n",
		   conn->id, task->itt);

	/* Send the cmd PDU */
	if (!iser_task->command_sent) {
		error = iser_send_command(conn, task);
		if (error)
			goto iscsi_iser_task_xmit_exit;
		iser_task->command_sent = 1;
	}

	/* Send unsolicited data-out PDU(s) if necessary */
	if (iscsi_task_has_unsol_data(task))
		error = iscsi_iser_task_xmit_unsol_data(conn, task);

 iscsi_iser_task_xmit_exit:
	return error;
}

<<<<<<< HEAD
static void iscsi_iser_cleanup_task(struct iscsi_task *task)
{
	struct iscsi_iser_task *iser_task = task->dd_data;
	struct iser_tx_desc	*tx_desc = &iser_task->desc;

	struct iscsi_iser_conn *iser_conn = task->conn->dd_data;
	struct iser_device     *device    = iser_conn->ib_conn->device;

	ib_dma_unmap_single(device->ib_device,
		tx_desc->dma_addr, ISER_HEADERS_LEN, DMA_TO_DEVICE);
=======
/**
 * iscsi_iser_cleanup_task() - cleanup an iscsi-iser task
 * @task: iscsi task
 *
 * Notes: In case the RDMA device is already NULL (might have
 *        been removed in DEVICE_REMOVAL CM event it will bail-out
 *        without doing dma unmapping.
 */
static void iscsi_iser_cleanup_task(struct iscsi_task *task)
{
	struct iscsi_iser_task *iser_task = task->dd_data;
	struct iser_tx_desc *tx_desc = &iser_task->desc;
	struct iser_conn *iser_conn = task->conn->dd_data;
	struct iser_device *device = iser_conn->ib_conn.device;

	/* DEVICE_REMOVAL event might have already released the device */
	if (!device)
		return;

	if (likely(tx_desc->mapped)) {
		ib_dma_unmap_single(device->ib_device, tx_desc->dma_addr,
				    ISER_HEADERS_LEN, DMA_TO_DEVICE);
		tx_desc->mapped = false;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* mgmt tasks do not need special cleanup */
	if (!task->sc)
		return;

	if (iser_task->status == ISER_TASK_STATUS_STARTED) {
		iser_task->status = ISER_TASK_STATUS_COMPLETED;
		iser_task_rdma_finalize(iser_task);
	}
}

<<<<<<< HEAD
static struct iscsi_cls_conn *
iscsi_iser_conn_create(struct iscsi_cls_session *cls_session, uint32_t conn_idx)
{
	struct iscsi_conn *conn;
	struct iscsi_cls_conn *cls_conn;
	struct iscsi_iser_conn *iser_conn;

	cls_conn = iscsi_conn_setup(cls_session, sizeof(*iser_conn), conn_idx);
=======
/**
 * iscsi_iser_check_protection() - check protection information status of task.
 * @task:     iscsi task
 * @sector:   error sector if exsists (output)
 *
 * Return: zero if no data-integrity errors have occured
 *         0x1: data-integrity error occured in the guard-block
 *         0x2: data-integrity error occured in the reference tag
 *         0x3: data-integrity error occured in the application tag
 *
 *         In addition the error sector is marked.
 */
static u8
iscsi_iser_check_protection(struct iscsi_task *task, sector_t *sector)
{
	struct iscsi_iser_task *iser_task = task->dd_data;

	if (iser_task->dir[ISER_DIR_IN])
		return iser_check_task_pi_status(iser_task, ISER_DIR_IN,
						 sector);
	else
		return iser_check_task_pi_status(iser_task, ISER_DIR_OUT,
						 sector);
}

/**
 * iscsi_iser_conn_create() - create a new iscsi-iser connection
 * @cls_session: iscsi class connection
 * @conn_idx:    connection index within the session (for MCS)
 *
 * Return: iscsi_cls_conn when iscsi_conn_setup succeeds or NULL
 *         otherwise.
 */
static struct iscsi_cls_conn *
iscsi_iser_conn_create(struct iscsi_cls_session *cls_session,
		       uint32_t conn_idx)
{
	struct iscsi_conn *conn;
	struct iscsi_cls_conn *cls_conn;

	cls_conn = iscsi_conn_setup(cls_session, 0, conn_idx);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!cls_conn)
		return NULL;
	conn = cls_conn->dd_data;

	/*
	 * due to issues with the login code re iser sematics
	 * this not set in iscsi_conn_setup - FIXME
	 */
	conn->max_recv_dlength = ISER_RECV_DATA_SEG_LEN;

<<<<<<< HEAD
	iser_conn = conn->dd_data;
	conn->dd_data = iser_conn;
	iser_conn->iscsi_conn = conn;

	return cls_conn;
}

static void
iscsi_iser_conn_destroy(struct iscsi_cls_conn *cls_conn)
{
	struct iscsi_conn *conn = cls_conn->dd_data;
	struct iscsi_iser_conn *iser_conn = conn->dd_data;
	struct iser_conn *ib_conn = iser_conn->ib_conn;

	iscsi_conn_teardown(cls_conn);
	/*
	 * Userspace will normally call the stop callback and
	 * already have freed the ib_conn, but if it goofed up then
	 * we free it here.
	 */
	if (ib_conn) {
		ib_conn->iser_conn = NULL;
		iser_conn_put(ib_conn, 1); /* deref iscsi/ib conn unbinding */
	}
}

static int
iscsi_iser_conn_bind(struct iscsi_cls_session *cls_session,
		     struct iscsi_cls_conn *cls_conn, uint64_t transport_eph,
		     int is_leading)
{
	struct iscsi_conn *conn = cls_conn->dd_data;
	struct iscsi_iser_conn *iser_conn;
	struct iser_conn *ib_conn;
=======
	return cls_conn;
}

/**
 * iscsi_iser_conn_bind() - bind iscsi and iser connection structures
 * @cls_session:     iscsi class session
 * @cls_conn:        iscsi class connection
 * @transport_eph:   transport end-point handle
 * @is_leading:      indicate if this is the session leading connection (MCS)
 *
 * Return: zero on success, $error if iscsi_conn_bind fails and
 *         -EINVAL in case end-point doesn't exsits anymore or iser connection
 *         state is not UP (teardown already started).
 */
static int
iscsi_iser_conn_bind(struct iscsi_cls_session *cls_session,
		     struct iscsi_cls_conn *cls_conn,
		     uint64_t transport_eph,
		     int is_leading)
{
	struct iscsi_conn *conn = cls_conn->dd_data;
	struct iser_conn *iser_conn;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct iscsi_endpoint *ep;
	int error;

	error = iscsi_conn_bind(cls_session, cls_conn, is_leading);
	if (error)
		return error;

	/* the transport ep handle comes from user space so it must be
	 * verified against the global ib connections list */
	ep = iscsi_lookup_endpoint(transport_eph);
	if (!ep) {
		iser_err("can't bind eph %llx\n",
			 (unsigned long long)transport_eph);
		return -EINVAL;
	}
<<<<<<< HEAD
	ib_conn = ep->dd_data;

	if (iser_alloc_rx_descriptors(ib_conn))
		return -ENOMEM;
=======
	iser_conn = ep->dd_data;

	mutex_lock(&iser_conn->state_mutex);
	if (iser_conn->state != ISER_CONN_UP) {
		error = -EINVAL;
		iser_err("iser_conn %p state is %d, teardown started\n",
			 iser_conn, iser_conn->state);
		goto out;
	}

	error = iser_alloc_rx_descriptors(iser_conn, conn->session);
	if (error)
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* binds the iSER connection retrieved from the previously
	 * connected ep_handle to the iSCSI layer connection. exchanges
	 * connection pointers */
<<<<<<< HEAD
	iser_info("binding iscsi/iser conn %p %p to ib_conn %p\n",
		  conn, conn->dd_data, ib_conn);
	iser_conn = conn->dd_data;
	ib_conn->iser_conn = iser_conn;
	iser_conn->ib_conn  = ib_conn;
	iser_conn_get(ib_conn); /* ref iscsi/ib conn binding */
	return 0;
}

=======
	iser_info("binding iscsi conn %p to iser_conn %p\n", conn, iser_conn);

	conn->dd_data = iser_conn;
	iser_conn->iscsi_conn = conn;

out:
	mutex_unlock(&iser_conn->state_mutex);
	return error;
}

/**
 * iscsi_iser_conn_start() - start iscsi-iser connection
 * @cls_conn: iscsi class connection
 *
 * Notes: Here iser intialize (or re-initialize) stop_completion as
 *        from this point iscsi must call conn_stop in session/connection
 *        teardown so iser transport must wait for it.
 */
static int
iscsi_iser_conn_start(struct iscsi_cls_conn *cls_conn)
{
	struct iscsi_conn *iscsi_conn;
	struct iser_conn *iser_conn;

	iscsi_conn = cls_conn->dd_data;
	iser_conn = iscsi_conn->dd_data;
	reinit_completion(&iser_conn->stop_completion);

	return iscsi_conn_start(cls_conn);
}

/**
 * iscsi_iser_conn_stop() - stop iscsi-iser connection
 * @cls_conn:  iscsi class connection
 * @flag:      indicate if recover or terminate (passed as is)
 *
 * Notes: Calling iscsi_conn_stop might theoretically race with
 *        DEVICE_REMOVAL event and dereference a previously freed RDMA device
 *        handle, so we call it under iser the state lock to protect against
 *        this kind of race.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void
iscsi_iser_conn_stop(struct iscsi_cls_conn *cls_conn, int flag)
{
	struct iscsi_conn *conn = cls_conn->dd_data;
<<<<<<< HEAD
	struct iscsi_iser_conn *iser_conn = conn->dd_data;
	struct iser_conn *ib_conn = iser_conn->ib_conn;
=======
	struct iser_conn *iser_conn = conn->dd_data;

	iser_info("stopping iscsi_conn: %p, iser_conn: %p\n", conn, iser_conn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Userspace may have goofed up and not bound the connection or
	 * might have only partially setup the connection.
	 */
<<<<<<< HEAD
	if (ib_conn) {
		iscsi_conn_stop(cls_conn, flag);
		/*
		 * There is no unbind event so the stop callback
		 * must release the ref from the bind.
		 */
		iser_conn_put(ib_conn, 1); /* deref iscsi/ib conn unbinding */
	}
	iser_conn->ib_conn = NULL;
}

static void iscsi_iser_session_destroy(struct iscsi_cls_session *cls_session)
=======
	if (iser_conn) {
		mutex_lock(&iser_conn->state_mutex);
		mutex_lock(&unbind_iser_conn_mutex);
		iser_conn_terminate(iser_conn);
		iscsi_conn_stop(cls_conn, flag);

		/* unbind */
		iser_conn->iscsi_conn = NULL;
		conn->dd_data = NULL;
		mutex_unlock(&unbind_iser_conn_mutex);

		complete(&iser_conn->stop_completion);
		mutex_unlock(&iser_conn->state_mutex);
	} else {
		iscsi_conn_stop(cls_conn, flag);
	}
}

/**
 * iscsi_iser_session_destroy() - destroy iscsi-iser session
 * @cls_session: iscsi class session
 *
 * Removes and free iscsi host.
 */
static void
iscsi_iser_session_destroy(struct iscsi_cls_session *cls_session)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct Scsi_Host *shost = iscsi_session_to_shost(cls_session);

	iscsi_session_teardown(cls_session);
	iscsi_host_remove(shost);
	iscsi_host_free(shost);
}

<<<<<<< HEAD
=======
static inline unsigned int
iser_dif_prot_caps(int prot_caps)
{
	return ((prot_caps & IB_PROT_T10DIF_TYPE_1) ?
		SHOST_DIF_TYPE1_PROTECTION | SHOST_DIX_TYPE0_PROTECTION |
		SHOST_DIX_TYPE1_PROTECTION : 0) |
	       ((prot_caps & IB_PROT_T10DIF_TYPE_2) ?
		SHOST_DIF_TYPE2_PROTECTION | SHOST_DIX_TYPE2_PROTECTION : 0) |
	       ((prot_caps & IB_PROT_T10DIF_TYPE_3) ?
		SHOST_DIF_TYPE3_PROTECTION | SHOST_DIX_TYPE3_PROTECTION : 0);
}

/**
 * iscsi_iser_session_create() - create an iscsi-iser session
 * @ep:             iscsi end-point handle
 * @cmds_max:       maximum commands in this session
 * @qdepth:         session command queue depth
 * @initial_cmdsn:  initiator command sequnce number
 *
 * Allocates and adds a scsi host, expose DIF supprot if
 * exists, and sets up an iscsi session.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct iscsi_cls_session *
iscsi_iser_session_create(struct iscsi_endpoint *ep,
			  uint16_t cmds_max, uint16_t qdepth,
			  uint32_t initial_cmdsn)
{
	struct iscsi_cls_session *cls_session;
	struct iscsi_session *session;
	struct Scsi_Host *shost;
<<<<<<< HEAD
	struct iser_conn *ib_conn;
=======
	struct iser_conn *iser_conn = NULL;
	struct ib_conn *ib_conn;
	u32 max_fr_sectors;
	u16 max_cmds;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	shost = iscsi_host_alloc(&iscsi_iser_sht, 0, 0);
	if (!shost)
		return NULL;
	shost->transportt = iscsi_iser_scsi_transport;
<<<<<<< HEAD
=======
	shost->cmd_per_lun = qdepth;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	shost->max_lun = iscsi_max_lun;
	shost->max_id = 0;
	shost->max_channel = 0;
	shost->max_cmd_len = 16;

	/*
	 * older userspace tools (before 2.0-870) did not pass us
	 * the leading conn's ep so this will be NULL;
	 */
<<<<<<< HEAD
	if (ep)
		ib_conn = ep->dd_data;

	if (iscsi_host_add(shost,
			   ep ? ib_conn->device->ib_device->dma_device : NULL))
		goto free_host;

	/*
	 * we do not support setting can_queue cmd_per_lun from userspace yet
	 * because we preallocate so many resources
	 */
	cls_session = iscsi_session_setup(&iscsi_iser_transport, shost,
					  ISCSI_DEF_XMIT_CMDS_MAX, 0,
=======
	if (ep) {
		iser_conn = ep->dd_data;
		max_cmds = iser_conn->max_cmds;
		shost->sg_tablesize = iser_conn->scsi_sg_tablesize;

		mutex_lock(&iser_conn->state_mutex);
		if (iser_conn->state != ISER_CONN_UP) {
			iser_err("iser conn %p already started teardown\n",
				 iser_conn);
			mutex_unlock(&iser_conn->state_mutex);
			goto free_host;
		}

		ib_conn = &iser_conn->ib_conn;
		if (ib_conn->pi_support) {
			u32 sig_caps = ib_conn->device->ib_device->attrs.sig_prot_cap;

			shost->sg_prot_tablesize = shost->sg_tablesize;
			scsi_host_set_prot(shost, iser_dif_prot_caps(sig_caps));
			scsi_host_set_guard(shost, SHOST_DIX_GUARD_IP |
						   SHOST_DIX_GUARD_CRC);
		}

		if (iscsi_host_add(shost,
				   ib_conn->device->ib_device->dma_device)) {
			mutex_unlock(&iser_conn->state_mutex);
			goto free_host;
		}
		mutex_unlock(&iser_conn->state_mutex);
	} else {
		max_cmds = ISER_DEF_XMIT_CMDS_MAX;
		if (iscsi_host_add(shost, NULL))
			goto free_host;
	}

	/*
	 * FRs or FMRs can only map up to a (device) page per entry, but if the
	 * first entry is misaligned we'll end up using using two entries
	 * (head and tail) for a single page worth data, so we have to drop
	 * one segment from the calculation.
	 */
	max_fr_sectors = ((shost->sg_tablesize - 1) * PAGE_SIZE) >> 9;
	shost->max_sectors = min(iser_max_sectors, max_fr_sectors);

	if (cmds_max > max_cmds) {
		iser_info("cmds_max changed from %u to %u\n",
			  cmds_max, max_cmds);
		cmds_max = max_cmds;
	}

	cls_session = iscsi_session_setup(&iscsi_iser_transport, shost,
					  cmds_max, 0,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					  sizeof(struct iscsi_iser_task),
					  initial_cmdsn, 0);
	if (!cls_session)
		goto remove_host;
	session = cls_session->dd_data;

	shost->can_queue = session->scsi_cmds_max;
	return cls_session;

remove_host:
	iscsi_host_remove(shost);
free_host:
	iscsi_host_free(shost);
	return NULL;
}

static int
iscsi_iser_set_param(struct iscsi_cls_conn *cls_conn,
		     enum iscsi_param param, char *buf, int buflen)
{
	int value;

	switch (param) {
	case ISCSI_PARAM_MAX_RECV_DLENGTH:
		/* TBD */
		break;
	case ISCSI_PARAM_HDRDGST_EN:
		sscanf(buf, "%d", &value);
		if (value) {
<<<<<<< HEAD
			iser_err("DataDigest wasn't negotiated to None");
=======
			iser_err("DataDigest wasn't negotiated to None\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EPROTO;
		}
		break;
	case ISCSI_PARAM_DATADGST_EN:
		sscanf(buf, "%d", &value);
		if (value) {
<<<<<<< HEAD
			iser_err("DataDigest wasn't negotiated to None");
=======
			iser_err("DataDigest wasn't negotiated to None\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EPROTO;
		}
		break;
	case ISCSI_PARAM_IFMARKER_EN:
		sscanf(buf, "%d", &value);
		if (value) {
<<<<<<< HEAD
			iser_err("IFMarker wasn't negotiated to No");
=======
			iser_err("IFMarker wasn't negotiated to No\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EPROTO;
		}
		break;
	case ISCSI_PARAM_OFMARKER_EN:
		sscanf(buf, "%d", &value);
		if (value) {
<<<<<<< HEAD
			iser_err("OFMarker wasn't negotiated to No");
=======
			iser_err("OFMarker wasn't negotiated to No\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EPROTO;
		}
		break;
	default:
		return iscsi_set_param(cls_conn, param, buf, buflen);
	}

	return 0;
}

<<<<<<< HEAD
=======
/**
 * iscsi_iser_set_param() - set class connection parameter
 * @cls_conn:    iscsi class connection
 * @stats:       iscsi stats to output
 *
 * Output connection statistics.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void
iscsi_iser_conn_get_stats(struct iscsi_cls_conn *cls_conn, struct iscsi_stats *stats)
{
	struct iscsi_conn *conn = cls_conn->dd_data;

	stats->txdata_octets = conn->txdata_octets;
	stats->rxdata_octets = conn->rxdata_octets;
	stats->scsicmd_pdus = conn->scsicmd_pdus_cnt;
	stats->dataout_pdus = conn->dataout_pdus_cnt;
	stats->scsirsp_pdus = conn->scsirsp_pdus_cnt;
	stats->datain_pdus = conn->datain_pdus_cnt; /* always 0 */
	stats->r2t_pdus = conn->r2t_pdus_cnt; /* always 0 */
	stats->tmfcmd_pdus = conn->tmfcmd_pdus_cnt;
	stats->tmfrsp_pdus = conn->tmfrsp_pdus_cnt;
<<<<<<< HEAD
	stats->custom_length = 4;
	strcpy(stats->custom[0].desc, "qp_tx_queue_full");
	stats->custom[0].value = 0; /* TB iser_conn->qp_tx_queue_full; */
	strcpy(stats->custom[1].desc, "fmr_map_not_avail");
	stats->custom[1].value = 0; /* TB iser_conn->fmr_map_not_avail */;
	strcpy(stats->custom[2].desc, "eh_abort_cnt");
	stats->custom[2].value = conn->eh_abort_cnt;
	strcpy(stats->custom[3].desc, "fmr_unalign_cnt");
	stats->custom[3].value = conn->fmr_unalign_cnt;
=======
	stats->custom_length = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int iscsi_iser_get_ep_param(struct iscsi_endpoint *ep,
				   enum iscsi_param param, char *buf)
{
<<<<<<< HEAD
	struct iser_conn *ib_conn = ep->dd_data;
=======
	struct iser_conn *iser_conn = ep->dd_data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int len;

	switch (param) {
	case ISCSI_PARAM_CONN_PORT:
	case ISCSI_PARAM_CONN_ADDRESS:
<<<<<<< HEAD
		if (!ib_conn || !ib_conn->cma_id)
			return -ENOTCONN;

		return iscsi_conn_get_addr_param((struct sockaddr_storage *)
					&ib_conn->cma_id->route.addr.dst_addr,
					param, buf);
=======
		if (!iser_conn || !iser_conn->ib_conn.cma_id)
			return -ENOTCONN;

		return iscsi_conn_get_addr_param((struct sockaddr_storage *)
				&iser_conn->ib_conn.cma_id->route.addr.dst_addr,
				param, buf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	default:
		return -ENOSYS;
	}

	return len;
}

<<<<<<< HEAD
=======
/**
 * iscsi_iser_ep_connect() - Initiate iSER connection establishment
 * @shost:          scsi_host
 * @dst_addr:       destination address
 * @non-blocking:   indicate if routine can block
 *
 * Allocate an iscsi endpoint, an iser_conn structure and bind them.
 * After that start RDMA connection establishment via rdma_cm. We
 * don't allocate iser_conn embedded in iscsi_endpoint since in teardown
 * the endpoint will be destroyed at ep_disconnect while iser_conn will
 * cleanup its resources asynchronuously.
 *
 * Return: iscsi_endpoint created by iscsi layer or ERR_PTR(error)
 *         if fails.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct iscsi_endpoint *
iscsi_iser_ep_connect(struct Scsi_Host *shost, struct sockaddr *dst_addr,
		      int non_blocking)
{
	int err;
<<<<<<< HEAD
	struct iser_conn *ib_conn;
	struct iscsi_endpoint *ep;

	ep = iscsi_create_endpoint(sizeof(*ib_conn));
	if (!ep)
		return ERR_PTR(-ENOMEM);

	ib_conn = ep->dd_data;
	ib_conn->ep = ep;
	iser_conn_init(ib_conn);

	err = iser_connect(ib_conn, NULL, (struct sockaddr_in *)dst_addr,
			   non_blocking);
	if (err)
		return ERR_PTR(err);

	return ep;
}

static int
iscsi_iser_ep_poll(struct iscsi_endpoint *ep, int timeout_ms)
{
	struct iser_conn *ib_conn;
	int rc;

	ib_conn = ep->dd_data;
	rc = wait_event_interruptible_timeout(ib_conn->wait,
			     ib_conn->state == ISER_CONN_UP,
			     msecs_to_jiffies(timeout_ms));

	/* if conn establishment failed, return error code to iscsi */
	if (!rc &&
	    (ib_conn->state == ISER_CONN_TERMINATING ||
	     ib_conn->state == ISER_CONN_DOWN))
		rc = -1;

	iser_info("ib conn %p rc = %d\n", ib_conn, rc);
=======
	struct iser_conn *iser_conn;
	struct iscsi_endpoint *ep;

	ep = iscsi_create_endpoint(0);
	if (!ep)
		return ERR_PTR(-ENOMEM);

	iser_conn = kzalloc(sizeof(*iser_conn), GFP_KERNEL);
	if (!iser_conn) {
		err = -ENOMEM;
		goto failure;
	}

	ep->dd_data = iser_conn;
	iser_conn->ep = ep;
	iser_conn_init(iser_conn);

	err = iser_connect(iser_conn, NULL, dst_addr, non_blocking);
	if (err)
		goto failure;

	return ep;
failure:
	iscsi_destroy_endpoint(ep);
	return ERR_PTR(err);
}

/**
 * iscsi_iser_ep_poll() - poll for iser connection establishment to complete
 * @ep:            iscsi endpoint (created at ep_connect)
 * @timeout_ms:    polling timeout allowed in ms.
 *
 * This routine boils down to waiting for up_completion signaling
 * that cma_id got CONNECTED event.
 *
 * Return: 1 if succeeded in connection establishment, 0 if timeout expired
 *         (libiscsi will retry will kick in) or -1 if interrupted by signal
 *         or more likely iser connection state transitioned to TEMINATING or
 *         DOWN during the wait period.
 */
static int
iscsi_iser_ep_poll(struct iscsi_endpoint *ep, int timeout_ms)
{
	struct iser_conn *iser_conn = ep->dd_data;
	int rc;

	rc = wait_for_completion_interruptible_timeout(&iser_conn->up_completion,
						       msecs_to_jiffies(timeout_ms));
	/* if conn establishment failed, return error code to iscsi */
	if (rc == 0) {
		mutex_lock(&iser_conn->state_mutex);
		if (iser_conn->state == ISER_CONN_TERMINATING ||
		    iser_conn->state == ISER_CONN_DOWN)
			rc = -1;
		mutex_unlock(&iser_conn->state_mutex);
	}

	iser_info("iser conn %p rc = %d\n", iser_conn, rc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (rc > 0)
		return 1; /* success, this is the equivalent of POLLOUT */
	else if (!rc)
		return 0; /* timeout */
	else
		return rc; /* signal */
}

<<<<<<< HEAD
static void
iscsi_iser_ep_disconnect(struct iscsi_endpoint *ep)
{
	struct iser_conn *ib_conn;

	ib_conn = ep->dd_data;
	if (ib_conn->iser_conn)
		/*
		 * Must suspend xmit path if the ep is bound to the
		 * iscsi_conn, so we know we are not accessing the ib_conn
		 * when we free it.
		 *
		 * This may not be bound if the ep poll failed.
		 */
		iscsi_suspend_tx(ib_conn->iser_conn->iscsi_conn);


	iser_info("ib conn %p state %d\n", ib_conn, ib_conn->state);
	iser_conn_terminate(ib_conn);
=======
/**
 * iscsi_iser_ep_disconnect() - Initiate connection teardown process
 * @ep:    iscsi endpoint handle
 *
 * This routine is not blocked by iser and RDMA termination process
 * completion as we queue a deffered work for iser/RDMA destruction
 * and cleanup or actually call it immediately in case we didn't pass
 * iscsi conn bind/start stage, thus it is safe.
 */
static void
iscsi_iser_ep_disconnect(struct iscsi_endpoint *ep)
{
	struct iser_conn *iser_conn = ep->dd_data;

	iser_info("ep %p iser conn %p\n", ep, iser_conn);

	mutex_lock(&iser_conn->state_mutex);
	iser_conn_terminate(iser_conn);

	/*
	 * if iser_conn and iscsi_conn are bound, we must wait for
	 * iscsi_conn_stop and flush errors completion before freeing
	 * the iser resources. Otherwise we are safe to free resources
	 * immediately.
	 */
	if (iser_conn->iscsi_conn) {
		INIT_WORK(&iser_conn->release_work, iser_release_work);
		queue_work(release_wq, &iser_conn->release_work);
		mutex_unlock(&iser_conn->state_mutex);
	} else {
		iser_conn->state = ISER_CONN_DOWN;
		mutex_unlock(&iser_conn->state_mutex);
		iser_conn_release(iser_conn);
	}

	iscsi_destroy_endpoint(ep);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static umode_t iser_attr_is_visible(int param_type, int param)
{
	switch (param_type) {
	case ISCSI_HOST_PARAM:
		switch (param) {
		case ISCSI_HOST_PARAM_NETDEV_NAME:
		case ISCSI_HOST_PARAM_HWADDRESS:
		case ISCSI_HOST_PARAM_INITIATOR_NAME:
			return S_IRUGO;
		default:
			return 0;
		}
	case ISCSI_PARAM:
		switch (param) {
		case ISCSI_PARAM_MAX_RECV_DLENGTH:
		case ISCSI_PARAM_MAX_XMIT_DLENGTH:
		case ISCSI_PARAM_HDRDGST_EN:
		case ISCSI_PARAM_DATADGST_EN:
		case ISCSI_PARAM_CONN_ADDRESS:
		case ISCSI_PARAM_CONN_PORT:
		case ISCSI_PARAM_EXP_STATSN:
		case ISCSI_PARAM_PERSISTENT_ADDRESS:
		case ISCSI_PARAM_PERSISTENT_PORT:
		case ISCSI_PARAM_PING_TMO:
		case ISCSI_PARAM_RECV_TMO:
		case ISCSI_PARAM_INITIAL_R2T_EN:
		case ISCSI_PARAM_MAX_R2T:
		case ISCSI_PARAM_IMM_DATA_EN:
		case ISCSI_PARAM_FIRST_BURST:
		case ISCSI_PARAM_MAX_BURST:
		case ISCSI_PARAM_PDU_INORDER_EN:
		case ISCSI_PARAM_DATASEQ_INORDER_EN:
		case ISCSI_PARAM_TARGET_NAME:
		case ISCSI_PARAM_TPGT:
		case ISCSI_PARAM_USERNAME:
		case ISCSI_PARAM_PASSWORD:
		case ISCSI_PARAM_USERNAME_IN:
		case ISCSI_PARAM_PASSWORD_IN:
		case ISCSI_PARAM_FAST_ABORT:
		case ISCSI_PARAM_ABORT_TMO:
		case ISCSI_PARAM_LU_RESET_TMO:
		case ISCSI_PARAM_TGT_RESET_TMO:
		case ISCSI_PARAM_IFACE_NAME:
		case ISCSI_PARAM_INITIATOR_NAME:
<<<<<<< HEAD
=======
		case ISCSI_PARAM_DISCOVERY_SESS:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return S_IRUGO;
		default:
			return 0;
		}
	}

	return 0;
}

<<<<<<< HEAD
=======
static int iscsi_iser_slave_alloc(struct scsi_device *sdev)
{
	struct iscsi_session *session;
	struct iser_conn *iser_conn;
	struct ib_device *ib_dev;

	mutex_lock(&unbind_iser_conn_mutex);

	session = starget_to_session(scsi_target(sdev))->dd_data;
	iser_conn = session->leadconn->dd_data;
	if (!iser_conn) {
		mutex_unlock(&unbind_iser_conn_mutex);
		return -ENOTCONN;
	}
	ib_dev = iser_conn->ib_conn.device->ib_device;

	if (!(ib_dev->attrs.device_cap_flags & IB_DEVICE_SG_GAPS_REG))
		blk_queue_virt_boundary(sdev->request_queue, ~MASK_4K);

	mutex_unlock(&unbind_iser_conn_mutex);

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct scsi_host_template iscsi_iser_sht = {
	.module                 = THIS_MODULE,
	.name                   = "iSCSI Initiator over iSER",
	.queuecommand           = iscsi_queuecommand,
<<<<<<< HEAD
	.change_queue_depth	= iscsi_change_queue_depth,
	.sg_tablesize           = ISCSI_ISER_SG_TABLESIZE,
	.max_sectors		= 1024,
=======
	.change_queue_depth	= scsi_change_queue_depth,
	.sg_tablesize           = ISCSI_ISER_DEF_SG_TABLESIZE,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.cmd_per_lun            = ISER_DEF_CMD_PER_LUN,
	.eh_abort_handler       = iscsi_eh_abort,
	.eh_device_reset_handler= iscsi_eh_device_reset,
	.eh_target_reset_handler = iscsi_eh_recover_target,
	.target_alloc		= iscsi_target_alloc,
<<<<<<< HEAD
	.use_clustering         = DISABLE_CLUSTERING,
	.proc_name              = "iscsi_iser",
	.this_id                = -1,
=======
	.use_clustering         = ENABLE_CLUSTERING,
	.slave_alloc            = iscsi_iser_slave_alloc,
	.proc_name              = "iscsi_iser",
	.this_id                = -1,
	.track_queue_depth	= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct iscsi_transport iscsi_iser_transport = {
	.owner                  = THIS_MODULE,
	.name                   = "iser",
<<<<<<< HEAD
	.caps                   = CAP_RECOVERY_L0 | CAP_MULTI_R2T,
=======
	.caps                   = CAP_RECOVERY_L0 | CAP_MULTI_R2T | CAP_TEXT_NEGO,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* session management */
	.create_session         = iscsi_iser_session_create,
	.destroy_session        = iscsi_iser_session_destroy,
	/* connection management */
	.create_conn            = iscsi_iser_conn_create,
	.bind_conn              = iscsi_iser_conn_bind,
<<<<<<< HEAD
	.destroy_conn           = iscsi_iser_conn_destroy,
=======
	.destroy_conn           = iscsi_conn_teardown,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.attr_is_visible	= iser_attr_is_visible,
	.set_param              = iscsi_iser_set_param,
	.get_conn_param		= iscsi_conn_get_param,
	.get_ep_param		= iscsi_iser_get_ep_param,
	.get_session_param	= iscsi_session_get_param,
<<<<<<< HEAD
	.start_conn             = iscsi_conn_start,
=======
	.start_conn             = iscsi_iser_conn_start,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.stop_conn              = iscsi_iser_conn_stop,
	/* iscsi host params */
	.get_host_param		= iscsi_host_get_param,
	.set_host_param		= iscsi_host_set_param,
	/* IO */
	.send_pdu		= iscsi_conn_send_pdu,
	.get_stats		= iscsi_iser_conn_get_stats,
	.init_task		= iscsi_iser_task_init,
	.xmit_task		= iscsi_iser_task_xmit,
	.cleanup_task		= iscsi_iser_cleanup_task,
	.alloc_pdu		= iscsi_iser_pdu_alloc,
<<<<<<< HEAD
=======
	.check_protection	= iscsi_iser_check_protection,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* recovery */
	.session_recovery_timedout = iscsi_session_recovery_timedout,

	.ep_connect             = iscsi_iser_ep_connect,
	.ep_poll                = iscsi_iser_ep_poll,
	.ep_disconnect          = iscsi_iser_ep_disconnect
};

static int __init iser_init(void)
{
	int err;

	iser_dbg("Starting iSER datamover...\n");

	if (iscsi_max_lun < 1) {
		iser_err("Invalid max_lun value of %u\n", iscsi_max_lun);
		return -EINVAL;
	}

	memset(&ig, 0, sizeof(struct iser_global));

	ig.desc_cache = kmem_cache_create("iser_descriptors",
					  sizeof(struct iser_tx_desc),
					  0, SLAB_HWCACHE_ALIGN,
					  NULL);
	if (ig.desc_cache == NULL)
		return -ENOMEM;

	/* device init is called only after the first addr resolution */
	mutex_init(&ig.device_list_mutex);
	INIT_LIST_HEAD(&ig.device_list);
	mutex_init(&ig.connlist_mutex);
	INIT_LIST_HEAD(&ig.connlist);

<<<<<<< HEAD
=======
	release_wq = alloc_workqueue("release workqueue", 0, 0);
	if (!release_wq) {
		iser_err("failed to allocate release workqueue\n");
		err = -ENOMEM;
		goto err_alloc_wq;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	iscsi_iser_scsi_transport = iscsi_register_transport(
							&iscsi_iser_transport);
	if (!iscsi_iser_scsi_transport) {
		iser_err("iscsi_register_transport failed\n");
		err = -EINVAL;
<<<<<<< HEAD
		goto register_transport_failure;
=======
		goto err_reg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;

<<<<<<< HEAD
register_transport_failure:
=======
err_reg:
	destroy_workqueue(release_wq);
err_alloc_wq:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kmem_cache_destroy(ig.desc_cache);

	return err;
}

static void __exit iser_exit(void)
{
<<<<<<< HEAD
	iser_dbg("Removing iSER datamover...\n");
=======
	struct iser_conn *iser_conn, *n;
	int connlist_empty;

	iser_dbg("Removing iSER datamover...\n");
	destroy_workqueue(release_wq);

	mutex_lock(&ig.connlist_mutex);
	connlist_empty = list_empty(&ig.connlist);
	mutex_unlock(&ig.connlist_mutex);

	if (!connlist_empty) {
		iser_err("Error cleanup stage completed but we still have iser "
			 "connections, destroying them anyway\n");
		list_for_each_entry_safe(iser_conn, n, &ig.connlist,
					 conn_list) {
			iser_conn_release(iser_conn);
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	iscsi_unregister_transport(&iscsi_iser_transport);
	kmem_cache_destroy(ig.desc_cache);
}

module_init(iser_init);
module_exit(iser_exit);
