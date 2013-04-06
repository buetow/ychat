SRCS=yc_admin.cpp yc_colors.cpp yc_help.cpp yc_options.cpp yc_register.cpp
MODS=$(addprefix ../../../mods/html/, $(SRCS:.cpp=.so))
CC=g++
INCLUDES=`cat ../../includes.add`
CFLAGS=`cat ../cflags.add`
all: mods
$(MODS): 
	@echo -n "Compiling html module `basename $@` " 
	@if ! test -d `dirname $@`; then mkdir -p `dirname $@`; fi
	@$(CC) $(CFLAGS) $(INCLUDES) -shared -s -o $@ `echo $(notdir $@) | sed s/.so/.cpp/`
	@du -hc $@ | tail -n 1 | sed s/total// | sed "s/ //g"
infotext:
	@echo Compiling html modules
mods:	infotext $(MODS) 
	@echo "Num of html modules: "
	@ls ../../../mods/html/*.so | wc -l
clean:
	@echo Cleaning html modules
	@if test -d ../../../mods/html; then rm -Rf ../../../mods/html; fi
../../../mods/html/yc_admin.so: yc_admin.cpp
../../../mods/html/yc_colors.so: yc_colors.cpp
../../../mods/html/yc_help.so: yc_help.cpp
../../../mods/html/yc_options.so: yc_options.cpp
../../../mods/html/yc_register.so: yc_register.cpp
