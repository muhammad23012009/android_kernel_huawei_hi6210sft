# trace-cmd version
EP_VERSION = 1
EP_PATCHLEVEL = 1
EP_EXTRAVERSION = 0

# file format version
FILE_VERSION = 6

MAKEFLAGS += --no-print-directory


# Makefiles suck: This macro sets a default value of $(2) for the
# variable named by $(1), unless the variable has been set by
# environment or command line. This is necessary for CC and AR
# because make sets default values, so the simpler ?= approach
# won't work as expected.
define allow-override
  $(if $(or $(findstring environment,$(origin $(1))),\
            $(findstring command line,$(origin $(1)))),,\
    $(eval $(1) = $(2)))
endef

# Allow setting CC and AR, or setting CROSS_COMPILE as a prefix.
$(call allow-override,CC,$(CROSS_COMPILE)gcc)
$(call allow-override,AR,$(CROSS_COMPILE)ar)
<<<<<<< HEAD
=======
$(call allow-override,NM,$(CROSS_COMPILE)nm)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

EXT = -std=gnu99
INSTALL = install

# Use DESTDIR for installing into a different root directory.
# This is useful for building a package. The program will be
# installed in this directory as if it was the root directory.
# Then the build tool can move it later.
DESTDIR ?=
DESTDIR_SQ = '$(subst ','\'',$(DESTDIR))'

<<<<<<< HEAD
prefix ?= /usr/local
bindir_relative = bin
bindir = $(prefix)/$(bindir_relative)
man_dir = $(prefix)/share/man
man_dir_SQ = '$(subst ','\'',$(man_dir))'
html_install = $(prefix)/share/kernelshark/html
html_install_SQ = '$(subst ','\'',$(html_install))'
img_install = $(prefix)/share/kernelshark/html/images
img_install_SQ = '$(subst ','\'',$(img_install))'

export man_dir man_dir_SQ html_install html_install_SQ INSTALL
export img_install img_install_SQ
export DESTDIR DESTDIR_SQ

=======
LP64 := $(shell echo __LP64__ | ${CC} ${CFLAGS} -E -x c - | tail -n 1)
ifeq ($(LP64), 1)
  libdir_relative = lib64
else
  libdir_relative = lib
endif

prefix ?= /usr/local
libdir = $(prefix)/$(libdir_relative)
man_dir = $(prefix)/share/man
man_dir_SQ = '$(subst ','\'',$(man_dir))'

export man_dir man_dir_SQ INSTALL
export DESTDIR DESTDIR_SQ

set_plugin_dir := 1

# Set plugin_dir to preffered global plugin location
# If we install under $HOME directory we go under
# $(HOME)/.local/lib/traceevent/plugins
#
# We dont set PLUGIN_DIR in case we install under $HOME
# directory, because by default the code looks under:
# $(HOME)/.local/lib/traceevent/plugins by default.
#
ifeq ($(plugin_dir),)
ifeq ($(prefix),$(HOME))
override plugin_dir = $(HOME)/.local/lib/traceevent/plugins
set_plugin_dir := 0
else
override plugin_dir = $(libdir)/traceevent/plugins
endif
endif

ifeq ($(set_plugin_dir),1)
PLUGIN_DIR = -DPLUGIN_DIR="$(plugin_dir)"
PLUGIN_DIR_SQ = '$(subst ','\'',$(PLUGIN_DIR))'
endif

include ../../scripts/Makefile.include

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# copy a bit from Linux kbuild

ifeq ("$(origin V)", "command line")
  VERBOSE = $(V)
endif
ifndef VERBOSE
  VERBOSE = 0
endif

<<<<<<< HEAD
ifeq ("$(origin O)", "command line")
  BUILD_OUTPUT := $(O)
endif

ifeq ($(BUILD_SRC),)
ifneq ($(BUILD_OUTPUT),)

define build_output
	$(if $(VERBOSE:1=),@)$(MAKE) -C $(BUILD_OUTPUT) 	\
	BUILD_SRC=$(CURDIR) -f $(CURDIR)/Makefile $1
endef

saved-output := $(BUILD_OUTPUT)
BUILD_OUTPUT := $(shell cd $(BUILD_OUTPUT) && /bin/pwd)
$(if $(BUILD_OUTPUT),, \
     $(error output directory "$(saved-output)" does not exist))

all: sub-make

gui: force
	$(call build_output, all_cmd)

$(filter-out gui,$(MAKECMDGOALS)): sub-make

