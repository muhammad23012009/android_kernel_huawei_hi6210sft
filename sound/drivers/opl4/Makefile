#
# Makefile for ALSA
# Copyright (c) 2001 by Jaroslav Kysela <perex@perex.cz>
#

<<<<<<< HEAD
snd-opl4-lib-objs := opl4_lib.o opl4_mixer.o opl4_proc.o
=======
snd-opl4-lib-objs := opl4_lib.o opl4_mixer.o
snd-opl4-lib-$(CONFIG_SND_PROC_FS) += opl4_proc.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
snd-opl4-synth-objs := opl4_seq.o opl4_synth.o yrw801.o

obj-$(CONFIG_SND_OPL4_LIB) += snd-opl4-lib.o
obj-$(CONFIG_SND_OPL4_LIB_SEQ) += snd-opl4-synth.o
