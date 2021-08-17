/*
 *
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Copyright (c) 2003-2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
<<<<<<< HEAD

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/slab.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
<<<<<<< HEAD
#include <linux/aio.h>
#include <linux/pci.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/uuid.h>
#include <linux/compat.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/miscdevice.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/mei.h>

#include "mei_dev.h"
<<<<<<< HEAD
#include "hw-me.h"
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "client.h"

/**
 * mei_open - the open function
 *
 * @inode: pointer to inode structure
 * @file: pointer to file structure
<<<<<<< HEAD
 e
 * returns 0 on success, <0 on error
 */
static int mei_open(struct inode *inode, struct file *file)
{
	struct miscdevice *misc = file->private_data;
	struct pci_dev *pdev;
	struct mei_cl *cl;
	struct mei_device *dev;

	int err;

	err = -ENODEV;
	if (!misc->parent)
		goto out;

	pdev = container_of(misc->parent, struct pci_dev, dev);

	dev = pci_get_drvdata(pdev);
	if (!dev)
		goto out;

	mutex_lock(&dev->device_lock);
	err = -ENOMEM;
	cl = mei_cl_allocate(dev);
	if (!cl)
		goto out_unlock;

	err = -ENODEV;
	if (dev->dev_state != MEI_DEV_ENABLED) {
		dev_dbg(&dev->pdev->dev, "dev_state != MEI_ENABLED  dev_state = %s\n",
		    mei_dev_state_str(dev->dev_state));
		goto out_unlock;
	}
	err = -EMFILE;
	if (dev->open_handle_count >= MEI_MAX_OPEN_HANDLE_COUNT) {
		dev_err(&dev->pdev->dev, "open_handle_count exceded %d",
			MEI_MAX_OPEN_HANDLE_COUNT);
		goto out_unlock;
	}

	err = mei_cl_link(cl, MEI_HOST_CLIENT_ID_ANY);
	if (err)
		goto out_unlock;

	file->private_data = cl;
=======
 *
 * Return: 0 on success, <0 on error
 */
static int mei_open(struct inode *inode, struct file *file)
{
	struct mei_device *dev;
	struct mei_cl *cl;

	int err;

	dev = container_of(inode->i_cdev, struct mei_device, cdev);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->device_lock);

	if (dev->dev_state != MEI_DEV_ENABLED) {
		dev_dbg(dev->dev, "dev_state != MEI_ENABLED  dev_state = %s\n",
		    mei_dev_state_str(dev->dev_state));
		err = -ENODEV;
		goto err_unlock;
	}

	cl = mei_cl_alloc_linked(dev);
	if (IS_ERR(cl)) {
		err = PTR_ERR(cl);
		goto err_unlock;
	}

	cl->fp = file;
	file->private_data = cl;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&dev->device_lock);

	return nonseekable_open(inode, file);

<<<<<<< HEAD
out_unlock:
	mutex_unlock(&dev->device_lock);
	kfree(cl);
out:
=======
err_unlock:
	mutex_unlock(&dev->device_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return err;
}

/**
 * mei_release - the release function
 *
 * @inode: pointer to inode structure
 * @file: pointer to file structure
 *
<<<<<<< HEAD
 * returns 0 on success, <0 on error
=======
 * Return: 0 on success, <0 on error
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int mei_release(struct inode *inode, struct file *file)
{
	struct mei_cl *cl = file->private_data;
<<<<<<< HEAD
	struct mei_cl_cb *cb;
	struct mei_device *dev;
	int rets = 0;
=======
	struct mei_device *dev;
	int rets;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (WARN_ON(!cl || !cl->dev))
		return -ENODEV;

	dev = cl->dev;

	mutex_lock(&dev->device_lock);
	if (cl == &dev->iamthif_cl) {
		rets = mei_amthif_release(dev, file);
		goto out;
	}
<<<<<<< HEAD
	if (cl->state == MEI_FILE_CONNECTED) {
		cl->state = MEI_FILE_DISCONNECTING;
		dev_dbg(&dev->pdev->dev,
			"disconnecting client host client = %d, "
		    "ME client = %d\n",
		    cl->host_client_id,
		    cl->me_client_id);
		rets = mei_cl_disconnect(cl);
	}
	mei_cl_flush_queues(cl);
	dev_dbg(&dev->pdev->dev, "remove client host client = %d, ME client = %d\n",
	    cl->host_client_id,
	    cl->me_client_id);

	if (dev->open_handle_count > 0) {
		clear_bit(cl->host_client_id, dev->host_clients_map);
		dev->open_handle_count--;
	}
	mei_cl_unlink(cl);


	/* free read cb */
	cb = NULL;
	if (cl->read_cb) {
		cb = mei_cl_find_read_cb(cl);
		/* Remove entry from read list */
		if (cb)
			list_del(&cb->list);

		cb = cl->read_cb;
		cl->read_cb = NULL;
	}

	file->private_data = NULL;

	if (cb) {
		mei_io_cb_free(cb);
		cb = NULL;
	}

