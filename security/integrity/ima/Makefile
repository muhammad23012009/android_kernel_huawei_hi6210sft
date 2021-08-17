#
# Makefile for building Trusted Computing Group's(TCG) runtime Integrity
# Measurement Architecture(IMA).
#

obj-$(CONFIG_IMA) += ima.o

ima-y := ima_fs.o ima_queue.o ima_init.o ima_main.o ima_crypto.o ima_api.o \
<<<<<<< HEAD
	 ima_policy.o
ima-$(CONFIG_IMA_AUDIT) += ima_audit.o
ima-$(CONFIG_IMA_APPRAISE) += ima_appraise.o
=======
	 ima_policy.o ima_template.o ima_template_lib.o
ima-$(CONFIG_IMA_APPRAISE) += ima_appraise.o
obj-$(CONFIG_IMA_BLACKLIST_KEYRING) += ima_mok.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
