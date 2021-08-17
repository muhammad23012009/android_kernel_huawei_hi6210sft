#
# Makefile for the Ethernet Bonding driver
#

obj-$(CONFIG_BONDING) += bonding.o

<<<<<<< HEAD
bonding-objs := bond_main.o bond_3ad.o bond_alb.o bond_sysfs.o bond_debugfs.o
=======
bonding-objs := bond_main.o bond_3ad.o bond_alb.o bond_sysfs.o bond_sysfs_slave.o bond_debugfs.o bond_netlink.o bond_options.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

proc-$(CONFIG_PROC_FS) += bond_procfs.o
bonding-objs += $(proc-y)

