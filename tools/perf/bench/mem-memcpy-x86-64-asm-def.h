
<<<<<<< HEAD
MEMCPY_FN(__memcpy,
	"x86-64-unrolled",
	"unrolled memcpy() in arch/x86/lib/memcpy_64.S")

MEMCPY_FN(memcpy_c,
	"x86-64-movsq",
	"movsq-based memcpy() in arch/x86/lib/memcpy_64.S")

MEMCPY_FN(memcpy_c_e,
=======
MEMCPY_FN(memcpy_orig,
	"x86-64-unrolled",
	"unrolled memcpy() in arch/x86/lib/memcpy_64.S")

MEMCPY_FN(__memcpy,
	"x86-64-movsq",
	"movsq-based memcpy() in arch/x86/lib/memcpy_64.S")

MEMCPY_FN(memcpy_erms,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"x86-64-movsb",
	"movsb-based memcpy() in arch/x86/lib/memcpy_64.S")
