MAKE=gmake
all:    base 
	@echo "Now edit the ./conf.txt file and run ./ychat!"
	@echo WARNING! This software is EXPERIMENTAL!
install: deinstall
uninstall: deinstall
deinstall:
	@echo Install/deinstall is not supported!
	@echo Start yChat with ./ychat instead! 
	@exit 1
base:	
	@${MAKE} -C ./src 
clean_base:
	@${MAKE} -C ./src clean
stats:
	@perl scripts/stats.pl
run: 
	./ychat
gpl:
	@more COPYING
clean:	clean_base 
debug:
	@gdb ./ychat ychat.core
