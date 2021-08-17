#
# Makefile for the Motorola 8xx FEC ethernet controller
#

obj-$(CONFIG_SERIAL_CPM) += cpm_uart.o

# Select the correct platform objects.
cpm_uart-objs-$(CONFIG_CPM2)	+= cpm_uart_cpm2.o
<<<<<<< HEAD
cpm_uart-objs-$(CONFIG_8xx)	+= cpm_uart_cpm1.o
=======
cpm_uart-objs-$(CONFIG_CPM1)	+= cpm_uart_cpm1.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

cpm_uart-objs	:= cpm_uart_core.o $(cpm_uart-objs-y)
