/// sizeof when applied to a pointer typed expression gives the size of
/// the pointer
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
virtual patch

@depends on patch@
expression *x;
expression f;
<<<<<<< HEAD
=======
expression i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
type T;
@@

(
x = <+... sizeof(
- x
+ *x
   ) ...+>
|
f(...,(T)(x),...,sizeof(
- x
+ *x
   ),...)
|
<<<<<<< HEAD
f(...,sizeof(x),...,(T)(
- x
+ *x
   ),...)
=======
f(...,sizeof(
- x
+ *x
   ),...,(T)(x),...)
|
f(...,(T)(x),...,i*sizeof(
- x
+ *x
   ),...)
|
f(...,i*sizeof(
- x
+ *x
   ),...,(T)(x),...)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
)

@r depends on !patch@
expression *x;
expression f;
<<<<<<< HEAD
=======
expression i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
position p;
type T;
@@

(
*x = <+... sizeof@p(x) ...+>
|
*f(...,(T)(x),...,sizeof@p(x),...)
|
*f(...,sizeof@p(x),...,(T)(x),...)
<<<<<<< HEAD
=======
|
*f(...,(T)(x),...,i*sizeof@p(x),...)
|
*f(...,i*sizeof@p(x),...,(T)(x),...)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
)

@script:python depends on org@
p << r.p;
@@

cocci.print_main("application of sizeof to pointer",p)

@script:python depends on report@
p << r.p;
@@

msg = "ERROR: application of sizeof to pointer"
coccilib.report.print_report(p[0],msg)
