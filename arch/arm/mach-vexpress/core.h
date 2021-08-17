<<<<<<< HEAD
/* 2MB large area for motherboard's peripherals static mapping */
#define V2M_PERIPH 0xf8000000

/* Tile's peripherals static mappings should start here */
#define V2T_PERIPH 0xf8200000

void vexpress_dt_smp_map_io(void);

bool vexpress_smp_init_ops(void);

extern struct smp_operations	vexpress_smp_ops;
=======
bool vexpress_smp_init_ops(void);

extern const struct smp_operations vexpress_smp_dt_ops;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void vexpress_cpu_die(unsigned int cpu);
