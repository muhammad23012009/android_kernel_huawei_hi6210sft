<<<<<<< HEAD
/*******************************************************************
 * This file is part of the Emulex RoCE Device Driver for          *
 * RoCE (RDMA over Converged Ethernet) adapters.                   *
 * Copyright (C) 2008-2012 Emulex. All rights reserved.            *
 * EMULEX and SLI are trademarks of Emulex.                        *
 * www.emulex.com                                                  *
 *                                                                 *
 * This program is free software; you can redistribute it and/or   *
 * modify it under the terms of version 2 of the GNU General       *
 * Public License as published by the Free Software Foundation.    *
 * This program is distributed in the hope that it will be useful. *
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND          *
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,  *
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE      *
 * DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS ARE HELD *
 * TO BE LEGALLY INVALID.  See the GNU General Public License for  *
 * more details, a copy of which can be found in the file COPYING  *
 * included with this package.                                     *
=======
/* This file is part of the Emulex RoCE Device Driver for
 * RoCE (RDMA over Converged Ethernet) adapters.
 * Copyright (C) 2012-2015 Emulex. All rights reserved.
 * EMULEX and SLI are trademarks of Emulex.
 * www.emulex.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file COPYING in the main
 * directory of this source tree, or the BSD license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Contact Information:
 * linux-drivers@emulex.com
 *
 * Emulex
 * 3333 Susan Street
 * Costa Mesa, CA 92626
<<<<<<< HEAD
 *******************************************************************/
=======
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/module.h>
#include <linux/idr.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_addr.h>
<<<<<<< HEAD
=======
#include <rdma/ib_mad.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/netdevice.h>
#include <net/addrconf.h>

#include "ocrdma.h"
#include "ocrdma_verbs.h"
#include "ocrdma_ah.h"
#include "be_roce.h"
#include "ocrdma_hw.h"
<<<<<<< HEAD

MODULE_VERSION(OCRDMA_ROCE_DEV_VERSION);
MODULE_DESCRIPTION("Emulex RoCE HCA Driver");
MODULE_AUTHOR("Emulex Corporation");
MODULE_LICENSE("GPL");

static LIST_HEAD(ocrdma_dev_list);
static DEFINE_SPINLOCK(ocrdma_devlist_lock);
static DEFINE_IDR(ocrdma_dev_id);

static union ib_gid ocrdma_zero_sgid;

=======
#include "ocrdma_stats.h"
#include <rdma/ocrdma-abi.h>

MODULE_VERSION(OCRDMA_ROCE_DRV_VERSION);
MODULE_DESCRIPTION(OCRDMA_ROCE_DRV_DESC " " OCRDMA_ROCE_DRV_VERSION);
MODULE_AUTHOR("Emulex Corporation");
MODULE_LICENSE("Dual BSD/GPL");

static DEFINE_IDR(ocrdma_dev_id);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void ocrdma_get_guid(struct ocrdma_dev *dev, u8 *guid)
{
	u8 mac_addr[6];

	memcpy(&mac_addr[0], &dev->nic_info.mac_addr[0], ETH_ALEN);
	guid[0] = mac_addr[0] ^ 2;
	guid[1] = mac_addr[1];
	guid[2] = mac_addr[2];
	guid[3] = 0xff;
	guid[4] = 0xfe;
	guid[5] = mac_addr[3];
	guid[6] = mac_addr[4];
	guid[7] = mac_addr[5];
}
<<<<<<< HEAD

static void ocrdma_build_sgid_mac(union ib_gid *sgid, unsigned char *mac_addr,
				  bool is_vlan, u16 vlan_id)
{
	sgid->global.subnet_prefix = cpu_to_be64(0xfe80000000000000LL);
	sgid->raw[8] = mac_addr[0] ^ 2;
	sgid->raw[9] = mac_addr[1];
	sgid->raw[10] = mac_addr[2];
	if (is_vlan) {
		sgid->raw[11] = vlan_id >> 8;
		sgid->raw[12] = vlan_id & 0xff;
	} else {
		sgid->raw[11] = 0xff;
		sgid->raw[12] = 0xfe;
	}
	sgid->raw[13] = mac_addr[3];
	sgid->raw[14] = mac_addr[4];
	sgid->raw[15] = mac_addr[5];
}

