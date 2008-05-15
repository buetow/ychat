SRCS=./conf/conf.cpp ./time/timo.cpp ./time/timr.cpp ./data/con.cpp ./data/data_base.cpp ./data/con_base.cpp ./data/data.cpp ./tool/dir.cpp ./tool/tool.cpp ./chat/sman.cpp ./chat/perm.cpp ./chat/room.cpp ./chat/sess.cpp ./chat/user.cpp ./chat/chat.cpp ./chat/gcol.cpp ./monitor/stats.cpp ./monitor/dump.cpp ./sock/sock.cpp ./sock/sslsock.cpp ./sock/context.cpp ./contrib/xml/tinyxmlerror.cpp ./contrib/xml/tinyxmlparser.cpp ./contrib/xml/tinyxml.cpp ./contrib/crypt/md5.cpp ./contrib/crypt/md5crypt.cpp ./cli/cli.cpp ./html.cpp ./reqp.cpp ./main.cpp ./sign.cpp ./wrap.cpp ./name.cpp ./logd.cpp ./modl.cpp
OBJS= ../obj/./conf/conf.o ../obj/./time/timo.o ../obj/./time/timr.o ../obj/./data/con.o ../obj/./data/data_base.o ../obj/./data/con_base.o ../obj/./data/data.o ../obj/./tool/dir.o ../obj/./tool/tool.o ../obj/./chat/sman.o ../obj/./chat/perm.o ../obj/./chat/room.o ../obj/./chat/sess.o ../obj/./chat/user.o ../obj/./chat/chat.o ../obj/./chat/gcol.o ../obj/./monitor/stats.o ../obj/./monitor/dump.o ../obj/./sock/sock.o ../obj/./sock/sslsock.o ../obj/./sock/context.o ../obj/./contrib/xml/tinyxmlerror.o ../obj/./contrib/xml/tinyxmlparser.o ../obj/./contrib/xml/tinyxml.o ../obj/./contrib/crypt/md5.o ../obj/./contrib/crypt/md5crypt.o ../obj/./cli/cli.o ../obj/./html.o ../obj/./reqp.o ../obj/./main.o ../obj/./sign.o ../obj/./wrap.o ../obj/./name.o ../obj/./logd.o ../obj/./modl.o
BIN=../bin/ychat
CXX=g++
#EFLAGS=-frepo
CXXFLAGS=-fno-inline -fno-default-inline -g -O2 ${EFLAGS} 
#LDADD+=-lreadline -levent -lpthread -lstdc++ -D_THREAD_SAVE -export-dynamic
LDADD+=-lreadline -levent -lpthread -lstdc++ -export-dynamic
PREFIX=/usr/local
all: message ${OBJS} 
	@dirname=`dirname ${BIN}`; if ! test -d $$dirname; then mkdir -p $$dirname; fi
	@echo; echo "===> Linking base sources"
	@${CXX} -o ${BIN} ${OBJS} ${LDADD}
	@echo "===> LDADD	        :      ${LDADD}"	
	@echo "===> CXXFLAGS	        :      ${CXXFLAGS}"	
	@echo "===> CXX		:      ${CXX}"	
	@echo "===> Binary size        :      `du -hs ${BIN} | sed 's/\.\.\/bin\///'`"	
	@echo "===> `cd ..; ./scripts/version.sh`"
	@echo 
${OBJS}:
	@dirname=`dirname $@`; if ! test -d $$dirname; then mkdir -p $$dirname; fi
	@${CXX} ${CXXFLAGS} -c -o $@ `echo $@ | \
		sed 's/\.\.\/obj\/\.\/\(.*\)\.o/\.\/\1.cpp/'`
	@echo "base:`echo $@ | sed 's/\.\.\/obj\/\.\/\(.*\)\.o$$/\1/'` (`du \
		-hs $@ | awk '{ print $$1 }'`) "
message:
	@echo "===> Building base sources"
clean:
	@echo "===> Cleaning base"
	@find ../obj -name '*.o' | grep -v mods | xargs rm -f 
	@if [ -f ${BIN} ]; then rm -f ${BIN}; fi
mrproper: clean
	@for i in autom4te.cache; \
		do if [ -d $$i ]; then rm -Rf $$i; fi; done
	@for i in Makefile config.h config.log config.status; \
		do if [ -f $$i ]; then rm -f $$i; fi; done

../obj/./conf/conf.o: ./conf/conf.cpp
../obj/./time/timo.o: ./time/timo.cpp
../obj/./time/timr.o: ./time/timr.cpp
../obj/./data/con.o: ./data/con.cpp
../obj/./data/data_base.o: ./data/data_base.cpp
../obj/./data/con_base.o: ./data/con_base.cpp
../obj/./data/data.o: ./data/data.cpp
../obj/./tool/dir.o: ./tool/dir.cpp
../obj/./tool/tool.o: ./tool/tool.cpp
../obj/./chat/sman.o: ./chat/sman.cpp
../obj/./chat/perm.o: ./chat/perm.cpp
../obj/./chat/room.o: ./chat/room.cpp
../obj/./chat/sess.o: ./chat/sess.cpp
../obj/./chat/user.o: ./chat/user.cpp
../obj/./chat/chat.o: ./chat/chat.cpp
../obj/./chat/gcol.o: ./chat/gcol.cpp
../obj/./monitor/stats.o: ./monitor/stats.cpp
../obj/./monitor/dump.o: ./monitor/dump.cpp
../obj/./sock/sock.o: ./sock/sock.cpp
../obj/./sock/sslsock.o: ./sock/sslsock.cpp
../obj/./sock/context.o: ./sock/context.cpp
../obj/./contrib/xml/tinyxmlerror.o: ./contrib/xml/tinyxmlerror.cpp
../obj/./contrib/xml/tinyxmlparser.o: ./contrib/xml/tinyxmlparser.cpp
../obj/./contrib/xml/tinyxml.o: ./contrib/xml/tinyxml.cpp
../obj/./contrib/crypt/md5.o: ./contrib/crypt/md5.cpp
../obj/./contrib/crypt/md5crypt.o: ./contrib/crypt/md5crypt.cpp
../obj/./cli/cli.o: ./cli/cli.cpp
../obj/./html.o: ./html.cpp
../obj/./reqp.o: ./reqp.cpp
../obj/./main.o: ./main.cpp
../obj/./sign.o: ./sign.cpp
../obj/./wrap.o: ./wrap.cpp
../obj/./name.o: ./name.cpp
../obj/./logd.o: ./logd.cpp
../obj/./modl.o: ./modl.cpp
