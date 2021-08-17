/*
 * Copyright (c) 2004, 2005, 2006 Voltaire, Inc. All rights reserved.
 * Copyright (c) 2005, 2006 Cisco Systems.  All rights reserved.
<<<<<<< HEAD
 * Copyright (c) 2013 Mellanox Technologies. All rights reserved.
=======
 * Copyright (c) 2013-2014 Mellanox Technologies. All rights reserved.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
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
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include "iscsi_iser.h"

#define ISCSI_ISER_MAX_CONN	8
<<<<<<< HEAD
#define ISER_MAX_RX_CQ_LEN	(ISER_QP_MAX_RECV_DTOS * ISCSI_ISER_MAX_CONN)
#define ISER_MAX_TX_CQ_LEN	(ISER_QP_MAX_REQ_DTOS  * ISCSI_ISER_MAX_CONN)

static void iser_cq_tasklet_fn(unsigned long data);
static void iser_cq_callback(struct ib_cq *cq, void *cq_context);

static void iser_cq_event_callback(struct ib_event *cause, void *context)
{
	iser_err("got cq event %d \n", cause->event);
}

static void iser_qp_event_callback(struct ib_event *cause, void *context)
{
	iser_err("got qp event %d\n",cause->event);
=======
#define ISER_MAX_RX_LEN		(ISER_QP_MAX_RECV_DTOS * ISCSI_ISER_MAX_CONN)
#define ISER_MAX_TX_LEN		(ISER_QP_MAX_REQ_DTOS  * ISCSI_ISER_MAX_CONN)
#define ISER_MAX_CQ_LEN		(ISER_MAX_RX_LEN + ISER_MAX_TX_LEN + \
				 ISCSI_ISER_MAX_CONN)

static void iser_qp_event_callback(struct ib_event *cause, void *context)
{
	iser_err("qp event %s (%d)\n",
		 ib_event_msg(cause->event), cause->event);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void iser_event_handler(struct ib_event_handler *handler,
				struct ib_event *event)
{
<<<<<<< HEAD
	iser_err("async event %d on device %s port %d\n", event->event,
		event->device->name, event->element.port_num);
=======
	iser_err("async event %s (%d) on device %s port %d\n",
		 ib_event_msg(event->event), event->event,
		 event->device->name, event->element.port_num);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * iser_create_device_ib_res - creates Protection Domain (PD), Completion
 * Queue (CQ), DMA Memory Region (DMA MR) with the device associated with
 * the adapator.
 *
 * returns 0 on success, -1 on failure
 */
static int iser_create_device_ib_res(struct iser_device *device)
{
<<<<<<< HEAD
	int i, j;
	struct iser_cq_desc *cq_desc;

	device->cqs_used = min(ISER_MAX_CQ, device->ib_device->num_comp_vectors);
	iser_info("using %d CQs, device %s supports %d vectors\n",
		  device->cqs_used, device->ib_device->name,
		  device->ib_device->num_comp_vectors);

	device->cq_desc = kmalloc(sizeof(struct iser_cq_desc) * device->cqs_used,
				  GFP_KERNEL);
	if (device->cq_desc == NULL)
		goto cq_desc_err;
	cq_desc = device->cq_desc;

	device->pd = ib_alloc_pd(device->ib_device);
	if (IS_ERR(device->pd))
		goto pd_err;

	for (i = 0; i < device->cqs_used; i++) {
		cq_desc[i].device   = device;
		cq_desc[i].cq_index = i;

		device->rx_cq[i] = ib_create_cq(device->ib_device,
					  iser_cq_callback,
					  iser_cq_event_callback,
					  (void *)&cq_desc[i],
					  ISER_MAX_RX_CQ_LEN, i);
		if (IS_ERR(device->rx_cq[i]))
			goto cq_err;

		device->tx_cq[i] = ib_create_cq(device->ib_device,
					  NULL, iser_cq_event_callback,
					  (void *)&cq_desc[i],
					  ISER_MAX_TX_CQ_LEN, i);

		if (IS_ERR(device->tx_cq[i]))
			goto cq_err;

		if (ib_req_notify_cq(device->rx_cq[i], IB_CQ_NEXT_COMP))
			goto cq_err;

		tasklet_init(&device->cq_tasklet[i],
			     iser_cq_tasklet_fn,
			(unsigned long)&cq_desc[i]);
	}

	device->mr = ib_get_dma_mr(device->pd, IB_ACCESS_LOCAL_WRITE |
				   IB_ACCESS_REMOTE_WRITE |
				   IB_ACCESS_REMOTE_READ);
	if (IS_ERR(device->mr))
		goto dma_mr_err;

	INIT_IB_EVENT_HANDLER(&device->event_handler, device->ib_device,
				iser_event_handler);
	if (ib_register_event_handler(&device->event_handler))
		goto handler_err;

	return 0;

handler_err:
	ib_dereg_mr(device->mr);
dma_mr_err:
	for (j = 0; j < device->cqs_used; j++)
		tasklet_kill(&device->cq_tasklet[j]);
cq_err:
	for (j = 0; j < i; j++) {
		if (device->tx_cq[j])
			ib_destroy_cq(device->tx_cq[j]);
		if (device->rx_cq[j])
			ib_destroy_cq(device->rx_cq[j]);
	}
	ib_dealloc_pd(device->pd);
pd_err:
	kfree(device->cq_desc);
cq_desc_err:
=======
	struct ib_device *ib_dev = device->ib_device;
	int ret, i, max_cqe;

	ret = iser_assign_reg_ops(device);
	if (ret)
		return ret;

	device->comps_used = min_t(int, num_online_cpus(),
				 ib_dev->num_comp_vectors);

	device->comps = kcalloc(device->comps_used, sizeof(*device->comps),
				GFP_KERNEL);
	if (!device->comps)
		goto comps_err;

	max_cqe = min(ISER_MAX_CQ_LEN, ib_dev->attrs.max_cqe);

	iser_info("using %d CQs, device %s supports %d vectors max_cqe %d\n",
		  device->comps_used, ib_dev->name,
		  ib_dev->num_comp_vectors, max_cqe);

	device->pd = ib_alloc_pd(ib_dev,
		iser_always_reg ? 0 : IB_PD_UNSAFE_GLOBAL_RKEY);
	if (IS_ERR(device->pd))
		goto pd_err;

	for (i = 0; i < device->comps_used; i++) {
		struct iser_comp *comp = &device->comps[i];

		comp->cq = ib_alloc_cq(ib_dev, comp, max_cqe, i,
				       IB_POLL_SOFTIRQ);
		if (IS_ERR(comp->cq)) {
			comp->cq = NULL;
			goto cq_err;
		}
	}

	INIT_IB_EVENT_HANDLER(&device->event_handler, ib_dev,
			      iser_event_handler);
	if (ib_register_event_handler(&device->event_handler))
		goto cq_err;

	return 0;

cq_err:
	for (i = 0; i < device->comps_used; i++) {
		struct iser_comp *comp = &device->comps[i];

		if (comp->cq)
			ib_free_cq(comp->cq);
	}
	ib_dealloc_pd(device->pd);
pd_err:
	kfree(device->comps);
comps_err:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	iser_err("failed to allocate an IB resource\n");
	return -1;
}

/**
 * iser_free_device_ib_res - destroy/dealloc/dereg the DMA MR,
 * CQ and PD created with the device associated with the adapator.
 */
static void iser_free_device_ib_res(struct iser_device *device)
{
	int i;
<<<<<<< HEAD
	BUG_ON(device->mr == NULL);

	for (i = 0; i < device->cqs_used; i++) {
		tasklet_kill(&device->cq_tasklet[i]);
		(void)ib_destroy_cq(device->tx_cq[i]);
		(void)ib_destroy_cq(device->rx_cq[i]);
		device->tx_cq[i] = NULL;
		device->rx_cq[i] = NULL;
	}

	(void)ib_unregister_event_handler(&device->event_handler);
	(void)ib_dereg_mr(device->mr);
	(void)ib_dealloc_pd(device->pd);

	kfree(device->cq_desc);

	device->mr = NULL;
=======

	for (i = 0; i < device->comps_used; i++) {
		struct iser_comp *comp = &device->comps[i];

		ib_free_cq(comp->cq);
		comp->cq = NULL;
	}

	(void)ib_unregister_event_handler(&device->event_handler);
	ib_dealloc_pd(device->pd);

	kfree(device->comps);
	device->comps = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	device->pd = NULL;
}

