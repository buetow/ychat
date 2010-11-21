SRCS=$(shell find ./ -name '*.cpp')
MODS=$(addprefix ../../../mods/commands/, $(SRCS:.cpp=.so))
CXX=g++
CXXFLAGS=-fno-inline -fno-default-inline -g -O2 ${EFLAGS} 
all: mods 
${MODS}: 
	@if ! test -d `dirname $@`; then mkdir -p `dirname $@`; fi
	@${CXX} ${CXXGLAGS} -fPIC -shared -s -o $@ ` \
		echo $(notdir $@) | sed s/.so/.cpp/`
	@echo "mods:commands:`basename $@ | sed s/\.so// | sed s/yc_//` (` \
		du -hs $@ | awk '{ print $$1 }'`) "
infotext:
	@echo "===> Compiling commands modules"
mods:	infotext ${MODS} 
	@echo "===> Num of commands modules: `ls \
		../../../mods/commands/*.so | wc -l | sed 's/ //g;'`"
clean:
	@echo "===> Cleaning commands modules"
	@if test -d ../../../mods/commands; then rm -Rf ../../../mods/commands; fi
../../../mods/commands/./yc_about.so: yc_about.cpp
../../../mods/commands/./yc_all.so: yc_all.cpp
../../../mods/commands/./yc_away.so: yc_away.cpp
../../../mods/commands/./yc_ban.so: yc_ban.cpp
../../../mods/commands/./yc_banned.so: yc_banned.cpp
../../../mods/commands/./yc_col.so: yc_col.cpp
../../../mods/commands/./yc_compopt.so: yc_compopt.cpp
../../../mods/commands/./yc_debug.so: yc_debug.cpp
../../../mods/commands/./yc_exec.so: yc_exec.cpp
../../../mods/commands/./yc_fake.so: yc_fake.cpp
../../../mods/commands/./yc_gag.so: yc_gag.cpp
../../../mods/commands/./yc_getroom.so: yc_getroom.cpp
../../../mods/commands/./yc_getrusage.so: yc_getrusage.cpp
../../../mods/commands/./yc_help.so: yc_help.cpp
../../../mods/commands/./yc_invisible.so: yc_invisible.cpp
../../../mods/commands/./yc_j.so: yc_j.cpp
../../../mods/commands/./yc_ko.so: yc_ko.cpp
../../../mods/commands/./yc_m.so: yc_m.cpp
../../../mods/commands/./yc_md5.so: yc_md5.cpp
../../../mods/commands/./yc_me.so: yc_me.cpp
../../../mods/commands/./yc_morph.so: yc_morph.cpp
../../../mods/commands/./yc_msg.so: yc_msg.cpp
../../../mods/commands/./yc_q.so: yc_q.cpp
../../../mods/commands/./yc_reload.so: yc_reload.cpp
../../../mods/commands/./yc_ren.so: yc_ren.cpp
../../../mods/commands/./yc_s.so: yc_s.cpp
../../../mods/commands/./yc_set.so: yc_set.cpp
../../../mods/commands/./yc_time.so: yc_time.cpp
../../../mods/commands/./yc_topic.so: yc_topic.cpp
../../../mods/commands/./yc_unban.so: yc_unban.cpp
../../../mods/commands/./yc_ungag.so: yc_ungag.cpp
../../../mods/commands/./yc_uptime.so: yc_uptime.cpp
../../../mods/commands/./yc_users.so: yc_users.cpp
../../../mods/commands/./yc_version.so: yc_version.cpp
