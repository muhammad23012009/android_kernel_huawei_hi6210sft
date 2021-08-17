#
# Makefile for the NetLabel subsystem.
#

# base objects
obj-y	:= netlabel_user.o netlabel_kapi.o
obj-y	+= netlabel_domainhash.o netlabel_addrlist.o

# management objects
obj-y	+= netlabel_mgmt.o

# protocol modules
obj-y	+= netlabel_unlabeled.o
obj-y	+= netlabel_cipso_v4.o
<<<<<<< HEAD

=======
obj-$(subst m,y,$(CONFIG_IPV6)) += netlabel_calipso.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
