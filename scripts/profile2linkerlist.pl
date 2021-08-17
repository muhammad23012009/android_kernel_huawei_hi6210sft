<<<<<<< HEAD
#!/usr/bin/perl
=======
#!/usr/bin/env perl
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#
# Takes a (sorted) output of readprofile and turns it into a list suitable for
# linker scripts
#
# usage:
#	 readprofile | sort -rn | perl profile2linkerlist.pl > functionlist
#
use strict;

while (<>) {
  my $line = $_;

  $_ =~ /\W*[0-9]+\W*([a-zA-Z\_0-9]+)\W*[0-9]+/;

  print "*(.text.$1)\n"
      unless ($line =~ /unknown/) || ($line =~ /total/);
}