static bool ocrdma_add_sgid(struct ocrdma_dev *dev, unsigned char *mac_addr,
			    bool is_vlan, u16 vlan_id)
{
	int i;
	union ib_gid new_sgid;
	unsigned long flags;

	memset(&ocrdma_zero_sgid, 0, sizeof(union ib_gid));

	ocrdma_build_sgid_mac(&new_sgid, mac_addr, is_vlan, vlan_id);

	spin_lock_irqsave(&dev->sgid_lock, flags);
	for (i = 0; i < OCRDMA_MAX_SGID; i++) {
		if (!memcmp(&dev->sgid_tbl[i], &ocrdma_zero_sgid,
			    sizeof(union ib_gid))) {
			/* found free entry */
			memcpy(&dev->sgid_tbl[i], &new_sgid,
			       sizeof(union ib_gid));
			spin_unlock_irqrestore(&dev->sgid_lock, flags);
			return true;
		} else if (!memcmp(&dev->sgid_tbl[i], &new_sgid,
				   sizeof(union ib_gid))) {
			/* entry already present, no addition is required. */
			spin_unlock_irqrestore(&dev->sgid_lock, flags);
			return false;
		}
	}
	spin_unlock_irqrestore(&dev->sgid_lock, flags);
	return false;
}

static bool ocrdma_del_sgid(struct ocrdma_dev *dev, unsigned char *mac_addr,
			    bool is_vlan, u16 vlan_id)
{
	int found = false;
	int i;
	union ib_gid sgid;
	unsigned long flags;

	ocrdma_build_sgid_mac(&sgid, mac_addr, is_vlan, vlan_id);

	spin_lock_irqsave(&dev->sgid_lock, flags);
	/* first is default sgid, which cannot be deleted. */
	for (i = 1; i < OCRDMA_MAX_SGID; i++) {
		if (!memcmp(&dev->sgid_tbl[i], &sgid, sizeof(union ib_gid))) {
			/* found matching entry */
			memset(&dev->sgid_tbl[i], 0, sizeof(union ib_gid));
			found = true;
			break;
		}
	}
	spin_unlock_irqrestore(&dev->sgid_lock, flags);
	return found;
}

static void ocrdma_add_default_sgid(struct ocrdma_dev *dev)
{
	/* GID Index 0 - Invariant manufacturer-assigned EUI-64 */
	union ib_gid *sgid = &dev->sgid_tbl[0];

	sgid->global.subnet_prefix = cpu_to_be64(0xfe80000000000000LL);
	ocrdma_get_guid(dev, &sgid->raw[8]);
}

#if IS_ENABLED(CONFIG_VLAN_8021Q)
static void ocrdma_add_vlan_sgids(struct ocrdma_dev *dev)
{
	struct net_device *netdev, *tmp;
	u16 vlan_id;
	bool is_vlan;

	netdev = dev->nic_info.netdev;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, tmp) {
		if (netdev == tmp || vlan_dev_real_dev(tmp) == netdev) {
			if (!netif_running(tmp) || !netif_oper_up(tmp))
				continue;
			if (netdev != tmp) {
				vlan_id = vlan_dev_vlan_id(tmp);
				is_vlan = true;
			} else {
				is_vlan = false;
				vlan_id = 0;
				tmp = netdev;
			}
			ocrdma_add_sgid(dev, tmp->dev_addr, is_vlan, vlan_id);
		}
	}
	rcu_read_unlock();
}
#else
static void ocrdma_add_vlan_sgids(struct ocrdma_dev *dev)
{

}
#endif /* VLAN */

static int ocrdma_build_sgid_tbl(struct ocrdma_dev *dev)
{
	ocrdma_add_default_sgid(dev);
	ocrdma_add_vlan_sgids(dev);
	return 0;
}

#if IS_ENABLED(CONFIG_IPV6)

