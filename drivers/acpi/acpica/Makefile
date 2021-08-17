#
# Makefile for ACPICA Core interpreter
#

<<<<<<< HEAD
ccflags-y			:= -Os
=======
ccflags-y			:= -Os -DBUILDING_ACPICA
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
ccflags-$(CONFIG_ACPI_DEBUG)	+= -DACPI_DEBUG_OUTPUT

# use acpi.o to put all files here into acpi.o modparam namespace
obj-y	+= acpi.o

acpi-y :=		\
	dsargs.o	\
	dscontrol.o	\
<<<<<<< HEAD
=======
	dsdebug.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dsfield.o	\
	dsinit.o	\
	dsmethod.o	\
	dsmthdat.o	\
	dsobject.o	\
	dsopcode.o	\
	dsutils.o	\
	dswexec.o	\
	dswload.o	\
	dswload2.o	\
	dswscope.o	\
	dswstate.o

acpi-y +=		\
	evevent.o	\
	evgpe.o		\
	evgpeblk.o	\
	evgpeinit.o	\
	evgpeutil.o	\
	evglock.o	\
	evhandler.o	\
	evmisc.o	\
	evregion.o	\
	evrgnini.o	\
	evsci.o		\
	evxface.o	\
	evxfevnt.o	\
	evxfgpe.o	\
	evxfregn.o

acpi-y +=		\
<<<<<<< HEAD
=======
	exconcat.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	exconfig.o	\
	exconvrt.o	\
	excreate.o	\
	exdebug.o	\
	exdump.o	\
	exfield.o	\
	exfldio.o	\
<<<<<<< HEAD
=======
	exmisc.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	exmutex.o	\
	exnames.o	\
	exoparg1.o	\
	exoparg2.o	\
	exoparg3.o	\
	exoparg6.o	\
	exprep.o	\
<<<<<<< HEAD
	exmisc.o	\
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	exregion.o	\
	exresnte.o	\
	exresolv.o	\
	exresop.o	\
	exstore.o	\
	exstoren.o	\
	exstorob.o	\
	exsystem.o	\
<<<<<<< HEAD
=======
	extrace.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	exutils.o

acpi-y +=		\
	hwacpi.o	\
	hwesleep.o	\
	hwgpe.o		\
	hwpci.o		\
	hwregs.o	\
	hwsleep.o	\
	hwvalid.o	\
	hwxface.o	\
	hwxfsleep.o

acpi-$(ACPI_FUTURE_USAGE) += hwtimer.o

acpi-y +=		\
	nsaccess.o	\
	nsalloc.o	\
<<<<<<< HEAD
=======
	nsarguments.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	nsconvert.o	\
	nsdump.o	\
	nseval.o	\
	nsinit.o	\
	nsload.o	\
	nsnames.o	\
	nsobject.o	\
	nsparse.o	\
	nspredef.o	\
	nsprepkg.o	\
	nsrepair.o	\
	nsrepair2.o	\
	nssearch.o	\
	nsutils.o	\
	nswalk.o	\
	nsxfeval.o	\
	nsxfname.o	\
	nsxfobj.o

acpi-$(ACPI_FUTURE_USAGE) += nsdumpdv.o

acpi-y +=		\
	psargs.o	\
	psloop.o	\
	psobject.o	\
	psopcode.o	\
	psopinfo.o	\
	psparse.o	\
	psscope.o	\
	pstree.o	\
	psutils.o	\
	pswalk.o	\
	psxface.o

acpi-y +=		\
	rsaddr.o	\
	rscalc.o	\
	rscreate.o	\
<<<<<<< HEAD
=======
	rsdumpinfo.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rsinfo.o	\
	rsio.o		\
	rsirq.o		\
	rslist.o	\
	rsmemory.o	\
	rsmisc.o	\
	rsserial.o	\
	rsutils.o	\
	rsxface.o

<<<<<<< HEAD
acpi-$(ACPI_FUTURE_USAGE) += rsdump.o rsdumpinfo.o

acpi-y +=		\
	tbfadt.o	\
	tbfind.o	\
	tbinstal.o	\
=======
acpi-y +=		\
	tbdata.o	\
	tbfadt.o	\
	tbfind.o	\
	tbinstal.o	\
	tbprint.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	tbutils.o	\
	tbxface.o	\
	tbxfload.o	\
	tbxfroot.o

acpi-y +=		\
	utaddress.o	\
	utalloc.o	\
<<<<<<< HEAD
=======
	utascii.o	\
	utbuffer.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	utcopy.o	\
	utexcep.o	\
	utdebug.o	\
	utdecode.o	\
	utdelete.o	\
<<<<<<< HEAD
	uteval.o	\
	utglobal.o	\
=======
	uterror.o	\
	uteval.o	\
	utglobal.o	\
	uthex.o		\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	utids.o		\
	utinit.o	\
	utlock.o	\
	utmath.o	\
	utmisc.o	\
	utmutex.o	\
<<<<<<< HEAD
=======
	utnonansi.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	utobject.o	\
	utosi.o		\
	utownerid.o	\
	utpredef.o	\
	utresrc.o	\
	utstate.o	\
	utstring.o	\
<<<<<<< HEAD
=======
	utstrtoul64.o	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	utxface.o	\
	utxfinit.o	\
	utxferror.o	\
	utxfmutex.o

<<<<<<< HEAD
acpi-$(ACPI_FUTURE_USAGE) += uttrack.o utcache.o
=======
acpi-$(CONFIG_ACPI_DEBUGGER) +=	\
	dbcmds.o		\
	dbconvert.o		\
	dbdisply.o		\
	dbexec.o		\
	dbhistry.o		\
	dbinput.o		\
	dbmethod.o		\
	dbnames.o		\
	dbobject.o		\
	dbstats.o		\
	dbutils.o		\
	dbxface.o		\
	rsdump.o		\

acpi-$(ACPI_FUTURE_USAGE) +=	\
	dbfileio.o		\
	dbtest.o		\
	utcache.o		\
	utprint.o		\
	uttrack.o		\
	utuuid.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

