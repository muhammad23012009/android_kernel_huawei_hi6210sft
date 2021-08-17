
<<<<<<< HEAD
MEMSET_FN(__memset,
	"x86-64-unrolled",
	"unrolled memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(memset_c,
	"x86-64-stosq",
	"movsq-based memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(memset_c_e,
=======
MEMSET_FN(memset_orig,
	"x86-64-unrolled",
	"unrolled memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(__memset,
	"x86-64-stosq",
	"movsq-based memset() in arch/x86/lib/memset_64.S")

MEMSET_FN(memset_erms,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"x86-64-stosb",
	"movsb-based memset() in arch/x86/lib/memset_64.S")
