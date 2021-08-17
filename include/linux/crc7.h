#ifndef _LINUX_CRC7_H
#define _LINUX_CRC7_H
#include <linux/types.h>

<<<<<<< HEAD
extern const u8 crc7_syndrome_table[256];

static inline u8 crc7_byte(u8 crc, u8 data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}

extern u8 crc7(u8 crc, const u8 *buffer, size_t len);
=======
extern const u8 crc7_be_syndrome_table[256];

static inline u8 crc7_be_byte(u8 crc, u8 data)
{
	return crc7_be_syndrome_table[crc ^ data];
}

extern u8 crc7_be(u8 crc, const u8 *buffer, size_t len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