=======
	rets = mei_cl_disconnect(cl);

	mei_cl_flush_queues(cl, file);
	cl_dbg(dev, cl, "removing\n");

	mei_cl_unlink(cl);

	file->private_data = NULL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(cl);
out:
	mutex_unlock(&dev->device_lock);
	return rets;
}


/**
 * mei_read - the read function.
 *
 * @file: pointer to file structure
 * @ubuf: pointer to user buffer
 * @length: buffer length
 * @offset: data offset in buffer
 *
<<<<<<< HEAD
 * returns >=0 data length on success , <0 on error
=======
 * Return: >=0 data length on success , <0 on error
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static ssize_t mei_read(struct file *file, char __user *ubuf,
			size_t length, loff_t *offset)
{
	struct mei_cl *cl = file->private_data;
<<<<<<< HEAD
	struct mei_cl_cb *cb_pos = NULL;
	struct mei_cl_cb *cb = NULL;
	struct mei_device *dev;
	int i;
	int rets;
	int err;

=======
	struct mei_device *dev;
	struct mei_cl_cb *cb = NULL;
	bool nonblock = !!(file->f_flags & O_NONBLOCK);
	int rets;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (WARN_ON(!cl || !cl->dev))
		return -ENODEV;

	dev = cl->dev;

<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&dev->device_lock);
	if (dev->dev_state != MEI_DEV_ENABLED) {
		rets = -ENODEV;
		goto out;
	}

<<<<<<< HEAD
	if ((cl->sm_state & MEI_WD_STATE_INDEPENDENCE_MSG_SENT) == 0) {
		/* Do not allow to read watchdog client */
		i = mei_me_cl_by_uuid(dev, &mei_wd_guid);
		if (i >= 0) {
			struct mei_me_client *me_client = &dev->me_clients[i];
			if (cl->me_client_id == me_client->client_id) {
				rets = -EBADF;
				goto out;
			}
		}
	} else {
		cl->sm_state &= ~MEI_WD_STATE_INDEPENDENCE_MSG_SENT;
	}

	if (cl == &dev->iamthif_cl) {
		rets = mei_amthif_read(dev, file, ubuf, length, offset);
		goto out;
	}

	if (cl->read_cb && cl->read_cb->buf_idx > *offset) {
		cb = cl->read_cb;
		goto copy_buffer;
	} else if (cl->read_cb && cl->read_cb->buf_idx > 0 &&
		   cl->read_cb->buf_idx <= *offset) {
		cb = cl->read_cb;
		rets = 0;
		goto free;
	} else if ((!cl->read_cb || !cl->read_cb->buf_idx) && *offset > 0) {
		/*Offset needs to be cleaned for contiguous reads*/
		*offset = 0;
		rets = 0;
		goto out;
	}

	err = mei_cl_read_start(cl, length);
	if (err && err != -EBUSY) {
		dev_dbg(&dev->pdev->dev,
			"mei start read failure with status = %d\n", err);
		rets = err;
		goto out;
	}

	if (MEI_READ_COMPLETE != cl->reading_state &&
			!waitqueue_active(&cl->rx_wait)) {
		if (file->f_flags & O_NONBLOCK) {
			rets = -EAGAIN;
			goto out;
		}

		mutex_unlock(&dev->device_lock);

		if (wait_event_interruptible(cl->rx_wait,
				MEI_READ_COMPLETE == cl->reading_state ||
				mei_cl_is_transitioning(cl))) {

			if (signal_pending(current))
				return -EINTR;
			return -ERESTARTSYS;
		}

		mutex_lock(&dev->device_lock);
		if (mei_cl_is_transitioning(cl)) {
			rets = -EBUSY;
			goto out;
		}
	}

	cb = cl->read_cb;

	if (!cb) {
		rets = -ENODEV;
		goto out;
	}
	if (cl->reading_state != MEI_READ_COMPLETE) {
		rets = 0;
		goto out;
	}
	/* now copy the data to user space */
