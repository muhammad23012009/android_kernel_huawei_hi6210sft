<<<<<<< HEAD
#define EXPORT_SYMBOL(sym)
#define EXPORT_SYMBOL_GPL(sym)
#define EXPORT_SYMBOL_GPL_FUTURE(sym)
#define EXPORT_UNUSED_SYMBOL(sym)
#define EXPORT_UNUSED_SYMBOL_GPL(sym)
=======
#define EXPORT_SYMBOL_GPL(sym) extern typeof(sym) sym
#define EXPORT_SYMBOL(sym) extern typeof(sym) sym

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
