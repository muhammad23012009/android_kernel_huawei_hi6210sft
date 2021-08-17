#
# Makefile for the kernel tpm device drivers.
#
obj-$(CONFIG_TCG_TPM) += tpm.o
<<<<<<< HEAD
ifdef CONFIG_ACPI
	obj-$(CONFIG_TCG_TPM) += tpm_bios.o
	tpm_bios-objs += tpm_eventlog.o tpm_acpi.o tpm_ppi.o
else
ifdef CONFIG_TCG_IBMVTPM
	obj-$(CONFIG_TCG_TPM) += tpm_bios.o
	tpm_bios-objs += tpm_eventlog.o tpm_of.o
endif
endif
obj-$(CONFIG_TCG_TIS) += tpm_tis.o
obj-$(CONFIG_TCG_TIS_I2C_INFINEON) += tpm_i2c_infineon.o
=======
tpm-y := tpm-interface.o tpm-dev.o tpm-sysfs.o tpm-chip.o tpm2-cmd.o
tpm-$(CONFIG_ACPI) += tpm_ppi.o

ifdef CONFIG_ACPI
	tpm-y += tpm_eventlog.o tpm_acpi.o
else
ifdef CONFIG_TCG_IBMVTPM
	tpm-y += tpm_eventlog.o tpm_of.o
endif
endif
obj-$(CONFIG_TCG_TIS_CORE) += tpm_tis_core.o
obj-$(CONFIG_TCG_TIS) += tpm_tis.o
obj-$(CONFIG_TCG_TIS_SPI) += tpm_tis_spi.o
obj-$(CONFIG_TCG_TIS_I2C_ATMEL) += tpm_i2c_atmel.o
obj-$(CONFIG_TCG_TIS_I2C_INFINEON) += tpm_i2c_infineon.o
obj-$(CONFIG_TCG_TIS_I2C_NUVOTON) += tpm_i2c_nuvoton.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_TCG_NSC) += tpm_nsc.o
obj-$(CONFIG_TCG_ATMEL) += tpm_atmel.o
obj-$(CONFIG_TCG_INFINEON) += tpm_infineon.o
obj-$(CONFIG_TCG_IBMVTPM) += tpm_ibmvtpm.o
<<<<<<< HEAD
obj-$(CONFIG_TCG_ST33_I2C) += tpm_i2c_stm_st33.o
=======
obj-$(CONFIG_TCG_TIS_ST33ZP24) += st33zp24/
obj-$(CONFIG_TCG_XEN) += xen-tpmfront.o
obj-$(CONFIG_TCG_CRB) += tpm_crb.o
obj-$(CONFIG_TCG_VTPM_PROXY) += tpm_vtpm_proxy.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