/**
<<<<<<< HEAD
 * iser_create_ib_conn_res - Creates FMR pool and Queue-Pair (QP)
 *
 * returns 0 on success, -1 on failure
 */
static int iser_create_ib_conn_res(struct iser_conn *ib_conn)
{
	struct iser_device	*device;
	struct ib_qp_init_attr	init_attr;
	int			req_err, resp_err, ret = -ENOMEM;
	struct ib_fmr_pool_param params;
	int index, min_index = 0;

	BUG_ON(ib_conn->device == NULL);

	device = ib_conn->device;

	ib_conn->login_buf = kmalloc(ISCSI_DEF_MAX_RECV_SEG_LEN +
					ISER_RX_LOGIN_SIZE, GFP_KERNEL);
	if (!ib_conn->login_buf)
		goto out_err;

	ib_conn->login_req_buf  = ib_conn->login_buf;
	ib_conn->login_resp_buf = ib_conn->login_buf + ISCSI_DEF_MAX_RECV_SEG_LEN;

	ib_conn->login_req_dma = ib_dma_map_single(ib_conn->device->ib_device,
				(void *)ib_conn->login_req_buf,
				ISCSI_DEF_MAX_RECV_SEG_LEN, DMA_TO_DEVICE);

	ib_conn->login_resp_dma = ib_dma_map_single(ib_conn->device->ib_device,
				(void *)ib_conn->login_resp_buf,
				ISER_RX_LOGIN_SIZE, DMA_FROM_DEVICE);

	req_err  = ib_dma_mapping_error(device->ib_device, ib_conn->login_req_dma);
	resp_err = ib_dma_mapping_error(device->ib_device, ib_conn->login_resp_dma);

	if (req_err || resp_err) {
		if (req_err)
			ib_conn->login_req_dma = 0;
		if (resp_err)
			ib_conn->login_resp_dma = 0;
		goto out_err;
	}

	ib_conn->page_vec = kmalloc(sizeof(struct iser_page_vec) +
				    (sizeof(u64) * (ISCSI_ISER_SG_TABLESIZE +1)),
				    GFP_KERNEL);
	if (!ib_conn->page_vec)
		goto out_err;

	ib_conn->page_vec->pages = (u64 *) (ib_conn->page_vec + 1);

	params.page_shift        = SHIFT_4K;
	/* when the first/last SG element are not start/end *
	 * page aligned, the map whould be of N+1 pages     */
	params.max_pages_per_fmr = ISCSI_ISER_SG_TABLESIZE + 1;
	/* make the pool size twice the max number of SCSI commands *
	 * the ML is expected to queue, watermark for unmap at 50%  */
	params.pool_size	 = ISCSI_DEF_XMIT_CMDS_MAX * 2;
	params.dirty_watermark	 = ISCSI_DEF_XMIT_CMDS_MAX;
=======
 * iser_alloc_fmr_pool - Creates FMR pool and page_vector
 *
 * returns 0 on success, or errno code on failure
 */
int iser_alloc_fmr_pool(struct ib_conn *ib_conn,
			unsigned cmds_max,
			unsigned int size)
{
	struct iser_device *device = ib_conn->device;
	struct iser_fr_pool *fr_pool = &ib_conn->fr_pool;
	struct iser_page_vec *page_vec;
	struct iser_fr_desc *desc;
	struct ib_fmr_pool *fmr_pool;
	struct ib_fmr_pool_param params;
	int ret;

	INIT_LIST_HEAD(&fr_pool->list);
	spin_lock_init(&fr_pool->lock);

	desc = kzalloc(sizeof(*desc), GFP_KERNEL);
	if (!desc)
		return -ENOMEM;

	page_vec = kmalloc(sizeof(*page_vec) + (sizeof(u64) * size),
			   GFP_KERNEL);
	if (!page_vec) {
		ret = -ENOMEM;
		goto err_frpl;
	}

	page_vec->pages = (u64 *)(page_vec + 1);

	params.page_shift        = SHIFT_4K;
	params.max_pages_per_fmr = size;
	/* make the pool size twice the max number of SCSI commands *
	 * the ML is expected to queue, watermark for unmap at 50%  */
	params.pool_size	 = cmds_max * 2;
	params.dirty_watermark	 = cmds_max;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	params.cache		 = 0;
	params.flush_function	 = NULL;
	params.access		 = (IB_ACCESS_LOCAL_WRITE  |
				    IB_ACCESS_REMOTE_WRITE |
				    IB_ACCESS_REMOTE_READ);

<<<<<<< HEAD
	ib_conn->fmr_pool = ib_create_fmr_pool(device->pd, &params);
	ret = PTR_ERR(ib_conn->fmr_pool);
	if (IS_ERR(ib_conn->fmr_pool) && ret != -ENOSYS) {
		ib_conn->fmr_pool = NULL;
		goto out_err;
	} else if (ret == -ENOSYS) {
		ib_conn->fmr_pool = NULL;
		iser_warn("FMRs are not supported, using unaligned mode\n");
		ret = 0;
	}

=======
	fmr_pool = ib_create_fmr_pool(device->pd, &params);
	if (IS_ERR(fmr_pool)) {
		ret = PTR_ERR(fmr_pool);
		iser_err("FMR allocation failed, err %d\n", ret);
		goto err_fmr;
	}

	desc->rsc.page_vec = page_vec;
	desc->rsc.fmr_pool = fmr_pool;
	list_add(&desc->list, &fr_pool->list);

	return 0;

err_fmr:
	kfree(page_vec);
err_frpl:
	kfree(desc);

	return ret;
}

/**
 * iser_free_fmr_pool - releases the FMR pool and page vec
 */
void iser_free_fmr_pool(struct ib_conn *ib_conn)
{
	struct iser_fr_pool *fr_pool = &ib_conn->fr_pool;
	struct iser_fr_desc *desc;

	desc = list_first_entry(&fr_pool->list,
				struct iser_fr_desc, list);
	list_del(&desc->list);

	iser_info("freeing conn %p fmr pool %p\n",
		  ib_conn, desc->rsc.fmr_pool);

	ib_destroy_fmr_pool(desc->rsc.fmr_pool);
	kfree(desc->rsc.page_vec);
	kfree(desc);
}

static int
iser_alloc_reg_res(struct iser_device *device,
		   struct ib_pd *pd,
		   struct iser_reg_resources *res,
		   unsigned int size)
{
	struct ib_device *ib_dev = device->ib_device;
	enum ib_mr_type mr_type;
	int ret;

	if (ib_dev->attrs.device_cap_flags & IB_DEVICE_SG_GAPS_REG)
		mr_type = IB_MR_TYPE_SG_GAPS;
	else
		mr_type = IB_MR_TYPE_MEM_REG;

	res->mr = ib_alloc_mr(pd, mr_type, size);
	if (IS_ERR(res->mr)) {
		ret = PTR_ERR(res->mr);
		iser_err("Failed to allocate ib_fast_reg_mr err=%d\n", ret);
		return ret;
	}
	res->mr_valid = 0;

	return 0;
}

static void
iser_free_reg_res(struct iser_reg_resources *rsc)
{
	ib_dereg_mr(rsc->mr);
}

static int
iser_alloc_pi_ctx(struct iser_device *device,
		  struct ib_pd *pd,
		  struct iser_fr_desc *desc,
		  unsigned int size)
{
	struct iser_pi_context *pi_ctx = NULL;
	int ret;

	desc->pi_ctx = kzalloc(sizeof(*desc->pi_ctx), GFP_KERNEL);
	if (!desc->pi_ctx)
		return -ENOMEM;

	pi_ctx = desc->pi_ctx;

	ret = iser_alloc_reg_res(device, pd, &pi_ctx->rsc, size);
	if (ret) {
		iser_err("failed to allocate reg_resources\n");
		goto alloc_reg_res_err;
	}

	pi_ctx->sig_mr = ib_alloc_mr(pd, IB_MR_TYPE_SIGNATURE, 2);
	if (IS_ERR(pi_ctx->sig_mr)) {
		ret = PTR_ERR(pi_ctx->sig_mr);
		goto sig_mr_failure;
	}
	pi_ctx->sig_mr_valid = 0;
	desc->pi_ctx->sig_protected = 0;

	return 0;

sig_mr_failure:
	iser_free_reg_res(&pi_ctx->rsc);
alloc_reg_res_err:
	kfree(desc->pi_ctx);

	return ret;
}

static void
iser_free_pi_ctx(struct iser_pi_context *pi_ctx)
{
	iser_free_reg_res(&pi_ctx->rsc);
	ib_dereg_mr(pi_ctx->sig_mr);
	kfree(pi_ctx);
}

static struct iser_fr_desc *
iser_create_fastreg_desc(struct iser_device *device,
			 struct ib_pd *pd,
			 bool pi_enable,
			 unsigned int size)
{
	struct iser_fr_desc *desc;
	int ret;

	desc = kzalloc(sizeof(*desc), GFP_KERNEL);
	if (!desc)
		return ERR_PTR(-ENOMEM);

	ret = iser_alloc_reg_res(device, pd, &desc->rsc, size);
	if (ret)
		goto reg_res_alloc_failure;

	if (pi_enable) {
		ret = iser_alloc_pi_ctx(device, pd, desc, size);
		if (ret)
			goto pi_ctx_alloc_failure;
	}

	return desc;

pi_ctx_alloc_failure:
	iser_free_reg_res(&desc->rsc);
reg_res_alloc_failure:
	kfree(desc);

	return ERR_PTR(ret);
}

/**
 * iser_alloc_fastreg_pool - Creates pool of fast_reg descriptors
 * for fast registration work requests.
 * returns 0 on success, or errno code on failure
 */
int iser_alloc_fastreg_pool(struct ib_conn *ib_conn,
			    unsigned cmds_max,
			    unsigned int size)
{
	struct iser_device *device = ib_conn->device;
	struct iser_fr_pool *fr_pool = &ib_conn->fr_pool;
	struct iser_fr_desc *desc;
	int i, ret;

	INIT_LIST_HEAD(&fr_pool->list);
	INIT_LIST_HEAD(&fr_pool->all_list);
	spin_lock_init(&fr_pool->lock);
	fr_pool->size = 0;
	for (i = 0; i < cmds_max; i++) {
		desc = iser_create_fastreg_desc(device, device->pd,
						ib_conn->pi_support, size);
		if (IS_ERR(desc)) {
			ret = PTR_ERR(desc);
			goto err;
		}

		list_add_tail(&desc->list, &fr_pool->list);
		list_add_tail(&desc->all_list, &fr_pool->all_list);
		fr_pool->size++;
	}

	return 0;

err:
	iser_free_fastreg_pool(ib_conn);
	return ret;
}

/**
 * iser_free_fastreg_pool - releases the pool of fast_reg descriptors
 */
void iser_free_fastreg_pool(struct ib_conn *ib_conn)
{
	struct iser_fr_pool *fr_pool = &ib_conn->fr_pool;
	struct iser_fr_desc *desc, *tmp;
	int i = 0;

	if (list_empty(&fr_pool->all_list))
		return;

	iser_info("freeing conn %p fr pool\n", ib_conn);

	list_for_each_entry_safe(desc, tmp, &fr_pool->all_list, all_list) {
		list_del(&desc->all_list);
		iser_free_reg_res(&desc->rsc);
		if (desc->pi_ctx)
			iser_free_pi_ctx(desc->pi_ctx);
		kfree(desc);
		++i;
	}

	if (i < fr_pool->size)
		iser_warn("pool still has %d regions registered\n",
			  fr_pool->size - i);
}

/**
 * iser_create_ib_conn_res - Queue-Pair (QP)
 *
 * returns 0 on success, -1 on failure
 */
static int iser_create_ib_conn_res(struct ib_conn *ib_conn)
{
	struct iser_conn *iser_conn = to_iser_conn(ib_conn);
	struct iser_device	*device;
	struct ib_device	*ib_dev;
	struct ib_qp_init_attr	init_attr;
	int			ret = -ENOMEM;
	int index, min_index = 0;

	BUG_ON(ib_conn->device == NULL);

	device = ib_conn->device;
	ib_dev = device->ib_device;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	memset(&init_attr, 0, sizeof init_attr);

	mutex_lock(&ig.connlist_mutex);
	/* select the CQ with the minimal number of usages */
<<<<<<< HEAD
	for (index = 0; index < device->cqs_used; index++)
		if (device->cq_active_qps[index] <
		    device->cq_active_qps[min_index])
			min_index = index;
	device->cq_active_qps[min_index]++;
=======
	for (index = 0; index < device->comps_used; index++) {
		if (device->comps[index].active_qps <
		    device->comps[min_index].active_qps)
			min_index = index;
	}
	ib_conn->comp = &device->comps[min_index];
	ib_conn->comp->active_qps++;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&ig.connlist_mutex);
	iser_info("cq index %d used for ib_conn %p\n", min_index, ib_conn);

	init_attr.event_handler = iser_qp_event_callback;
	init_attr.qp_context	= (void *)ib_conn;
<<<<<<< HEAD
	init_attr.send_cq	= device->tx_cq[min_index];
	init_attr.recv_cq	= device->rx_cq[min_index];
	init_attr.cap.max_send_wr  = ISER_QP_MAX_REQ_DTOS;
=======
	init_attr.send_cq	= ib_conn->comp->cq;
	init_attr.recv_cq	= ib_conn->comp->cq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	init_attr.cap.max_recv_wr  = ISER_QP_MAX_RECV_DTOS;
	init_attr.cap.max_send_sge = 2;
	init_attr.cap.max_recv_sge = 1;
	init_attr.sq_sig_type	= IB_SIGNAL_REQ_WR;
	init_attr.qp_type	= IB_QPT_RC;
<<<<<<< HEAD
=======
	if (ib_conn->pi_support) {
		init_attr.cap.max_send_wr = ISER_QP_SIG_MAX_REQ_DTOS + 1;
		init_attr.create_flags |= IB_QP_CREATE_SIGNATURE_EN;
		iser_conn->max_cmds =
			ISER_GET_MAX_XMIT_CMDS(ISER_QP_SIG_MAX_REQ_DTOS);
	} else {
		if (ib_dev->attrs.max_qp_wr > ISER_QP_MAX_REQ_DTOS) {
			init_attr.cap.max_send_wr  = ISER_QP_MAX_REQ_DTOS + 1;
			iser_conn->max_cmds =
				ISER_GET_MAX_XMIT_CMDS(ISER_QP_MAX_REQ_DTOS);
		} else {
			init_attr.cap.max_send_wr = ib_dev->attrs.max_qp_wr;
			iser_conn->max_cmds =
				ISER_GET_MAX_XMIT_CMDS(ib_dev->attrs.max_qp_wr);
			iser_dbg("device %s supports max_send_wr %d\n",
				 device->ib_device->name, ib_dev->attrs.max_qp_wr);
		}
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = rdma_create_qp(ib_conn->cma_id, device->pd, &init_attr);
	if (ret)
		goto out_err;

	ib_conn->qp = ib_conn->cma_id->qp;
<<<<<<< HEAD
	iser_info("setting conn %p cma_id %p: fmr_pool %p qp %p\n",
		  ib_conn, ib_conn->cma_id,
		  ib_conn->fmr_pool, ib_conn->cma_id->qp);
	return ret;

out_err:
	iser_err("unable to alloc mem or create resource, err %d\n", ret);
	return ret;
}

/**
 * releases the FMR pool and QP objects, returns 0 on success,
 * -1 on failure
 */
static int iser_free_ib_conn_res(struct iser_conn *ib_conn)
{
	int cq_index;
	BUG_ON(ib_conn == NULL);

	iser_info("freeing conn %p cma_id %p fmr pool %p qp %p\n",
		  ib_conn, ib_conn->cma_id,
		  ib_conn->fmr_pool, ib_conn->qp);

	/* qp is created only once both addr & route are resolved */
	if (ib_conn->fmr_pool != NULL)
		ib_destroy_fmr_pool(ib_conn->fmr_pool);

	if (ib_conn->qp != NULL) {
		cq_index = ((struct iser_cq_desc *)ib_conn->qp->recv_cq->cq_context)->cq_index;
		ib_conn->device->cq_active_qps[cq_index]--;

		rdma_destroy_qp(ib_conn->cma_id);
	}

	ib_conn->fmr_pool = NULL;
	ib_conn->qp	  = NULL;
	kfree(ib_conn->page_vec);

	if (ib_conn->login_buf) {
		if (ib_conn->login_req_dma)
			ib_dma_unmap_single(ib_conn->device->ib_device,
				ib_conn->login_req_dma,
				ISCSI_DEF_MAX_RECV_SEG_LEN, DMA_TO_DEVICE);
		if (ib_conn->login_resp_dma)
			ib_dma_unmap_single(ib_conn->device->ib_device,
				ib_conn->login_resp_dma,
				ISER_RX_LOGIN_SIZE, DMA_FROM_DEVICE);
		kfree(ib_conn->login_buf);
	}

	return 0;
=======
	iser_info("setting conn %p cma_id %p qp %p\n",
		  ib_conn, ib_conn->cma_id,
		  ib_conn->cma_id->qp);
	return ret;

out_err:
	mutex_lock(&ig.connlist_mutex);
	ib_conn->comp->active_qps--;
	mutex_unlock(&ig.connlist_mutex);
	iser_err("unable to alloc mem or create resource, err %d\n", ret);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * based on the resolved device node GUID see if there already allocated
 * device for this device. If there's no such, create one.
 */
static
struct iser_device *iser_device_find_by_ib_device(struct rdma_cm_id *cma_id)
{
	struct iser_device *device;

	mutex_lock(&ig.device_list_mutex);

	list_for_each_entry(device, &ig.device_list, ig_list)
		/* find if there's a match using the node GUID */
		if (device->ib_device->node_guid == cma_id->device->node_guid)
			goto inc_refcnt;

	device = kzalloc(sizeof *device, GFP_KERNEL);
	if (device == NULL)
		goto out;

	/* assign this device to the device */
	device->ib_device = cma_id->device;
	/* init the device and link it into ig device list */
	if (iser_create_device_ib_res(device)) {
		kfree(device);
		device = NULL;
		goto out;
	}
	list_add(&device->ig_list, &ig.device_list);

inc_refcnt:
	device->refcount++;
out:
	mutex_unlock(&ig.device_list_mutex);
	return device;
}

/* if there's no demand for this device, release it */
static void iser_device_try_release(struct iser_device *device)
{
	mutex_lock(&ig.device_list_mutex);
	device->refcount--;
	iser_info("device %p refcount %d\n", device, device->refcount);
	if (!device->refcount) {
		iser_free_device_ib_res(device);
		list_del(&device->ig_list);
		kfree(device);
	}
	mutex_unlock(&ig.device_list_mutex);
}

<<<<<<< HEAD
static int iser_conn_state_comp_exch(struct iser_conn *ib_conn,
				     enum iser_ib_conn_state comp,
				     enum iser_ib_conn_state exch)
{
	int ret;

	spin_lock_bh(&ib_conn->lock);
	if ((ret = (ib_conn->state == comp)))
		ib_conn->state = exch;
	spin_unlock_bh(&ib_conn->lock);
	return ret;
}

/**
 * Frees all conn objects and deallocs conn descriptor
 */
static void iser_conn_release(struct iser_conn *ib_conn, int can_destroy_id)
{
	struct iser_device  *device = ib_conn->device;

	BUG_ON(ib_conn->state != ISER_CONN_DOWN);

	mutex_lock(&ig.connlist_mutex);
	list_del(&ib_conn->conn_list);
	mutex_unlock(&ig.connlist_mutex);
	iser_free_rx_descriptors(ib_conn);
	iser_free_ib_conn_res(ib_conn);
	ib_conn->device = NULL;
	/* on EVENT_ADDR_ERROR there's no device yet for this conn */
	if (device != NULL)
		iser_device_try_release(device);
	/* if cma handler context, the caller actually destroy the id */
	if (ib_conn->cma_id != NULL && can_destroy_id) {
		rdma_destroy_id(ib_conn->cma_id);
		ib_conn->cma_id = NULL;
	}
	iscsi_destroy_endpoint(ib_conn->ep);
}

void iser_conn_get(struct iser_conn *ib_conn)
{
	atomic_inc(&ib_conn->refcount);
}

int iser_conn_put(struct iser_conn *ib_conn, int can_destroy_id)
{
	if (atomic_dec_and_test(&ib_conn->refcount)) {
		iser_conn_release(ib_conn, can_destroy_id);
		return 1;
	}
	return 0;
=======
/**
 * Called with state mutex held
 **/
static int iser_conn_state_comp_exch(struct iser_conn *iser_conn,
				     enum iser_conn_state comp,
				     enum iser_conn_state exch)
{
	int ret;

	ret = (iser_conn->state == comp);
	if (ret)
		iser_conn->state = exch;

	return ret;
}

void iser_release_work(struct work_struct *work)
{
	struct iser_conn *iser_conn;

	iser_conn = container_of(work, struct iser_conn, release_work);

	/* Wait for conn_stop to complete */
	wait_for_completion(&iser_conn->stop_completion);
	/* Wait for IB resouces cleanup to complete */
	wait_for_completion(&iser_conn->ib_completion);

	mutex_lock(&iser_conn->state_mutex);
	iser_conn->state = ISER_CONN_DOWN;
	mutex_unlock(&iser_conn->state_mutex);

	iser_conn_release(iser_conn);
}

/**
 * iser_free_ib_conn_res - release IB related resources
 * @iser_conn: iser connection struct
 * @destroy: indicator if we need to try to release the
 *     iser device and memory regoins pool (only iscsi
 *     shutdown and DEVICE_REMOVAL will use this).
 *
 * This routine is called with the iser state mutex held
 * so the cm_id removal is out of here. It is Safe to
 * be invoked multiple times.
 */
static void iser_free_ib_conn_res(struct iser_conn *iser_conn,
				  bool destroy)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	struct iser_device *device = ib_conn->device;

	iser_info("freeing conn %p cma_id %p qp %p\n",
		  iser_conn, ib_conn->cma_id, ib_conn->qp);

	if (ib_conn->qp != NULL) {
		ib_conn->comp->active_qps--;
		rdma_destroy_qp(ib_conn->cma_id);
		ib_conn->qp = NULL;
	}

	if (destroy) {
		if (iser_conn->rx_descs)
			iser_free_rx_descriptors(iser_conn);

		if (device != NULL) {
			iser_device_try_release(device);
			ib_conn->device = NULL;
		}
	}
}

/**
 * Frees all conn objects and deallocs conn descriptor
 */
void iser_conn_release(struct iser_conn *iser_conn)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;

	mutex_lock(&ig.connlist_mutex);
	list_del(&iser_conn->conn_list);
	mutex_unlock(&ig.connlist_mutex);

	mutex_lock(&iser_conn->state_mutex);
	/* In case we endup here without ep_disconnect being invoked. */
	if (iser_conn->state != ISER_CONN_DOWN) {
		iser_warn("iser conn %p state %d, expected state down.\n",
			  iser_conn, iser_conn->state);
		iscsi_destroy_endpoint(iser_conn->ep);
		iser_conn->state = ISER_CONN_DOWN;
	}
	/*
	 * In case we never got to bind stage, we still need to
	 * release IB resources (which is safe to call more than once).
	 */
	iser_free_ib_conn_res(iser_conn, true);
	mutex_unlock(&iser_conn->state_mutex);

	if (ib_conn->cma_id != NULL) {
		rdma_destroy_id(ib_conn->cma_id);
		ib_conn->cma_id = NULL;
	}

	kfree(iser_conn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * triggers start of the disconnect procedures and wait for them to be done
<<<<<<< HEAD
 */
void iser_conn_terminate(struct iser_conn *ib_conn)
{
	int err = 0;

	/* change the ib conn state only if the conn is UP, however always call
	 * rdma_disconnect since this is the only way to cause the CMA to change
	 * the QP state to ERROR
	 */

	iser_conn_state_comp_exch(ib_conn, ISER_CONN_UP, ISER_CONN_TERMINATING);
	err = rdma_disconnect(ib_conn->cma_id);
	if (err)
		iser_err("Failed to disconnect, conn: 0x%p err %d\n",
			 ib_conn,err);

	wait_event_interruptible(ib_conn->wait,
				 ib_conn->state == ISER_CONN_DOWN);

	iser_conn_put(ib_conn, 1); /* deref ib conn deallocate */
}

static int iser_connect_error(struct rdma_cm_id *cma_id)
{
	struct iser_conn *ib_conn;
	ib_conn = (struct iser_conn *)cma_id->context;

	ib_conn->state = ISER_CONN_DOWN;
	wake_up_interruptible(&ib_conn->wait);
	return iser_conn_put(ib_conn, 0); /* deref ib conn's cma id */
}

static int iser_addr_handler(struct rdma_cm_id *cma_id)
{
	struct iser_device *device;
	struct iser_conn   *ib_conn;
	int    ret;

	device = iser_device_find_by_ib_device(cma_id);
	if (!device) {
		iser_err("device lookup/creation failed\n");
		return iser_connect_error(cma_id);
	}

	ib_conn = (struct iser_conn *)cma_id->context;
	ib_conn->device = device;

	ret = rdma_resolve_route(cma_id, 1000);
	if (ret) {
		iser_err("resolve route failed: %d\n", ret);
		return iser_connect_error(cma_id);
	}

	return 0;
}

static int iser_route_handler(struct rdma_cm_id *cma_id)
=======
 * Called with state mutex held
 */
int iser_conn_terminate(struct iser_conn *iser_conn)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	int err = 0;

	/* terminate the iser conn only if the conn state is UP */
	if (!iser_conn_state_comp_exch(iser_conn, ISER_CONN_UP,
				       ISER_CONN_TERMINATING))
		return 0;

	iser_info("iser_conn %p state %d\n", iser_conn, iser_conn->state);

	/* suspend queuing of new iscsi commands */
	if (iser_conn->iscsi_conn)
		iscsi_suspend_queue(iser_conn->iscsi_conn);

	/*
	 * In case we didn't already clean up the cma_id (peer initiated
	 * a disconnection), we need to Cause the CMA to change the QP
	 * state to ERROR.
	 */
	if (ib_conn->cma_id) {
		err = rdma_disconnect(ib_conn->cma_id);
		if (err)
			iser_err("Failed to disconnect, conn: 0x%p err %d\n",
				 iser_conn, err);

		/* block until all flush errors are consumed */
		ib_drain_sq(ib_conn->qp);
	}

	return 1;
}

/**
 * Called with state mutex held
 **/
static void iser_connect_error(struct rdma_cm_id *cma_id)
{
	struct iser_conn *iser_conn;

	iser_conn = (struct iser_conn *)cma_id->context;
	iser_conn->state = ISER_CONN_TERMINATING;
}

static void
iser_calc_scsi_params(struct iser_conn *iser_conn,
		      unsigned int max_sectors)
{
	struct iser_device *device = iser_conn->ib_conn.device;
	unsigned short sg_tablesize, sup_sg_tablesize;

	sg_tablesize = DIV_ROUND_UP(max_sectors * 512, SIZE_4K);
	sup_sg_tablesize = min_t(unsigned, ISCSI_ISER_MAX_SG_TABLESIZE,
				 device->ib_device->attrs.max_fast_reg_page_list_len);

	if (sg_tablesize > sup_sg_tablesize) {
		sg_tablesize = sup_sg_tablesize;
		iser_conn->scsi_max_sectors = sg_tablesize * SIZE_4K / 512;
	} else {
		iser_conn->scsi_max_sectors = max_sectors;
	}

	iser_conn->scsi_sg_tablesize = sg_tablesize;

	iser_dbg("iser_conn %p, sg_tablesize %u, max_sectors %u\n",
		 iser_conn, iser_conn->scsi_sg_tablesize,
		 iser_conn->scsi_max_sectors);
}

/**
 * Called with state mutex held
 **/
static void iser_addr_handler(struct rdma_cm_id *cma_id)
{
	struct iser_device *device;
	struct iser_conn   *iser_conn;
	struct ib_conn   *ib_conn;
	int    ret;

	iser_conn = (struct iser_conn *)cma_id->context;
	if (iser_conn->state != ISER_CONN_PENDING)
		/* bailout */
		return;

	ib_conn = &iser_conn->ib_conn;
	device = iser_device_find_by_ib_device(cma_id);
	if (!device) {
		iser_err("device lookup/creation failed\n");
		iser_connect_error(cma_id);
		return;
	}

	ib_conn->device = device;

	/* connection T10-PI support */
	if (iser_pi_enable) {
		if (!(device->ib_device->attrs.device_cap_flags &
		      IB_DEVICE_SIGNATURE_HANDOVER)) {
			iser_warn("T10-PI requested but not supported on %s, "
				  "continue without T10-PI\n",
				  ib_conn->device->ib_device->name);
			ib_conn->pi_support = false;
		} else {
			ib_conn->pi_support = true;
		}
	}

	iser_calc_scsi_params(iser_conn, iser_max_sectors);

	ret = rdma_resolve_route(cma_id, 1000);
	if (ret) {
		iser_err("resolve route failed: %d\n", ret);
		iser_connect_error(cma_id);
		return;
	}
}

/**
 * Called with state mutex held
 **/
static void iser_route_handler(struct rdma_cm_id *cma_id)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct rdma_conn_param conn_param;
	int    ret;
	struct iser_cm_hdr req_hdr;
<<<<<<< HEAD

	ret = iser_create_ib_conn_res((struct iser_conn *)cma_id->context);
=======
	struct iser_conn *iser_conn = (struct iser_conn *)cma_id->context;
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	struct iser_device *device = ib_conn->device;

	if (iser_conn->state != ISER_CONN_PENDING)
		/* bailout */
		return;

	ret = iser_create_ib_conn_res(ib_conn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		goto failure;

	memset(&conn_param, 0, sizeof conn_param);
<<<<<<< HEAD
	conn_param.responder_resources = 4;
=======
	conn_param.responder_resources = device->ib_device->attrs.max_qp_rd_atom;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	conn_param.initiator_depth     = 1;
	conn_param.retry_count	       = 7;
	conn_param.rnr_retry_count     = 6;

	memset(&req_hdr, 0, sizeof(req_hdr));
<<<<<<< HEAD
	req_hdr.flags = (ISER_ZBVA_NOT_SUPPORTED |
			ISER_SEND_W_INV_NOT_SUPPORTED);
	conn_param.private_data		= (void *)&req_hdr;
	conn_param.private_data_len	= sizeof(struct iser_cm_hdr);
=======
	req_hdr.flags = ISER_ZBVA_NOT_SUP;
	if (!device->remote_inv_sup)
		req_hdr.flags |= ISER_SEND_W_INV_NOT_SUP;
	conn_param.private_data	= (void *)&req_hdr;
	conn_param.private_data_len = sizeof(struct iser_cm_hdr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = rdma_connect(cma_id, &conn_param);
	if (ret) {
		iser_err("failure connecting: %d\n", ret);
		goto failure;
	}

<<<<<<< HEAD
	return 0;
failure:
	return iser_connect_error(cma_id);
}

static void iser_connected_handler(struct rdma_cm_id *cma_id)
{
	struct iser_conn *ib_conn;

	ib_conn = (struct iser_conn *)cma_id->context;
	ib_conn->state = ISER_CONN_UP;
	wake_up_interruptible(&ib_conn->wait);
}

static int iser_disconnected_handler(struct rdma_cm_id *cma_id)
{
	struct iser_conn *ib_conn;
	int ret;

	ib_conn = (struct iser_conn *)cma_id->context;

	/* getting here when the state is UP means that the conn is being *
	 * terminated asynchronously from the iSCSI layer's perspective.  */
	if (iser_conn_state_comp_exch(ib_conn, ISER_CONN_UP,
				      ISER_CONN_TERMINATING))
		iscsi_conn_failure(ib_conn->iser_conn->iscsi_conn,
				   ISCSI_ERR_CONN_FAILED);

	/* Complete the termination process if no posts are pending */
	if (ib_conn->post_recv_buf_count == 0 &&
	    (atomic_read(&ib_conn->post_send_buf_count) == 0)) {
		ib_conn->state = ISER_CONN_DOWN;
		wake_up_interruptible(&ib_conn->wait);
	}

	ret = iser_conn_put(ib_conn, 0); /* deref ib conn's cma id */
	return ret;
}

static int iser_cma_handler(struct rdma_cm_id *cma_id, struct rdma_cm_event *event)
{
	int ret = 0;

	iser_info("event %d status %d conn %p id %p\n",
		  event->event, event->status, cma_id->context, cma_id);

	switch (event->event) {
	case RDMA_CM_EVENT_ADDR_RESOLVED:
		ret = iser_addr_handler(cma_id);
		break;
	case RDMA_CM_EVENT_ROUTE_RESOLVED:
		ret = iser_route_handler(cma_id);
		break;
	case RDMA_CM_EVENT_ESTABLISHED:
		iser_connected_handler(cma_id);
=======
	return;
failure:
	iser_connect_error(cma_id);
}

static void iser_connected_handler(struct rdma_cm_id *cma_id,
				   const void *private_data)
{
	struct iser_conn *iser_conn;
	struct ib_qp_attr attr;
	struct ib_qp_init_attr init_attr;

	iser_conn = (struct iser_conn *)cma_id->context;
	if (iser_conn->state != ISER_CONN_PENDING)
		/* bailout */
		return;

	(void)ib_query_qp(cma_id->qp, &attr, ~0, &init_attr);
	iser_info("remote qpn:%x my qpn:%x\n", attr.dest_qp_num, cma_id->qp->qp_num);

	if (private_data) {
		u8 flags = *(u8 *)private_data;

		iser_conn->snd_w_inv = !(flags & ISER_SEND_W_INV_NOT_SUP);
	}

	iser_info("conn %p: negotiated %s invalidation\n",
		  iser_conn, iser_conn->snd_w_inv ? "remote" : "local");

	iser_conn->state = ISER_CONN_UP;
	complete(&iser_conn->up_completion);
}

static void iser_disconnected_handler(struct rdma_cm_id *cma_id)
{
	struct iser_conn *iser_conn = (struct iser_conn *)cma_id->context;

	if (iser_conn_terminate(iser_conn)) {
		if (iser_conn->iscsi_conn)
			iscsi_conn_failure(iser_conn->iscsi_conn,
					   ISCSI_ERR_CONN_FAILED);
		else
			iser_err("iscsi_iser connection isn't bound\n");
	}
}

static void iser_cleanup_handler(struct rdma_cm_id *cma_id,
				 bool destroy)
{
	struct iser_conn *iser_conn = (struct iser_conn *)cma_id->context;

	/*
	 * We are not guaranteed that we visited disconnected_handler
	 * by now, call it here to be safe that we handle CM drep
	 * and flush errors.
	 */
	iser_disconnected_handler(cma_id);
	iser_free_ib_conn_res(iser_conn, destroy);
	complete(&iser_conn->ib_completion);
};

static int iser_cma_handler(struct rdma_cm_id *cma_id, struct rdma_cm_event *event)
{
	struct iser_conn *iser_conn;
	int ret = 0;

	iser_conn = (struct iser_conn *)cma_id->context;
	iser_info("%s (%d): status %d conn %p id %p\n",
		  rdma_event_msg(event->event), event->event,
		  event->status, cma_id->context, cma_id);

	mutex_lock(&iser_conn->state_mutex);
	switch (event->event) {
	case RDMA_CM_EVENT_ADDR_RESOLVED:
		iser_addr_handler(cma_id);
		break;
	case RDMA_CM_EVENT_ROUTE_RESOLVED:
		iser_route_handler(cma_id);
		break;
	case RDMA_CM_EVENT_ESTABLISHED:
		iser_connected_handler(cma_id, event->param.conn.private_data);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case RDMA_CM_EVENT_ADDR_ERROR:
	case RDMA_CM_EVENT_ROUTE_ERROR:
	case RDMA_CM_EVENT_CONNECT_ERROR:
	case RDMA_CM_EVENT_UNREACHABLE:
	case RDMA_CM_EVENT_REJECTED:
<<<<<<< HEAD
		ret = iser_connect_error(cma_id);
		break;
	case RDMA_CM_EVENT_DISCONNECTED:
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
	case RDMA_CM_EVENT_ADDR_CHANGE:
		ret = iser_disconnected_handler(cma_id);
		break;
	default:
		iser_err("Unexpected RDMA CM event (%d)\n", event->event);
		break;
	}
	return ret;
}

void iser_conn_init(struct iser_conn *ib_conn)
{
	ib_conn->state = ISER_CONN_INIT;
	init_waitqueue_head(&ib_conn->wait);
	ib_conn->post_recv_buf_count = 0;
	atomic_set(&ib_conn->post_send_buf_count, 0);
	atomic_set(&ib_conn->refcount, 1); /* ref ib conn allocation */
	INIT_LIST_HEAD(&ib_conn->conn_list);
	spin_lock_init(&ib_conn->lock);
=======
		iser_connect_error(cma_id);
		break;
	case RDMA_CM_EVENT_DISCONNECTED:
	case RDMA_CM_EVENT_ADDR_CHANGE:
	case RDMA_CM_EVENT_TIMEWAIT_EXIT:
		iser_cleanup_handler(cma_id, false);
		break;
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		/*
		 * we *must* destroy the device as we cannot rely
		 * on iscsid to be around to initiate error handling.
		 * also if we are not in state DOWN implicitly destroy
		 * the cma_id.
		 */
		iser_cleanup_handler(cma_id, true);
		if (iser_conn->state != ISER_CONN_DOWN) {
			iser_conn->ib_conn.cma_id = NULL;
			ret = 1;
		}
		break;
	default:
		iser_err("Unexpected RDMA CM event: %s (%d)\n",
			 rdma_event_msg(event->event), event->event);
		break;
	}
	mutex_unlock(&iser_conn->state_mutex);

	return ret;
}

void iser_conn_init(struct iser_conn *iser_conn)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;

	iser_conn->state = ISER_CONN_INIT;
	init_completion(&iser_conn->stop_completion);
	init_completion(&iser_conn->ib_completion);
	init_completion(&iser_conn->up_completion);
	INIT_LIST_HEAD(&iser_conn->conn_list);
	mutex_init(&iser_conn->state_mutex);

	ib_conn->post_recv_buf_count = 0;
	ib_conn->reg_cqe.done = iser_reg_comp;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

 /**
 * starts the process of connecting to the target
 * sleeps until the connection is established or rejected
 */
<<<<<<< HEAD
int iser_connect(struct iser_conn   *ib_conn,
		 struct sockaddr_in *src_addr,
		 struct sockaddr_in *dst_addr,
		 int                 non_blocking)
{
	struct sockaddr *src, *dst;
	int err = 0;

	sprintf(ib_conn->name, "%pI4:%d",
		&dst_addr->sin_addr.s_addr, dst_addr->sin_port);
=======
int iser_connect(struct iser_conn   *iser_conn,
		 struct sockaddr    *src_addr,
		 struct sockaddr    *dst_addr,
		 int                 non_blocking)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	int err = 0;

	mutex_lock(&iser_conn->state_mutex);

	sprintf(iser_conn->name, "%pISp", dst_addr);

	iser_info("connecting to: %s\n", iser_conn->name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* the device is known only --after-- address resolution */
	ib_conn->device = NULL;

<<<<<<< HEAD
	iser_info("connecting to: %pI4, port 0x%x\n",
		  &dst_addr->sin_addr, dst_addr->sin_port);

	ib_conn->state = ISER_CONN_PENDING;

	iser_conn_get(ib_conn); /* ref ib conn's cma id */
	ib_conn->cma_id = rdma_create_id(iser_cma_handler,
					     (void *)ib_conn,
					     RDMA_PS_TCP, IB_QPT_RC);
=======
	iser_conn->state = ISER_CONN_PENDING;

	ib_conn->cma_id = rdma_create_id(&init_net, iser_cma_handler,
					 (void *)iser_conn,
					 RDMA_PS_TCP, IB_QPT_RC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (IS_ERR(ib_conn->cma_id)) {
		err = PTR_ERR(ib_conn->cma_id);
		iser_err("rdma_create_id failed: %d\n", err);
		goto id_failure;
	}

<<<<<<< HEAD
	src = (struct sockaddr *)src_addr;
	dst = (struct sockaddr *)dst_addr;
	err = rdma_resolve_addr(ib_conn->cma_id, src, dst, 1000);
=======
	err = rdma_resolve_addr(ib_conn->cma_id, src_addr, dst_addr, 1000);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err) {
		iser_err("rdma_resolve_addr failed: %d\n", err);
		goto addr_failure;
	}

	if (!non_blocking) {
<<<<<<< HEAD
		wait_event_interruptible(ib_conn->wait,
					 (ib_conn->state != ISER_CONN_PENDING));

		if (ib_conn->state != ISER_CONN_UP) {
=======
		wait_for_completion_interruptible(&iser_conn->up_completion);

		if (iser_conn->state != ISER_CONN_UP) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			err =  -EIO;
			goto connect_failure;
		}
	}
<<<<<<< HEAD

	mutex_lock(&ig.connlist_mutex);
	list_add(&ib_conn->conn_list, &ig.connlist);
=======
	mutex_unlock(&iser_conn->state_mutex);

	mutex_lock(&ig.connlist_mutex);
	list_add(&iser_conn->conn_list, &ig.connlist);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&ig.connlist_mutex);
	return 0;

id_failure:
	ib_conn->cma_id = NULL;
addr_failure:
<<<<<<< HEAD
	ib_conn->state = ISER_CONN_DOWN;
	iser_conn_put(ib_conn, 1); /* deref ib conn's cma id */
connect_failure:
	iser_conn_put(ib_conn, 1); /* deref ib conn deallocate */
	return err;
}

/**
 * iser_reg_page_vec - Register physical memory
 *
 * returns: 0 on success, errno code on failure
 */
int iser_reg_page_vec(struct iser_conn     *ib_conn,
		      struct iser_page_vec *page_vec,
		      struct iser_mem_reg  *mem_reg)
{
	struct ib_pool_fmr *mem;
	u64		   io_addr;
	u64		   *page_list;
	int		   status;

	page_list = page_vec->pages;
	io_addr	  = page_list[0];

	mem  = ib_fmr_pool_map_phys(ib_conn->fmr_pool,
				    page_list,
				    page_vec->length,
				    io_addr);

	if (IS_ERR(mem)) {
		status = (int)PTR_ERR(mem);
		iser_err("ib_fmr_pool_map_phys failed: %d\n", status);
		return status;
	}

	mem_reg->lkey  = mem->fmr->lkey;
	mem_reg->rkey  = mem->fmr->rkey;
	mem_reg->len   = page_vec->length * SIZE_4K;
	mem_reg->va    = io_addr;
	mem_reg->is_fmr = 1;
	mem_reg->mem_h = (void *)mem;

	mem_reg->va   += page_vec->offset;
	mem_reg->len   = page_vec->data_size;

	iser_dbg("PHYSICAL Mem.register, [PHYS p_array: 0x%p, sz: %d, "
		 "entry[0]: (0x%08lx,%ld)] -> "
		 "[lkey: 0x%08X mem_h: 0x%p va: 0x%08lX sz: %ld]\n",
		 page_vec, page_vec->length,
		 (unsigned long)page_vec->pages[0],
		 (unsigned long)page_vec->data_size,
		 (unsigned int)mem_reg->lkey, mem_reg->mem_h,
		 (unsigned long)mem_reg->va, (unsigned long)mem_reg->len);
	return 0;
}

/**
 * Unregister (previosuly registered) memory.
 */
void iser_unreg_mem(struct iser_mem_reg *reg)
{
	int ret;

	iser_dbg("PHYSICAL Mem.Unregister mem_h %p\n",reg->mem_h);

	ret = ib_fmr_pool_unmap((struct ib_pool_fmr *)reg->mem_h);
	if (ret)
		iser_err("ib_fmr_pool_unmap failed %d\n", ret);

	reg->mem_h = NULL;
}

int iser_post_recvl(struct iser_conn *ib_conn)
{
	struct ib_recv_wr rx_wr, *rx_wr_failed;
	struct ib_sge	  sge;
	int ib_ret;

	sge.addr   = ib_conn->login_resp_dma;
	sge.length = ISER_RX_LOGIN_SIZE;
	sge.lkey   = ib_conn->device->mr->lkey;

	rx_wr.wr_id   = (unsigned long)ib_conn->login_resp_buf;
	rx_wr.sg_list = &sge;
	rx_wr.num_sge = 1;
	rx_wr.next    = NULL;

	ib_conn->post_recv_buf_count++;
	ib_ret	= ib_post_recv(ib_conn->qp, &rx_wr, &rx_wr_failed);
=======
	iser_conn->state = ISER_CONN_DOWN;
connect_failure:
	mutex_unlock(&iser_conn->state_mutex);
	iser_conn_release(iser_conn);
	return err;
}

int iser_post_recvl(struct iser_conn *iser_conn)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	struct iser_login_desc *desc = &iser_conn->login_desc;
	struct ib_recv_wr wr, *wr_failed;
	int ib_ret;

	desc->sge.addr = desc->rsp_dma;
	desc->sge.length = ISER_RX_LOGIN_SIZE;
	desc->sge.lkey = ib_conn->device->pd->local_dma_lkey;

	desc->cqe.done = iser_login_rsp;
	wr.wr_cqe = &desc->cqe;
	wr.sg_list = &desc->sge;
	wr.num_sge = 1;
	wr.next = NULL;

	ib_conn->post_recv_buf_count++;
	ib_ret = ib_post_recv(ib_conn->qp, &wr, &wr_failed);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ib_ret) {
		iser_err("ib_post_recv failed ret=%d\n", ib_ret);
		ib_conn->post_recv_buf_count--;
	}
<<<<<<< HEAD
	return ib_ret;
}

int iser_post_recvm(struct iser_conn *ib_conn, int count)
{
	struct ib_recv_wr *rx_wr, *rx_wr_failed;
	int i, ib_ret;
	unsigned int my_rx_head = ib_conn->rx_desc_head;
	struct iser_rx_desc *rx_desc;

	for (rx_wr = ib_conn->rx_wr, i = 0; i < count; i++, rx_wr++) {
		rx_desc		= &ib_conn->rx_descs[my_rx_head];
		rx_wr->wr_id	= (unsigned long)rx_desc;
		rx_wr->sg_list	= &rx_desc->rx_sg;
		rx_wr->num_sge	= 1;
		rx_wr->next	= rx_wr + 1;
		my_rx_head = (my_rx_head + 1) & (ISER_QP_MAX_RECV_DTOS - 1);
	}

	rx_wr--;
	rx_wr->next = NULL; /* mark end of work requests list */

	ib_conn->post_recv_buf_count += count;
	ib_ret	= ib_post_recv(ib_conn->qp, ib_conn->rx_wr, &rx_wr_failed);
=======

	return ib_ret;
}

int iser_post_recvm(struct iser_conn *iser_conn, int count)
{
	struct ib_conn *ib_conn = &iser_conn->ib_conn;
	unsigned int my_rx_head = iser_conn->rx_desc_head;
	struct iser_rx_desc *rx_desc;
	struct ib_recv_wr *wr, *wr_failed;
	int i, ib_ret;

	for (wr = ib_conn->rx_wr, i = 0; i < count; i++, wr++) {
		rx_desc = &iser_conn->rx_descs[my_rx_head];
		rx_desc->cqe.done = iser_task_rsp;
		wr->wr_cqe = &rx_desc->cqe;
		wr->sg_list = &rx_desc->rx_sg;
		wr->num_sge = 1;
		wr->next = wr + 1;
		my_rx_head = (my_rx_head + 1) & iser_conn->qp_max_recv_dtos_mask;
	}

	wr--;
	wr->next = NULL; /* mark end of work requests list */

	ib_conn->post_recv_buf_count += count;
	ib_ret = ib_post_recv(ib_conn->qp, ib_conn->rx_wr, &wr_failed);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ib_ret) {
		iser_err("ib_post_recv failed ret=%d\n", ib_ret);
		ib_conn->post_recv_buf_count -= count;
	} else
<<<<<<< HEAD
		ib_conn->rx_desc_head = my_rx_head;
=======
		iser_conn->rx_desc_head = my_rx_head;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ib_ret;
}


