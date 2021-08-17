# Makefile for vm tools
#
<<<<<<< HEAD
TARGETS=page-types slabinfo

LK_DIR = ../lib/lk
LIBLK = $(LK_DIR)/liblk.a

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -I../lib/
LDFLAGS = $(LIBLK)

$(TARGETS): liblk

liblk:
	make -C $(LK_DIR)
=======
TARGETS=page-types slabinfo page_owner_sort

LIB_DIR = ../lib/api
LIBS = $(LIB_DIR)/libapi.a

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -I../lib/
LDFLAGS = $(LIBS)

$(TARGETS): $(LIBS)

$(LIBS):
	make -C $(LIB_DIR)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
<<<<<<< HEAD
	$(RM) page-types slabinfo
	make -C ../lib/lk clean
=======
	$(RM) page-types slabinfo page_owner_sort
	make -C $(LIB_DIR) clean
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
