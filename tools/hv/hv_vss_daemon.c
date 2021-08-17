/*
 * An implementation of the host initiated guest snapshot for Hyper-V.
 *
 *
 * Copyright (C) 2013, Microsoft, Inc.
 * Author : K. Y. Srinivasan <kys@microsoft.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 */


#include <sys/types.h>
<<<<<<< HEAD
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <linux/types.h>
=======
#include <sys/poll.h>
#include <sys/ioctl.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <fcntl.h>
#include <stdio.h>
#include <mntent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
<<<<<<< HEAD
#include <arpa/inet.h>
#include <linux/fs.h>
#include <linux/connector.h>
#include <linux/hyperv.h>
#include <linux/netlink.h>
#include <syslog.h>

static char vss_recv_buffer[4096];
static char vss_send_buffer[4096];
static struct sockaddr_nl addr;

#ifndef SOL_NETLINK
#define SOL_NETLINK 270
#endif


static int vss_do_freeze(char *dir, unsigned int cmd, char *fs_op)
=======
#include <linux/fs.h>
#include <linux/hyperv.h>
#include <syslog.h>
#include <getopt.h>

/* Don't use syslog() in the function since that can cause write to disk */
static int vss_do_freeze(char *dir, unsigned int cmd)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int ret, fd = open(dir, O_RDONLY);

	if (fd < 0)
		return 1;
<<<<<<< HEAD
	ret = ioctl(fd, cmd, 0);
	syslog(LOG_INFO, "VSS: %s of %s: %s\n", fs_op, dir, strerror(errno));
