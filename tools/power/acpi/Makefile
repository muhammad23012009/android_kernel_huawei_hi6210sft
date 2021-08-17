<<<<<<< HEAD
PROG= acpidump
SRCS=	acpidump.c
KERNEL_INCLUDE := ../../../include
CFLAGS += -Wall -Wstrict-prototypes -Wdeclaration-after-statement -Os -s -D_LINUX -DDEFINE_ALTERNATE_TYPES -I$(KERNEL_INCLUDE) 

all: acpidump
$(PROG) : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(PROG)

CLEANFILES= $(PROG)

clean : 
	rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS)) *~

install :
	install acpidump /usr/sbin/acpidump
	install acpidump.8 /usr/share/man/man8

=======
# tools/power/acpi/Makefile - ACPI tool Makefile
#
# Copyright (c) 2013, Intel Corporation
#   Author: Lv Zheng <lv.zheng@intel.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2
# of the License.

include ../../scripts/Makefile.include

all: acpidbg acpidump ec
clean: acpidbg_clean acpidump_clean ec_clean
install: acpidbg_install acpidump_install ec_install
uninstall: acpidbg_uninstall acpidump_uninstall ec_uninstall

acpidbg acpidump ec: FORCE
	$(call descend,tools/$@,all)
acpidbg_clean acpidump_clean ec_clean:
	$(call descend,tools/$(@:_clean=),clean)
acpidbg_install acpidump_install ec_install:
	$(call descend,tools/$(@:_install=),install)
acpidbg_uninstall acpidump_uninstall ec_uninstall:
	$(call descend,tools/$(@:_uninstall=),uninstall)

.PHONY: FORCE
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
