#
# Makefile for Linux kernel RxRPC
#

af-rxrpc-y := \
	af_rxrpc.o \
<<<<<<< HEAD
	ar-accept.o \
	ar-ack.o \
	ar-call.o \
	ar-connection.o \
	ar-connevent.o \
	ar-error.o \
	ar-input.o \
	ar-key.o \
	ar-local.o \
	ar-output.o \
	ar-peer.o \
	ar-recvmsg.o \
	ar-security.o \
	ar-skbuff.o \
	ar-transport.o

ifeq ($(CONFIG_PROC_FS),y)
af-rxrpc-y += ar-proc.o
endif

obj-$(CONFIG_AF_RXRPC) += af-rxrpc.o

obj-$(CONFIG_RXKAD) += rxkad.o
=======
	call_accept.o \
	call_event.o \
	call_object.o \
	conn_client.o \
	conn_event.o \
	conn_object.o \
	conn_service.o \
	input.o \
	insecure.o \
	key.o \
	local_event.o \
	local_object.o \
	misc.o \
	output.o \
	peer_event.o \
	peer_object.o \
	recvmsg.o \
	security.o \
	sendmsg.o \
	skbuff.o \
	utils.o

af-rxrpc-$(CONFIG_PROC_FS) += proc.o
af-rxrpc-$(CONFIG_RXKAD) += rxkad.o
af-rxrpc-$(CONFIG_SYSCTL) += sysctl.o

obj-$(CONFIG_AF_RXRPC) += af-rxrpc.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
