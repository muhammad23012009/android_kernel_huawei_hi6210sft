#
# Makefile for the kernel security code
#

obj-$(CONFIG_KEYS)			+= keys/
subdir-$(CONFIG_SECURITY_SELINUX)	+= selinux
subdir-$(CONFIG_SECURITY_SMACK)		+= smack
subdir-$(CONFIG_SECURITY_TOMOYO)        += tomoyo
subdir-$(CONFIG_SECURITY_APPARMOR)	+= apparmor
subdir-$(CONFIG_SECURITY_YAMA)		+= yama
<<<<<<< HEAD
=======
subdir-$(CONFIG_SECURITY_LOADPIN)	+= loadpin
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

# always enable default capabilities
obj-y					+= commoncap.o
obj-$(CONFIG_MMU)			+= min_addr.o

# Object file lists
<<<<<<< HEAD
obj-$(CONFIG_SECURITY)			+= security.o capability.o
obj-$(CONFIG_SECURITYFS)		+= inode.o
# Must precede capability.o in order to stack properly.
obj-$(CONFIG_SECURITY_SELINUX)		+= selinux/built-in.o
obj-$(CONFIG_SECURITY_SMACK)		+= smack/built-in.o
obj-$(CONFIG_AUDIT)			+= lsm_audit.o
obj-$(CONFIG_SECURITY_TOMOYO)		+= tomoyo/built-in.o
obj-$(CONFIG_SECURITY_APPARMOR)		+= apparmor/built-in.o
obj-$(CONFIG_SECURITY_YAMA)		+= yama/built-in.o
=======
obj-$(CONFIG_SECURITY)			+= security.o
obj-$(CONFIG_SECURITYFS)		+= inode.o
obj-$(CONFIG_SECURITY_SELINUX)		+= selinux/
obj-$(CONFIG_SECURITY_SMACK)		+= smack/
obj-$(CONFIG_AUDIT)			+= lsm_audit.o
obj-$(CONFIG_SECURITY_TOMOYO)		+= tomoyo/
obj-$(CONFIG_SECURITY_APPARMOR)		+= apparmor/
obj-$(CONFIG_SECURITY_YAMA)		+= yama/
obj-$(CONFIG_SECURITY_LOADPIN)		+= loadpin/
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_CGROUP_DEVICE)		+= device_cgroup.o

# Object integrity file lists
subdir-$(CONFIG_INTEGRITY)		+= integrity
<<<<<<< HEAD
obj-$(CONFIG_INTEGRITY)			+= integrity/built-in.o

# HW Object 33
         subdir-$(CONFIG_HUAWEI_SELINUX_DSM)       += hwselinux
         obj-$(CONFIG_HUAWEI_SELINUX_DSM)          += hwselinux/built-in.o
=======
obj-$(CONFIG_INTEGRITY)			+= integrity/
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
