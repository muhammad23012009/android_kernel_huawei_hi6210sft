#
# S/390 crypto devices
#

ap-objs := ap_bus.o
<<<<<<< HEAD
obj-$(CONFIG_ZCRYPT) += ap.o zcrypt_api.o zcrypt_pcicc.o zcrypt_pcixcc.o
obj-$(CONFIG_ZCRYPT) += zcrypt_pcica.o zcrypt_cex2a.o zcrypt_cex4.o
obj-$(CONFIG_ZCRYPT) += zcrypt_msgtype6.o zcrypt_msgtype50.o
=======
# zcrypt_api depends on ap
obj-$(CONFIG_ZCRYPT) += ap.o zcrypt_api.o
# msgtype* depend on zcrypt_api
obj-$(CONFIG_ZCRYPT) += zcrypt_msgtype6.o zcrypt_msgtype50.o
# adapter drivers depend on ap, zcrypt_api and msgtype*
obj-$(CONFIG_ZCRYPT) += zcrypt_pcixcc.o zcrypt_cex2a.o zcrypt_cex4.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
