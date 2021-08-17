/*
 * Copyright (C) 2005,2006,2007,2008 IBM Corporation
 *
 * Authors:
 * Reiner Sailer      <sailer@watson.ibm.com>
 * Leendert van Doorn <leendert@watson.ibm.com>
 * Mimi Zohar         <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * File: ima_init.c
 *             initialization and cleanup functions
 */
<<<<<<< HEAD
=======

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/err.h>
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "ima.h"

/* name for boot aggregate entry */
static const char *boot_aggregate_name = "boot_aggregate";
int ima_used_chip;

/* Add the boot aggregate to the IMA measurement list and extend
 * the PCR register.
 *
 * Calculate the boot aggregate, a SHA1 over tpm registers 0-7,
 * assuming a TPM chip exists, and zeroes if the TPM chip does not
 * exist.  Add the boot aggregate measurement to the measurement
 * list and extend the PCR register.
 *
 * If a tpm chip does not exist, indicate the core root of trust is
 * not hardware based by invalidating the aggregate PCR value.
 * (The aggregate PCR value is invalidated by adding one value to
 * the measurement list and extending the aggregate PCR value with
 * a different value.) Violations add a zero entry to the measurement
 * list and extend the aggregate PCR value with ff...ff's.
 */
<<<<<<< HEAD
static void __init ima_add_boot_aggregate(void)
{
	struct ima_template_entry *entry;
	const char *op = "add_boot_aggregate";
	const char *audit_cause = "ENOMEM";
	int result = -ENOMEM;
	int violation = 1;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		goto err_out;

	memset(&entry->template, 0, sizeof(entry->template));
	strncpy(entry->template.file_name, boot_aggregate_name,
		IMA_EVENT_NAME_LEN_MAX);
	if (ima_used_chip) {
		violation = 0;
		result = ima_calc_boot_aggregate(entry->template.digest);
		if (result < 0) {
			audit_cause = "hashing_error";
			kfree(entry);
			goto err_out;
		}
	}
	result = ima_store_template(entry, violation, NULL);
	if (result < 0)
		kfree(entry);
	return;
err_out:
	integrity_audit_msg(AUDIT_INTEGRITY_PCR, NULL, boot_aggregate_name, op,
			    audit_cause, result, 0);
}

int __init ima_init(void)
{
	u8 pcr_i[IMA_DIGEST_SIZE];
=======
static int __init ima_add_boot_aggregate(void)
{
	static const char op[] = "add_boot_aggregate";
	const char *audit_cause = "ENOMEM";
	struct ima_template_entry *entry;
	struct integrity_iint_cache tmp_iint, *iint = &tmp_iint;
	struct ima_event_data event_data = {iint, NULL, boot_aggregate_name,
					    NULL, 0, NULL};
	int result = -ENOMEM;
	int violation = 0;
	struct {
		struct ima_digest_data hdr;
		char digest[TPM_DIGEST_SIZE];
	} hash;

	memset(iint, 0, sizeof(*iint));
	memset(&hash, 0, sizeof(hash));
	iint->ima_hash = &hash.hdr;
	iint->ima_hash->algo = HASH_ALGO_SHA1;
	iint->ima_hash->length = SHA1_DIGEST_SIZE;

	if (ima_used_chip) {
		result = ima_calc_boot_aggregate(&hash.hdr);
		if (result < 0) {
			audit_cause = "hashing_error";
			goto err_out;
		}
	}

	result = ima_alloc_init_template(&event_data, &entry);
	if (result < 0) {
		audit_cause = "alloc_entry";
		goto err_out;
	}

	result = ima_store_template(entry, violation, NULL,
				    boot_aggregate_name,
				    CONFIG_IMA_MEASURE_PCR_IDX);
	if (result < 0) {
		ima_free_template_entry(entry);
		audit_cause = "store_entry";
		goto err_out;
	}
	return 0;
err_out:
	integrity_audit_msg(AUDIT_INTEGRITY_PCR, NULL, boot_aggregate_name, op,
			    audit_cause, result, 0);
	return result;
}

#ifdef CONFIG_IMA_LOAD_X509
void __init ima_load_x509(void)
{
	int unset_flags = ima_policy_flag & IMA_APPRAISE;

	ima_policy_flag &= ~unset_flags;
	integrity_load_x509(INTEGRITY_KEYRING_IMA, CONFIG_IMA_X509_PATH);
	ima_policy_flag |= unset_flags;
}
#endif

int __init ima_init(void)
{
	u8 pcr_i[TPM_DIGEST_SIZE];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int rc;

	ima_used_chip = 0;
	rc = tpm_pcr_read(TPM_ANY_NUM, 0, pcr_i);
	if (rc == 0)
		ima_used_chip = 1;

	if (!ima_used_chip)
<<<<<<< HEAD
		pr_info("IMA: No TPM chip found, activating TPM-bypass!\n");
=======
		pr_info("No TPM chip found, activating TPM-bypass!\n");

	rc = integrity_init_keyring(INTEGRITY_KEYRING_IMA);
	if (rc)
		return rc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rc = ima_init_crypto();
	if (rc)
		return rc;
<<<<<<< HEAD
	ima_add_boot_aggregate();	/* boot aggregate must be first entry */
=======
	rc = ima_init_template();
	if (rc != 0)
		return rc;

	rc = ima_add_boot_aggregate();	/* boot aggregate must be first entry */
	if (rc != 0)
		return rc;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ima_init_policy();

	return ima_fs_init();
}
