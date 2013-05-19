include config.mk

SRC=..
BUILD=$(SRC)/build
LIB=$(SRC)/lib

MAKE=make
MAKEFLAGS=

build : 
	$(INSTALL) $(INSTALLFLAGS) config.mk $(LIB)
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/rendering build
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/rendering/librendering.a .
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/rendering/kernel
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/rendering/kernel/backend.h $(BUILD)/rendering/kernel/backend.h

	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/tse build
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/libtse.a $(BUILD)
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/tse/core
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/tse/os/linux
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/tse.h $(BUILD)/tse
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/core/async.h $(BUILD)/tse/core
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/core/region.h $(BUILD)/tse/core
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/core/stream.h $(BUILD)/tse/core
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/os/os.h $(BUILD)/tse/os
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/tse/os/linux/linux.h $(BUILD)/tse/os/linux

	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/parsing build
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/libparsing.a $(BUILD)
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/parsing/lexical
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/parsing/semantic
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/parsing.h $(BUILD)/parsing
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/lexical/pattern.h $(BUILD)/parsing/lexical
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/lexical/tokenizer.h $(BUILD)/parsing/lexical
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/semantic/ast.h $(BUILD)/parsing/semantic
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/semantic/error.h $(BUILD)/parsing/semantic
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/parsing/semantic/parser.h $(BUILD)/parsing/semantic

	$(INSTALL) $(INSTALLFLAGS) config.mk $(SRC)/mitten/nmctaurus
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/mitten/nmctaurus build
	
	$(INSTALL) $(INSTALLFLAGS) config.mk $(SRC)/moph
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/moph
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/moph/moph $(BUILD)

	$(INSTALL) $(INSTALLFLAGS) config.mk $(SRC)/nightowl
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/nightowl
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/libnightowl.a $(BUILD)
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/libnightowl_kernel.so $(BUILD)
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/nightowl/kernel/arch
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/builder.h $(BUILD)/nightowl
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/generator.h $(BUILD)/nightowl/kernel
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/vector.h $(BUILD)/nightowl/kernel
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/arch/arch.h $(BUILD)/nightowl/kernel/arch

test : build
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/rendering test
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/tse test
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/mitten/nmctaurus test

clean :
	$(RM) $(RMFLAGS) -r $(BUILD)/rendering $(BUILD)/parsing $(BUILD)/librendering.a $(BUILD)/libparsing.a $(BUILD)/libnightowl.a $(BUILD)/libnightowl_kernel.so $(BUILD)/libtse.a $(BUILD)/moph $(BUILD)/nightowl $(BUILD)/tse
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/rendering clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/parsing clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/mitten/nmctaurus clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/moph clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/nightowl clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/tse clean