copy_buffer:
	dev_dbg(&dev->pdev->dev, "buf.size = %d buf.idx= %ld\n",
	    cb->response_buffer.size, cb->buf_idx);
	if (length == 0 || ubuf == NULL || *offset > cb->buf_idx) {
		rets = -EMSGSIZE;
=======
	if (length == 0) {
		rets = 0;
		goto out;
	}

	if (ubuf == NULL) {
		rets = -EMSGSIZE;
		goto out;
	}

	cb = mei_cl_read_cb(cl, file);
	if (cb)
		goto copy_buffer;

	if (*offset > 0)
		*offset = 0;

	rets = mei_cl_read_start(cl, length, file);
	if (rets && rets != -EBUSY) {
		cl_dbg(dev, cl, "mei start read failure status = %d\n", rets);
		goto out;
	}

	if (nonblock) {
		rets = -EAGAIN;
		goto out;
	}


again:
	mutex_unlock(&dev->device_lock);
	if (wait_event_interruptible(cl->rx_wait,
				     !list_empty(&cl->rd_completed) ||
				     !mei_cl_is_connected(cl))) {
		if (signal_pending(current))
			return -EINTR;
		return -ERESTARTSYS;
	}
	mutex_lock(&dev->device_lock);

	if (!mei_cl_is_connected(cl)) {
		rets = -ENODEV;
		goto out;
	}

	cb = mei_cl_read_cb(cl, file);
	if (!cb) {
		/*
		 * For amthif all the waiters are woken up,
		 * but only fp with matching cb->fp get the cb,
		 * the others have to return to wait on read.
		 */
		if (cl == &dev->iamthif_cl)
			goto again;

		rets = 0;
		goto out;
	}

copy_buffer:
	/* now copy the data to user space */
	if (cb->status) {
		rets = cb->status;
		cl_dbg(dev, cl, "read operation failed %d\n", rets);
		goto free;
	}

	cl_dbg(dev, cl, "buf.size = %zu buf.idx = %zu offset = %lld\n",
	       cb->buf.size, cb->buf_idx, *offset);
	if (*offset >= cb->buf_idx) {
		rets = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto free;
	}

	/* length is being truncated to PAGE_SIZE,
	 * however buf_idx may point beyond that */
	length = min_t(size_t, length, cb->buf_idx - *offset);

<<<<<<< HEAD
	if (copy_to_user(ubuf, cb->response_buffer.data + *offset, length)) {
=======
	if (copy_to_user(ubuf, cb->buf.data + *offset, length)) {
		dev_dbg(dev->dev, "failed to copy data to userland\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rets = -EFAULT;
		goto free;
	}

	rets = length;
	*offset += length;
<<<<<<< HEAD
	if ((unsigned long)*offset < cb->buf_idx)
		goto out;

free:
	cb_pos = mei_cl_find_read_cb(cl);
	/* Remove entry from read list */
	if (cb_pos)
		list_del(&cb_pos->list);
	mei_io_cb_free(cb);
	cl->reading_state = MEI_IDLE;
	cl->read_cb = NULL;
out:
	dev_dbg(&dev->pdev->dev, "end mei read rets= %d\n", rets);
=======
	/* not all data was read, keep the cb */
	if (*offset < cb->buf_idx)
		goto out;

free:
	mei_io_cb_free(cb);
	*offset = 0;

out:
	cl_dbg(dev, cl, "end mei read rets = %d\n", rets);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&dev->device_lock);
	return rets;
}
/**
 * mei_write - the write function.
 *
 * @file: pointer to file structure
 * @ubuf: pointer to user buffer
 * @length: buffer length
 * @offset: data offset in buffer
 *
<<<<<<< HEAD
 * returns >=0 data length on success , <0 on error
=======
 * Return: >=0 data length on success , <0 on error
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static ssize_t mei_write(struct file *file, const char __user *ubuf,
			 size_t length, loff_t *offset)
{
	struct mei_cl *cl = file->private_data;
<<<<<<< HEAD
	struct mei_cl_cb *write_cb = NULL;
	struct mei_device *dev;
	unsigned long timeout = 0;
	int rets;
	int id;
=======
	struct mei_cl_cb *cb;
	struct mei_device *dev;
	int rets;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (WARN_ON(!cl || !cl->dev))
		return -ENODEV;

	dev = cl->dev;

	mutex_lock(&dev->device_lock);

	if (dev->dev_state != MEI_DEV_ENABLED) {
		rets = -ENODEV;
		goto out;
	}

<<<<<<< HEAD
	id = mei_me_cl_by_id(dev, cl->me_client_id);
	if (id < 0) {
		rets = -ENODEV;
		goto out;
	}
	if (length > dev->me_clients[id].props.max_msg_length || length <= 0) {
		rets = -EMSGSIZE;
		goto out;
	}

	if (cl->state != MEI_FILE_CONNECTED) {
		dev_err(&dev->pdev->dev, "host client = %d,  is not connected to ME client = %d",
			cl->host_client_id, cl->me_client_id);
		rets = -ENODEV;
		goto out;
	}
	if (cl == &dev->iamthif_cl) {
		write_cb = mei_amthif_find_read_list_entry(dev, file);

		if (write_cb) {
			timeout = write_cb->read_time +
				mei_secs_to_jiffies(MEI_IAMTHIF_READ_TIMER);

			if (time_after(jiffies, timeout) ||
			    cl->reading_state == MEI_READ_COMPLETE) {
				*offset = 0;
				list_del(&write_cb->list);
				mei_io_cb_free(write_cb);
				write_cb = NULL;
			}
		}
	}

	/* free entry used in read */
	if (cl->reading_state == MEI_READ_COMPLETE) {
		*offset = 0;
		write_cb = mei_cl_find_read_cb(cl);
		if (write_cb) {
			list_del(&write_cb->list);
			mei_io_cb_free(write_cb);
			write_cb = NULL;
			cl->reading_state = MEI_IDLE;
			cl->read_cb = NULL;
		}
	} else if (cl->reading_state == MEI_IDLE)
		*offset = 0;


	write_cb = mei_io_cb_init(cl, file);
	if (!write_cb) {
		dev_err(&dev->pdev->dev, "write cb allocation failed\n");
		rets = -ENOMEM;
		goto out;
	}
	rets = mei_io_cb_alloc_req_buf(write_cb, length);
	if (rets)
		goto out;

	rets = copy_from_user(write_cb->request_buffer.data, ubuf, length);
	if (rets)
		goto out;

	cl->sm_state = 0;
	if (length == 4 &&
	    ((memcmp(mei_wd_state_independence_msg[0],
				 write_cb->request_buffer.data, 4) == 0) ||
	     (memcmp(mei_wd_state_independence_msg[1],
				 write_cb->request_buffer.data, 4) == 0) ||
	     (memcmp(mei_wd_state_independence_msg[2],
				 write_cb->request_buffer.data, 4) == 0)))
		cl->sm_state |= MEI_WD_STATE_INDEPENDENCE_MSG_SENT;

	if (cl == &dev->iamthif_cl) {
		rets = mei_amthif_write(dev, write_cb);

		if (rets) {
			dev_err(&dev->pdev->dev,
				"amthif write failed with status = %d\n", rets);
			goto out;
		}
		mutex_unlock(&dev->device_lock);
		return length;
	}

	rets = mei_cl_write(cl, write_cb, false);
out:
	mutex_unlock(&dev->device_lock);
	if (rets < 0)
		mei_io_cb_free(write_cb);
=======
	if (!mei_cl_is_connected(cl)) {
		cl_err(dev, cl, "is not connected");
		rets = -ENODEV;
		goto out;
	}

	if (!mei_me_cl_is_active(cl->me_cl)) {
		rets = -ENOTTY;
		goto out;
	}

	if (length > mei_cl_mtu(cl)) {
		rets = -EFBIG;
		goto out;
	}

	if (length == 0) {
		rets = 0;
		goto out;
	}

	cb = mei_cl_alloc_cb(cl, length, MEI_FOP_WRITE, file);
	if (!cb) {
		rets = -ENOMEM;
		goto out;
	}

	rets = copy_from_user(cb->buf.data, ubuf, length);
	if (rets) {
		dev_dbg(dev->dev, "failed to copy data from userland\n");
		rets = -EFAULT;
		mei_io_cb_free(cb);
		goto out;
	}

	if (cl == &dev->iamthif_cl) {
		rets = mei_amthif_write(cl, cb);
		if (!rets)
			rets = length;
		goto out;
	}

	rets = mei_cl_write(cl, cb, false);
out:
	mutex_unlock(&dev->device_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return rets;
}

/**
 * mei_ioctl_connect_client - the connect to fw client IOCTL function
 *
<<<<<<< HEAD
 * @dev: the device structure
 * @data: IOCTL connect data, input and output parameters
 * @file: private data of the file object
 *
 * Locking: called under "dev->device_lock" lock
 *
 * returns 0 on success, <0 on failure.
=======
 * @file: private data of the file object
 * @data: IOCTL connect data, input and output parameters
 *
 * Locking: called under "dev->device_lock" lock
 *
 * Return: 0 on success, <0 on failure.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int mei_ioctl_connect_client(struct file *file,
			struct mei_connect_client_data *data)
{
	struct mei_device *dev;
	struct mei_client *client;
<<<<<<< HEAD
	struct mei_cl *cl;
	int i;
	int rets;

	cl = file->private_data;
	if (WARN_ON(!cl || !cl->dev))
		return -ENODEV;

	dev = cl->dev;

	if (dev->dev_state != MEI_DEV_ENABLED) {
		rets = -ENODEV;
		goto end;
	}

	if (cl->state != MEI_FILE_INITIALIZING &&
	    cl->state != MEI_FILE_DISCONNECTED) {
		rets = -EBUSY;
		goto end;
	}

	/* find ME client we're trying to connect to */
	i = mei_me_cl_by_uuid(dev, &data->in_client_uuid);
	if (i < 0 || dev->me_clients[i].props.fixed_address) {
		dev_dbg(&dev->pdev->dev, "Cannot connect to FW Client UUID = %pUl\n",
				&data->in_client_uuid);
		rets = -ENODEV;
		goto end;
	}

	cl->me_client_id = dev->me_clients[i].client_id;
	cl->state = MEI_FILE_CONNECTING;

	dev_dbg(&dev->pdev->dev, "Connect to FW Client ID = %d\n",
			cl->me_client_id);
	dev_dbg(&dev->pdev->dev, "FW Client - Protocol Version = %d\n",
			dev->me_clients[i].props.protocol_version);
	dev_dbg(&dev->pdev->dev, "FW Client - Max Msg Len = %d\n",
			dev->me_clients[i].props.max_msg_length);
=======
	struct mei_me_client *me_cl;
	struct mei_cl *cl;
	int rets;

	cl = file->private_data;
	dev = cl->dev;

	if (dev->dev_state != MEI_DEV_ENABLED)
		return -ENODEV;

	if (cl->state != MEI_FILE_INITIALIZING &&
	    cl->state != MEI_FILE_DISCONNECTED)
		return  -EBUSY;

	/* find ME client we're trying to connect to */
	me_cl = mei_me_cl_by_uuid(dev, &data->in_client_uuid);
	if (!me_cl) {
		dev_dbg(dev->dev, "Cannot connect to FW Client UUID = %pUl\n",
			&data->in_client_uuid);
		rets = -ENOTTY;
		goto end;
	}

	if (me_cl->props.fixed_address) {
		bool forbidden = dev->override_fixed_address ?
			 !dev->allow_fixed_address : !dev->hbm_f_fa_supported;
		if (forbidden) {
			dev_dbg(dev->dev, "Connection forbidden to FW Client UUID = %pUl\n",
				&data->in_client_uuid);
			rets = -ENOTTY;
			goto end;
		}
	}

	dev_dbg(dev->dev, "Connect to FW Client ID = %d\n",
			me_cl->client_id);
	dev_dbg(dev->dev, "FW Client - Protocol Version = %d\n",
			me_cl->props.protocol_version);
	dev_dbg(dev->dev, "FW Client - Max Msg Len = %d\n",
			me_cl->props.max_msg_length);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* if we're connecting to amthif client then we will use the
	 * existing connection
	 */
	if (uuid_le_cmp(data->in_client_uuid, mei_amthif_guid) == 0) {
<<<<<<< HEAD
		dev_dbg(&dev->pdev->dev, "FW Client is amthi\n");
		if (dev->iamthif_cl.state != MEI_FILE_CONNECTED) {
			rets = -ENODEV;
			goto end;
		}
		clear_bit(cl->host_client_id, dev->host_clients_map);
=======
		dev_dbg(dev->dev, "FW Client is amthi\n");
		if (!mei_cl_is_connected(&dev->iamthif_cl)) {
			rets = -ENODEV;
			goto end;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		mei_cl_unlink(cl);

		kfree(cl);
		cl = NULL;
<<<<<<< HEAD
		file->private_data = &dev->iamthif_cl;

		client = &data->out_client_properties;
		client->max_msg_length =
			dev->me_clients[i].props.max_msg_length;
		client->protocol_version =
			dev->me_clients[i].props.protocol_version;
=======
		dev->iamthif_open_count++;
		file->private_data = &dev->iamthif_cl;

		client = &data->out_client_properties;
		client->max_msg_length = me_cl->props.max_msg_length;
		client->protocol_version = me_cl->props.protocol_version;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rets = dev->iamthif_cl.status;

		goto end;
	}

<<<<<<< HEAD

	/* prepare the output buffer */
	client = &data->out_client_properties;
	client->max_msg_length = dev->me_clients[i].props.max_msg_length;
	client->protocol_version = dev->me_clients[i].props.protocol_version;
	dev_dbg(&dev->pdev->dev, "Can connect?\n");


	rets = mei_cl_connect(cl, file);

end:
	return rets;
}

=======
	/* prepare the output buffer */
	client = &data->out_client_properties;
	client->max_msg_length = me_cl->props.max_msg_length;
	client->protocol_version = me_cl->props.protocol_version;
	dev_dbg(dev->dev, "Can connect?\n");

	rets = mei_cl_connect(cl, me_cl, file);

end:
	mei_me_cl_put(me_cl);
	return rets;
}

