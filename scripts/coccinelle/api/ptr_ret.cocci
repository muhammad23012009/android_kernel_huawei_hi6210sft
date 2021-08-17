///
<<<<<<< HEAD
/// Use PTR_RET rather than if(IS_ERR(...)) + PTR_ERR
=======
/// Use PTR_ERR_OR_ZERO rather than if(IS_ERR(...)) + PTR_ERR
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
///
// Confidence: High
// Copyright: (C) 2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2012 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
<<<<<<< HEAD
// Options: -no_includes -include_headers
//
// Keywords: ERR_PTR, PTR_ERR, PTR_RET
=======
// Options: --no-includes --include-headers
//
// Keywords: ERR_PTR, PTR_ERR, PTR_ERR_OR_ZERO
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
// Version min: 2.6.39
//

virtual context
virtual patch
virtual org
virtual report

@depends on patch@
expression ptr;
@@

- if (IS_ERR(ptr)) return PTR_ERR(ptr); else return 0;
<<<<<<< HEAD
+ return PTR_RET(ptr);
=======
+ return PTR_ERR_OR_ZERO(ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@depends on patch@
expression ptr;
@@

- if (IS_ERR(ptr)) return PTR_ERR(ptr); return 0;
<<<<<<< HEAD
+ return PTR_RET(ptr);
=======
+ return PTR_ERR_OR_ZERO(ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@depends on patch@
expression ptr;
@@

- (IS_ERR(ptr) ? PTR_ERR(ptr) : 0)
<<<<<<< HEAD
+ PTR_RET(ptr)
=======
+ PTR_ERR_OR_ZERO(ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@r1 depends on !patch@
expression ptr;
position p1;
@@

* if@p1 (IS_ERR(ptr)) return PTR_ERR(ptr); else return 0;

@r2 depends on !patch@
expression ptr;
position p2;
@@

* if@p2 (IS_ERR(ptr)) return PTR_ERR(ptr); return 0;

@r3 depends on !patch@
expression ptr;
position p3;
@@

* IS_ERR@p3(ptr) ? PTR_ERR(ptr) : 0

@script:python depends on org@
p << r1.p1;
@@

<<<<<<< HEAD
coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.org.print_todo(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414


@script:python depends on org@
p << r2.p2;
@@

<<<<<<< HEAD
coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.org.print_todo(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@script:python depends on org@
p << r3.p3;
@@

<<<<<<< HEAD
coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.org.print_todo(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@script:python depends on report@
p << r1.p1;
@@

<<<<<<< HEAD
coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.report.print_report(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@script:python depends on report@
p << r2.p2;
@@

<<<<<<< HEAD
coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.report.print_report(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@script:python depends on report@
p << r3.p3;
@@

<<<<<<< HEAD
coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")
=======
coccilib.report.print_report(p[0], "WARNING: PTR_ERR_OR_ZERO can be used")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