sub-make: force
	$(call build_output, $(MAKECMDGOALS))


# Leave processing to above invocation of make
skip-makefile := 1

endif # BUILD_OUTPUT
endif # BUILD_SRC

# We process the rest of the Makefile if this is the final invocation of make
ifeq ($(skip-makefile),)

srctree		:= $(if $(BUILD_SRC),$(BUILD_SRC),$(CURDIR))
objtree		:= $(CURDIR)
src		:= $(srctree)
obj		:= $(objtree)

export prefix bindir src obj

# Shell quotes
bindir_SQ = $(subst ','\'',$(bindir))
bindir_relative_SQ = $(subst ','\'',$(bindir_relative))
=======
ifeq ($(srctree),)
srctree := $(patsubst %/,%,$(dir $(shell pwd)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
#$(info Determined 'srctree' to be $(srctree))
endif

export prefix libdir src obj

# Shell quotes
libdir_SQ = $(subst ','\'',$(libdir))
libdir_relative_SQ = $(subst ','\'',$(libdir_relative))
plugin_dir_SQ = $(subst ','\'',$(plugin_dir))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

LIB_FILE = libtraceevent.a libtraceevent.so

CONFIG_INCLUDES = 
CONFIG_LIBS	=
CONFIG_FLAGS	=

VERSION		= $(EP_VERSION)
PATCHLEVEL	= $(EP_PATCHLEVEL)
EXTRAVERSION	= $(EP_EXTRAVERSION)

OBJ		= $@
N		=

<<<<<<< HEAD
export Q VERBOSE

EVENT_PARSE_VERSION = $(EP_VERSION).$(EP_PATCHLEVEL).$(EP_EXTRAVERSION)

INCLUDES = -I. $(CONFIG_INCLUDES)

# Set compile option CFLAGS if not set elsewhere
CFLAGS ?= -g -Wall

# Append required CFLAGS
=======
EVENT_PARSE_VERSION = $(EP_VERSION).$(EP_PATCHLEVEL).$(EP_EXTRAVERSION)

INCLUDES = -I. -I $(srctree)/tools/include $(CONFIG_INCLUDES)

# Set compile option CFLAGS
ifdef EXTRA_CFLAGS
  CFLAGS := $(EXTRA_CFLAGS)
else
  CFLAGS := -g -Wall
endif

# Append required CFLAGS
override CFLAGS += -fPIC
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
override CFLAGS += $(CONFIG_FLAGS) $(INCLUDES) $(PLUGIN_DIR_SQ)
override CFLAGS += $(udis86-flags) -D_GNU_SOURCE

ifeq ($(VERBOSE),1)
  Q =
<<<<<<< HEAD
  print_compile =
  print_app_build =
  print_fpic_compile =
  print_shared_lib_compile =
  print_plugin_obj_compile =
  print_plugin_build =
  print_install =
else
  Q = @
  print_compile =		echo '  CC                 '$(OBJ);
  print_app_build =		echo '  BUILD              '$(OBJ);
  print_fpic_compile =		echo '  CC FPIC            '$(OBJ);
  print_shared_lib_compile =	echo '  BUILD SHARED LIB   '$(OBJ);
  print_plugin_obj_compile =	echo '  CC PLUGIN OBJ      '$(OBJ);
  print_plugin_build =		echo '  CC PLUGI           '$(OBJ);
  print_static_lib_build =	echo '  BUILD STATIC LIB   '$(OBJ);
  print_install =		echo '  INSTALL     '$1'	to	$(DESTDIR_SQ)$2';
endif

do_fpic_compile =					\
	($(print_fpic_compile)				\
	$(CC) -c $(CFLAGS) $(EXT) -fPIC $< -o $@)

do_app_build =						\
	($(print_app_build)				\
	$(CC) $^ -rdynamic -o $@ $(CONFIG_LIBS) $(LIBS))

do_compile_shared_library =			\
	($(print_shared_lib_compile)		\
	$(CC) --shared $^ -o $@)

do_compile_plugin_obj =				\
	($(print_plugin_obj_compile)		\
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<)

do_plugin_build =				\
	($(print_plugin_build)			\
	$(CC) $(CFLAGS) -shared -nostartfiles -o $@ $<)

do_build_static_lib =				\
	($(print_static_lib_build)		\
	$(RM) $@;  $(AR) rcs $@ $^)


define do_compile
	$(print_compile)						\
	$(CC) -c $(CFLAGS) $(EXT) $< -o $(obj)/$@;
endef

$(obj)/%.o: $(src)/%.c
	$(Q)$(call do_compile)

%.o: $(src)/%.c
	$(Q)$(call do_compile)

PEVENT_LIB_OBJS = event-parse.o trace-seq.o parse-filter.o parse-utils.o

ALL_OBJS = $(PEVENT_LIB_OBJS)

CMD_TARGETS = $(LIB_FILE)

TARGETS = $(CMD_TARGETS)


all: all_cmd

all_cmd: $(CMD_TARGETS)

libtraceevent.so: $(PEVENT_LIB_OBJS)
	$(Q)$(do_compile_shared_library)

libtraceevent.a: $(PEVENT_LIB_OBJS)
	$(Q)$(do_build_static_lib)

$(PEVENT_LIB_OBJS): %.o: $(src)/%.c TRACEEVENT-CFLAGS
	$(Q)$(do_fpic_compile)

define make_version.h
	(echo '/* This file is automatically generated. Do not modify. */';		\
	echo \#define VERSION_CODE $(shell						\
	expr $(VERSION) \* 256 + $(PATCHLEVEL));					\
	echo '#define EXTRAVERSION ' $(EXTRAVERSION);					\
	echo '#define VERSION_STRING "'$(VERSION).$(PATCHLEVEL).$(EXTRAVERSION)'"';	\
	echo '#define FILE_VERSION '$(FILE_VERSION);					\
	) > $1
endef

define update_version.h
	($(call make_version.h, $@.tmp);		\
	if [ -r $@ ] && cmp -s $@ $@.tmp; then		\
		rm -f $@.tmp;				\
	else						\
		echo '  UPDATE                 $@';	\
		mv -f $@.tmp $@;			\
	fi);
=======
else
  Q = @
endif

# Disable command line variables (CFLAGS) overide from top
# level Makefile (perf), otherwise build Makefile will get
# the same command line setup.
MAKEOVERRIDES=

export srctree OUTPUT CC LD CFLAGS V
build := -f $(srctree)/tools/build/Makefile.build dir=. obj

PLUGINS  = plugin_jbd2.so
PLUGINS += plugin_hrtimer.so
PLUGINS += plugin_kmem.so
PLUGINS += plugin_kvm.so
PLUGINS += plugin_mac80211.so
PLUGINS += plugin_sched_switch.so
PLUGINS += plugin_function.so
PLUGINS += plugin_xen.so
PLUGINS += plugin_scsi.so
PLUGINS += plugin_cfg80211.so

PLUGINS    := $(addprefix $(OUTPUT),$(PLUGINS))
PLUGINS_IN := $(PLUGINS:.so=-in.o)

TE_IN    := $(OUTPUT)libtraceevent-in.o
LIB_FILE := $(addprefix $(OUTPUT),$(LIB_FILE))
DYNAMIC_LIST_FILE := $(OUTPUT)libtraceevent-dynamic-list

CMD_TARGETS = $(LIB_FILE) $(PLUGINS) $(DYNAMIC_LIST_FILE)

TARGETS = $(CMD_TARGETS)

all: all_cmd

all_cmd: $(CMD_TARGETS)

$(TE_IN): force
	$(Q)$(MAKE) $(build)=libtraceevent

$(OUTPUT)libtraceevent.so: $(TE_IN)
	$(QUIET_LINK)$(CC) --shared $^ -o $@

$(OUTPUT)libtraceevent.a: $(TE_IN)
	$(QUIET_LINK)$(RM) $@; $(AR) rcs $@ $^

$(OUTPUT)libtraceevent-dynamic-list: $(PLUGINS)
	$(QUIET_GEN)$(call do_generate_dynamic_list_file, $(PLUGINS), $@)

plugins: $(PLUGINS)

__plugin_obj = $(notdir $@)
  plugin_obj = $(__plugin_obj:-in.o=)

$(PLUGINS_IN): force
	$(Q)$(MAKE) $(build)=$(plugin_obj)

$(OUTPUT)%.so: $(OUTPUT)%-in.o
	$(QUIET_LINK)$(CC) $(CFLAGS) -shared -nostartfiles -o $@ $^

define make_version.h
  (echo '/* This file is automatically generated. Do not modify. */';		\
   echo \#define VERSION_CODE $(shell						\
   expr $(VERSION) \* 256 + $(PATCHLEVEL));					\
   echo '#define EXTRAVERSION ' $(EXTRAVERSION);				\
   echo '#define VERSION_STRING "'$(VERSION).$(PATCHLEVEL).$(EXTRAVERSION)'"';	\
   echo '#define FILE_VERSION '$(FILE_VERSION);					\
  ) > $1
