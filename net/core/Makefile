#
# Makefile for the Linux networking core.
#

<<<<<<< HEAD
obj-y := sock.o request_sock.o skbuff.o iovec.o datagram.o stream.o scm.o \
=======
obj-y := sock.o request_sock.o skbuff.o datagram.o stream.o scm.o \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 gen_stats.o gen_estimator.o net_namespace.o secure_seq.o flow_dissector.o

obj-$(CONFIG_SYSCTL) += sysctl_net_core.o

obj-y		     += dev.o ethtool.o dev_addr_lists.o dst.o netevent.o \
			neighbour.o rtnetlink.o utils.o link_watch.o filter.o \
<<<<<<< HEAD
			sock_diag.o dev_ioctl.o
=======
			sock_diag.o dev_ioctl.o tso.o sock_reuseport.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_XFRM) += flow.o
obj-y += net-sysfs.o
obj-$(CONFIG_PROC_FS) += net-procfs.o
obj-$(CONFIG_NET_PKTGEN) += pktgen.o
obj-$(CONFIG_NETPOLL) += netpoll.o
<<<<<<< HEAD
obj-$(CONFIG_NET_DMA) += user_dma.o
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_FIB_RULES) += fib_rules.o
obj-$(CONFIG_TRACEPOINTS) += net-traces.o
obj-$(CONFIG_NET_DROP_MONITOR) += drop_monitor.o
obj-$(CONFIG_NETWORK_PHY_TIMESTAMPING) += timestamping.o
<<<<<<< HEAD
obj-$(CONFIG_NETPRIO_CGROUP) += netprio_cgroup.o
=======
obj-$(CONFIG_NET_PTP_CLASSIFY) += ptp_classifier.o
obj-$(CONFIG_CGROUP_NET_PRIO) += netprio_cgroup.o
obj-$(CONFIG_CGROUP_NET_CLASSID) += netclassid_cgroup.o
obj-$(CONFIG_LWTUNNEL) += lwtunnel.o
obj-$(CONFIG_DST_CACHE) += dst_cache.o
obj-$(CONFIG_HWBM) += hwbm.o
obj-$(CONFIG_NET_DEVLINK) += devlink.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
