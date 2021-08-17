/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleloader.h>
#include <linux/kernel.h>
#include <linux/elf.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/unwind.h>

static inline void arc_write_me(unsigned short *addr, unsigned long value)
{
	*addr = (value & 0xffff0000) >> 16;
	*(addr + 1) = (value & 0xffff);
}

<<<<<<< HEAD
/* ARC specific section quirks - before relocation loop in generic loader
 *
 * For dwarf unwinding out of modules, this needs to
 * 1. Ensure the .debug_frame is allocatable (ARC Linker bug: despite
 *    -fasynchronous-unwind-tables it doesn't).
 * 2. Since we are iterating thru sec hdr tbl anyways, make a note of
 *    the exact section index, for later use.
=======
/*
 * This gets called before relocation loop in generic loader
 * Make a note of the section index of unwinding section
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
int module_frob_arch_sections(Elf_Ehdr *hdr, Elf_Shdr *sechdrs,
			      char *secstr, struct module *mod)
{
#ifdef CONFIG_ARC_DW2_UNWIND
<<<<<<< HEAD
	int i;

	mod->arch.unw_sec_idx = 0;
	mod->arch.unw_info = NULL;

	for (i = 1; i < hdr->e_shnum; i++) {
		if (strcmp(secstr+sechdrs[i].sh_name, ".debug_frame") == 0) {
			sechdrs[i].sh_flags |= SHF_ALLOC;
			mod->arch.unw_sec_idx = i;
			break;
		}
	}
#endif
=======
	mod->arch.unw_sec_idx = 0;
	mod->arch.unw_info = NULL;
#endif
	mod->arch.secstr = secstr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

void module_arch_cleanup(struct module *mod)
{
#ifdef CONFIG_ARC_DW2_UNWIND
	if (mod->arch.unw_info)
		unwind_remove_table(mod->arch.unw_info, 0);
#endif
}

int apply_relocate_add(Elf32_Shdr *sechdrs,
		       const char *strtab,
		       unsigned int symindex,	/* sec index for sym tbl */
		       unsigned int relsec,	/* sec index for relo sec */
		       struct module *module)
{
<<<<<<< HEAD
	int i, n;
	Elf32_Rela *rel_entry = (void *)sechdrs[relsec].sh_addr;
	Elf32_Sym *sym_entry, *sym_sec;
	Elf32_Addr relocation;
	Elf32_Addr location;
	Elf32_Addr sec_to_patch;
	int relo_type;

	sec_to_patch = sechdrs[sechdrs[relsec].sh_info].sh_addr;
	sym_sec = (Elf32_Sym *) sechdrs[symindex].sh_addr;
	n = sechdrs[relsec].sh_size / sizeof(*rel_entry);

	pr_debug("\n========== Module Sym reloc ===========================\n");
	pr_debug("Section to fixup %x\n", sec_to_patch);
	pr_debug("=========================================================\n");
	pr_debug("rela->r_off | rela->addend | sym->st_value | ADDR | VALUE\n");
=======
	int i, n, relo_type;
	Elf32_Rela *rel_entry = (void *)sechdrs[relsec].sh_addr;
	Elf32_Sym *sym_entry, *sym_sec;
	Elf32_Addr relocation, location, tgt_addr;
	unsigned int tgtsec;

	/*
	 * @relsec has relocations e.g. .rela.init.text
	 * @tgtsec is section to patch e.g. .init.text
	 */
	tgtsec = sechdrs[relsec].sh_info;
	tgt_addr = sechdrs[tgtsec].sh_addr;
	sym_sec = (Elf32_Sym *) sechdrs[symindex].sh_addr;
	n = sechdrs[relsec].sh_size / sizeof(*rel_entry);

	pr_debug("\nSection to fixup %s @%x\n",
		 module->arch.secstr + sechdrs[tgtsec].sh_name, tgt_addr);
	pr_debug("=========================================================\n");
	pr_debug("r_off\tr_add\tst_value ADDRESS  VALUE\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pr_debug("=========================================================\n");

	/* Loop thru entries in relocation section */
	for (i = 0; i < n; i++) {
<<<<<<< HEAD

		/* This is where to make the change */
		location = sec_to_patch + rel_entry[i].r_offset;
=======
		const char *s;

		/* This is where to make the change */
		location = tgt_addr + rel_entry[i].r_offset;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* This is the symbol it is referring to.  Note that all
		   undefined symbols have been resolved.  */
		sym_entry = sym_sec + ELF32_R_SYM(rel_entry[i].r_info);

		relocation = sym_entry->st_value + rel_entry[i].r_addend;

<<<<<<< HEAD
		pr_debug("\t%x\t\t%x\t\t%x  %x %x [%s]\n",
			rel_entry[i].r_offset, rel_entry[i].r_addend,
			sym_entry->st_value, location, relocation,
			strtab + sym_entry->st_name);
=======
		if (sym_entry->st_name == 0 && ELF_ST_TYPE (sym_entry->st_info) == STT_SECTION) {
			s = module->arch.secstr + sechdrs[sym_entry->st_shndx].sh_name;
		} else {
			s = strtab + sym_entry->st_name;
		}

		pr_debug("   %x\t%x\t%x %x %x [%s]\n",
			 rel_entry[i].r_offset, rel_entry[i].r_addend,
			 sym_entry->st_value, location, relocation, s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* This assumes modules are built with -mlong-calls
		 * so any branches/jumps are absolute 32 bit jmps
		 * global data access again is abs 32 bit.
		 * Both of these are handled by same relocation type
		 */
		relo_type = ELF32_R_TYPE(rel_entry[i].r_info);

<<<<<<< HEAD
		if (likely(R_ARC_32_ME == relo_type))
			arc_write_me((unsigned short *)location, relocation);
		else if (R_ARC_32 == relo_type)
			*((Elf32_Addr *) location) = relocation;
=======
		if (likely(R_ARC_32_ME == relo_type))	/* ME ( S + A ) */
			arc_write_me((unsigned short *)location, relocation);
		else if (R_ARC_32 == relo_type)		/* ( S + A ) */
			*((Elf32_Addr *) location) = relocation;
		else if (R_ARC_32_PCREL == relo_type)	/* ( S + A ) - PDATA ) */
			*((Elf32_Addr *) location) = relocation - location;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		else
			goto relo_err;

	}
<<<<<<< HEAD
=======

#ifdef CONFIG_ARC_DW2_UNWIND
	if (strcmp(module->arch.secstr+sechdrs[tgtsec].sh_name, ".eh_frame") == 0)
		module->arch.unw_sec_idx = tgtsec;
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;

relo_err:
	pr_err("%s: unknown relocation: %u\n",
		module->name, ELF32_R_TYPE(rel_entry[i].r_info));
	return -ENOEXEC;

}

/* Just before lift off: After sections have been relocated, we add the
 * dwarf section to unwinder table pool
 * This couldn't be done in module_frob_arch_sections() because
 * relocations had not been applied by then
 */
int module_finalize(const Elf32_Ehdr *hdr, const Elf_Shdr *sechdrs,
		    struct module *mod)
{
#ifdef CONFIG_ARC_DW2_UNWIND
	void *unw;
	int unwsec = mod->arch.unw_sec_idx;

	if (unwsec) {
		unw = unwind_add_table(mod, (void *)sechdrs[unwsec].sh_addr,
				       sechdrs[unwsec].sh_size);
		mod->arch.unw_info = unw;
	}
#endif
	return 0;
}
