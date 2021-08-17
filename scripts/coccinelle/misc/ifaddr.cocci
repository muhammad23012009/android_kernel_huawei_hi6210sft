<<<<<<< HEAD
/// the address of a variable or field is non-zero is likely always to bo
/// non-zero
=======
/// The address of a variable or field is likely always to be non-zero.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
///
// Confidence: High
// Copyright: (C) 2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2012 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
// Comments:
<<<<<<< HEAD
// Options: -no_includes -include_headers
=======
// Options: --no-includes --include-headers
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

virtual org
virtual report
virtual context

@r@
expression x;
statement S1,S2;
position p;
@@

*if@p (&x)
 S1 else S2

@script:python depends on org@
p << r.p;
@@

cocci.print_main("test of a variable/field address",p)

@script:python depends on report@
p << r.p;
@@

msg = "ERROR: test of a variable/field address"
coccilib.report.print_report(p[0],msg)
