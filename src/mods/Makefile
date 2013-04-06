all: mods
mods:
	gmake -C ./commands #//<<
	gmake -C ./html
clean:
	gmake -C ./commands clean #//<<	
	gmake -C ./html clean	
	@if test -d ../../mods; then rm -Rf ../../mods; fi
