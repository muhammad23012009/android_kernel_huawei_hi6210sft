<<<<<<< HEAD
/// Make sure threaded IRQs without a primary handler are always request with
/// IRQF_ONESHOT
=======
/// Since commit 1c6c69525b40 ("genirq: Reject bogus threaded irq requests")
/// threaded IRQs without a primary handler need to be requested with
/// IRQF_ONESHOT, otherwise the request will fail.
///
/// So pass the IRQF_ONESHOT flag in this case.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
///
//
// Confidence: Good
// Comments:
// Options: --no-includes

virtual patch
virtual context
virtual org
virtual report

@r1@
<<<<<<< HEAD
=======
expression dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
expression irq;
expression thread_fn;
expression flags;
position p;
@@
<<<<<<< HEAD
=======
(
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
request_threaded_irq@p(irq, NULL, thread_fn,
(
flags | IRQF_ONESHOT
|
IRQF_ONESHOT
)
, ...)
<<<<<<< HEAD

@depends on patch@
=======
|
devm_request_threaded_irq@p(dev, irq, NULL, thread_fn,
(
flags | IRQF_ONESHOT
|
IRQF_ONESHOT
)
, ...)
)

@depends on patch@
expression dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
expression irq;
expression thread_fn;
expression flags;
position p != r1.p;
@@
<<<<<<< HEAD
=======
(
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
request_threaded_irq@p(irq, NULL, thread_fn,
(
-0
+IRQF_ONESHOT
|
-flags
+flags | IRQF_ONESHOT
)
, ...)
<<<<<<< HEAD
=======
|
devm_request_threaded_irq@p(dev, irq, NULL, thread_fn,
(
-0
+IRQF_ONESHOT
|
-flags
+flags | IRQF_ONESHOT
)
, ...)
)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

@depends on context@
position p != r1.p;
@@
*request_threaded_irq@p(...)

@match depends on report || org@
expression irq;
position p != r1.p;
@@
request_threaded_irq@p(irq, NULL, ...)

@script:python depends on org@
p << match.p;
@@
msg = "ERROR: Threaded IRQ with no primary handler requested without IRQF_ONESHOT"
coccilib.org.print_todo(p[0],msg)

@script:python depends on report@
p << match.p;
@@
msg = "ERROR: Threaded IRQ with no primary handler requested without IRQF_ONESHOT"
coccilib.report.print_report(p[0],msg)
