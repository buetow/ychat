MAKE=`tail -1 make.version`
PREFIX=`grep "define PREFIX" src/glob.h | cut -d'"' -f2`
all:    base modules version
	@echo "Now edit the yhttpd.conf and run yhttpd!"
	@echo "The config file is searched in the following order:"
	@echo " ./yhttpd.conf " 
	@echo " ~/.yhttpd/yhttpd.conf "
	@echo " ./etc/yhttpd.conf "
	@echo " /etc/yhttpd.conf "
	@echo " $(PREFIX)/etc/yhttpd.conf "
install:	deinstall
	@echo "Instaling yhttpd to $(PREFIX)"
	@cp bin/yhttpd $(PREFIX)/bin 
	@if ! test -d $(PREFIX)/lib/yhttpd; then mkdir -p $(PREFIX)/lib/yhttpd; fi
	@if ! test -d $(PREFIX)/share/yhttpd/log/rooms; then mkdir -p $(PREFIX)/share/yhttpd/log/rooms; fi
	@if ! test -d $(PREFIX)/etc; then mkdir $(PREFIX)/etc; fi
	@if test -d mods; then cp -Rp mods $(PREFIX)/lib/yhttpd/mods; fi
	@cp -Rp lang $(PREFIX)/share/yhttpd/lang
	@if test -f $(PREFIX)/etc/yhttpd.conf; then mv $(PREFIX)/etc/yhttpd.conf $(PREFIX)/etc/yhttpd.conf.bak; fi
	@cp etc/yhttpd.conf etc/yhttpd.conf.tmp
	@sed "s#mods/#$(PREFIX)/lib/yhttpd/mods/#" etc/yhttpd.conf.tmp > etc/yhttpd.conf.tmp.2 && mv etc/yhttpd.conf.tmp.2 etc/yhttpd.conf.tmp 
	@sed "s#\"log/#\"$(PREFIX)/share/yhttpd/log/#" etc/yhttpd.conf.tmp > etc/yhttpd.conf.tmp.2 && mv etc/yhttpd.conf.tmp.2 etc/yhttpd.conf.tmp 
	@sed "s#LANGUAGE_DIR=\"lang/#LANGUAGE_DIR=\"$(PREFIX)/share/yhttpd/lang/#" etc/yhttpd.conf.tmp > etc/yhttpd.conf.tmp.2 && mv etc/yhttpd.conf.tmp.2 etc/yhttpd.conf.tmp 
	@mv etc/yhttpd.conf.tmp $(PREFIX)/etc/yhttpd.conf
	@echo "yhttpd configuration file can be found under"
	@echo " $(PREFIX)/etc/yhttpd.conf"
	@echo "Copy it to ~/.yhttpd/yhttpd.conf to use local settings :-)"
	@echo "Be sure that $(PREFIX)/share/yhttpd/logs is writable by your user or modify "
	@echo "logging dirs in the yhttpd.conf to a local directory."
	@echo "The most secure would be an additional user 'yhttpd'!"
uninstall: deinstall
deinstall:
	@echo "Deinstalling yhttpd from $(PREFIX)"
	@if test -f $(PREFIX)/bin/yhttpd; then rm -f $(PREFIX)/bin/yhttpd; fi
	@if test -d $(PREFIX)/lib/yhttpd; then rm -Rf $(PREFIX)/lib/yhttpd; fi
	@if test -d $(PREFIX)/share/yhttpd; then rm -Rf $(PREFIX)/share/yhttpd; fi
	@echo "Done. Please remove manually $(PREFIX)/etc/yhttpd.conf to complete"
modules:
	@if test -d ./src/mods; then ${MAKE} -C ./src/mods; fi 
clean_modules:
	@if test -d ./src/mods; then ${MAKE} -C ./src/mods clean; fi
base:	
	@if test -f bin/yhttpd; then echo "Backing up old binary";if test -f bin/yhttpd.old; then rm -f bin/yhttpd.old; fi; mv bin/yhttpd bin/yhttpd.old; fi
	@perl ./scripts/buildnr.pl 
	@perl ./scripts/setglobvals.pl
	@${MAKE} -C ./src 
clean_base:
	@${MAKE} -C ./src clean
stats:
	@perl scripts/stats.pl
run: 
	./bin/yhttpd
base_start: base 
	./bin/yhttpd
start:	base modules 
	./bin/yhttpd
gpl:
	@more COPYING
clean:	clean_base clean_modules
help:
	@echo "You may run ${MAKE} with the following parameters:"
	@grep "^ ${MAKE} " README 
	@echo "For more questions read the README file or contact mail@yhttpd.org!"
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
	@echo "`grep VERSION src/msgs.h | cut -d'"' -f2`-`grep BRANCH src/msgs.h| cut -d'"' -f2` Build `grep BUILD src/msgs.h| cut -d' ' -f3`" 
debug:
	@gdb bin/yhttpd yhttpd.core
