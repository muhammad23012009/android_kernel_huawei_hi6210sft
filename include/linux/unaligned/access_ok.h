#ifndef _LINUX_UNALIGNED_ACCESS_OK_H
#define _LINUX_UNALIGNED_ACCESS_OK_H

#include <linux/kernel.h>
#include <asm/byteorder.h>

<<<<<<< HEAD
static inline u16 get_unaligned_le16(const void *p)
=======
static __always_inline u16 get_unaligned_le16(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return le16_to_cpup((__le16 *)p);
}

<<<<<<< HEAD
static inline u32 get_unaligned_le32(const void *p)
=======
static __always_inline u32 get_unaligned_le32(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return le32_to_cpup((__le32 *)p);
}

<<<<<<< HEAD
static inline u64 get_unaligned_le64(const void *p)
=======
static __always_inline u64 get_unaligned_le64(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return le64_to_cpup((__le64 *)p);
}

<<<<<<< HEAD
static inline u16 get_unaligned_be16(const void *p)
=======
static __always_inline u16 get_unaligned_be16(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return be16_to_cpup((__be16 *)p);
}

<<<<<<< HEAD
static inline u32 get_unaligned_be32(const void *p)
=======
static __always_inline u32 get_unaligned_be32(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return be32_to_cpup((__be32 *)p);
}

<<<<<<< HEAD
static inline u64 get_unaligned_be64(const void *p)
=======
static __always_inline u64 get_unaligned_be64(const void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return be64_to_cpup((__be64 *)p);
}

<<<<<<< HEAD
static inline void put_unaligned_le16(u16 val, void *p)
=======
static __always_inline void put_unaligned_le16(u16 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__le16 *)p) = cpu_to_le16(val);
}

<<<<<<< HEAD
static inline void put_unaligned_le32(u32 val, void *p)
=======
static __always_inline void put_unaligned_le32(u32 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__le32 *)p) = cpu_to_le32(val);
}

<<<<<<< HEAD
static inline void put_unaligned_le64(u64 val, void *p)
=======
static __always_inline void put_unaligned_le64(u64 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__le64 *)p) = cpu_to_le64(val);
}

<<<<<<< HEAD
static inline void put_unaligned_be16(u16 val, void *p)
=======
static __always_inline void put_unaligned_be16(u16 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__be16 *)p) = cpu_to_be16(val);
}

<<<<<<< HEAD
static inline void put_unaligned_be32(u32 val, void *p)
=======
static __always_inline void put_unaligned_be32(u32 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__be32 *)p) = cpu_to_be32(val);
}

<<<<<<< HEAD
static inline void put_unaligned_be64(u64 val, void *p)
=======
static __always_inline void put_unaligned_be64(u64 val, void *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*((__be64 *)p) = cpu_to_be64(val);
}

#endif /* _LINUX_UNALIGNED_ACCESS_OK_H */
