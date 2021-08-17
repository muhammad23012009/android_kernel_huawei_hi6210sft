/******************************************************************************
 *
 * Module Name: tbinstal - ACPI table installation and removal
 *
 *****************************************************************************/

/*
<<<<<<< HEAD
 * Copyright (C) 2000 - 2013, Intel Corp.
=======
 * Copyright (C) 2000 - 2016, Intel Corp.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#include <acpi/acpi.h>
#include "accommon.h"
<<<<<<< HEAD
#include "acnamesp.h"
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "actables.h"

#define _COMPONENT          ACPI_TABLES
ACPI_MODULE_NAME("tbinstal")

<<<<<<< HEAD
/******************************************************************************
 *
 * FUNCTION:    acpi_tb_verify_table
 *
 * PARAMETERS:  table_desc          - table
 *
 * RETURN:      Status
 *
 * DESCRIPTION: this function is called to verify and map table
 *
 *****************************************************************************/
acpi_status acpi_tb_verify_table(struct acpi_table_desc *table_desc)
{
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(tb_verify_table);

	/* Map the table if necessary */

	if (!table_desc->pointer) {
		if ((table_desc->flags & ACPI_TABLE_ORIGIN_MASK) ==
		    ACPI_TABLE_ORIGIN_MAPPED) {
			table_desc->pointer =
			    acpi_os_map_memory(table_desc->address,
					       table_desc->length);
		}
		if (!table_desc->pointer) {
			return_ACPI_STATUS(AE_NO_MEMORY);
		}
	}

	/* FACS is the odd table, has no standard ACPI header and no checksum */

	if (!ACPI_COMPARE_NAME(&table_desc->signature, ACPI_SIG_FACS)) {

		/* Always calculate checksum, ignore bad checksum if requested */

		status =
		    acpi_tb_verify_checksum(table_desc->pointer,
					    table_desc->length);
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_add_table
 *
 * PARAMETERS:  table_desc          - Table descriptor
 *              table_index         - Where the table index is returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: This function is called to add an ACPI table. It is used to
 *              dynamically load tables via the Load and load_table AML
 *              operators.
 *
 ******************************************************************************/

acpi_status
acpi_tb_add_table(struct acpi_table_desc *table_desc, u32 *table_index)
{
	u32 i;
	acpi_status status = AE_OK;

	ACPI_FUNCTION_TRACE(tb_add_table);

	if (!table_desc->pointer) {
		status = acpi_tb_verify_table(table_desc);
		if (ACPI_FAILURE(status) || !table_desc->pointer) {
			return_ACPI_STATUS(status);
		}
	}

	/*
	 * Validate the incoming table signature.
	 *
	 * 1) Originally, we checked the table signature for "SSDT" or "PSDT".
	 * 2) We added support for OEMx tables, signature "OEM".
	 * 3) Valid tables were encountered with a null signature, so we just
	 *    gave up on validating the signature, (05/2008).
	 * 4) We encountered non-AML tables such as the MADT, which caused
	 *    interpreter errors and kernel faults. So now, we once again allow
	 *    only "SSDT", "OEMx", and now, also a null signature. (05/2011).
	 */
	if ((table_desc->pointer->signature[0] != 0x00) &&
	    (!ACPI_COMPARE_NAME(table_desc->pointer->signature, ACPI_SIG_SSDT))
	    && (ACPI_STRNCMP(table_desc->pointer->signature, "OEM", 3))) {
		ACPI_BIOS_ERROR((AE_INFO,
				 "Table has invalid signature [%4.4s] (0x%8.8X), "
				 "must be SSDT or OEMx",
				 acpi_ut_valid_acpi_name(*(u32 *)table_desc->
							 pointer->
							 signature) ?
				 table_desc->pointer->signature : "????",
				 *(u32 *)table_desc->pointer->signature));

		return_ACPI_STATUS(AE_BAD_SIGNATURE);
	}

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);

	/* Check if table is already registered */

	for (i = 0; i < acpi_gbl_root_table_list.current_table_count; ++i) {
		if (!acpi_gbl_root_table_list.tables[i].pointer) {
			status =
			    acpi_tb_verify_table(&acpi_gbl_root_table_list.
						 tables[i]);
			if (ACPI_FAILURE(status)
			    || !acpi_gbl_root_table_list.tables[i].pointer) {
				continue;
			}
		}

		/*
		 * Check for a table match on the entire table length,
		 * not just the header.
		 */
		if (table_desc->length !=
		    acpi_gbl_root_table_list.tables[i].length) {
			continue;
		}

		if (ACPI_MEMCMP(table_desc->pointer,
				acpi_gbl_root_table_list.tables[i].pointer,
				acpi_gbl_root_table_list.tables[i].length)) {
			continue;
		}

		/*
		 * Note: the current mechanism does not unregister a table if it is
		 * dynamically unloaded. The related namespace entries are deleted,
		 * but the table remains in the root table list.
		 *
		 * The assumption here is that the number of different tables that
		 * will be loaded is actually small, and there is minimal overhead
		 * in just keeping the table in case it is needed again.
		 *
		 * If this assumption changes in the future (perhaps on large
		 * machines with many table load/unload operations), tables will
		 * need to be unregistered when they are unloaded, and slots in the
		 * root table list should be reused when empty.
		 */

		/*
		 * Table is already registered.
		 * We can delete the table that was passed as a parameter.
		 */
		acpi_tb_delete_table(table_desc);
		*table_index = i;

		if (acpi_gbl_root_table_list.tables[i].
		    flags & ACPI_TABLE_IS_LOADED) {

			/* Table is still loaded, this is an error */

			status = AE_ALREADY_EXISTS;
			goto release;
		} else {
			/* Table was unloaded, allow it to be reloaded */

			table_desc->pointer =
			    acpi_gbl_root_table_list.tables[i].pointer;
			table_desc->address =
			    acpi_gbl_root_table_list.tables[i].address;
			status = AE_OK;
			goto print_header;
		}
	}

	/*
	 * ACPI Table Override:
	 * Allow the host to override dynamically loaded tables.
	 * NOTE: the table is fully mapped at this point, and the mapping will
	 * be deleted by tb_table_override if the table is actually overridden.
	 */
	(void)acpi_tb_table_override(table_desc->pointer, table_desc);

	/* Add the table to the global root table list */

	status = acpi_tb_store_table(table_desc->address, table_desc->pointer,
				     table_desc->length, table_desc->flags,
				     table_index);
	if (ACPI_FAILURE(status)) {
		goto release;
	}

      print_header:
	acpi_tb_print_table_header(table_desc->address, table_desc->pointer);

      release:
	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
	return_ACPI_STATUS(status);
=======
/* Local prototypes */
static u8
acpi_tb_compare_tables(struct acpi_table_desc *table_desc, u32 table_index);

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_compare_tables
 *
 * PARAMETERS:  table_desc          - Table 1 descriptor to be compared
 *              table_index         - Index of table 2 to be compared
 *
 * RETURN:      TRUE if both tables are identical.
 *
 * DESCRIPTION: This function compares a table with another table that has
 *              already been installed in the root table list.
 *
 ******************************************************************************/

static u8
acpi_tb_compare_tables(struct acpi_table_desc *table_desc, u32 table_index)
{
	acpi_status status = AE_OK;
	u8 is_identical;
	struct acpi_table_header *table;
	u32 table_length;
	u8 table_flags;

	status =
	    acpi_tb_acquire_table(&acpi_gbl_root_table_list.tables[table_index],
				  &table, &table_length, &table_flags);
	if (ACPI_FAILURE(status)) {
		return (FALSE);
	}

	/*
	 * Check for a table match on the entire table length,
	 * not just the header.
	 */
	is_identical = (u8)((table_desc->length != table_length ||
			     memcmp(table_desc->pointer, table, table_length)) ?
			    FALSE : TRUE);

	/* Release the acquired table */

	acpi_tb_release_table(table, table_length, table_flags);
	return (is_identical);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*******************************************************************************
 *
<<<<<<< HEAD
 * FUNCTION:    acpi_tb_table_override
 *
 * PARAMETERS:  table_header        - Header for the original table
 *              table_desc          - Table descriptor initialized for the
 *                                    original table. May or may not be mapped.
 *
 * RETURN:      Pointer to the entire new table. NULL if table not overridden.
 *              If overridden, installs the new table within the input table
 *              descriptor.
 *
 * DESCRIPTION: Attempt table override by calling the OSL override functions.
 *              Note: If the table is overridden, then the entire new table
 *              is mapped and returned by this function.
 *
 ******************************************************************************/

struct acpi_table_header *acpi_tb_table_override(struct acpi_table_header
						 *table_header,
						 struct acpi_table_desc
						 *table_desc)
{
	acpi_status status;
	struct acpi_table_header *new_table = NULL;
	acpi_physical_address new_address = 0;
	u32 new_table_length = 0;
	u8 new_flags;
	char *override_type;

	/* (1) Attempt logical override (returns a logical address) */

	status = acpi_os_table_override(table_header, &new_table);
	if (ACPI_SUCCESS(status) && new_table) {
		new_address = ACPI_PTR_TO_PHYSADDR(new_table);
		new_table_length = new_table->length;
		new_flags = ACPI_TABLE_ORIGIN_OVERRIDE;
		override_type = "Logical";
		goto finish_override;
	}

	/* (2) Attempt physical override (returns a physical address) */

	status = acpi_os_physical_table_override(table_header,
						 &new_address,
						 &new_table_length);
	if (ACPI_SUCCESS(status) && new_address && new_table_length) {

		/* Map the entire new table */

		new_table = acpi_os_map_memory(new_address, new_table_length);
		if (!new_table) {
			ACPI_EXCEPTION((AE_INFO, AE_NO_MEMORY,
					"%4.4s %p Attempted physical table override failed",
					table_header->signature,
					ACPI_CAST_PTR(void,
						      table_desc->address)));
			return (NULL);
		}

		override_type = "Physical";
		new_flags = ACPI_TABLE_ORIGIN_MAPPED;
		goto finish_override;
	}

	return (NULL);		/* There was no override */

      finish_override:

	ACPI_INFO((AE_INFO,
		   "%4.4s %p %s table override, new table: %p",
		   table_header->signature,
		   ACPI_CAST_PTR(void, table_desc->address),
		   override_type, new_table));

	/* We can now unmap/delete the original table (if fully mapped) */

	acpi_tb_delete_table(table_desc);

	/* Setup descriptor for the new table */

	table_desc->address = new_address;
	table_desc->pointer = new_table;
	table_desc->length = new_table_length;
	table_desc->flags = new_flags;

	return (new_table);
=======
 * FUNCTION:    acpi_tb_install_table_with_override
 *
 * PARAMETERS:  new_table_desc          - New table descriptor to install
 *              override                - Whether override should be performed
 *              table_index             - Where the table index is returned
 *
 * RETURN:      None
 *
 * DESCRIPTION: Install an ACPI table into the global data structure. The
 *              table override mechanism is called to allow the host
 *              OS to replace any table before it is installed in the root
 *              table array.
 *
 ******************************************************************************/

void
acpi_tb_install_table_with_override(struct acpi_table_desc *new_table_desc,
				    u8 override, u32 *table_index)
{
	u32 i;
	acpi_status status;

	status = acpi_tb_get_next_table_descriptor(&i, NULL);
	if (ACPI_FAILURE(status)) {
		return;
	}

	/*
	 * ACPI Table Override:
	 *
	 * Before we install the table, let the host OS override it with a new
	 * one if desired. Any table within the RSDT/XSDT can be replaced,
	 * including the DSDT which is pointed to by the FADT.
	 */
	if (override) {
		acpi_tb_override_table(new_table_desc);
	}

	acpi_tb_init_table_descriptor(&acpi_gbl_root_table_list.tables[i],
				      new_table_desc->address,
				      new_table_desc->flags,
				      new_table_desc->pointer);

	acpi_tb_print_table_header(new_table_desc->address,
				   new_table_desc->pointer);

	/* This synchronizes acpi_gbl_dsdt_index */

	*table_index = i;

	/* Set the global integer width (based upon revision of the DSDT) */

	if (i == acpi_gbl_dsdt_index) {
		acpi_ut_set_integer_width(new_table_desc->pointer->revision);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*******************************************************************************
 *
<<<<<<< HEAD
 * FUNCTION:    acpi_tb_resize_root_table_list
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Expand the size of global table array
 *
 ******************************************************************************/

acpi_status acpi_tb_resize_root_table_list(void)
{
	struct acpi_table_desc *tables;
	u32 table_count;

	ACPI_FUNCTION_TRACE(tb_resize_root_table_list);

	/* allow_resize flag is a parameter to acpi_initialize_tables */

	if (!(acpi_gbl_root_table_list.flags & ACPI_ROOT_ALLOW_RESIZE)) {
		ACPI_ERROR((AE_INFO,
			    "Resize of Root Table Array is not allowed"));
		return_ACPI_STATUS(AE_SUPPORT);
	}

	/* Increase the Table Array size */

	if (acpi_gbl_root_table_list.flags & ACPI_ROOT_ORIGIN_ALLOCATED) {
		table_count = acpi_gbl_root_table_list.max_table_count;
	} else {
		table_count = acpi_gbl_root_table_list.current_table_count;
	}

	tables = ACPI_ALLOCATE_ZEROED(((acpi_size) table_count +
				       ACPI_ROOT_TABLE_SIZE_INCREMENT) *
				      sizeof(struct acpi_table_desc));
	if (!tables) {
		ACPI_ERROR((AE_INFO,
			    "Could not allocate new root table array"));
		return_ACPI_STATUS(AE_NO_MEMORY);
	}

	/* Copy and free the previous table array */

	if (acpi_gbl_root_table_list.tables) {
		ACPI_MEMCPY(tables, acpi_gbl_root_table_list.tables,
			    (acpi_size) table_count *
			    sizeof(struct acpi_table_desc));

		if (acpi_gbl_root_table_list.flags & ACPI_ROOT_ORIGIN_ALLOCATED) {
			ACPI_FREE(acpi_gbl_root_table_list.tables);
		}
	}

	acpi_gbl_root_table_list.tables = tables;
	acpi_gbl_root_table_list.max_table_count =
	    table_count + ACPI_ROOT_TABLE_SIZE_INCREMENT;
	acpi_gbl_root_table_list.flags |= ACPI_ROOT_ORIGIN_ALLOCATED;

	return_ACPI_STATUS(AE_OK);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_store_table
 *
 * PARAMETERS:  address             - Table address
 *              table               - Table header
 *              length              - Table length
 *              flags               - flags
 *
 * RETURN:      Status and table index.
 *
 * DESCRIPTION: Add an ACPI table to the global table list
 *
 ******************************************************************************/

acpi_status
acpi_tb_store_table(acpi_physical_address address,
		    struct acpi_table_header *table,
		    u32 length, u8 flags, u32 *table_index)
{
	acpi_status status;
	struct acpi_table_desc *new_table;

	/* Ensure that there is room for the table in the Root Table List */

	if (acpi_gbl_root_table_list.current_table_count >=
	    acpi_gbl_root_table_list.max_table_count) {
		status = acpi_tb_resize_root_table_list();
		if (ACPI_FAILURE(status)) {
			return (status);
		}
	}

	new_table =
	    &acpi_gbl_root_table_list.tables[acpi_gbl_root_table_list.
					     current_table_count];

	/* Initialize added table */

	new_table->address = address;
	new_table->pointer = table;
	new_table->length = length;
	new_table->owner_id = 0;
	new_table->flags = flags;

	ACPI_MOVE_32_TO_32(&new_table->signature, table->signature);

	*table_index = acpi_gbl_root_table_list.current_table_count;
	acpi_gbl_root_table_list.current_table_count++;
	return (AE_OK);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_delete_table
 *
 * PARAMETERS:  table_index         - Table index
 *
 * RETURN:      None
 *
 * DESCRIPTION: Delete one internal ACPI table
 *
 ******************************************************************************/

void acpi_tb_delete_table(struct acpi_table_desc *table_desc)
{
	/* Table must be mapped or allocated */
	if (!table_desc->pointer) {
		return;
	}
	switch (table_desc->flags & ACPI_TABLE_ORIGIN_MASK) {
	case ACPI_TABLE_ORIGIN_MAPPED:
		acpi_os_unmap_memory(table_desc->pointer, table_desc->length);
		break;
	case ACPI_TABLE_ORIGIN_ALLOCATED:
		ACPI_FREE(table_desc->pointer);
		break;

		/* Not mapped or allocated, there is nothing we can do */

	default:
		return;
	}

	table_desc->pointer = NULL;
=======
 * FUNCTION:    acpi_tb_install_standard_table
 *
 * PARAMETERS:  address             - Address of the table (might be a virtual
 *                                    address depending on the table_flags)
 *              flags               - Flags for the table
 *              reload              - Whether reload should be performed
 *              override            - Whether override should be performed
 *              table_index         - Where the table index is returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: This function is called to verify and install an ACPI table.
 *              When this function is called by "Load" or "LoadTable" opcodes,
 *              or by acpi_load_table() API, the "Reload" parameter is set.
 *              After sucessfully returning from this function, table is
 *              "INSTALLED" but not "VALIDATED".
 *
 ******************************************************************************/

acpi_status
acpi_tb_install_standard_table(acpi_physical_address address,
			       u8 flags,
			       u8 reload, u8 override, u32 *table_index)
{
	u32 i;
	acpi_status status = AE_OK;
	struct acpi_table_desc new_table_desc;

	ACPI_FUNCTION_TRACE(tb_install_standard_table);

	/* Acquire a temporary table descriptor for validation */

	status = acpi_tb_acquire_temp_table(&new_table_desc, address, flags);
	if (ACPI_FAILURE(status)) {
		ACPI_ERROR((AE_INFO,
			    "Could not acquire table length at %8.8X%8.8X",
			    ACPI_FORMAT_UINT64(address)));
		return_ACPI_STATUS(status);
	}

	/*
	 * Optionally do not load any SSDTs from the RSDT/XSDT. This can
	 * be useful for debugging ACPI problems on some machines.
	 */
	if (!reload &&
	    acpi_gbl_disable_ssdt_table_install &&
	    ACPI_COMPARE_NAME(&new_table_desc.signature, ACPI_SIG_SSDT)) {
		ACPI_INFO(("Ignoring installation of %4.4s at %8.8X%8.8X",
			   new_table_desc.signature.ascii,
			   ACPI_FORMAT_UINT64(address)));
		goto release_and_exit;
	}

	/* Validate and verify a table before installation */

	status = acpi_tb_verify_temp_table(&new_table_desc, NULL);
	if (ACPI_FAILURE(status)) {
		goto release_and_exit;
	}

	if (reload) {
		/*
		 * Validate the incoming table signature.
		 *
		 * 1) Originally, we checked the table signature for "SSDT" or "PSDT".
		 * 2) We added support for OEMx tables, signature "OEM".
		 * 3) Valid tables were encountered with a null signature, so we just
		 *    gave up on validating the signature, (05/2008).
		 * 4) We encountered non-AML tables such as the MADT, which caused
		 *    interpreter errors and kernel faults. So now, we once again allow
		 *    only "SSDT", "OEMx", and now, also a null signature. (05/2011).
		 */
		if ((new_table_desc.signature.ascii[0] != 0x00) &&
		    (!ACPI_COMPARE_NAME
		     (&new_table_desc.signature, ACPI_SIG_SSDT))
		    && (strncmp(new_table_desc.signature.ascii, "OEM", 3))) {
			ACPI_BIOS_ERROR((AE_INFO,
					 "Table has invalid signature [%4.4s] (0x%8.8X), "
					 "must be SSDT or OEMx",
					 acpi_ut_valid_nameseg(new_table_desc.
							       signature.
							       ascii) ?
					 new_table_desc.signature.
					 ascii : "????",
					 new_table_desc.signature.integer));

			status = AE_BAD_SIGNATURE;
			goto release_and_exit;
		}

		/* Check if table is already registered */

		for (i = 0; i < acpi_gbl_root_table_list.current_table_count;
		     ++i) {
			/*
			 * Check for a table match on the entire table length,
			 * not just the header.
			 */
			if (!acpi_tb_compare_tables(&new_table_desc, i)) {
				continue;
			}

			/*
			 * Note: the current mechanism does not unregister a table if it is
			 * dynamically unloaded. The related namespace entries are deleted,
			 * but the table remains in the root table list.
			 *
			 * The assumption here is that the number of different tables that
			 * will be loaded is actually small, and there is minimal overhead
			 * in just keeping the table in case it is needed again.
			 *
			 * If this assumption changes in the future (perhaps on large
			 * machines with many table load/unload operations), tables will
			 * need to be unregistered when they are unloaded, and slots in the
			 * root table list should be reused when empty.
			 */
			if (acpi_gbl_root_table_list.tables[i].flags &
			    ACPI_TABLE_IS_LOADED) {

				/* Table is still loaded, this is an error */

				status = AE_ALREADY_EXISTS;
				goto release_and_exit;
			} else {
				/*
				 * Table was unloaded, allow it to be reloaded.
				 * As we are going to return AE_OK to the caller, we should
				 * take the responsibility of freeing the input descriptor.
				 * Refill the input descriptor to ensure
				 * acpi_tb_install_table_with_override() can be called again to
				 * indicate the re-installation.
				 */
				acpi_tb_uninstall_table(&new_table_desc);
				*table_index = i;
				return_ACPI_STATUS(AE_OK);
			}
		}
	}

	/* Add the table to the global root table list */

	acpi_tb_install_table_with_override(&new_table_desc, override,
					    table_index);

	/* Invoke table handler if present */

	if (acpi_gbl_table_handler) {
		(void)acpi_gbl_table_handler(ACPI_TABLE_EVENT_INSTALL,
					     new_table_desc.pointer,
					     acpi_gbl_table_handler_context);
	}

release_and_exit:

	/* Release the temporary table descriptor */

	acpi_tb_release_temp_table(&new_table_desc);
	return_ACPI_STATUS(status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*******************************************************************************
 *
<<<<<<< HEAD
 * FUNCTION:    acpi_tb_terminate
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Delete all internal ACPI tables
 *
 ******************************************************************************/

void acpi_tb_terminate(void)
{
	u32 i;

	ACPI_FUNCTION_TRACE(tb_terminate);

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);

	/* Delete the individual tables */

	for (i = 0; i < acpi_gbl_root_table_list.current_table_count; i++) {
		acpi_tb_delete_table(&acpi_gbl_root_table_list.tables[i]);
	}

	/*
	 * Delete the root table array if allocated locally. Array cannot be
	 * mapped, so we don't need to check for that flag.
	 */
	if (acpi_gbl_root_table_list.flags & ACPI_ROOT_ORIGIN_ALLOCATED) {
		ACPI_FREE(acpi_gbl_root_table_list.tables);
	}

	acpi_gbl_root_table_list.tables = NULL;
	acpi_gbl_root_table_list.flags = 0;
	acpi_gbl_root_table_list.current_table_count = 0;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "ACPI Tables freed\n"));
	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);

	return_VOID;
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_delete_namespace_by_owner
 *
 * PARAMETERS:  table_index         - Table index
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Delete all namespace objects created when this table was loaded.
 *
 ******************************************************************************/

acpi_status acpi_tb_delete_namespace_by_owner(u32 table_index)
{
	acpi_owner_id owner_id;
	acpi_status status;

	ACPI_FUNCTION_TRACE(tb_delete_namespace_by_owner);

	status = acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (ACPI_FAILURE(status)) {
		return_ACPI_STATUS(status);
	}

	if (table_index >= acpi_gbl_root_table_list.current_table_count) {

		/* The table index does not exist */

		(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
		return_ACPI_STATUS(AE_NOT_EXIST);
	}

	/* Get the owner ID for this table, used to delete namespace nodes */

	owner_id = acpi_gbl_root_table_list.tables[table_index].owner_id;
	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);

	/*
	 * Need to acquire the namespace writer lock to prevent interference
	 * with any concurrent namespace walks. The interpreter must be
	 * released during the deletion since the acquisition of the deletion
	 * lock may block, and also since the execution of a namespace walk
	 * must be allowed to use the interpreter.
	 */
	(void)acpi_ut_release_mutex(ACPI_MTX_INTERPRETER);
	status = acpi_ut_acquire_write_lock(&acpi_gbl_namespace_rw_lock);

	acpi_ns_delete_namespace_by_owner(owner_id);
	if (ACPI_FAILURE(status)) {
		return_ACPI_STATUS(status);
	}

	acpi_ut_release_write_lock(&acpi_gbl_namespace_rw_lock);

	status = acpi_ut_acquire_mutex(ACPI_MTX_INTERPRETER);
	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_allocate_owner_id
 *
 * PARAMETERS:  table_index         - Table index
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Allocates owner_id in table_desc
 *
 ******************************************************************************/

acpi_status acpi_tb_allocate_owner_id(u32 table_index)
{
	acpi_status status = AE_BAD_PARAMETER;

	ACPI_FUNCTION_TRACE(tb_allocate_owner_id);

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (table_index < acpi_gbl_root_table_list.current_table_count) {
		status = acpi_ut_allocate_owner_id
		    (&(acpi_gbl_root_table_list.tables[table_index].owner_id));
	}

	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_release_owner_id
 *
 * PARAMETERS:  table_index         - Table index
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Releases owner_id in table_desc
 *
 ******************************************************************************/

acpi_status acpi_tb_release_owner_id(u32 table_index)
{
	acpi_status status = AE_BAD_PARAMETER;

	ACPI_FUNCTION_TRACE(tb_release_owner_id);

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (table_index < acpi_gbl_root_table_list.current_table_count) {
		acpi_ut_release_owner_id(&
					 (acpi_gbl_root_table_list.
					  tables[table_index].owner_id));
		status = AE_OK;
	}

	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
	return_ACPI_STATUS(status);
=======
 * FUNCTION:    acpi_tb_override_table
 *
 * PARAMETERS:  old_table_desc      - Validated table descriptor to be
 *                                    overridden
 *
 * RETURN:      None
 *
 * DESCRIPTION: Attempt table override by calling the OSL override functions.
 *              Note: If the table is overridden, then the entire new table
 *              is acquired and returned by this function.
 *              Before/after invocation, the table descriptor is in a state
 *              that is "VALIDATED".
 *
 ******************************************************************************/

void acpi_tb_override_table(struct acpi_table_desc *old_table_desc)
{
	acpi_status status;
	char *override_type;
	struct acpi_table_desc new_table_desc;
	struct acpi_table_header *table;
	acpi_physical_address address;
	u32 length;

	/* (1) Attempt logical override (returns a logical address) */

	status = acpi_os_table_override(old_table_desc->pointer, &table);
	if (ACPI_SUCCESS(status) && table) {
		acpi_tb_acquire_temp_table(&new_table_desc,
					   ACPI_PTR_TO_PHYSADDR(table),
					   ACPI_TABLE_ORIGIN_EXTERNAL_VIRTUAL);
		override_type = "Logical";
		goto finish_override;
	}

	/* (2) Attempt physical override (returns a physical address) */

	status = acpi_os_physical_table_override(old_table_desc->pointer,
						 &address, &length);
	if (ACPI_SUCCESS(status) && address && length) {
		acpi_tb_acquire_temp_table(&new_table_desc, address,
					   ACPI_TABLE_ORIGIN_INTERNAL_PHYSICAL);
		override_type = "Physical";
		goto finish_override;
	}

	return;			/* There was no override */

finish_override:

	/* Validate and verify a table before overriding */

	status = acpi_tb_verify_temp_table(&new_table_desc, NULL);
	if (ACPI_FAILURE(status)) {
		return;
	}

	ACPI_INFO(("%4.4s 0x%8.8X%8.8X"
		   " %s table override, new table: 0x%8.8X%8.8X",
		   old_table_desc->signature.ascii,
		   ACPI_FORMAT_UINT64(old_table_desc->address),
		   override_type, ACPI_FORMAT_UINT64(new_table_desc.address)));

	/* We can now uninstall the original table */

	acpi_tb_uninstall_table(old_table_desc);

	/*
	 * Replace the original table descriptor and keep its state as
	 * "VALIDATED".
	 */
	acpi_tb_init_table_descriptor(old_table_desc, new_table_desc.address,
				      new_table_desc.flags,
				      new_table_desc.pointer);
	acpi_tb_validate_temp_table(old_table_desc);

	/* Release the temporary table descriptor */

	acpi_tb_release_temp_table(&new_table_desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*******************************************************************************
 *
<<<<<<< HEAD
 * FUNCTION:    acpi_tb_get_owner_id
 *
 * PARAMETERS:  table_index         - Table index
 *              owner_id            - Where the table owner_id is returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: returns owner_id for the ACPI table
 *
 ******************************************************************************/

acpi_status acpi_tb_get_owner_id(u32 table_index, acpi_owner_id *owner_id)
{
	acpi_status status = AE_BAD_PARAMETER;

	ACPI_FUNCTION_TRACE(tb_get_owner_id);

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (table_index < acpi_gbl_root_table_list.current_table_count) {
		*owner_id =
		    acpi_gbl_root_table_list.tables[table_index].owner_id;
		status = AE_OK;
	}

	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_is_table_loaded
 *
 * PARAMETERS:  table_index         - Table index
 *
 * RETURN:      Table Loaded Flag
 *
 ******************************************************************************/

u8 acpi_tb_is_table_loaded(u32 table_index)
{
	u8 is_loaded = FALSE;

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (table_index < acpi_gbl_root_table_list.current_table_count) {
		is_loaded = (u8)
		    (acpi_gbl_root_table_list.tables[table_index].flags &
		     ACPI_TABLE_IS_LOADED);
	}

	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
	return (is_loaded);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_tb_set_table_loaded_flag
 *
 * PARAMETERS:  table_index         - Table index
 *              is_loaded           - TRUE if table is loaded, FALSE otherwise
 *
 * RETURN:      None
 *
 * DESCRIPTION: Sets the table loaded flag to either TRUE or FALSE.
 *
 ******************************************************************************/

void acpi_tb_set_table_loaded_flag(u32 table_index, u8 is_loaded)
{

	(void)acpi_ut_acquire_mutex(ACPI_MTX_TABLES);
	if (table_index < acpi_gbl_root_table_list.current_table_count) {
		if (is_loaded) {
			acpi_gbl_root_table_list.tables[table_index].flags |=
			    ACPI_TABLE_IS_LOADED;
		} else {
			acpi_gbl_root_table_list.tables[table_index].flags &=
			    ~ACPI_TABLE_IS_LOADED;
		}
	}

	(void)acpi_ut_release_mutex(ACPI_MTX_TABLES);
=======
 * FUNCTION:    acpi_tb_uninstall_table
 *
 * PARAMETERS:  table_desc          - Table descriptor
 *
 * RETURN:      None
 *
 * DESCRIPTION: Delete one internal ACPI table
 *
 ******************************************************************************/

void acpi_tb_uninstall_table(struct acpi_table_desc *table_desc)
{

	ACPI_FUNCTION_TRACE(tb_uninstall_table);

	/* Table must be installed */

	if (!table_desc->address) {
		return_VOID;
	}

	acpi_tb_invalidate_table(table_desc);

	if ((table_desc->flags & ACPI_TABLE_ORIGIN_MASK) ==
	    ACPI_TABLE_ORIGIN_INTERNAL_VIRTUAL) {
		ACPI_FREE(ACPI_PHYSADDR_TO_PTR(table_desc->address));
	}

	table_desc->address = ACPI_PTR_TO_PHYSADDR(NULL);
	return_VOID;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