/**
 * iser_start_send - Initiate a Send DTO operation
 *
 * returns 0 on success, -1 on failure
 */
<<<<<<< HEAD
int iser_post_send(struct iser_conn *ib_conn, struct iser_tx_desc *tx_desc)
{
	int		  ib_ret;
	struct ib_send_wr send_wr, *send_wr_failed;

	ib_dma_sync_single_for_device(ib_conn->device->ib_device,
		tx_desc->dma_addr, ISER_HEADERS_LEN, DMA_TO_DEVICE);

	send_wr.next	   = NULL;
	send_wr.wr_id	   = (unsigned long)tx_desc;
	send_wr.sg_list	   = tx_desc->tx_sg;
	send_wr.num_sge	   = tx_desc->num_sge;
	send_wr.opcode	   = IB_WR_SEND;
	send_wr.send_flags = IB_SEND_SIGNALED;

	atomic_inc(&ib_conn->post_send_buf_count);

	ib_ret = ib_post_send(ib_conn->qp, &send_wr, &send_wr_failed);
	if (ib_ret) {
		iser_err("ib_post_send failed, ret:%d\n", ib_ret);
		atomic_dec(&ib_conn->post_send_buf_count);
	}
	return ib_ret;
}

static void iser_handle_comp_error(struct iser_tx_desc *desc,
				struct iser_conn *ib_conn)
{
	if (desc && desc->type == ISCSI_TX_DATAOUT)
		kmem_cache_free(ig.desc_cache, desc);

