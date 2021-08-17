#
# Makefile for caching inode integrity data (iint)
#

obj-$(CONFIG_INTEGRITY) += integrity.o
<<<<<<< HEAD
obj-$(CONFIG_INTEGRITY_SIGNATURE) += digsig.o
obj-$(CONFIG_INTEGRITY_ASYMMETRIC_KEYS) += digsig_asymmetric.o

integrity-y := iint.o

subdir-$(CONFIG_IMA)			+= ima
obj-$(CONFIG_IMA)			+= ima/built-in.o
subdir-$(CONFIG_EVM)			+= evm
obj-$(CONFIG_EVM)			+= evm/built-in.o
=======

integrity-y := iint.o
integrity-$(CONFIG_INTEGRITY_AUDIT) += integrity_audit.o
integrity-$(CONFIG_INTEGRITY_SIGNATURE) += digsig.o
integrity-$(CONFIG_INTEGRITY_ASYMMETRIC_KEYS) += digsig_asymmetric.o

subdir-$(CONFIG_IMA)			+= ima
obj-$(CONFIG_IMA)			+= ima/
subdir-$(CONFIG_EVM)			+= evm
obj-$(CONFIG_EVM)			+= evm/
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
