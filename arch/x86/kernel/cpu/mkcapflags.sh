#!/bin/sh
#
<<<<<<< HEAD
# Generate the x86_cap_flags[] array from include/asm/cpufeature.h
#

IN=$1
OUT=$2

TABS="$(printf '\t\t\t\t\t')"
trap 'rm "$OUT"' EXIT

(
	echo "#ifndef _ASM_X86_CPUFEATURE_H"
	echo "#include <asm/cpufeature.h>"
	echo "#endif"
	echo ""
	echo "const char * const x86_cap_flags[NCAPINTS*32] = {"

	# Iterate through any input lines starting with #define X86_FEATURE_
	sed -n -e 's/\t/ /g' -e 's/^ *# *define *X86_FEATURE_//p' $IN |
=======
# Generate the x86_cap/bug_flags[] arrays from include/asm/cpufeatures.h
#

set -e

IN=$1
OUT=$2

dump_array()
{
	ARRAY=$1
	SIZE=$2
	PFX=$3
	POSTFIX=$4

	PFX_SZ=$(echo $PFX | wc -c)
	TABS="$(printf '\t\t\t\t\t')"

	echo "const char * const $ARRAY[$SIZE] = {"

	# Iterate through any input lines starting with #define $PFX
	sed -n -e 's/\t/ /g' -e "s/^ *# *define *$PFX//p" $IN |
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	while read i
	do
		# Name is everything up to the first whitespace
		NAME="$(echo "$i" | sed 's/ .*//')"

		# If the /* comment */ starts with a quote string, grab that.
		VALUE="$(echo "$i" | sed -n 's@.*/\* *\("[^"]*"\).*\*/@\1@p')"
		[ -z "$VALUE" ] && VALUE="\"$NAME\""
<<<<<<< HEAD
		[ "$VALUE" == '""' ] && continue
=======
		[ "$VALUE" = '""' ] && continue
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		# Name is uppercase, VALUE is all lowercase
		VALUE="$(echo "$VALUE" | tr A-Z a-z)"

<<<<<<< HEAD
		TABCOUNT=$(( ( 5*8 - 14 - $(echo "$NAME" | wc -c) ) / 8 ))
		printf "\t[%s]%.*s = %s,\n" \
			"X86_FEATURE_$NAME" "$TABCOUNT" "$TABS" "$VALUE"
	done
	echo "};"
=======
        if [ -n "$POSTFIX" ]; then
            T=$(( $PFX_SZ + $(echo $POSTFIX | wc -c) + 2 ))
	        TABS="$(printf '\t\t\t\t\t\t')"
		    TABCOUNT=$(( ( 6*8 - ($T + 1) - $(echo "$NAME" | wc -c) ) / 8 ))
		    printf "\t[%s - %s]%.*s = %s,\n" "$PFX$NAME" "$POSTFIX" "$TABCOUNT" "$TABS" "$VALUE"
        else
		    TABCOUNT=$(( ( 5*8 - ($PFX_SZ + 1) - $(echo "$NAME" | wc -c) ) / 8 ))
            printf "\t[%s]%.*s = %s,\n" "$PFX$NAME" "$TABCOUNT" "$TABS" "$VALUE"
        fi
	done
	echo "};"
}

trap 'rm "$OUT"' EXIT

(
	echo "#ifndef _ASM_X86_CPUFEATURES_H"
	echo "#include <asm/cpufeatures.h>"
	echo "#endif"
	echo ""

	dump_array "x86_cap_flags" "NCAPINTS*32" "X86_FEATURE_" ""
	echo ""

	dump_array "x86_bug_flags" "NBUGINTS*32" "X86_BUG_" "NCAPINTS*32"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
) > $OUT

trap - EXIT
