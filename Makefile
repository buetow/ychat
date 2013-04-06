SRCS=base.cpp chat.cpp CHAT.cpp cmnd.cpp conf.cpp CONF.cpp cont.cpp html.cpp HTML.cpp main.cpp mutx.cpp MUTX.cpp name.cpp pool.cpp reqp.cpp room.cpp sock.cpp SOCK.cpp thrd.cpp TOOL.cpp user.cpp 
OBJS=$(SRCS:.cpp=.o)
CC=c++
LDFLAGS=-lstdc++ -g
LDADD=-pthread -D_THREAD_SAFE
all: ychat	
$(SRCS):
	$(CC) $(CFLAGS) -c $*.cpp
ychat:  $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDADD)
clean:
	rm *.o
