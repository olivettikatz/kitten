include config.mk

MAKE=make
MAKEFLAGS=--no-print-directory

build : 
	$(INSTALL) $(INSTALLFLAGS) config.mk ../krd
	@$(MAKE) $(MAKEFLAGS) -C ../krd build
	$(INSTALL) $(INSTALLFLAGS) ../krd/libkrd.a .
	$(MKDIR) $(MKDIRFLAGS) krd/kernel
	$(INSTALL) $(INSTALLFLAGS) ../krd/kernel/backend.h krd/kernel/backend.h

	$(INSTALL) $(INSTALLFLAGS) config.mk ../tse
	@$(MAKE) $(MAKEFLAGS) -C ../tse build
	$(INSTALL) $(INSTALLFLAGS) ../tse/libtse.a .
	$(MKDIR) $(MKDIRFLAGS) tse/core
	$(MKDIR) $(MKDIRFLAGS) tse/os/linux
	$(INSTALL) $(INSTALLFLAGS) ../tse/tse.h tse
	$(INSTALL) $(INSTALLFLAGS) ../tse/core/async.h tse/core
	$(INSTALL) $(INSTALLFLAGS) ../tse/core/region.h tse/core
	$(INSTALL) $(INSTALLFLAGS) ../tse/core/stream.h tse/core
	$(INSTALL) $(INSTALLFLAGS) ../tse/os/os.h tse/os
	$(INSTALL) $(INSTALLFLAGS) ../tse/os/linux/linux.h tse/os/linux

	$(INSTALL) $(INSTALLFLAGS) config.mk ../ktp
	@$(MAKE) $(MAKEFLAGS) -C ../ktp build
	$(INSTALL) $(INSTALLFLAGS) ../ktp/libktp.a .
	$(MKDIR) $(MKDIRFLAGS) ktp/lexical
	$(MKDIR) $(MKDIRFLAGS) ktp/semantic
	$(INSTALL) $(INSTALLFLAGS) ../ktp/ktp.h ktp
	$(INSTALL) $(INSTALLFLAGS) ../ktp/lexical/pattern.h ktp/lexical
	$(INSTALL) $(INSTALLFLAGS) ../ktp/lexical/tokenizer.h ktp/lexical
	$(INSTALL) $(INSTALLFLAGS) ../ktp/semantic/ast.h ktp/semantic
	$(INSTALL) $(INSTALLFLAGS) ../ktp/semantic/error.h ktp/semantic
	$(INSTALL) $(INSTALLFLAGS) ../ktp/semantic/parser.h ktp/semantic

	$(INSTALL) $(INSTALLFLAGS) config.mk ../mitten/nmctaurus
	@$(MAKE) $(MAKEFLAGS) -C ../mitten/nmctaurus build
	
	$(INSTALL) $(INSTALLFLAGS) config.mk ../moph
	@$(MAKE) $(MAKEFLAGS) -C ../moph
	$(INSTALL) $(INSTALLFLAGS) ../moph/moph .

	$(INSTALL) $(INSTALLFLAGS) config.mk ../nightowl
	@$(MAKE) $(MAKEFLAGS) -C ../nightowl
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/libnightowl.a .
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/kernel/libnightowl_kernel.so .
	$(MKDIR) $(MKDIRFLAGS) nightowl/kernel/arch
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/builder.h nightowl
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/kernel/generator.h nightowl/kernel
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/kernel/vector.h nightowl/kernel
	$(INSTALL) $(INSTALLFLAGS) ../nightowl/kernel/arch/arch.h nightowl/kernel/arch

test : 
	@$(MAKE) $(MAKEFLAGS) -C ../krd test
	@$(MAKE) $(MAKEFLAGS) -C ../mitten/nmctaurus test
	@$(MAKE) $(MAKEFLAGS) -C ../tse test

clean :
	$(RM) $(RMFLAGS) -r krd ktp libkrd.a libktp.a libnightowl.a libnightowl_kernel.so libtse.a moph nightowl tse
	@$(MAKE) $(MAKEFLAGS) -C ../krd clean
	@$(MAKE) $(MAKEFLAGS) -C ../ktp clean
	@$(MAKE) $(MAKEFLAGS) -C ../mitten/nmctaurus clean
	@$(MAKE) $(MAKEFLAGS) -C ../moph clean
	@$(MAKE) $(MAKEFLAGS) -C ../nightowl clean
	@$(MAKE) $(MAKEFLAGS) -C ../tse clean
