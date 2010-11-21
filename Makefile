MAKE=gmake
HEADER?=docs/header.txt
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
headers:
	@find -E ./ -regex '\./src/.*\.(h|(cpp)|(tmpl))' -exec \
		sh -c 'export FILE={}; ${MAKE} header' \;
header:
	@echo "===> Processing ${FILE}"
	@sed -n '/*:/d; w .tmp' ${FILE} 
	@header=`sed 's/\(.*\)/ echo " \*: \1"/' ${HEADER}`; \
		echo '/*:*' > ${FILE}; eval "$$header" >> ${FILE}; \
		echo ' *:*/' >> ${FILE}; cat .tmp >> ${FILE}; rm -f .tmp
replace:
	@find ./ -type f -exec sh -c 'sed -n "s/$(FROM)/$(INTO)/g; \
		w .tmp" {} && mv -f .tmp {}' \;
