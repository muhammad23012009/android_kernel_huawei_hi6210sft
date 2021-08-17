# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

# List of programs to build
hostprogs-y := hid-example

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_hid-example.o += -I$(objtree)/usr/include
<<<<<<< HEAD
=======

all: hid-example
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
