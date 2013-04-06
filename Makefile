# Generated automatically from Makefile.in by configure.
SRCS=base.cpp chat.cpp s_chat.cpp cmnd.cpp conf.cpp s_conf.cpp cont.cpp html.cpp s_html.cpp main.cpp mutx.cpp s_mutx.cpp name.cpp pool.cpp reqp.cpp room.cpp sock.cpp s_sock.cpp thrd.cpp s_tool.cpp user.cpp 
OBJS=$(SRCS:.cpp=.o)
CC=g++
LDFLAGS=-ldl -lstdc++ -g
LDADD=-pthread -D_THREAD_SAFE
all: ychat	
$(SRCS):
	$(CC) $(CFLAGS) -c $*.cpp
ychat:  $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDADD)
clean:
	rm *.o