static int ocrdma_inet6addr_event(struct notifier_block *notifier,
				  unsigned long event, void *ptr)
{
	struct inet6_ifaddr *ifa = (struct inet6_ifaddr *)ptr;
	struct net_device *netdev = ifa->idev->dev;
	struct ib_event gid_event;
	struct ocrdma_dev *dev;
	bool found = false;
	bool updated = false;
	bool is_vlan = false;
	u16 vid = 0;

	is_vlan = netdev->priv_flags & IFF_802_1Q_VLAN;
	if (is_vlan) {
		vid = vlan_dev_vlan_id(netdev);
		netdev = vlan_dev_real_dev(netdev);
	}

	rcu_read_lock();
	list_for_each_entry_rcu(dev, &ocrdma_dev_list, entry) {
		if (dev->nic_info.netdev == netdev) {
			found = true;
			break;
		}
	}
	rcu_read_unlock();

	if (!found)
		return NOTIFY_DONE;
	if (!rdma_link_local_addr((struct in6_addr *)&ifa->addr))
		return NOTIFY_DONE;

	mutex_lock(&dev->dev_lock);
	switch (event) {
	case NETDEV_UP:
		updated = ocrdma_add_sgid(dev, netdev->dev_addr, is_vlan, vid);
		break;
	case NETDEV_DOWN:
		updated = ocrdma_del_sgid(dev, netdev->dev_addr, is_vlan, vid);
		break;
	default:
		break;
	}
	if (updated) {
		/* GID table updated, notify the consumers about it */
		gid_event.device = &dev->ibdev;
		gid_event.element.port_num = 1;
		gid_event.event = IB_EVENT_GID_CHANGE;
		ib_dispatch_event(&gid_event);
	}
	mutex_unlock(&dev->dev_lock);
	return NOTIFY_OK;
}

static struct notifier_block ocrdma_inet6addr_notifier = {
	.notifier_call = ocrdma_inet6addr_event
};

#endif /* IPV6 and VLAN */

