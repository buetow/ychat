MAKE=gmake
all:    base 
	@echo "Now edit the ./conf.txt file and run ./ychat!"
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
version:
	@sh ./scripts/version.sh
