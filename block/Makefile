#
# Makefile for the kernel block layer
#

<<<<<<< HEAD
obj-$(CONFIG_BLOCK) := elevator.o blk-core.o blk-tag.o blk-sysfs.o \
			blk-flush.o blk-settings.o blk-ioc.o blk-map.o \
			blk-exec.o blk-merge.o blk-softirq.o blk-timeout.o \
			blk-iopoll.o blk-lib.o ioctl.o genhd.o scsi_ioctl.o \
			partition-generic.o partitions/

=======
obj-$(CONFIG_BLOCK) := bio.o elevator.o blk-core.o blk-tag.o blk-sysfs.o \
			blk-flush.o blk-settings.o blk-ioc.o blk-map.o \
			blk-exec.o blk-merge.o blk-softirq.o blk-timeout.o \
			blk-lib.o blk-mq.o blk-mq-tag.o \
			blk-mq-sysfs.o blk-mq-cpumap.o ioctl.o \
			genhd.o scsi_ioctl.o partition-generic.o ioprio.o \
			badblocks.o partitions/

obj-$(CONFIG_BOUNCE)	+= bounce.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_BLK_DEV_BSG)	+= bsg.o
obj-$(CONFIG_BLK_DEV_BSGLIB)	+= bsg-lib.o
obj-$(CONFIG_BLK_CGROUP)	+= blk-cgroup.o
obj-$(CONFIG_BLK_DEV_THROTTLING)	+= blk-throttle.o
obj-$(CONFIG_IOSCHED_NOOP)	+= noop-iosched.o
obj-$(CONFIG_IOSCHED_DEADLINE)	+= deadline-iosched.o
obj-$(CONFIG_IOSCHED_CFQ)	+= cfq-iosched.o

obj-$(CONFIG_BLOCK_COMPAT)	+= compat_ioctl.o
<<<<<<< HEAD
obj-$(CONFIG_BLK_DEV_INTEGRITY)	+= blk-integrity.o

obj-$(CONFIG_HISI_BLOCK_FREQUENCE_CONTROL) += hisi_freq_ctl.o
=======
obj-$(CONFIG_BLK_CMDLINE_PARSER)	+= cmdline-parser.o
obj-$(CONFIG_BLK_DEV_INTEGRITY) += bio-integrity.o blk-integrity.o t10-pi.o
obj-$(CONFIG_BLK_MQ_PCI)	+= blk-mq-pci.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
