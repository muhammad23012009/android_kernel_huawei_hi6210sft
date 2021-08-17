<<<<<<< HEAD
obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_$(BITS).o
=======
OBJECT_FILES_NON_STANDARD_wakeup_$(BITS).o := y

obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_$(BITS).o
obj-$(CONFIG_ACPI_APEI)		+= apei.o
obj-$(CONFIG_ACPI_CPPC_LIB)	+= cppc_msr.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ifneq ($(CONFIG_ACPI_PROCESSOR),)
obj-y				+= cstate.o
endif

