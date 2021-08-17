/*
 * Copyright 2012 Intel Corporation
 * Author: Josh Triplett <josh@joshtriplett.org>
 *
 * Based on the bgrt driver:
 * Copyright 2012 Red Hat, Inc <mjg@redhat.com>
 * Author: Matthew Garrett
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
=======

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/efi.h>
#include <linux/efi-bgrt.h>

struct acpi_table_bgrt *bgrt_tab;
void *__initdata bgrt_image;
size_t __initdata bgrt_image_size;

struct bmp_header {
	u16 id;
	u32 size;
} __packed;

void __init efi_bgrt_init(void)
{
	acpi_status status;
<<<<<<< HEAD
	void __iomem *image;
	bool ioremapped = false;
=======
	void *image;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct bmp_header bmp_header;

	if (acpi_disabled)
		return;

	status = acpi_get_table("BGRT", 0,
	                        (struct acpi_table_header **)&bgrt_tab);
	if (ACPI_FAILURE(status))
		return;

<<<<<<< HEAD
	if (bgrt_tab->header.length < sizeof(*bgrt_tab))
		return;
	if (bgrt_tab->version != 1)
		return;
	if (bgrt_tab->image_type != 0 || !bgrt_tab->image_address)
		return;

	image = efi_lookup_mapped_addr(bgrt_tab->image_address);
	if (!image) {
		image = ioremap(bgrt_tab->image_address, sizeof(bmp_header));
		ioremapped = true;
		if (!image)
			return;
	}

	memcpy_fromio(&bmp_header, image, sizeof(bmp_header));
	if (ioremapped)
		iounmap(image);
	bgrt_image_size = bmp_header.size;

	bgrt_image = kmalloc(bgrt_image_size, GFP_KERNEL);
	if (!bgrt_image)
		return;

	if (ioremapped) {
		image = ioremap(bgrt_tab->image_address, bmp_header.size);
		if (!image) {
			kfree(bgrt_image);
			bgrt_image = NULL;
			return;
		}
	}

	memcpy_fromio(bgrt_image, image, bgrt_image_size);
	if (ioremapped)
		iounmap(image);
=======
	if (bgrt_tab->header.length < sizeof(*bgrt_tab)) {
		pr_notice("Ignoring BGRT: invalid length %u (expected %zu)\n",
		       bgrt_tab->header.length, sizeof(*bgrt_tab));
		return;
	}
	if (bgrt_tab->version != 1) {
		pr_notice("Ignoring BGRT: invalid version %u (expected 1)\n",
		       bgrt_tab->version);
		return;
	}
	if (bgrt_tab->status & 0xfe) {
		pr_notice("Ignoring BGRT: reserved status bits are non-zero %u\n",
		       bgrt_tab->status);
		return;
	}
	if (bgrt_tab->image_type != 0) {
		pr_notice("Ignoring BGRT: invalid image type %u (expected 0)\n",
		       bgrt_tab->image_type);
		return;
	}
	if (!bgrt_tab->image_address) {
		pr_notice("Ignoring BGRT: null image address\n");
		return;
	}

	image = memremap(bgrt_tab->image_address, sizeof(bmp_header), MEMREMAP_WB);
	if (!image) {
		pr_notice("Ignoring BGRT: failed to map image header memory\n");
		return;
	}

	memcpy(&bmp_header, image, sizeof(bmp_header));
	memunmap(image);
	if (bmp_header.id != 0x4d42) {
		pr_notice("Ignoring BGRT: Incorrect BMP magic number 0x%x (expected 0x4d42)\n",
			bmp_header.id);
		return;
	}
	bgrt_image_size = bmp_header.size;

	bgrt_image = memremap(bgrt_tab->image_address, bmp_header.size, MEMREMAP_WB);
	if (!bgrt_image) {
		pr_notice("Ignoring BGRT: failed to map image memory\n");
		bgrt_image = NULL;
		return;
	}

	efi_mem_reserve(bgrt_tab->image_address, bgrt_image_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