endef

define update_version.h
  ($(call make_version.h, $@.tmp);		\
    if [ -r $@ ] && cmp -s $@ $@.tmp; then	\
      rm -f $@.tmp;				\
    else					\
      echo '  UPDATE                 $@';	\
      mv -f $@.tmp $@;				\
    fi);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
endef

ep_version.h: force
	$(Q)$(N)$(call update_version.h)

VERSION_FILES = ep_version.h

define update_dir
<<<<<<< HEAD
	(echo $1 > $@.tmp;	\
	if [ -r $@ ] && cmp -s $@ $@.tmp; then		\
		rm -f $@.tmp;				\
	else						\
		echo '  UPDATE                 $@';	\
		mv -f $@.tmp $@;			\
	fi);
endef

## make deps

all_objs := $(sort $(ALL_OBJS))
all_deps := $(all_objs:%.o=.%.d)

# let .d file also depends on the source and header files
define check_deps
		@set -e; $(RM) $@; \
		$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		$(RM) $@.$$$$
endef

$(gui_deps): ks_version.h
$(non_gui_deps): tc_version.h

$(all_deps): .%.d: $(src)/%.c
	$(Q)$(call check_deps)

$(all_objs) : %.o : .%.d

dep_includes := $(wildcard $(all_deps))

ifneq ($(dep_includes),)
 include $(dep_includes)
