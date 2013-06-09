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
	
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/args build
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/args/libargs.a $(BUILD)
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/args
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/args/args.h $(BUILD)/args

	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/extract build
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/extract/libextract.a $(BUILD)
	$(MKDIR) $(MKDIRFLAGS) $(BUILD)/extract
	$(INSTALL) $(INSTALLFLAGS) $(LIB)/extract/extract.h $(BUILD)/extract

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
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/extract test
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/rendering test
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/tse test
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/mitten/nmctaurus test

clean :
	$(RM) $(RMFLAGS) -r $(BUILD)/rendering $(BUILD)/parsing $(BUILD)/librendering.a $(BUILD)/libparsing.a $(BUILD)/libnightowl.a $(BUILD)/libnightowl_kernel.so $(BUILD)/libtse.a $(BUILD)/libextract.a $(BUILD)/libargs.a $(BUILD)/moph $(BUILD)/nightowl $(BUILD)/tse $(BUILD)/extract $(BUILD)/args
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/rendering clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/parsing clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/extract clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/args clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/mitten/nmctaurus clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/moph clean
	@$(MAKE) $(MAKEFLAGS) -C $(SRC)/nightowl clean
	@$(MAKE) $(MAKEFLAGS) -C $(LIB)/tse clean

install :
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)
	$(MKDIR) $(MKDIRFLAGS) $(PATH_LIB)

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/ipc
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/ipc/crypt.h $(PATH_INCLUDE)/ipc/crypt.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/ipc/node.h $(PATH_INCLUDE)/ipc/node.h

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/parsing
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/parsing/lexical
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/parsing/semantic
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/parsing.h $(PATH_INCLUDE)/parsing/parsing.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/lexical/pattern.h $(PATH_INCLUDE)/parsing/lexical/pattern.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/lexical/tokenizer.h $(PATH_INCLUDE)/parsing/lexical/tokenizer.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/semantic/ast.h $(PATH_INCLUDE)/parsing/semantic/ast.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/semantic/error.h $(PATH_INCLUDE)/parsing/semantic/error.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/semantic/parser.h $(PATH_INCLUDE)/parsing/semantic/parser.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/parsing/libparsing.a $(PATH_LIB)/libparsing.a

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/rendering
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/rendering/kernel
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/rendering/kernel/backend.h $(PATH_INCLUDE)/rendering/kernel/backend.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/rendering/librendering.a $(PATH_LIB)/librendering.a

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/tse
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/tse/core
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/tse/os
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/tse/os/linux
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/tse.h $(PATH_INCLUDE)/tse/tse.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/core/async.h $(PATH_INCLUDE)/tse/core/async.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/core/region.h $(PATH_INCLUDE)/tse/core/region.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/core/stream.h $(PATH_INCLUDE)/tse/core/stream.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/os/os.h $(PATH_INCLUDE)/tse/os/os.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/os/linux/linux.h $(PATH_INCLUDE)/tse/os/linux/linux.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/tse/libtse.a $(PATH_LIB)/libtse.a

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/extract
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/extract/extract.h $(PATH_INCLUDE)/extract/extract.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/extract/libextract.a $(PATH_LIB)/libextract.a

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/args
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/args/args.h $(PATH_INCLUDE)/args/args.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/lib/args/libargs.a $(PATH_LIB)/libargs.a

	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/nightowl
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/nightowl/kernel
	$(MKDIR) $(MKDIRFLAGS) $(PATH_INCLUDE)/nightowl/kernel/arch
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/builder.h $(PATH_INCLUDE)/nightowl/builder.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/nightowl.h $(PATH_INCLUDE)/nightowl/nightowl.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/generator.h $(PATH_INCLUDE)/nightowl/kernel/generator.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/vector.h $(PATH_INCLUDE)/nightowl/kernel/vector.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/arch/arch.h $(PATH_INCLUDE)/nightowl/kernel/arch/arch.h
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/libnightowl.a $(PATH_LIB)/libnightowl.a
	$(INSTALL) $(INSTALLFLAGS) $(SRC)/nightowl/kernel/libnightowl_kernel.so $(PATH_LIB)/libnightowl_kernel.so

uninstall :
	$(RM) $(RMFLAGS) $(PATH_INCLUDE)/ipc $(PATH_INCLUDE)/nightowl $(PATH_INCLUDE)/parsing $(PATH_INCLUDE)/rendering $(PATH_INCLUDE)/tse  $(PATH_LIB)/libnightowl.a $(PATH_LIB)/libnightowl_kernel.so $(PATH_LIB)/libparsing.a $(PATH_LIB)/librendering.a $(PATH_LIB)/libtse.a $(PATH_INCLUDE)/extract $(PATH_INCLUDE)/args $(PATH_LIB)/libextract.a $(PATH_LIB)/libargs.a
