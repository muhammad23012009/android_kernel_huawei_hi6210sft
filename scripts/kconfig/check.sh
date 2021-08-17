#!/bin/sh
# Needed for systems without gettext
$* -x c -o /dev/null - > /dev/null 2>&1 << EOF
#include <libintl.h>
int main()
{
	gettext("");
	return 0;
}
EOF
if [ ! "$?" -eq "0"  ]; then
	echo -DKBUILD_NO_NLS;
fi
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
