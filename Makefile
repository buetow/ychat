MAKE=`tail -n 1 make.version`
PREFIX=`grep "define PREFIX" src/glob.h | cut -d'"' -f2`
all:    version base modules version 
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
	@echo "Instaling ychat to $(PREFIX)"
	@cp bin/ychat $(PREFIX)/bin 
	@if ! test -d $(PREFIX)/lib/ychat; then mkdir -p $(PREFIX)/lib/ychat; fi
	@if ! test -d $(PREFIX)/share/ychat/log/rooms; then mkdir -p $(PREFIX)/share/ychat/log/rooms; fi
	@if ! test -d $(PREFIX)/etc; then mkdir $(PREFIX)/etc; fi
	@if test -d mods; then cp -Rp mods $(PREFIX)/lib/ychat/mods; fi
	@cp -Rp html $(PREFIX)/share/ychat/html
	@if test -f $(PREFIX)/etc/ychat.conf; then mv $(PREFIX)/etc/ychat.conf $(PREFIX)/etc/ychat.conf.bak; fi
	@cp etc/ychat.conf etc/ychat.conf.tmp
	@sed "s#mods/#$(PREFIX)/lib/ychat/mods/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@sed "s#\"log/#\"$(PREFIX)/share/ychat/log/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@mv etc/ychat.conf.tmp $(PREFIX)/etc/ychat.conf
	@echo "yChat configuration file can be found under"
	@echo " $(PREFIX)/etc/ychat.conf"
	@echo "Copy it to ~/.ychat/ychat.conf to use local settings :-)"
	@echo "Be sure that $(PREFIX)/share/ychat/logs is writable by your user or modify "
	@echo "logging dirs in the ychat.conf to a local directory."
	@echo "The most secure would be an additional user 'ychat'!"
uninstall: deinstall
deinstall:
	@echo Install/deinstall is currently not working!
	@echo Start yChat with ./bin/ychat instead! 
	@exit 1
	@echo "Deinstalling ychat from $(PREFIX)"
	@if test -f $(PREFIX)/bin/ychat; then rm -f $(PREFIX)/bin/ychat; fi
	@if test -d $(PREFIX)/lib/ychat; then rm -Rf $(PREFIX)/lib/ychat; fi
	@if test -d $(PREFIX)/share/ychat; then rm -Rf $(PREFIX)/share/ychat; fi
	@echo "Done. Please remove manually $(PREFIX)/etc/ychat.conf to complete"
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
start:	base modules 
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
	@if test -d src/mods; then find src/mods/*/ -name Makefile | xargs rm -f; fi
	@find . -name "*.add" | xargs rm -f
	@ls | grep core | xargs rm -f
version:
	@./scripts/version.sh
debug:
	@gdb bin/ychat ychat.core
