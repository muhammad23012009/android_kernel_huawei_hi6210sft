#
# Makefile for key management
#

#
# Core
#
obj-y := \
	gc.o \
	key.o \
	keyring.o \
	keyctl.o \
	permission.o \
	process_keys.o \
	request_key.o \
	request_key_auth.o \
	user_defined.o
obj-$(CONFIG_KEYS_COMPAT) += compat.o
obj-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_SYSCTL) += sysctl.o
<<<<<<< HEAD
=======
obj-$(CONFIG_PERSISTENT_KEYRINGS) += persistent.o
obj-$(CONFIG_KEY_DH_OPERATIONS) += dh.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#
# Key types
#
<<<<<<< HEAD
=======
obj-$(CONFIG_BIG_KEYS) += big_key.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_TRUSTED_KEYS) += trusted.o
obj-$(CONFIG_ENCRYPTED_KEYS) += encrypted-keys/