=======

	ret = ioctl(fd, cmd, 0);

	/*
	 * If a partition is mounted more than once, only the first
	 * FREEZE/THAW can succeed and the later ones will get
	 * EBUSY/EINVAL respectively: there could be 2 cases:
	 * 1) a user may mount the same partition to differnt directories
	 *  by mistake or on purpose;
	 * 2) The subvolume of btrfs appears to have the same partition
	 * mounted more than once.
	 */
	if (ret) {
		if ((cmd == FIFREEZE && errno == EBUSY) ||
		    (cmd == FITHAW && errno == EINVAL)) {
			close(fd);
			return 0;
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	close(fd);
	return !!ret;
}

static int vss_operate(int operation)
{
<<<<<<< HEAD
	char *fs_op;
	char match[] = "/dev/";
	FILE *mounts;
	struct mntent *ent;
	unsigned int cmd;
	int error = 0, root_seen = 0;
=======
	char match[] = "/dev/";
	FILE *mounts;
	struct mntent *ent;
	char errdir[1024] = {0};
	unsigned int cmd;
	int error = 0, root_seen = 0, save_errno = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (operation) {
	case VSS_OP_FREEZE:
		cmd = FIFREEZE;
<<<<<<< HEAD
		fs_op = "freeze";
		break;
	case VSS_OP_THAW:
		cmd = FITHAW;
		fs_op = "thaw";
=======
		break;
	case VSS_OP_THAW:
		cmd = FITHAW;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	default:
		return -1;
	}

	mounts = setmntent("/proc/mounts", "r");
	if (mounts == NULL)
		return -1;

	while ((ent = getmntent(mounts))) {
		if (strncmp(ent->mnt_fsname, match, strlen(match)))
			continue;
<<<<<<< HEAD
		if (strcmp(ent->mnt_type, "iso9660") == 0)
=======
		if (hasmntopt(ent, MNTOPT_RO) != NULL)
			continue;
		if (strcmp(ent->mnt_type, "vfat") == 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		if (strcmp(ent->mnt_dir, "/") == 0) {
			root_seen = 1;
			continue;
		}
<<<<<<< HEAD
		error |= vss_do_freeze(ent->mnt_dir, cmd, fs_op);
	}
	endmntent(mounts);

	if (root_seen) {
		error |= vss_do_freeze("/", cmd, fs_op);
	}

	return error;
}

static int netlink_send(int fd, struct cn_msg *msg)
{
	struct nlmsghdr *nlh;
	unsigned int size;
	struct msghdr message;
	char buffer[64];
	struct iovec iov[2];

	size = NLMSG_SPACE(sizeof(struct cn_msg) + msg->len);

	nlh = (struct nlmsghdr *)buffer;
	nlh->nlmsg_seq = 0;
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_type = NLMSG_DONE;
	nlh->nlmsg_len = NLMSG_LENGTH(size - sizeof(*nlh));
	nlh->nlmsg_flags = 0;

	iov[0].iov_base = nlh;
	iov[0].iov_len = sizeof(*nlh);

	iov[1].iov_base = msg;
	iov[1].iov_len = size;

	memset(&message, 0, sizeof(message));
	message.msg_name = &addr;
	message.msg_namelen = sizeof(addr);
	message.msg_iov = iov;
	message.msg_iovlen = 2;

	return sendmsg(fd, &message, 0);
}

int main(void)
{
	int fd, len, nl_group;
	int error;
	struct cn_msg *message;
	struct pollfd pfd;
	struct nlmsghdr *incoming_msg;
	struct cn_msg	*incoming_cn_msg;
	int	op;
	struct hv_vss_msg *vss_msg;

	if (daemon(1, 0))
=======
		error |= vss_do_freeze(ent->mnt_dir, cmd);
		if (error && operation == VSS_OP_FREEZE)
			goto err;
	}

	endmntent(mounts);

	if (root_seen) {
		error |= vss_do_freeze("/", cmd);
		if (error && operation == VSS_OP_FREEZE)
			goto err;
	}

	goto out;
err:
	save_errno = errno;
	if (ent) {
		strncpy(errdir, ent->mnt_dir, sizeof(errdir)-1);
		endmntent(mounts);
	}
	vss_operate(VSS_OP_THAW);
	/* Call syslog after we thaw all filesystems */
	if (ent)
		syslog(LOG_ERR, "FREEZE of %s failed; error:%d %s",
		       errdir, save_errno, strerror(save_errno));
	else
		syslog(LOG_ERR, "FREEZE of / failed; error:%d %s", save_errno,
		       strerror(save_errno));
out:
	return error;
}

void print_usage(char *argv[])
{
	fprintf(stderr, "Usage: %s [options]\n"
		"Options are:\n"
		"  -n, --no-daemon        stay in foreground, don't daemonize\n"
		"  -h, --help             print this help\n", argv[0]);
}

int main(int argc, char *argv[])
{
	int vss_fd, len;
	int error;
	struct pollfd pfd;
	int	op;
	struct hv_vss_msg vss_msg[1];
	int daemonize = 1, long_index = 0, opt;
	int in_handshake = 1;
	__u32 kernel_modver;

	static struct option long_options[] = {
		{"help",	no_argument,	   0,  'h' },
		{"no-daemon",	no_argument,	   0,  'n' },
		{0,		0,		   0,  0   }
	};

	while ((opt = getopt_long(argc, argv, "hn", long_options,
				  &long_index)) != -1) {
		switch (opt) {
		case 'n':
			daemonize = 0;
			break;
		case 'h':
			print_usage(argv);
			exit(0);
		default:
			print_usage(argv);
			exit(EXIT_FAILURE);
		}
	}

	if (daemonize && daemon(1, 0))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 1;

	openlog("Hyper-V VSS", 0, LOG_USER);
	syslog(LOG_INFO, "VSS starting; pid is:%d", getpid());

<<<<<<< HEAD
	fd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (fd < 0) {
		syslog(LOG_ERR, "netlink socket creation failed; error:%d", fd);
		exit(EXIT_FAILURE);
	}
	addr.nl_family = AF_NETLINK;
	addr.nl_pad = 0;
	addr.nl_pid = 0;
	addr.nl_groups = 0;


	error = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (error < 0) {
		syslog(LOG_ERR, "bind failed; error:%d", error);
		close(fd);
		exit(EXIT_FAILURE);
	}
	nl_group = CN_VSS_IDX;
	setsockopt(fd, SOL_NETLINK, NETLINK_ADD_MEMBERSHIP, &nl_group, sizeof(nl_group));
	/*
	 * Register ourselves with the kernel.
	 */
	message = (struct cn_msg *)vss_send_buffer;
	message->id.idx = CN_VSS_IDX;
	message->id.val = CN_VSS_VAL;
	message->ack = 0;
	vss_msg = (struct hv_vss_msg *)message->data;
	vss_msg->vss_hdr.operation = VSS_OP_REGISTER;

	message->len = sizeof(struct hv_vss_msg);

	len = netlink_send(fd, message);
	if (len < 0) {
		syslog(LOG_ERR, "netlink_send failed; error:%d", len);
		close(fd);
		exit(EXIT_FAILURE);
	}

	pfd.fd = fd;

	while (1) {
		struct sockaddr *addr_p = (struct sockaddr *) &addr;
		socklen_t addr_l = sizeof(addr);
		pfd.events = POLLIN;
		pfd.revents = 0;
		poll(&pfd, 1, -1);

		len = recvfrom(fd, vss_recv_buffer, sizeof(vss_recv_buffer), 0,
				addr_p, &addr_l);

		if (len < 0) {
			syslog(LOG_ERR, "recvfrom failed; pid:%u error:%d %s",
					addr.nl_pid, errno, strerror(errno));
			close(fd);
			return -1;
		}

		if (addr.nl_pid) {
			syslog(LOG_WARNING,
				"Received packet from untrusted pid:%u",
				addr.nl_pid);
			continue;
		}

		incoming_msg = (struct nlmsghdr *)vss_recv_buffer;

		if (incoming_msg->nlmsg_type != NLMSG_DONE)
			continue;

		incoming_cn_msg = (struct cn_msg *)NLMSG_DATA(incoming_msg);
		vss_msg = (struct hv_vss_msg *)incoming_cn_msg->data;
=======
	vss_fd = open("/dev/vmbus/hv_vss", O_RDWR);
	if (vss_fd < 0) {
		syslog(LOG_ERR, "open /dev/vmbus/hv_vss failed; error: %d %s",
		       errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	/*
	 * Register ourselves with the kernel.
	 */
	vss_msg->vss_hdr.operation = VSS_OP_REGISTER1;

	len = write(vss_fd, vss_msg, sizeof(struct hv_vss_msg));
	if (len < 0) {
		syslog(LOG_ERR, "registration to kernel failed; error: %d %s",
		       errno, strerror(errno));
		close(vss_fd);
		exit(EXIT_FAILURE);
	}

	pfd.fd = vss_fd;

	while (1) {
		pfd.events = POLLIN;
		pfd.revents = 0;

		if (poll(&pfd, 1, -1) < 0) {
			syslog(LOG_ERR, "poll failed; error:%d %s", errno, strerror(errno));
			if (errno == EINVAL) {
				close(vss_fd);
				exit(EXIT_FAILURE);
			}
			else
				continue;
		}

		len = read(vss_fd, vss_msg, sizeof(struct hv_vss_msg));

		if (in_handshake) {
			if (len != sizeof(kernel_modver)) {
				syslog(LOG_ERR, "invalid version negotiation");
				exit(EXIT_FAILURE);
			}
			kernel_modver = *(__u32 *)vss_msg;
			in_handshake = 0;
			syslog(LOG_INFO, "VSS: kernel module version: %d",
			       kernel_modver);
			continue;
		}

		if (len != sizeof(struct hv_vss_msg)) {
			syslog(LOG_ERR, "read failed; error:%d %s",
			       errno, strerror(errno));
			close(vss_fd);
			return EXIT_FAILURE;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		op = vss_msg->vss_hdr.operation;
		error =  HV_S_OK;

		switch (op) {
		case VSS_OP_FREEZE:
		case VSS_OP_THAW:
			error = vss_operate(op);
<<<<<<< HEAD
			if (error)
				error = HV_E_FAIL;
=======
			syslog(LOG_INFO, "VSS: op=%s: %s\n",
				op == VSS_OP_FREEZE ? "FREEZE" : "THAW",
				error ? "failed" : "succeeded");

			if (error) {
				error = HV_E_FAIL;
				syslog(LOG_ERR, "op=%d failed!", op);
				syslog(LOG_ERR, "report it with these files:");
				syslog(LOG_ERR, "/etc/fstab and /proc/mounts");
			}
			break;
		case VSS_OP_HOT_BACKUP:
			syslog(LOG_INFO, "VSS: op=CHECK HOT BACKUP\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		default:
			syslog(LOG_ERR, "Illegal op:%d\n", op);
		}
		vss_msg->error = error;
<<<<<<< HEAD
		len = netlink_send(fd, incoming_cn_msg);
		if (len < 0) {
			syslog(LOG_ERR, "net_link send failed; error:%d", len);
=======
		len = write(vss_fd, vss_msg, sizeof(struct hv_vss_msg));
		if (len != sizeof(struct hv_vss_msg)) {
			syslog(LOG_ERR, "write failed; error: %d %s", errno,
			       strerror(errno));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			exit(EXIT_FAILURE);
		}
	}

<<<<<<< HEAD
=======
	close(vss_fd);
	exit(0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
