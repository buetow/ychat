MAKE=gmake
all: mods
mods:
	@${MAKE} -C ./commands #//<<
	@${MAKE} -C ./html
clean:
	@for i in commands html; do \
		if [ -f $$i/Makefile ]; then ${MAKE} -C ./$$i clean; \
		fi; done
	@if test -d ../../mods; then rm -Rf ../../mods; fi
mrproper: clean
	@for i in commands html; do \
		if [ -f $$i/Makefile ]; then rm -f $$i/Makefile; \
		fi; done

