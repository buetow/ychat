SRCS=yc_about.cpp yc_all.cpp yc_away.cpp yc_col.cpp yc_compopt.cpp yc_exec.cpp yc_fake.cpp yc_getroom.cpp yc_getrusage.cpp yc_help.cpp yc_invisible.cpp yc_j.cpp yc_ko.cpp yc_m.cpp yc_md5.cpp yc_me.cpp yc_morph.cpp yc_msg.cpp yc_q.cpp yc_reload.cpp yc_ren.cpp yc_s.cpp yc_set.cpp yc_template.cpp yc_time.cpp yc_topic.cpp yc_uptime.cpp yc_users.cpp yc_version.cpp
MODS=$(addprefix ../../../mods/commands/, $(SRCS:.cpp=.so))
CC=g++33
INCLUDES=`cat ../../includes.add`
CFLAGS=`cat ../cflags.add`
all: mods
$(MODS): 
	@echo -n "Compiling command module `basename $@ | sed s/\.so// | sed s/yc_//` " 
	@if ! test -d `dirname $@`; then mkdir -p `dirname $@`; fi
	@$(CC) $(CFLAGS) $(INCLUDES) -shared -s -o $@ `echo $(notdir $@) | sed s/.so/.cpp/`
	@du -hc $@ | tail -n 1 | sed s/total// | sed "s/ //g"
infotext:
	@echo Compiling command modules
mods:	infotext $(MODS) 
	@echo "Num of command modules: "
	@ls ../../../mods/commands/*.so | wc -l
clean:
	@echo Cleaning command modules
	@if test -d ../../../mods/commands; then rm -Rf ../../../mods/commands; fi
../../../mods/commands/yc_about.so: yc_about.cpp
../../../mods/commands/yc_all.so: yc_all.cpp
../../../mods/commands/yc_away.so: yc_away.cpp
../../../mods/commands/yc_col.so: yc_col.cpp
../../../mods/commands/yc_compopt.so: yc_compopt.cpp
../../../mods/commands/yc_exec.so: yc_exec.cpp
../../../mods/commands/yc_fake.so: yc_fake.cpp
../../../mods/commands/yc_getroom.so: yc_getroom.cpp
../../../mods/commands/yc_getrusage.so: yc_getrusage.cpp
../../../mods/commands/yc_help.so: yc_help.cpp
../../../mods/commands/yc_invisible.so: yc_invisible.cpp
../../../mods/commands/yc_j.so: yc_j.cpp
../../../mods/commands/yc_ko.so: yc_ko.cpp
../../../mods/commands/yc_m.so: yc_m.cpp
../../../mods/commands/yc_md5.so: yc_md5.cpp
../../../mods/commands/yc_me.so: yc_me.cpp
../../../mods/commands/yc_morph.so: yc_morph.cpp
../../../mods/commands/yc_msg.so: yc_msg.cpp
../../../mods/commands/yc_q.so: yc_q.cpp
../../../mods/commands/yc_reload.so: yc_reload.cpp
../../../mods/commands/yc_ren.so: yc_ren.cpp
../../../mods/commands/yc_s.so: yc_s.cpp
../../../mods/commands/yc_set.so: yc_set.cpp
../../../mods/commands/yc_template.so: yc_template.cpp
../../../mods/commands/yc_time.so: yc_time.cpp
../../../mods/commands/yc_topic.so: yc_topic.cpp
../../../mods/commands/yc_uptime.so: yc_uptime.cpp
../../../mods/commands/yc_users.so: yc_users.cpp
../../../mods/commands/yc_version.so: yc_version.cpp
