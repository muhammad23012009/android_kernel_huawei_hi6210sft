<<<<<<< HEAD
=======
OBJECT_FILES_NON_STANDARD_hibernate_asm_$(BITS).o := y

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# __restore_processor_state() restores %gs after S3 resume and so should not
# itself be stack-protected
nostackp := $(call cc-option, -fno-stack-protector)
CFLAGS_cpu.o	:= $(nostackp)

obj-$(CONFIG_PM_SLEEP)		+= cpu.o
obj-$(CONFIG_HIBERNATION)	+= hibernate_$(BITS).o hibernate_asm_$(BITS).o
