include config.mk

MAKE=make
MAKEFLAGS=--no-print-directory

build : 
	@echo "building krd..."
	@$(MAKE) $(MAKEFLAGS) -C ../krd build
	@echo "installing krd within build..."
	@$(CP) $(CPFLAGS) ../krd/libkrd.a .
	@$(MKDIR) $(MKDIRFLAGS) krd/kernel
	@$(CP) $(CPFLAGS) ../krd/kernel/backend.h krd/kernel/backend.h

	@echo "building tse..."
	@$(MAKE) $(MAKEFLAGS) -C ../tse build
	@echo "installing tse within build..."
	@$(CP) $(CPFLAGS) ../tse/libtse.a .
	@$(MKDIR) $(MKDIRFLAGS) tse/core
	@$(MKDIR) $(MKDIRFLAGS) tse/os/linux
	@$(CP) $(CPFLAGS) ../tse/tse.h tse
	@$(CP) $(CPFLAGS) ../tse/core/async.h tse/core
	@$(CP) $(CPFLAGS) ../tse/core/region.h tse/core
	@$(CP) $(CPFLAGS) ../tse/core/stream.h tse/core
	@$(CP) $(CPFLAGS) ../tse/os/os.h tse/os
	@$(CP) $(CPFLAGS) ../tse/os/linux/linux.h tse/os/linux

	@echo "building ktp..."
	@$(MAKE) $(MAKEFLAGS) -C ../ktp build
	@echo "installing ktp within build..."
	@$(CP) $(CPFLAGS) ../ktp/libktp.a .
	@$(MKDIR) $(MKDIRFLAGS) ktp/lexical
	@$(MKDIR) $(MKDIRFLAGS) ktp/semantic
	@$(CP) $(CPFLAGS) ../ktp/ktp.h ktp
	@$(CP) $(CPFLAGS) ../ktp/lexical/pattern.h ktp/lexical
	@$(CP) $(CPFLAGS) ../ktp/lexical/tokenizer.h ktp/lexical
	@$(CP) $(CPFLAGS) ../ktp/semantic/ast.h ktp/semantic
	@$(CP) $(CPFLAGS) ../ktp/semantic/error.h ktp/semantic
	@$(CP) $(CPFLAGS) ../ktp/semantic/parser.h ktp/semantic

	@echo "building nmctaurus..."
	@$(MAKE) $(MAKEFLAGS) -C ../mitten/nmctaurus build
	
	@echo "building moph..."
	@$(MAKE) $(MAKEFLAGS) -C ../moph
	@echo "installing moph within build..."
	@$(CP) $(CPFLAGS) ../moph/moph .

	@echo "building nightowl..."
	@$(MAKE) $(MAKEFLAGS) -C ../nightowl
	@echo "installing nightowl within build..."
	@$(CP) $(CPFLAGS) ../nightowl/libnightowl.a .
	@$(CP) $(CPFLAGS) ../nightowl/kernel/libnightowl_kernel.so .
	@$(MKDIR) $(MKDIRFLAGS) nightowl/kernel/arch
	@$(CP) $(CPFLAGS) ../nightowl/builder.h nightowl
	@$(CP) $(CPFLAGS) ../nightowl/kernel/generator.h nightowl/kernel
	@$(CP) $(CPFLAGS) ../nightowl/kernel/vector.h nightowl/kernel
	@$(CP) $(CPFLAGS) ../nightowl/kernel/arch/arch.h nightowl/kernel/arch

clean :
	@echo "cleaning build..."
	@$(RM) $(RMFLAGS) -r krd ktp libkrd.a libktp.a libnightowl.a libnightowl_kernel.so libtse.a moph nightowl tse
	@$(MAKE) $(MAKEFLAGS) -C ../krd clean
	@$(MAKE) $(MAKEFLAGS) -C ../ktp clean
	@$(MAKE) $(MAKEFLAGS) -C ../mitten/nmctaurus clean
	@$(MAKE) $(MAKEFLAGS) -C ../moph clean
	@$(MAKE) $(MAKEFLAGS) -C ../nightowl clean
	@$(MAKE) $(MAKEFLAGS) -C ../tse clean
