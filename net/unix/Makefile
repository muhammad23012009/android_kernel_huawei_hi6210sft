#
# Makefile for the Linux unix domain socket layer.
#

obj-$(CONFIG_UNIX)	+= unix.o

unix-y			:= af_unix.o garbage.o
unix-$(CONFIG_SYSCTL)	+= sysctl_net_unix.o

obj-$(CONFIG_UNIX_DIAG)	+= unix_diag.o
unix_diag-y		:= diag.o
<<<<<<< HEAD
=======

obj-$(CONFIG_UNIX_SCM)	+= scm.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
