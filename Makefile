PREFIX=`grep "define PREFIX" src/glob.h | cut -d'"' -f2`
all:    base modules version
	@echo "Now edit the ychat.conf and run ychat!"
	@echo "The config file is searched in the following order:"
	@echo " ./ychat.conf " 
	@echo " ~/.ychat/ychat.conf "
	@echo " ./etc/ychat.conf "
	@echo " /etc/ychat.conf "
	@echo " $(PREFIX)/etc/ychat.conf "
install:	deinstall
	@echo "Instaling ychat to $(PREFIX)"
	@cp bin/ychat $(PREFIX)/bin 
	@if ! test -d $(PREFIX)/lib/ychat; then mkdir -p $(PREFIX)/lib/ychat; fi
	@if ! test -d $(PREFIX)/share/ychat/log/rooms; then mkdir -p $(PREFIX)/share/ychat/log/rooms; fi
	@if ! test -d $(PREFIX)/etc; then mkdir $(PREFIX)/etc; fi
	@if test -d mods; then cp -Rp mods $(PREFIX)/lib/ychat/mods; fi
	@cp -Rp html $(PREFIX)/share/ychat/html
	@cp -Rp lang $(PREFIX)/share/ychat/lang
	@if test -f $(PREFIX)/etc/ychat.conf; then mv $(PREFIX)/etc/ychat.conf $(PREFIX)/etc/ychat.conf.bak; fi
	@cp etc/ychat.conf etc/ychat.conf.tmp
	@sed "s#mods/#$(PREFIX)/lib/ychat/mods/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@sed "s#\"log/#\"$(PREFIX)/share/ychat/log/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@sed "s#LANGUAGE_DIR=\"lang/#LANGUAGE_DIR=\"$(PREFIX)/share/ychat/lang/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@sed "s#HTML_TEMPLATE_DIR=\"html/#HTML_TEMPLATE_DIR=\"$(PREFIX)/share/ychat/html/#" etc/ychat.conf.tmp > etc/ychat.conf.tmp.2 && mv etc/ychat.conf.tmp.2 etc/ychat.conf.tmp 
	@mv etc/ychat.conf.tmp $(PREFIX)/etc/ychat.conf
	@echo "yChat configuration file can be found under"
	@echo " $(PREFIX)/etc/ychat.conf"
	@echo "Copy it to ~/.ychat/ychat.conf to use local settings :-)"
	@echo "Be sure that $(PREFIX)/share/ychat/logs is writable by your user or modify "
	@echo "logging dirs in the ychat.conf to a local directory."
	@echo "The most secure would be an additional user 'ychat'!"
uninstall: deinstall
deinstall:
	@echo "Deinstalling ychat from $(PREFIX)"
	@if test -f $(PREFIX)/bin/ychat; then rm -f $(PREFIX)/bin/ychat; fi
	@if test -d $(PREFIX)/lib/ychat; then rm -Rf $(PREFIX)/lib/ychat; fi
	@if test -d $(PREFIX)/share/ychat; then rm -Rf $(PREFIX)/share/ychat; fi
	@echo "Done. Please remove manually $(PREFIX)/etc/ychat.conf to complete"
modules:
	@if test -d ./src/mods; then gmake -C ./src/mods; fi 
clean_modules:
	@if test -d ./src/mods; then gmake -C ./src/mods clean; fi
base:	
	@if test -f bin/ychat; then echo "Backing up old binary";if test -f bin/ychat.old; then rm -f bin/ychat.old; fi; mv bin/ychat bin/ychat.old; fi
	@perl ./scripts/buildnr.pl 
	@perl ./scripts/setglobvals.pl
	@gmake -C ./src 
clean_base:
	@gmake -C ./src clean
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
	@echo "You may run gmake with the following parameters:"
	@grep "^ gmake " README 
	@echo "For more questions read the README file or contact mail@ychat.org!"
setup:
	@./configure
	@gmake
config:
	@sh -c "scripts/config.sh"
	@echo If you run gmake config from the command line then you may need 
	@echo to rerun ./configure and recompile all now!
	
mrproper: clean	
	@if test -f src/glob.h.org; then mv -f src/glob.h.org src/glob.h;fi
	@if test -f g++.version; then rm -f g++.version; fi 
	@if test -f src/Makefile; then rm -f src/Makefile; fi 
	@if test -d src/mods; then find src/mods/*/ -name Makefile | xargs rm -f; fi
	@find . -name "*.add" | xargs rm -f
	@ls | grep core | xargs rm -f
version:
	@echo "`grep VERSION src/msgs.h | cut -d'"' -f2`-`grep BRANCH src/msgs.h| cut -d'"' -f2` Build `grep BUILD src/msgs.h| cut -d' ' -f3`" 
debug:
	@gdb bin/ychat ychat.core
