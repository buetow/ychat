# Generated automatically from Makefile.in by configure.
SRCS=chat.cpp s_chat.cpp conf.cpp s_conf.cpp cont.cpp html.cpp s_html.cpp lang.cpp s_lang.cpp main.cpp mutx.cpp s_mutx.cpp name.cpp pool.cpp reqp.cpp room.cpp sock.cpp s_sock.cpp thrd.cpp s_tool.cpp user.cpp sess.cpp sman.cpp s_sman.cpp 
#logd.cpp modl.cpp s_modl.cpp cmnd.cpp
OBJS=$(SRCS:.cpp=.o)
CC=g++
LDFLAGS= -lstdc++ -g
LDADD=-pthread -D_THREAD_SAFE
all: ychat	
$(SRCS):
	$(CC) $(CFLAGS) -c $*.cpp
ychat:  $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDADD)
	@mv -f ychat ..
mrproper: clean
	@rm -f Makefile config.log config.cache config.status
	rm -f ../ychat
clean:	
	rm *.o
