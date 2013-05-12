include ../build/config.mk

LIBS+=-ltse

build :
	@echo " link moph"
	@$(CC) $(CFLAGS) moph.c -o moph $(LIBS)

clean :
	@$(RM) $(RMFLAGS) moph