	if (ib_conn->post_recv_buf_count == 0 &&
	    atomic_read(&ib_conn->post_send_buf_count) == 0) {
		/* getting here when the state is UP means that the conn is *
		 * being terminated asynchronously from the iSCSI layer's   *
		 * perspective.                                             */
		if (iser_conn_state_comp_exch(ib_conn, ISER_CONN_UP,
		    ISER_CONN_TERMINATING))
			iscsi_conn_failure(ib_conn->iser_conn->iscsi_conn,
					   ISCSI_ERR_CONN_FAILED);

		/* no more non completed posts to the QP, complete the
		 * termination process w.o worrying on disconnect event */
		ib_conn->state = ISER_CONN_DOWN;
		wake_up_interruptible(&ib_conn->wait);
	}
}

static int iser_drain_tx_cq(struct iser_device  *device, int cq_index)
{
	struct ib_cq  *cq = device->tx_cq[cq_index];
	struct ib_wc  wc;
	struct iser_tx_desc *tx_desc;
	struct iser_conn *ib_conn;
	int completed_tx = 0;

	while (ib_poll_cq(cq, 1, &wc) == 1) {
		tx_desc	= (struct iser_tx_desc *) (unsigned long) wc.wr_id;
		ib_conn = wc.qp->qp_context;
		if (wc.status == IB_WC_SUCCESS) {
			if (wc.opcode == IB_WC_SEND)
				iser_snd_completion(tx_desc, ib_conn);
			else
				iser_err("expected opcode %d got %d\n",
					IB_WC_SEND, wc.opcode);
		} else {
			iser_err("tx id %llx status %d vend_err %x\n",
				wc.wr_id, wc.status, wc.vendor_err);
			atomic_dec(&ib_conn->post_send_buf_count);
			iser_handle_comp_error(tx_desc, ib_conn);
		}
		completed_tx++;
	}
	return completed_tx;
}


