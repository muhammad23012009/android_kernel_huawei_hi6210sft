/*
 * Copyright 2003-2004, Instant802 Networks, Inc.
 * Copyright 2006, Devicescape Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef AES_CCM_H
#define AES_CCM_H

#include <linux/crypto.h>

<<<<<<< HEAD
struct crypto_cipher *ieee80211_aes_key_setup_encrypt(const u8 key[]);
void ieee80211_aes_ccm_encrypt(struct crypto_cipher *tfm, u8 *scratch,
			       u8 *data, size_t data_len,
			       u8 *cdata, u8 *mic);
int ieee80211_aes_ccm_decrypt(struct crypto_cipher *tfm, u8 *scratch,
			      u8 *cdata, size_t data_len,
			      u8 *mic, u8 *data);
void ieee80211_aes_key_free(struct crypto_cipher *tfm);
=======
#define CCM_AAD_LEN	32

struct crypto_aead *ieee80211_aes_key_setup_encrypt(const u8 key[],
						    size_t key_len,
						    size_t mic_len);
int ieee80211_aes_ccm_encrypt(struct crypto_aead *tfm, u8 *b_0, u8 *aad,
			      u8 *data, size_t data_len, u8 *mic,
			      size_t mic_len);
int ieee80211_aes_ccm_decrypt(struct crypto_aead *tfm, u8 *b_0, u8 *aad,
			      u8 *data, size_t data_len, u8 *mic,
			      size_t mic_len);
void ieee80211_aes_key_free(struct crypto_aead *tfm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* AES_CCM_H */
