MAKE=`tail -n 1 make.version`
PREFIX=`grep "define PREFIX" src/glob.h | cut -d'"' -f2`
all:    version base modules 
	@echo "Now edit the ychat.conf and run ychat!"
	@echo "The config file is searched in the following order:"
	@echo " ./ychat.conf " 
	@echo " ~/.ychat/ychat.conf "
	@echo " ./etc/ychat.conf "
	@echo " /etc/ychat.conf "
	@echo " $(PREFIX)/etc/ychat.conf "
	@echo If you want to help the yChat project please run gmake mail
	@echo so that the developers receive an email about the platform
	@echo being used.
	@echo WARNING! This software is EXPERIMENTAL!
mail:
	@echo "VERSION:" > mail.tmp
	@${MAKE} version >> mail.tmp
	@echo >> mail.tmp
	@echo "UNAME:" >> mail.tmp
	@uname -a >> mail.tmp
	@echo >> mail.tmp
	@echo "DATE:" >> mail.tmp
	@date >> mail.tmp
	@echo >> mail.tmp
	@echo "COMPILER AND MAKE:" >> mail.tmp
	@cat g++.version make.version >> mail.tmp
	@cat mail.tmp | mail -s "Successfull build of yChat" successfullbuild@yhttpd.org
	@rm -f mail.tmp 
install: deinstall
uninstall: deinstall
deinstall:
	@echo Install/deinstall is not supported!
	@echo Start yChat with ./bin/ychat instead! 
	@exit 1
modules:
	@if test -d ./src/mods; then ${MAKE} -C ./src/mods; fi 
clean_modules:
	@if test -d ./src/mods; then ${MAKE} -C ./src/mods clean; fi
base:	
	@if test -f bin/ychat; then echo "Backing up old binary";if test -f bin/ychat.old; then rm -f bin/ychat.old; fi; mv bin/ychat bin/ychat.old; fi
	@perl ./scripts/buildnr.pl 
	@perl ./scripts/setglobvals.pl
	@${MAKE} -C ./src 
clean_base:
	@${MAKE} -C ./src clean
stats:
	@perl scripts/stats.pl
run: 
	./bin/ychat
base_start: base 
	./bin/ychat
gpl:
	@more COPYING
#//<<*
yhttpdbase:
	@perl scripts/makeyhttpd.pl || echo "You need to have perl to do this!"
	@echo yhttpd code base has been generated in ../yhttpd	
#//*>>
clean:	clean_base clean_modules
help:
	@echo "You may run ${MAKE} with the following parameters:"
	@grep "^ ${MAKE} " README 
	@echo "For more questions read the README file or contact mail@ychat.org!"
setup:
	@./configure
	@${MAKE}
config:
	@sh -c "scripts/config.sh"
	@echo If you run ${MAKE} config from the command line then you may need 
	@echo to rerun ./configure and recompile all now!
	
mrproper: clean	
	@if test -f src/glob.h.org; then mv -f src/glob.h.org src/glob.h;fi
	@if test -f g++.version; then rm -f g++.version; fi 
	@if test -f make.version; then rm -f make.version; fi 
	@if test -f src/Makefile; then rm -f src/Makefile; fi 
	@if test -f bin/ychat; then find bin/ -name "*ychat*" | xargs rm -f; fi
	@if test -d src/mods; then find src/mods/*/ -name Makefile | xargs rm -f; fi
	@find . -name "*.add" | xargs rm -f
	@ls | grep core | xargs rm -f
version:
	@./scripts/version.sh
debug:
	@gdb bin/ychat ychat.core
confdebug:
	./configure -g3 -ggdb