static void iser_cq_tasklet_fn(unsigned long data)
{
	struct iser_cq_desc *cq_desc = (struct iser_cq_desc *)data;
	struct iser_device  *device = cq_desc->device;
	int cq_index = cq_desc->cq_index;
	struct ib_cq	     *cq = device->rx_cq[cq_index];
	 struct ib_wc	     wc;
	 struct iser_rx_desc *desc;
	 unsigned long	     xfer_len;
	struct iser_conn *ib_conn;
	int completed_tx, completed_rx;
	completed_tx = completed_rx = 0;

	while (ib_poll_cq(cq, 1, &wc) == 1) {
		desc	 = (struct iser_rx_desc *) (unsigned long) wc.wr_id;
		BUG_ON(desc == NULL);
		ib_conn = wc.qp->qp_context;
		if (wc.status == IB_WC_SUCCESS) {
			if (wc.opcode == IB_WC_RECV) {
				xfer_len = (unsigned long)wc.byte_len;
				iser_rcv_completion(desc, xfer_len, ib_conn);
			} else
				iser_err("expected opcode %d got %d\n",
					IB_WC_RECV, wc.opcode);
		} else {
			if (wc.status != IB_WC_WR_FLUSH_ERR)
				iser_err("rx id %llx status %d vend_err %x\n",
					wc.wr_id, wc.status, wc.vendor_err);
			ib_conn->post_recv_buf_count--;
			iser_handle_comp_error(NULL, ib_conn);
		}
		completed_rx++;
		if (!(completed_rx & 63))
			completed_tx += iser_drain_tx_cq(device, cq_index);
	}
	/* #warning "it is assumed here that arming CQ only once its empty" *
	 * " would not cause interrupts to be missed"                       */
	ib_req_notify_cq(cq, IB_CQ_NEXT_COMP);

	completed_tx += iser_drain_tx_cq(device, cq_index);
	iser_dbg("got %d rx %d tx completions\n", completed_rx, completed_tx);
}