/**
 * mei_ioctl_client_notify_request -
 *     propagate event notification request to client
 *
 * @file: pointer to file structure
 * @request: 0 - disable, 1 - enable
 *
 * Return: 0 on success , <0 on error
 */
static int mei_ioctl_client_notify_request(const struct file *file, u32 request)
{
	struct mei_cl *cl = file->private_data;

	if (request != MEI_HBM_NOTIFICATION_START &&
	    request != MEI_HBM_NOTIFICATION_STOP)
		return -EINVAL;

	return mei_cl_notify_request(cl, file, (u8)request);
}

/**
 * mei_ioctl_client_notify_get -  wait for notification request
 *
 * @file: pointer to file structure
 * @notify_get: 0 - disable, 1 - enable
 *
 * Return: 0 on success , <0 on error
 */
static int mei_ioctl_client_notify_get(const struct file *file, u32 *notify_get)
{
	struct mei_cl *cl = file->private_data;
	bool notify_ev;
	bool block = (file->f_flags & O_NONBLOCK) == 0;
	int rets;

	rets = mei_cl_notify_get(cl, block, &notify_ev);
	if (rets)
		return rets;

	*notify_get = notify_ev ? 1 : 0;
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * mei_ioctl - the IOCTL function
 *
 * @file: pointer to file structure
 * @cmd: ioctl command
 * @data: pointer to mei message structure
 *
<<<<<<< HEAD
 * returns 0 on success , <0 on error
=======
 * Return: 0 on success , <0 on error
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static long mei_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{
	struct mei_device *dev;
	struct mei_cl *cl = file->private_data;
<<<<<<< HEAD
	struct mei_connect_client_data *connect_data = NULL;
	int rets;

	if (cmd != IOCTL_MEI_CONNECT_CLIENT)
		return -EINVAL;
=======
	struct mei_connect_client_data connect_data;
	u32 notify_get, notify_req;
	int rets;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (WARN_ON(!cl || !cl->dev))
		return -ENODEV;

	dev = cl->dev;

<<<<<<< HEAD
	dev_dbg(&dev->pdev->dev, "IOCTL cmd = 0x%x", cmd);
=======
	dev_dbg(dev->dev, "IOCTL cmd = 0x%x", cmd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&dev->device_lock);
	if (dev->dev_state != MEI_DEV_ENABLED) {
		rets = -ENODEV;
		goto out;
	}

<<<<<<< HEAD
	dev_dbg(&dev->pdev->dev, ": IOCTL_MEI_CONNECT_CLIENT.\n");

	connect_data = kzalloc(sizeof(struct mei_connect_client_data),
							GFP_KERNEL);
	if (!connect_data) {
		rets = -ENOMEM;
		goto out;
	}
	dev_dbg(&dev->pdev->dev, "copy connect data from user\n");
	if (copy_from_user(connect_data, (char __user *)data,
				sizeof(struct mei_connect_client_data))) {
		dev_dbg(&dev->pdev->dev, "failed to copy data from userland\n");
		rets = -EFAULT;
		goto out;
	}

	rets = mei_ioctl_connect_client(file, connect_data);

	/* if all is ok, copying the data back to user. */
	if (rets)
		goto out;

	dev_dbg(&dev->pdev->dev, "copy connect data to user\n");
	if (copy_to_user((char __user *)data, connect_data,
				sizeof(struct mei_connect_client_data))) {
		dev_dbg(&dev->pdev->dev, "failed to copy data to userland\n");
		rets = -EFAULT;
		goto out;
	}

out:
	kfree(connect_data);
=======
	switch (cmd) {
	case IOCTL_MEI_CONNECT_CLIENT:
		dev_dbg(dev->dev, ": IOCTL_MEI_CONNECT_CLIENT.\n");
		if (copy_from_user(&connect_data, (char __user *)data,
				sizeof(struct mei_connect_client_data))) {
			dev_dbg(dev->dev, "failed to copy data from userland\n");
			rets = -EFAULT;
			goto out;
		}

		rets = mei_ioctl_connect_client(file, &connect_data);
		if (rets)
			goto out;

		/* if all is ok, copying the data back to user. */
		if (copy_to_user((char __user *)data, &connect_data,
				sizeof(struct mei_connect_client_data))) {
			dev_dbg(dev->dev, "failed to copy data to userland\n");
			rets = -EFAULT;
			goto out;
		}

		break;

	case IOCTL_MEI_NOTIFY_SET:
		dev_dbg(dev->dev, ": IOCTL_MEI_NOTIFY_SET.\n");
		if (copy_from_user(&notify_req,
				   (char __user *)data, sizeof(notify_req))) {
			dev_dbg(dev->dev, "failed to copy data from userland\n");
			rets = -EFAULT;
			goto out;
		}
		rets = mei_ioctl_client_notify_request(file, notify_req);
		break;

	case IOCTL_MEI_NOTIFY_GET:
		dev_dbg(dev->dev, ": IOCTL_MEI_NOTIFY_GET.\n");
		rets = mei_ioctl_client_notify_get(file, &notify_get);
		if (rets)
			goto out;

		dev_dbg(dev->dev, "copy connect data to user\n");
		if (copy_to_user((char __user *)data,
				&notify_get, sizeof(notify_get))) {
			dev_dbg(dev->dev, "failed to copy data to userland\n");
			rets = -EFAULT;
			goto out;

		}
		break;

	default:
		rets = -ENOIOCTLCMD;
	}

out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_unlock(&dev->device_lock);
	return rets;
}

/**
 * mei_compat_ioctl - the compat IOCTL function
 *
 * @file: pointer to file structure
 * @cmd: ioctl command
 * @data: pointer to mei message structure
 *
<<<<<<< HEAD
 * returns 0 on success , <0 on error
=======
 * Return: 0 on success , <0 on error
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
#ifdef CONFIG_COMPAT
static long mei_compat_ioctl(struct file *file,
			unsigned int cmd, unsigned long data)
{
	return mei_ioctl(file, cmd, (unsigned long)compat_ptr(data));
}
#endif


/**
 * mei_poll - the poll function
 *
 * @file: pointer to file structure
 * @wait: pointer to poll_table structure
 *
<<<<<<< HEAD
 * returns poll mask
 */
static unsigned int mei_poll(struct file *file, poll_table *wait)
{
	struct mei_cl *cl = file->private_data;
	struct mei_device *dev;
	unsigned int mask = 0;

	if (WARN_ON(!cl || !cl->dev))
		return mask;
=======
 * Return: poll mask
 */
static unsigned int mei_poll(struct file *file, poll_table *wait)
{
	unsigned long req_events = poll_requested_events(wait);
	struct mei_cl *cl = file->private_data;
	struct mei_device *dev;
	unsigned int mask = 0;
	bool notify_en;

	if (WARN_ON(!cl || !cl->dev))
		return POLLERR;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev = cl->dev;

	mutex_lock(&dev->device_lock);

<<<<<<< HEAD
	if (dev->dev_state != MEI_DEV_ENABLED)
		goto out;


	if (cl == &dev->iamthif_cl) {
		mask = mei_amthif_poll(dev, file, wait);
		goto out;
	}

	mutex_unlock(&dev->device_lock);
	poll_wait(file, &cl->tx_wait, wait);
	mutex_lock(&dev->device_lock);
	if (MEI_WRITE_COMPLETE == cl->writing_state)
		mask |= (POLLIN | POLLRDNORM);
=======
	notify_en = cl->notify_en && (req_events & POLLPRI);

	if (dev->dev_state != MEI_DEV_ENABLED ||
	    !mei_cl_is_connected(cl)) {
		mask = POLLERR;
		goto out;
	}

	if (notify_en) {
		poll_wait(file, &cl->ev_wait, wait);
		if (cl->notify_ev)
			mask |= POLLPRI;
	}

	if (cl == &dev->iamthif_cl) {
		mask |= mei_amthif_poll(file, wait);
		goto out;
	}

	if (req_events & (POLLIN | POLLRDNORM)) {
		poll_wait(file, &cl->rx_wait, wait);

		if (!list_empty(&cl->rd_completed))
			mask |= POLLIN | POLLRDNORM;
		else
			mei_cl_read_start(cl, mei_cl_mtu(cl), file);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

out:
	mutex_unlock(&dev->device_lock);
	return mask;
}

<<<<<<< HEAD
=======
/**
 * mei_fasync - asynchronous io support
 *
 * @fd: file descriptor
 * @file: pointer to file structure
 * @band: band bitmap
 *
 * Return: negative on error,
 *         0 if it did no changes,
 *         and positive a process was added or deleted
 */
static int mei_fasync(int fd, struct file *file, int band)
{

	struct mei_cl *cl = file->private_data;

	if (!mei_cl_is_connected(cl))
		return -ENODEV;

	return fasync_helper(fd, file, band, &cl->ev_async);
}

/**
 * fw_status_show - mei device attribute show method
 *
 * @device: device pointer
 * @attr: attribute pointer
 * @buf:  char out buffer
 *
 * Return: number of the bytes printed into buf or error
 */
static ssize_t fw_status_show(struct device *device,
		struct device_attribute *attr, char *buf)
{
	struct mei_device *dev = dev_get_drvdata(device);
	struct mei_fw_status fw_status;
	int err, i;
	ssize_t cnt = 0;

	mutex_lock(&dev->device_lock);
	err = mei_fw_status(dev, &fw_status);
	mutex_unlock(&dev->device_lock);
	if (err) {
		dev_err(device, "read fw_status error = %d\n", err);
		return err;
	}

	for (i = 0; i < fw_status.count; i++)
		cnt += scnprintf(buf + cnt, PAGE_SIZE - cnt, "%08X\n",
				fw_status.status[i]);
	return cnt;
}
static DEVICE_ATTR_RO(fw_status);

static struct attribute *mei_attrs[] = {
	&dev_attr_fw_status.attr,
	NULL
};
ATTRIBUTE_GROUPS(mei);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * file operations structure will be used for mei char device.
 */
static const struct file_operations mei_fops = {
	.owner = THIS_MODULE,
	.read = mei_read,
	.unlocked_ioctl = mei_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = mei_compat_ioctl,
#endif
	.open = mei_open,
	.release = mei_release,
	.write = mei_write,
	.poll = mei_poll,
<<<<<<< HEAD
	.llseek = no_llseek
};

/*
 * Misc Device Struct
 */
static struct miscdevice  mei_misc_device = {
		.name = "mei",
		.fops = &mei_fops,
		.minor = MISC_DYNAMIC_MINOR,
};


int mei_register(struct mei_device *dev)
{
	int ret;
	mei_misc_device.parent = &dev->pdev->dev;
	ret = misc_register(&mei_misc_device);
	if (ret)
		return ret;

	if (mei_dbgfs_register(dev, mei_misc_device.name))
		dev_err(&dev->pdev->dev, "cannot register debugfs\n");

	return 0;
=======
	.fasync = mei_fasync,
	.llseek = no_llseek
};

static struct class *mei_class;
static dev_t mei_devt;
#define MEI_MAX_DEVS  MINORMASK
static DEFINE_MUTEX(mei_minor_lock);
static DEFINE_IDR(mei_idr);

/**
 * mei_minor_get - obtain next free device minor number
 *
 * @dev:  device pointer
 *
 * Return: allocated minor, or -ENOSPC if no free minor left
 */
static int mei_minor_get(struct mei_device *dev)
{
	int ret;

	mutex_lock(&mei_minor_lock);
	ret = idr_alloc(&mei_idr, dev, 0, MEI_MAX_DEVS, GFP_KERNEL);
	if (ret >= 0)
		dev->minor = ret;
	else if (ret == -ENOSPC)
		dev_err(dev->dev, "too many mei devices\n");

	mutex_unlock(&mei_minor_lock);
	return ret;
}

/**
 * mei_minor_free - mark device minor number as free
 *
 * @dev:  device pointer
 */
static void mei_minor_free(struct mei_device *dev)
{
	mutex_lock(&mei_minor_lock);
	idr_remove(&mei_idr, dev->minor);
	mutex_unlock(&mei_minor_lock);
}

int mei_register(struct mei_device *dev, struct device *parent)
{
	struct device *clsdev; /* class device */
	int ret, devno;

	ret = mei_minor_get(dev);
	if (ret < 0)
		return ret;

	/* Fill in the data structures */
	devno = MKDEV(MAJOR(mei_devt), dev->minor);
	cdev_init(&dev->cdev, &mei_fops);
	dev->cdev.owner = parent->driver->owner;

	/* Add the device */
	ret = cdev_add(&dev->cdev, devno, 1);
	if (ret) {
		dev_err(parent, "unable to add device %d:%d\n",
			MAJOR(mei_devt), dev->minor);
		goto err_dev_add;
	}

	clsdev = device_create_with_groups(mei_class, parent, devno,
					   dev, mei_groups,
					   "mei%d", dev->minor);

	if (IS_ERR(clsdev)) {
		dev_err(parent, "unable to create device %d:%d\n",
			MAJOR(mei_devt), dev->minor);
		ret = PTR_ERR(clsdev);
		goto err_dev_create;
	}

	ret = mei_dbgfs_register(dev, dev_name(clsdev));
	if (ret) {
		dev_err(clsdev, "cannot register debugfs ret = %d\n", ret);
		goto err_dev_dbgfs;
	}

	return 0;

err_dev_dbgfs:
	device_destroy(mei_class, devno);
err_dev_create:
	cdev_del(&dev->cdev);
err_dev_add:
	mei_minor_free(dev);
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(mei_register);

void mei_deregister(struct mei_device *dev)
{
<<<<<<< HEAD
	mei_dbgfs_deregister(dev);
	misc_deregister(&mei_misc_device);
	mei_misc_device.parent = NULL;
=======
	int devno;

	devno = dev->cdev.dev;
	cdev_del(&dev->cdev);

	mei_dbgfs_deregister(dev);

	device_destroy(mei_class, devno);

	mei_minor_free(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(mei_deregister);

static int __init mei_init(void)
{
<<<<<<< HEAD
	return mei_cl_bus_init();
=======
	int ret;

	mei_class = class_create(THIS_MODULE, "mei");
	if (IS_ERR(mei_class)) {
		pr_err("couldn't create class\n");
		ret = PTR_ERR(mei_class);
		goto err;
	}

	ret = alloc_chrdev_region(&mei_devt, 0, MEI_MAX_DEVS, "mei");
	if (ret < 0) {
		pr_err("unable to allocate char dev region\n");
		goto err_class;
	}

	ret = mei_cl_bus_init();
	if (ret < 0) {
		pr_err("unable to initialize bus\n");
		goto err_chrdev;
	}

	return 0;

err_chrdev:
	unregister_chrdev_region(mei_devt, MEI_MAX_DEVS);
err_class:
	class_destroy(mei_class);
err:
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void __exit mei_exit(void)
{
<<<<<<< HEAD
=======
	unregister_chrdev_region(mei_devt, MEI_MAX_DEVS);
	class_destroy(mei_class);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mei_cl_bus_exit();
}

module_init(mei_init);
module_exit(mei_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("Intel(R) Management Engine Interface");
MODULE_LICENSE("GPL v2");

