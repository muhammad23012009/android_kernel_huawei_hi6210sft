/* Instantiate a public key crypto key from an X.509 Certificate
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#define pr_fmt(fmt) "X.509: "fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/err.h>
#include <linux/mpi.h>
#include <linux/asn1_decoder.h>
#include <keys/asymmetric-subtype.h>
#include <keys/asymmetric-parser.h>
#include <crypto/hash.h>
#include "asymmetric_keys.h"
#include "public_key.h"
#include "x509_parser.h"

static const
struct public_key_algorithm *x509_public_key_algorithms[PKEY_ALGO__LAST] = {
	[PKEY_ALGO_DSA]		= NULL,
#if defined(CONFIG_PUBLIC_KEY_ALGO_RSA) || \
	defined(CONFIG_PUBLIC_KEY_ALGO_RSA_MODULE)
	[PKEY_ALGO_RSA]		= &RSA_public_key_algorithm,
#endif
};

/*
 * Check the signature on a certificate using the provided public key
 */
static int x509_check_signature(const struct public_key *pub,
				const struct x509_certificate *cert)
{
	struct public_key_signature *sig;
	struct crypto_shash *tfm;
	struct shash_desc *desc;
	size_t digest_size, desc_size;
	int ret;

	pr_devel("==>%s()\n", __func__);
	
	/* Allocate the hashing algorithm we're going to need and find out how
	 * big the hash operational data will be.
	 */
	tfm = crypto_alloc_shash(pkey_hash_algo[cert->sig_hash_algo], 0, 0);
	if (IS_ERR(tfm))
		return (PTR_ERR(tfm) == -ENOENT) ? -ENOPKG : PTR_ERR(tfm);

	desc_size = crypto_shash_descsize(tfm) + sizeof(*desc);
	digest_size = crypto_shash_digestsize(tfm);

	/* We allocate the hash operational data storage on the end of our
	 * context data.
	 */
	ret = -ENOMEM;
	sig = kzalloc(sizeof(*sig) + desc_size + digest_size, GFP_KERNEL);
	if (!sig)
		goto error_no_sig;

	sig->pkey_hash_algo	= cert->sig_hash_algo;
	sig->digest		= (u8 *)sig + sizeof(*sig) + desc_size;
	sig->digest_size	= digest_size;

	desc = (void *)sig + sizeof(*sig);
	desc->tfm	= tfm;
	desc->flags	= CRYPTO_TFM_REQ_MAY_SLEEP;

	ret = crypto_shash_init(desc);
	if (ret < 0)
		goto error;

	ret = -ENOMEM;
	sig->rsa.s = mpi_read_raw_data(cert->sig, cert->sig_size);
	if (!sig->rsa.s)
		goto error;

	ret = crypto_shash_finup(desc, cert->tbs, cert->tbs_size, sig->digest);
	if (ret < 0)
		goto error_mpi;

	ret = pub->algo->verify_signature(pub, sig);

	pr_debug("Cert Verification: %d\n", ret);

error_mpi:
	mpi_free(sig->rsa.s);
error:
	kfree(sig);
error_no_sig:
	crypto_free_shash(tfm);

	pr_devel("<==%s() = %d\n", __func__, ret);
	return ret;
=======
#include <keys/asymmetric-subtype.h>
#include <keys/asymmetric-parser.h>
#include <keys/system_keyring.h>
#include <crypto/hash.h>
#include "asymmetric_keys.h"
#include "x509_parser.h"

/*
 * Set up the signature parameters in an X.509 certificate.  This involves
 * digesting the signed data and extracting the signature.
 */
int x509_get_sig_params(struct x509_certificate *cert)
{
	struct public_key_signature *sig = cert->sig;
	struct crypto_shash *tfm;
	struct shash_desc *desc;
	size_t desc_size;
	int ret;

	pr_devel("==>%s()\n", __func__);

	if (!cert->pub->pkey_algo)
		cert->unsupported_key = true;

	if (!sig->pkey_algo)
		cert->unsupported_sig = true;

	/* We check the hash if we can - even if we can't then verify it */
	if (!sig->hash_algo) {
		cert->unsupported_sig = true;
		return 0;
	}

	sig->s = kmemdup(cert->raw_sig, cert->raw_sig_size, GFP_KERNEL);
	if (!sig->s)
		return -ENOMEM;

	sig->s_size = cert->raw_sig_size;

	/* Allocate the hashing algorithm we're going to need and find out how
	 * big the hash operational data will be.
	 */
	tfm = crypto_alloc_shash(sig->hash_algo, 0, 0);
	if (IS_ERR(tfm)) {
		if (PTR_ERR(tfm) == -ENOENT) {
			cert->unsupported_sig = true;
			return 0;
		}
		return PTR_ERR(tfm);
	}

	desc_size = crypto_shash_descsize(tfm) + sizeof(*desc);
	sig->digest_size = crypto_shash_digestsize(tfm);

	ret = -ENOMEM;
	sig->digest = kmalloc(sig->digest_size, GFP_KERNEL);
	if (!sig->digest)
		goto error;

	desc = kzalloc(desc_size, GFP_KERNEL);
	if (!desc)
		goto error;

	desc->tfm = tfm;
	desc->flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	ret = crypto_shash_init(desc);
	if (ret < 0)
		goto error_2;
	might_sleep();
	ret = crypto_shash_finup(desc, cert->tbs, cert->tbs_size, sig->digest);

error_2:
	kfree(desc);
error:
	crypto_free_shash(tfm);
	pr_devel("<==%s() = %d\n", __func__, ret);
	return ret;
}

/*
 * Check for self-signedness in an X.509 cert and if found, check the signature
 * immediately if we can.
 */
int x509_check_for_self_signed(struct x509_certificate *cert)
{
	int ret = 0;

	pr_devel("==>%s()\n", __func__);

	if (cert->raw_subject_size != cert->raw_issuer_size ||
	    memcmp(cert->raw_subject, cert->raw_issuer,
		   cert->raw_issuer_size) != 0)
		goto not_self_signed;

	if (cert->sig->auth_ids[0] || cert->sig->auth_ids[1]) {
		/* If the AKID is present it may have one or two parts.  If
		 * both are supplied, both must match.
		 */
		bool a = asymmetric_key_id_same(cert->skid, cert->sig->auth_ids[1]);
		bool b = asymmetric_key_id_same(cert->id, cert->sig->auth_ids[0]);

		if (!a && !b)
			goto not_self_signed;

		ret = -EKEYREJECTED;
		if (((a && !b) || (b && !a)) &&
		    cert->sig->auth_ids[0] && cert->sig->auth_ids[1])
			goto out;
	}

	ret = -EKEYREJECTED;
	if (strcmp(cert->pub->pkey_algo, cert->sig->pkey_algo) != 0)
		goto out;

	ret = public_key_verify_signature(cert->pub, cert->sig);
	if (ret < 0) {
		if (ret == -ENOPKG) {
			cert->unsupported_sig = true;
			ret = 0;
		}
		goto out;
	}

	pr_devel("Cert Self-signature verified");
	cert->self_signed = true;

out:
	pr_devel("<==%s() = %d\n", __func__, ret);
	return ret;

not_self_signed:
	pr_devel("<==%s() = 0 [not]\n", __func__);
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Attempt to parse a data blob for a key as an X509 certificate.
 */
static int x509_key_preparse(struct key_preparsed_payload *prep)
{
<<<<<<< HEAD
	struct x509_certificate *cert;
	size_t srlen, sulen;
	char *desc = NULL;
=======
	struct asymmetric_key_ids *kids;
	struct x509_certificate *cert;
	const char *q;
	size_t srlen, sulen;
	char *desc = NULL, *p;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	cert = x509_cert_parse(prep->data, prep->datalen);
	if (IS_ERR(cert))
		return PTR_ERR(cert);

	pr_devel("Cert Issuer: %s\n", cert->issuer);
	pr_devel("Cert Subject: %s\n", cert->subject);
<<<<<<< HEAD
	pr_devel("Cert Key Algo: %s\n", pkey_algo[cert->pkey_algo]);
	pr_devel("Cert Valid From: %04ld-%02d-%02d %02d:%02d:%02d\n",
		 cert->valid_from.tm_year + 1900, cert->valid_from.tm_mon + 1,
		 cert->valid_from.tm_mday, cert->valid_from.tm_hour,
		 cert->valid_from.tm_min,  cert->valid_from.tm_sec);
	pr_devel("Cert Valid To: %04ld-%02d-%02d %02d:%02d:%02d\n",
		 cert->valid_to.tm_year + 1900, cert->valid_to.tm_mon + 1,
		 cert->valid_to.tm_mday, cert->valid_to.tm_hour,
		 cert->valid_to.tm_min,  cert->valid_to.tm_sec);
	pr_devel("Cert Signature: %s + %s\n",
		 pkey_algo[cert->sig_pkey_algo],
		 pkey_hash_algo[cert->sig_hash_algo]);

	if (!cert->fingerprint || !cert->authority) {
		pr_warn("Cert for '%s' must have SubjKeyId and AuthKeyId extensions\n",
			cert->subject);
		ret = -EKEYREJECTED;
		goto error_free_cert;
	}

	cert->pub->algo = x509_public_key_algorithms[cert->pkey_algo];
	cert->pub->id_type = PKEY_ID_X509;

	/* Check the signature on the key */
	if (strcmp(cert->fingerprint, cert->authority) == 0) {
		ret = x509_check_signature(cert->pub, cert);
		if (ret < 0)
			goto error_free_cert;
=======

	if (cert->unsupported_key) {
		ret = -ENOPKG;
		goto error_free_cert;
	}

	pr_devel("Cert Key Algo: %s\n", cert->pub->pkey_algo);
	pr_devel("Cert Valid period: %lld-%lld\n", cert->valid_from, cert->valid_to);

	cert->pub->id_type = "X509";

	if (cert->unsupported_sig) {
		public_key_signature_free(cert->sig);
		cert->sig = NULL;
	} else {
		pr_devel("Cert Signature: %s + %s\n",
			 cert->sig->pkey_algo, cert->sig->hash_algo);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Propose a description */
	sulen = strlen(cert->subject);
<<<<<<< HEAD
	srlen = strlen(cert->fingerprint);
	ret = -ENOMEM;
	desc = kmalloc(sulen + 2 + srlen + 1, GFP_KERNEL);
	if (!desc)
		goto error_free_cert;
	memcpy(desc, cert->subject, sulen);
	desc[sulen] = ':';
	desc[sulen + 1] = ' ';
	memcpy(desc + sulen + 2, cert->fingerprint, srlen);
	desc[sulen + 2 + srlen] = 0;

	/* We're pinning the module by being linked against it */
	__module_get(public_key_subtype.owner);
	prep->type_data[0] = &public_key_subtype;
	prep->type_data[1] = cert->fingerprint;
	prep->payload = cert->pub;
=======
	if (cert->raw_skid) {
		srlen = cert->raw_skid_size;
		q = cert->raw_skid;
	} else {
		srlen = cert->raw_serial_size;
		q = cert->raw_serial;
	}

	ret = -ENOMEM;
	desc = kmalloc(sulen + 2 + srlen * 2 + 1, GFP_KERNEL);
	if (!desc)
		goto error_free_cert;
	p = memcpy(desc, cert->subject, sulen);
	p += sulen;
	*p++ = ':';
	*p++ = ' ';
	p = bin2hex(p, q, srlen);
	*p = 0;

	kids = kmalloc(sizeof(struct asymmetric_key_ids), GFP_KERNEL);
	if (!kids)
		goto error_free_desc;
	kids->id[0] = cert->id;
	kids->id[1] = cert->skid;

	/* We're pinning the module by being linked against it */
	__module_get(public_key_subtype.owner);
	prep->payload.data[asym_subtype] = &public_key_subtype;
	prep->payload.data[asym_key_ids] = kids;
	prep->payload.data[asym_crypto] = cert->pub;
	prep->payload.data[asym_auth] = cert->sig;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	prep->description = desc;
	prep->quotalen = 100;

	/* We've finished with the certificate */
	cert->pub = NULL;
<<<<<<< HEAD
	cert->fingerprint = NULL;
	desc = NULL;
	ret = 0;

=======
	cert->id = NULL;
	cert->skid = NULL;
	cert->sig = NULL;
	desc = NULL;
	ret = 0;

error_free_desc:
	kfree(desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
error_free_cert:
	x509_free_certificate(cert);
	return ret;
}

static struct asymmetric_key_parser x509_key_parser = {
	.owner	= THIS_MODULE,
	.name	= "x509",
	.parse	= x509_key_preparse,
};

/*
 * Module stuff
 */
static int __init x509_key_init(void)
{
	return register_asymmetric_key_parser(&x509_key_parser);
}

static void __exit x509_key_exit(void)
{
	unregister_asymmetric_key_parser(&x509_key_parser);
}

module_init(x509_key_init);
module_exit(x509_key_exit);
<<<<<<< HEAD
=======

MODULE_DESCRIPTION("X.509 certificate parser");
MODULE_LICENSE("GPL");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