static void iser_cq_callback(struct ib_cq *cq, void *cq_context)
{
	struct iser_cq_desc *cq_desc = (struct iser_cq_desc *)cq_context;
	struct iser_device  *device = cq_desc->device;
	int cq_index = cq_desc->cq_index;

	tasklet_schedule(&device->cq_tasklet[cq_index]);
=======
int iser_post_send(struct ib_conn *ib_conn, struct iser_tx_desc *tx_desc,
		   bool signal)
{
	struct ib_send_wr *bad_wr, *wr = iser_tx_next_wr(tx_desc);
	int ib_ret;

	ib_dma_sync_single_for_device(ib_conn->device->ib_device,
				      tx_desc->dma_addr, ISER_HEADERS_LEN,
				      DMA_TO_DEVICE);

	wr->next = NULL;
	wr->wr_cqe = &tx_desc->cqe;
	wr->sg_list = tx_desc->tx_sg;
	wr->num_sge = tx_desc->num_sge;
	wr->opcode = IB_WR_SEND;
	wr->send_flags = signal ? IB_SEND_SIGNALED : 0;

	ib_ret = ib_post_send(ib_conn->qp, &tx_desc->wrs[0].send, &bad_wr);
	if (ib_ret)
		iser_err("ib_post_send failed, ret:%d opcode:%d\n",
			 ib_ret, bad_wr->opcode);

	return ib_ret;
}

u8 iser_check_task_pi_status(struct iscsi_iser_task *iser_task,
			     enum iser_data_dir cmd_dir, sector_t *sector)
{
	struct iser_mem_reg *reg = &iser_task->rdma_reg[cmd_dir];
	struct iser_fr_desc *desc = reg->mem_h;
	unsigned long sector_size = iser_task->sc->device->sector_size;
	struct ib_mr_status mr_status;
	int ret;

	if (desc && desc->pi_ctx->sig_protected) {
		desc->pi_ctx->sig_protected = 0;
		ret = ib_check_mr_status(desc->pi_ctx->sig_mr,
					 IB_MR_CHECK_SIG_STATUS, &mr_status);
		if (ret) {
			pr_err("ib_check_mr_status failed, ret %d\n", ret);
			/* Not a lot we can do, return ambiguous guard error */
			*sector = 0;
			return 0x1;
		}

		if (mr_status.fail_status & IB_MR_CHECK_SIG_STATUS) {
			sector_t sector_off = mr_status.sig_err.sig_err_offset;

			sector_div(sector_off, sector_size + 8);
			*sector = scsi_get_lba(iser_task->sc) + sector_off;

			pr_err("PI error found type %d at sector %llx "
			       "expected %x vs actual %x\n",
			       mr_status.sig_err.err_type,
			       (unsigned long long)*sector,
			       mr_status.sig_err.expected,
			       mr_status.sig_err.actual);

			switch (mr_status.sig_err.err_type) {
			case IB_SIG_BAD_GUARD:
				return 0x1;
			case IB_SIG_BAD_REFTAG:
				return 0x3;
			case IB_SIG_BAD_APPTAG:
				return 0x2;
			}
		}
	}

	return 0;
}

void iser_err_comp(struct ib_wc *wc, const char *type)
{
	if (wc->status != IB_WC_WR_FLUSH_ERR) {
		struct iser_conn *iser_conn = to_iser_conn(wc->qp->qp_context);

		iser_err("%s failure: %s (%d) vend_err %x\n", type,
			 ib_wc_status_msg(wc->status), wc->status,
			 wc->vendor_err);

		if (iser_conn->iscsi_conn)
			iscsi_conn_failure(iser_conn->iscsi_conn,
					   ISCSI_ERR_CONN_FAILED);
	} else {
		iser_dbg("%s failure: %s (%d)\n", type,
			 ib_wc_status_msg(wc->status), wc->status);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
