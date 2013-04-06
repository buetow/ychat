MAKE=`tail -n 1 ../../make.version`
all: mods
mods:
	@${MAKE} -C ./commands #//<<
	@${MAKE} -C ./html
clean:
	@${MAKE} -C ./commands clean #//<<	
	@${MAKE} -C ./html clean	
	@if test -d ../../mods; then rm -Rf ../../mods; fi
