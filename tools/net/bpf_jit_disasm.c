/*
 * Minimal BPF JIT image disassembler
 *
 * Disassembles BPF JIT compiler emitted opcodes back to asm insn's for
 * debugging or verification purposes.
 *
 * To get the disassembly of the JIT code, do the following:
 *
 *  1) `echo 2 > /proc/sys/net/core/bpf_jit_enable`
 *  2) Load a BPF filter (e.g. `tcpdump -p -n -s 0 -i eth1 host 192.168.20.0/24`)
 *  3) Run e.g. `bpf_jit_disasm -o` to read out the last JIT code
 *
 * Copyright 2013 Daniel Borkmann <borkmann@redhat.com>
 * Licensed under the GNU General Public License, version 2.0 (GPLv2)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <bfd.h>
#include <dis-asm.h>
<<<<<<< HEAD
#include <sys/klog.h>
#include <sys/types.h>
#include <regex.h>
=======
#include <regex.h>
#include <fcntl.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CMD_ACTION_SIZE_BUFFER		10
#define CMD_ACTION_READ_ALL		3
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void get_exec_path(char *tpath, size_t size)
{
	char *path;
	ssize_t len;

	snprintf(tpath, size, "/proc/%d/exe", (int) getpid());
	tpath[size - 1] = 0;

	path = strdup(tpath);
	assert(path);

	len = readlink(path, tpath, size);
	tpath[len] = 0;

	free(path);
}

<<<<<<< HEAD
static void get_asm_insns(uint8_t *image, size_t len, unsigned long base,
			  int opcodes)
=======
static void get_asm_insns(uint8_t *image, size_t len, int opcodes)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int count, i, pc = 0;
	char tpath[256];
	struct disassemble_info info;
	disassembler_ftype disassemble;
	bfd *bfdf;

	memset(tpath, 0, sizeof(tpath));
	get_exec_path(tpath, sizeof(tpath));

	bfdf = bfd_openr(tpath, NULL);
	assert(bfdf);
	assert(bfd_check_format(bfdf, bfd_object));

	init_disassemble_info(&info, stdout, (fprintf_ftype) fprintf);
	info.arch = bfd_get_arch(bfdf);
	info.mach = bfd_get_mach(bfdf);
	info.buffer = image;
	info.buffer_length = len;

	disassemble_init_for_target(&info);

	disassemble = disassembler(bfdf);
	assert(disassemble);

	do {
		printf("%4x:\t", pc);

		count = disassemble(pc, &info);

		if (opcodes) {
			printf("\n\t");
			for (i = 0; i < count; ++i)
				printf("%02x ", (uint8_t) image[pc + i]);
		}
		printf("\n");

		pc += count;
	} while(count > 0 && pc < len);

	bfd_close(bfdf);
}

<<<<<<< HEAD
static char *get_klog_buff(int *klen)
{
	int ret, len = klogctl(10, NULL, 0);
	char *buff = malloc(len);

	assert(buff && klen);
	ret = klogctl(3, buff, len);
	assert(ret >= 0);
	*klen = ret;

	return buff;
}

static void put_klog_buff(char *buff)
=======
static char *get_klog_buff(unsigned int *klen)
{
	int ret, len;
	char *buff;

	len = klogctl(CMD_ACTION_SIZE_BUFFER, NULL, 0);
	if (len < 0)
		return NULL;

	buff = malloc(len);
	if (!buff)
		return NULL;

	ret = klogctl(CMD_ACTION_READ_ALL, buff, len);
	if (ret < 0) {
		free(buff);
		return NULL;
	}

	*klen = ret;
	return buff;
}

static char *get_flog_buff(const char *file, unsigned int *klen)
{
	int fd, ret, len;
	struct stat fi;
	char *buff;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return NULL;

	ret = fstat(fd, &fi);
	if (ret < 0 || !S_ISREG(fi.st_mode))
		goto out;

	len = fi.st_size + 1;
	buff = malloc(len);
	if (!buff)
		goto out;

	memset(buff, 0, len);
	ret = read(fd, buff, len - 1);
	if (ret <= 0)
		goto out_free;

	close(fd);
	*klen = ret;
	return buff;
out_free:
	free(buff);
out:
	close(fd);
	return NULL;
}

static char *get_log_buff(const char *file, unsigned int *klen)
{
	return file ? get_flog_buff(file, klen) : get_klog_buff(klen);
}

static void put_log_buff(char *buff)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	free(buff);
}

<<<<<<< HEAD
static int get_last_jit_image(char *haystack, size_t hlen,
			      uint8_t *image, size_t ilen,
			      unsigned long *base)
=======
static unsigned int get_last_jit_image(char *haystack, size_t hlen,
				       uint8_t *image, size_t ilen)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	char *ptr, *pptr, *tmp;
	off_t off = 0;
	int ret, flen, proglen, pass, ulen = 0;
	regmatch_t pmatch[1];
<<<<<<< HEAD
=======
	unsigned long base;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	regex_t regex;

	if (hlen == 0)
		return 0;

	ret = regcomp(&regex, "flen=[[:alnum:]]+ proglen=[[:digit:]]+ "
		      "pass=[[:digit:]]+ image=[[:xdigit:]]+", REG_EXTENDED);
	assert(ret == 0);

	ptr = haystack;
<<<<<<< HEAD
=======
	memset(pmatch, 0, sizeof(pmatch));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	while (1) {
		ret = regexec(&regex, ptr, 1, pmatch, 0);
		if (ret == 0) {
			ptr += pmatch[0].rm_eo;
			off += pmatch[0].rm_eo;
			assert(off < hlen);
		} else
			break;
	}

	ptr = haystack + off - (pmatch[0].rm_eo - pmatch[0].rm_so);
	ret = sscanf(ptr, "flen=%d proglen=%d pass=%d image=%lx",
<<<<<<< HEAD
		     &flen, &proglen, &pass, base);
	if (ret != 4)
		return 0;
=======
		     &flen, &proglen, &pass, &base);
	if (ret != 4) {
		regfree(&regex);
		return 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tmp = ptr = haystack + off;
	while ((ptr = strtok(tmp, "\n")) != NULL && ulen < ilen) {
		tmp = NULL;
		if (!strstr(ptr, "JIT code"))
			continue;
		pptr = ptr;
		while ((ptr = strstr(pptr, ":")))
			pptr = ptr + 1;
		ptr = pptr;
		do {
			image[ulen++] = (uint8_t) strtoul(pptr, &pptr, 16);
			if (ptr == pptr || ulen >= ilen) {
				ulen--;
				break;
			}
			ptr = pptr;
		} while (1);
	}

	assert(ulen == proglen);
	printf("%d bytes emitted from JIT compiler (pass:%d, flen:%d)\n",
	       proglen, pass, flen);
<<<<<<< HEAD
	printf("%lx + <x>:\n", *base);
=======
	printf("%lx + <x>:\n", base);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	regfree(&regex);
	return ulen;
}

<<<<<<< HEAD
int main(int argc, char **argv)
{
	int len, klen, opcodes = 0;
	char *kbuff;
	unsigned long base;
	uint8_t image[4096];

	if (argc > 1) {
		if (!strncmp("-o", argv[argc - 1], 2)) {
			opcodes = 1;
		} else {
			printf("usage: bpf_jit_disasm [-o: show opcodes]\n");
			exit(0);
=======
static void usage(void)
{
	printf("Usage: bpf_jit_disasm [...]\n");
	printf("       -o          Also display related opcodes (default: off).\n");
	printf("       -f <file>   Read last image dump from file or stdin (default: klog).\n");
	printf("       -h          Display this help.\n");
}

int main(int argc, char **argv)
{
	unsigned int len, klen, opt, opcodes = 0;
	static uint8_t image[32768];
	char *kbuff, *file = NULL;

	while ((opt = getopt(argc, argv, "of:")) != -1) {
		switch (opt) {
		case 'o':
			opcodes = 1;
			break;
		case 'f':
			file = optarg;
			break;
		default:
			usage();
			return -1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	bfd_init();
	memset(image, 0, sizeof(image));

<<<<<<< HEAD
	kbuff = get_klog_buff(&klen);

	len = get_last_jit_image(kbuff, klen, image, sizeof(image), &base);
	if (len > 0 && base > 0)
		get_asm_insns(image, len, base, opcodes);

	put_klog_buff(kbuff);

=======
	kbuff = get_log_buff(file, &klen);
	if (!kbuff) {
		fprintf(stderr, "Could not retrieve log buffer!\n");
		return -1;
	}

	len = get_last_jit_image(kbuff, klen, image, sizeof(image));
	if (len > 0)
		get_asm_insns(image, len, opcodes);
	else
		fprintf(stderr, "No JIT image found!\n");

	put_log_buff(kbuff);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
