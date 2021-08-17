#ifndef ACPI_PNP_H
#define ACPI_PNP_H

<<<<<<< HEAD
#include <acpi/acpi_bus.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/acpi.h>
#include <linux/pnp.h>

int pnpacpi_parse_allocated_resource(struct pnp_dev *);
int pnpacpi_parse_resource_option_data(struct pnp_dev *);
int pnpacpi_encode_resources(struct pnp_dev *, struct acpi_buffer *);
int pnpacpi_build_resource_template(struct pnp_dev *, struct acpi_buffer *);
#endif