static enum rdma_link_layer ocrdma_link_layer(struct ib_device *device,
					      u8 port_num)
{
	return IB_LINK_LAYER_ETHERNET;
=======
static enum rdma_link_layer ocrdma_link_layer(struct ib_device *device,
					      u8 port_num)
{
	return IB_LINK_LAYER_ETHERNET;
}

static int ocrdma_port_immutable(struct ib_device *ibdev, u8 port_num,
			         struct ib_port_immutable *immutable)
{
	struct ib_port_attr attr;
	struct ocrdma_dev *dev;
	int err;

	dev = get_ocrdma_dev(ibdev);
	err = ocrdma_query_port(ibdev, port_num, &attr);
	if (err)
		return err;

	immutable->pkey_tbl_len = attr.pkey_tbl_len;
	immutable->gid_tbl_len = attr.gid_tbl_len;
	immutable->core_cap_flags = RDMA_CORE_PORT_IBA_ROCE;
	if (ocrdma_is_udp_encap_supported(dev))
		immutable->core_cap_flags |= RDMA_CORE_CAP_PROT_ROCE_UDP_ENCAP;
	immutable->max_mad_size = IB_MGMT_MAD_SIZE;

	return 0;
}

static void get_dev_fw_str(struct ib_device *device, char *str,
			   size_t str_len)
{
	struct ocrdma_dev *dev = get_ocrdma_dev(device);

	snprintf(str, str_len, "%s", &dev->attr.fw_ver[0]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int ocrdma_register_device(struct ocrdma_dev *dev)
{
	strlcpy(dev->ibdev.name, "ocrdma%d", IB_DEVICE_NAME_MAX);
	ocrdma_get_guid(dev, (u8 *)&dev->ibdev.node_guid);
<<<<<<< HEAD
	memcpy(dev->ibdev.node_desc, OCRDMA_NODE_DESC,
	       sizeof(OCRDMA_NODE_DESC));
	dev->ibdev.owner = THIS_MODULE;
=======
	BUILD_BUG_ON(sizeof(OCRDMA_NODE_DESC) > IB_DEVICE_NODE_DESC_MAX);
	memcpy(dev->ibdev.node_desc, OCRDMA_NODE_DESC,
	       sizeof(OCRDMA_NODE_DESC));
	dev->ibdev.owner = THIS_MODULE;
	dev->ibdev.uverbs_abi_ver = OCRDMA_ABI_VERSION;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev->ibdev.uverbs_cmd_mask =
	    OCRDMA_UVERBS(GET_CONTEXT) |
	    OCRDMA_UVERBS(QUERY_DEVICE) |
	    OCRDMA_UVERBS(QUERY_PORT) |
	    OCRDMA_UVERBS(ALLOC_PD) |
	    OCRDMA_UVERBS(DEALLOC_PD) |
	    OCRDMA_UVERBS(REG_MR) |
	    OCRDMA_UVERBS(DEREG_MR) |
	    OCRDMA_UVERBS(CREATE_COMP_CHANNEL) |
	    OCRDMA_UVERBS(CREATE_CQ) |
	    OCRDMA_UVERBS(RESIZE_CQ) |
	    OCRDMA_UVERBS(DESTROY_CQ) |
	    OCRDMA_UVERBS(REQ_NOTIFY_CQ) |
	    OCRDMA_UVERBS(CREATE_QP) |
	    OCRDMA_UVERBS(MODIFY_QP) |
	    OCRDMA_UVERBS(QUERY_QP) |
	    OCRDMA_UVERBS(DESTROY_QP) |
	    OCRDMA_UVERBS(POLL_CQ) |
	    OCRDMA_UVERBS(POST_SEND) |
	    OCRDMA_UVERBS(POST_RECV);

	dev->ibdev.uverbs_cmd_mask |=
	    OCRDMA_UVERBS(CREATE_AH) |
	     OCRDMA_UVERBS(MODIFY_AH) |
	     OCRDMA_UVERBS(QUERY_AH) |
	     OCRDMA_UVERBS(DESTROY_AH);

	dev->ibdev.node_type = RDMA_NODE_IB_CA;
	dev->ibdev.phys_port_cnt = 1;
<<<<<<< HEAD
	dev->ibdev.num_comp_vectors = 1;
=======
	dev->ibdev.num_comp_vectors = dev->eq_cnt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* mandatory verbs. */
	dev->ibdev.query_device = ocrdma_query_device;
	dev->ibdev.query_port = ocrdma_query_port;
	dev->ibdev.modify_port = ocrdma_modify_port;
	dev->ibdev.query_gid = ocrdma_query_gid;
<<<<<<< HEAD
=======
	dev->ibdev.get_netdev = ocrdma_get_netdev;
	dev->ibdev.add_gid = ocrdma_add_gid;
	dev->ibdev.del_gid = ocrdma_del_gid;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev->ibdev.get_link_layer = ocrdma_link_layer;
	dev->ibdev.alloc_pd = ocrdma_alloc_pd;
	dev->ibdev.dealloc_pd = ocrdma_dealloc_pd;

	dev->ibdev.create_cq = ocrdma_create_cq;
	dev->ibdev.destroy_cq = ocrdma_destroy_cq;
	dev->ibdev.resize_cq = ocrdma_resize_cq;

	dev->ibdev.create_qp = ocrdma_create_qp;
	dev->ibdev.modify_qp = ocrdma_modify_qp;
	dev->ibdev.query_qp = ocrdma_query_qp;
	dev->ibdev.destroy_qp = ocrdma_destroy_qp;

	dev->ibdev.query_pkey = ocrdma_query_pkey;
	dev->ibdev.create_ah = ocrdma_create_ah;
	dev->ibdev.destroy_ah = ocrdma_destroy_ah;
	dev->ibdev.query_ah = ocrdma_query_ah;
	dev->ibdev.modify_ah = ocrdma_modify_ah;

	dev->ibdev.poll_cq = ocrdma_poll_cq;
	dev->ibdev.post_send = ocrdma_post_send;
	dev->ibdev.post_recv = ocrdma_post_recv;
	dev->ibdev.req_notify_cq = ocrdma_arm_cq;

	dev->ibdev.get_dma_mr = ocrdma_get_dma_mr;
	dev->ibdev.dereg_mr = ocrdma_dereg_mr;
	dev->ibdev.reg_user_mr = ocrdma_reg_user_mr;

<<<<<<< HEAD
=======
	dev->ibdev.alloc_mr = ocrdma_alloc_mr;
	dev->ibdev.map_mr_sg = ocrdma_map_mr_sg;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* mandatory to support user space verbs consumer. */
	dev->ibdev.alloc_ucontext = ocrdma_alloc_ucontext;
	dev->ibdev.dealloc_ucontext = ocrdma_dealloc_ucontext;
	dev->ibdev.mmap = ocrdma_mmap;
	dev->ibdev.dma_device = &dev->nic_info.pdev->dev;

	dev->ibdev.process_mad = ocrdma_process_mad;
<<<<<<< HEAD

	if (dev->nic_info.dev_family == OCRDMA_GEN2_FAMILY) {
=======
	dev->ibdev.get_port_immutable = ocrdma_port_immutable;
	dev->ibdev.get_dev_fw_str     = get_dev_fw_str;

	if (ocrdma_get_asic_type(dev) == OCRDMA_ASIC_GEN_SKH_R) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev->ibdev.uverbs_cmd_mask |=
		     OCRDMA_UVERBS(CREATE_SRQ) |
		     OCRDMA_UVERBS(MODIFY_SRQ) |
		     OCRDMA_UVERBS(QUERY_SRQ) |
		     OCRDMA_UVERBS(DESTROY_SRQ) |
		     OCRDMA_UVERBS(POST_SRQ_RECV);

		dev->ibdev.create_srq = ocrdma_create_srq;
		dev->ibdev.modify_srq = ocrdma_modify_srq;
		dev->ibdev.query_srq = ocrdma_query_srq;
		dev->ibdev.destroy_srq = ocrdma_destroy_srq;
		dev->ibdev.post_srq_recv = ocrdma_post_srq_recv;
	}
	return ib_register_device(&dev->ibdev, NULL);
}

static int ocrdma_alloc_resources(struct ocrdma_dev *dev)
{
	mutex_init(&dev->dev_lock);
<<<<<<< HEAD
	dev->sgid_tbl = kzalloc(sizeof(union ib_gid) *
				OCRDMA_MAX_SGID, GFP_KERNEL);
	if (!dev->sgid_tbl)
		goto alloc_err;
	spin_lock_init(&dev->sgid_lock);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev->cq_tbl = kzalloc(sizeof(struct ocrdma_cq *) *
			      OCRDMA_MAX_CQ, GFP_KERNEL);
	if (!dev->cq_tbl)
		goto alloc_err;

	if (dev->attr.max_qp) {
		dev->qp_tbl = kzalloc(sizeof(struct ocrdma_qp *) *
				      OCRDMA_MAX_QP, GFP_KERNEL);
		if (!dev->qp_tbl)
			goto alloc_err;
	}
<<<<<<< HEAD
=======

	dev->stag_arr = kzalloc(sizeof(u64) * OCRDMA_MAX_STAG, GFP_KERNEL);
	if (dev->stag_arr == NULL)
		goto alloc_err;

	ocrdma_alloc_pd_pool(dev);

	if (!ocrdma_alloc_stats_resources(dev)) {
		pr_err("%s: stats resource allocation failed\n", __func__);
		goto alloc_err;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_lock_init(&dev->av_tbl.lock);
	spin_lock_init(&dev->flush_q_lock);
	return 0;
alloc_err:
<<<<<<< HEAD
	ocrdma_err("%s(%d) error.\n", __func__, dev->id);
=======
	pr_err("%s(%d) error.\n", __func__, dev->id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return -ENOMEM;
}

static void ocrdma_free_resources(struct ocrdma_dev *dev)
{
<<<<<<< HEAD
	kfree(dev->qp_tbl);
	kfree(dev->cq_tbl);
	kfree(dev->sgid_tbl);
=======
	ocrdma_release_stats_resources(dev);
	kfree(dev->stag_arr);
	kfree(dev->qp_tbl);
	kfree(dev->cq_tbl);
}

/* OCRDMA sysfs interface */
static ssize_t show_rev(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct ocrdma_dev *dev = dev_get_drvdata(device);

	return scnprintf(buf, PAGE_SIZE, "0x%x\n", dev->nic_info.pdev->vendor);
}

static ssize_t show_hca_type(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct ocrdma_dev *dev = dev_get_drvdata(device);

	return scnprintf(buf, PAGE_SIZE, "%s\n", &dev->model_number[0]);
}

static DEVICE_ATTR(hw_rev, S_IRUGO, show_rev, NULL);
static DEVICE_ATTR(hca_type, S_IRUGO, show_hca_type, NULL);

static struct device_attribute *ocrdma_attributes[] = {
	&dev_attr_hw_rev,
	&dev_attr_hca_type
};

static void ocrdma_remove_sysfiles(struct ocrdma_dev *dev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ocrdma_attributes); i++)
		device_remove_file(&dev->ibdev.dev, ocrdma_attributes[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct ocrdma_dev *ocrdma_add(struct be_dev_info *dev_info)
{
<<<<<<< HEAD
	int status = 0;
=======
	int status = 0, i;
	u8 lstate = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct ocrdma_dev *dev;

	dev = (struct ocrdma_dev *)ib_alloc_device(sizeof(struct ocrdma_dev));
	if (!dev) {
<<<<<<< HEAD
		ocrdma_err("Unable to allocate ib device\n");
=======
		pr_err("Unable to allocate ib device\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return NULL;
	}
	dev->mbx_cmd = kzalloc(sizeof(struct ocrdma_mqe_emb_cmd), GFP_KERNEL);
	if (!dev->mbx_cmd)
		goto idr_err;

	memcpy(&dev->nic_info, dev_info, sizeof(*dev_info));
	dev->id = idr_alloc(&ocrdma_dev_id, NULL, 0, 0, GFP_KERNEL);
	if (dev->id < 0)
		goto idr_err;

	status = ocrdma_init_hw(dev);
	if (status)
		goto init_err;

	status = ocrdma_alloc_resources(dev);
	if (status)
		goto alloc_err;

<<<<<<< HEAD
	status = ocrdma_build_sgid_tbl(dev);
	if (status)
		goto alloc_err;

=======
	ocrdma_init_service_level(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	status = ocrdma_register_device(dev);
	if (status)
		goto alloc_err;

<<<<<<< HEAD
	spin_lock(&ocrdma_devlist_lock);
	list_add_tail_rcu(&dev->entry, &ocrdma_dev_list);
	spin_unlock(&ocrdma_devlist_lock);
	return dev;

=======
	/* Query Link state and update */
	status = ocrdma_mbx_get_link_speed(dev, NULL, &lstate);
	if (!status)
		ocrdma_update_link_state(dev, lstate);

	for (i = 0; i < ARRAY_SIZE(ocrdma_attributes); i++)
		if (device_create_file(&dev->ibdev.dev, ocrdma_attributes[i]))
			goto sysfs_err;
	/* Init stats */
	ocrdma_add_port_stats(dev);
	/* Interrupt Moderation */
	INIT_DELAYED_WORK(&dev->eqd_work, ocrdma_eqd_set_task);
	schedule_delayed_work(&dev->eqd_work, msecs_to_jiffies(1000));

	pr_info("%s %s: %s \"%s\" port %d\n",
		dev_name(&dev->nic_info.pdev->dev), hca_name(dev),
		port_speed_string(dev), dev->model_number,
		dev->hba_port_num);
	pr_info("%s ocrdma%d driver loaded successfully\n",
		dev_name(&dev->nic_info.pdev->dev), dev->id);
	return dev;

sysfs_err:
	ocrdma_remove_sysfiles(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
alloc_err:
	ocrdma_free_resources(dev);
	ocrdma_cleanup_hw(dev);
init_err:
	idr_remove(&ocrdma_dev_id, dev->id);
idr_err:
	kfree(dev->mbx_cmd);
	ib_dealloc_device(&dev->ibdev);
<<<<<<< HEAD
	ocrdma_err("%s() leaving. ret=%d\n", __func__, status);
	return NULL;
}

static void ocrdma_remove_free(struct rcu_head *rcu)
{
	struct ocrdma_dev *dev = container_of(rcu, struct ocrdma_dev, rcu);

	ocrdma_free_resources(dev);
	ocrdma_cleanup_hw(dev);
=======
	pr_err("%s() leaving. ret=%d\n", __func__, status);
	return NULL;
}

static void ocrdma_remove_free(struct ocrdma_dev *dev)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	idr_remove(&ocrdma_dev_id, dev->id);
	kfree(dev->mbx_cmd);
	ib_dealloc_device(&dev->ibdev);
}

static void ocrdma_remove(struct ocrdma_dev *dev)
{
	/* first unregister with stack to stop all the active traffic
	 * of the registered clients.
	 */
<<<<<<< HEAD
	ib_unregister_device(&dev->ibdev);

	spin_lock(&ocrdma_devlist_lock);
	list_del_rcu(&dev->entry);
	spin_unlock(&ocrdma_devlist_lock);
	call_rcu(&dev->rcu, ocrdma_remove_free);
}

static int ocrdma_open(struct ocrdma_dev *dev)
=======
	cancel_delayed_work_sync(&dev->eqd_work);
	ocrdma_remove_sysfiles(dev);
	ib_unregister_device(&dev->ibdev);

	ocrdma_rem_port_stats(dev);
	ocrdma_free_resources(dev);
	ocrdma_cleanup_hw(dev);
	ocrdma_remove_free(dev);
}

static int ocrdma_dispatch_port_active(struct ocrdma_dev *dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ib_event port_event;

	port_event.event = IB_EVENT_PORT_ACTIVE;
	port_event.element.port_num = 1;
	port_event.device = &dev->ibdev;
	ib_dispatch_event(&port_event);
	return 0;
}

<<<<<<< HEAD
static int ocrdma_close(struct ocrdma_dev *dev)
{
	int i;
	struct ocrdma_qp *qp, **cur_qp;
	struct ib_event err_event;
	struct ib_qp_attr attrs;
	int attr_mask = IB_QP_STATE;

	attrs.qp_state = IB_QPS_ERR;
	mutex_lock(&dev->dev_lock);
	if (dev->qp_tbl) {
		cur_qp = dev->qp_tbl;
		for (i = 0; i < OCRDMA_MAX_QP; i++) {
			qp = cur_qp[i];
			if (qp) {
				/* change the QP state to ERROR */
				_ocrdma_modify_qp(&qp->ibqp, &attrs, attr_mask);

				err_event.event = IB_EVENT_QP_FATAL;
				err_event.element.qp = &qp->ibqp;
				err_event.device = &dev->ibdev;
				ib_dispatch_event(&err_event);
			}
		}
	}
	mutex_unlock(&dev->dev_lock);
=======
static int ocrdma_dispatch_port_error(struct ocrdma_dev *dev)
{
	struct ib_event err_event;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err_event.event = IB_EVENT_PORT_ERR;
	err_event.element.port_num = 1;
	err_event.device = &dev->ibdev;
	ib_dispatch_event(&err_event);
	return 0;
}

<<<<<<< HEAD
=======
static void ocrdma_shutdown(struct ocrdma_dev *dev)
{
	ocrdma_dispatch_port_error(dev);
	ocrdma_remove(dev);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* event handling via NIC driver ensures that all the NIC specific
 * initialization done before RoCE driver notifies
 * event to stack.
 */
static void ocrdma_event_handler(struct ocrdma_dev *dev, u32 event)
{
	switch (event) {
<<<<<<< HEAD
	case BE_DEV_UP:
		ocrdma_open(dev);
		break;
	case BE_DEV_DOWN:
		ocrdma_close(dev);
		break;
	};
=======
	case BE_DEV_SHUTDOWN:
		ocrdma_shutdown(dev);
		break;
	default:
		break;
	}
}

void ocrdma_update_link_state(struct ocrdma_dev *dev, u8 lstate)
{
	if (!(dev->flags & OCRDMA_FLAGS_LINK_STATUS_INIT)) {
		dev->flags |= OCRDMA_FLAGS_LINK_STATUS_INIT;
		if (!lstate)
			return;
	}

	if (!lstate)
		ocrdma_dispatch_port_error(dev);
	else
		ocrdma_dispatch_port_active(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct ocrdma_driver ocrdma_drv = {
	.name			= "ocrdma_driver",
	.add			= ocrdma_add,
	.remove			= ocrdma_remove,
	.state_change_handler	= ocrdma_event_handler,
<<<<<<< HEAD
};

static void ocrdma_unregister_inet6addr_notifier(void)
{
#if IS_ENABLED(CONFIG_IPV6)
	unregister_inet6addr_notifier(&ocrdma_inet6addr_notifier);
#endif
}

=======
	.be_abi_version		= OCRDMA_BE_ROCE_ABI_VERSION,
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int __init ocrdma_init_module(void)
{
	int status;

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_IPV6)
	status = register_inet6addr_notifier(&ocrdma_inet6addr_notifier);
	if (status)
		return status;
#endif

	status = be_roce_register_driver(&ocrdma_drv);
	if (status)
		ocrdma_unregister_inet6addr_notifier();
=======
	ocrdma_init_debugfs();

	status = be_roce_register_driver(&ocrdma_drv);
	if (status)
		goto err_be_reg;

	return 0;

err_be_reg:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return status;
}

static void __exit ocrdma_exit_module(void)
{
	be_roce_unregister_driver(&ocrdma_drv);
<<<<<<< HEAD
	ocrdma_unregister_inet6addr_notifier();
=======
	ocrdma_rem_debugfs();
	idr_destroy(&ocrdma_dev_id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

module_init(ocrdma_init_module);
module_exit(ocrdma_exit_module);
