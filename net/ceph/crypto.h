#ifndef _FS_CEPH_CRYPTO_H
#define _FS_CEPH_CRYPTO_H

#include <linux/ceph/types.h>
#include <linux/ceph/buffer.h>

/*
 * cryptographic secret
 */
struct ceph_crypto_key {
	int type;
	struct ceph_timespec created;
	int len;
	void *key;
<<<<<<< HEAD
};

static inline void ceph_crypto_key_destroy(struct ceph_crypto_key *key)
{
	if (key)
		kfree(key->key);
}

extern int ceph_crypto_key_clone(struct ceph_crypto_key *dst,
				 const struct ceph_crypto_key *src);
extern int ceph_crypto_key_encode(struct ceph_crypto_key *key,
				  void **p, void *end);
extern int ceph_crypto_key_decode(struct ceph_crypto_key *key,
				  void **p, void *end);
extern int ceph_crypto_key_unarmor(struct ceph_crypto_key *key, const char *in);

/* crypto.c */
extern int ceph_decrypt(struct ceph_crypto_key *secret,
			void *dst, size_t *dst_len,
			const void *src, size_t src_len);
extern int ceph_encrypt(struct ceph_crypto_key *secret,
			void *dst, size_t *dst_len,
			const void *src, size_t src_len);
extern int ceph_decrypt2(struct ceph_crypto_key *secret,
			void *dst1, size_t *dst1_len,
			void *dst2, size_t *dst2_len,
			const void *src, size_t src_len);
extern int ceph_encrypt2(struct ceph_crypto_key *secret,
			 void *dst, size_t *dst_len,
			 const void *src1, size_t src1_len,
			 const void *src2, size_t src2_len);
int ceph_crypt(const struct ceph_crypto_key *key, bool encrypt,
	       void *buf, int buf_len, int in_len, int *pout_len);
extern int ceph_crypto_init(void);
extern void ceph_crypto_shutdown(void);

/* armor.c */
extern int ceph_armor(char *dst, const char *src, const char *end);
extern int ceph_unarmor(char *dst, const char *src, const char *end);
=======
	struct crypto_skcipher *tfm;
};

int ceph_crypto_key_clone(struct ceph_crypto_key *dst,
			  const struct ceph_crypto_key *src);
int ceph_crypto_key_encode(struct ceph_crypto_key *key, void **p, void *end);
int ceph_crypto_key_decode(struct ceph_crypto_key *key, void **p, void *end);
int ceph_crypto_key_unarmor(struct ceph_crypto_key *key, const char *in);
void ceph_crypto_key_destroy(struct ceph_crypto_key *key);

/* crypto.c */
int ceph_crypt(const struct ceph_crypto_key *key, bool encrypt,
	       void *buf, int buf_len, int in_len, int *pout_len);
int ceph_crypto_init(void);
void ceph_crypto_shutdown(void);

/* armor.c */
int ceph_armor(char *dst, const char *src, const char *end);
int ceph_unarmor(char *dst, const char *src, const char *end);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
