# This creates the demonstration utility "lguest" which runs a Linux guest.
<<<<<<< HEAD
# Missing headers?  Add "-I../../../include -I../../../arch/x86/include"
CFLAGS:=-m32 -Wall -Wmissing-declarations -Wmissing-prototypes -O3 -U_FORTIFY_SOURCE

all: lguest

clean:
	rm -f lguest
=======
CFLAGS:=-m32 -Wall -Wmissing-declarations -Wmissing-prototypes -O3 -U_FORTIFY_SOURCE -Iinclude

all: lguest

include/linux/virtio_types.h: ../../include/uapi/linux/virtio_types.h
	mkdir -p include/linux 2>&1 || true
	ln -sf ../../../../include/uapi/linux/virtio_types.h $@

lguest: include/linux/virtio_types.h

clean:
	rm -f lguest
	rm -rf include
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