endif

### Detect environment changes
TRACK_CFLAGS = $(subst ','\'',$(CFLAGS)):$(ARCH):$(CROSS_COMPILE)

TRACEEVENT-CFLAGS: force
	@FLAGS='$(TRACK_CFLAGS)'; \
	    if test x"$$FLAGS" != x"`cat TRACEEVENT-CFLAGS 2>/dev/null`" ; then \
		echo 1>&2 "    * new build flags or cross compiler"; \
		echo "$$FLAGS" >TRACEEVENT-CFLAGS; \
            fi

=======
  (echo $1 > $@.tmp;				\
   if [ -r $@ ] && cmp -s $@ $@.tmp; then	\
     rm -f $@.tmp;				\
   else						\
     echo '  UPDATE                 $@';	\
     mv -f $@.tmp $@;				\
   fi);
endef

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
tags:	force
	$(RM) tags
	find . -name '*.[ch]' | xargs ctags --extra=+f --c-kinds=+px \
	--regex-c++='/_PE\(([^,)]*).*/PEVENT_ERRNO__\1/'

TAGS:	force
	$(RM) TAGS
	find . -name '*.[ch]' | xargs etags \
	--regex='/_PE(\([^,)]*\).*/PEVENT_ERRNO__\1/'

define do_install
<<<<<<< HEAD
	$(print_install)				\
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if [ ! -d '$(DESTDIR_SQ)$2' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR_SQ)$2';	\
	fi;						\
	$(INSTALL) $1 '$(DESTDIR_SQ)$2'
endef

<<<<<<< HEAD
install_lib: all_cmd install_plugins install_python
	$(Q)$(call do_install,$(LIB_FILE),$(bindir_SQ))
=======
define do_install_plugins
	for plugin in $1; do				\
	  $(call do_install,$$plugin,$(plugin_dir_SQ));	\
	done
endef

define do_generate_dynamic_list_file
	(echo '{';							\
	$(NM) -u -D $1 | awk 'NF>1 {print "\t"$$2";"}' | sort -u;	\
	echo '};';							\
	) > $2
endef

install_lib: all_cmd install_plugins
	$(call QUIET_INSTALL, $(LIB_FILE)) \
		$(call do_install,$(LIB_FILE),$(libdir_SQ))

install_plugins: $(PLUGINS)
	$(call QUIET_INSTALL, trace_plugins) \
		$(call do_install_plugins, $(PLUGINS))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

install: install_lib

clean:
<<<<<<< HEAD
	$(RM) *.o *~ $(TARGETS) *.a *.so $(VERSION_FILES) .*.d
	$(RM) TRACEEVENT-CFLAGS tags TAGS

endif # skip-makefile

PHONY += force
=======
	$(call QUIET_CLEAN, libtraceevent) \
		$(RM) *.o *~ $(TARGETS) *.a *.so $(VERSION_FILES) .*.d .*.cmd \
		$(RM) TRACEEVENT-CFLAGS tags TAGS

PHONY += force plugins
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
force:

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
