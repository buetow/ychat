SRCS=$(shell find ./ -name '*.cpp')
MODS=$(addprefix ../../../mods/html/, $(SRCS:.cpp=.so))
CXX=g++
CXXFLAGS=-fno-inline -fno-default-inline -g -O2 ${EFLAGS} 
all: mods 
${MODS}: 
	@if ! test -d `dirname $@`; then mkdir -p `dirname $@`; fi
	@${CXX} ${CXXGLAGS} -fPIC -shared -s -o $@ ` \
		echo $(notdir $@) | sed s/.so/.cpp/`
	@echo "mods:html:`basename $@ | sed s/\.so// | sed s/yc_//` (` \
		du -hs $@ | awk '{ print $$1 }'`) "
infotext:
	@echo "===> Compiling html modules"
mods:	infotext ${MODS} 
	@echo "===> Num of html modules: `ls \
		../../../mods/html/*.so | wc -l | sed 's/ //g;'`"
clean:
	@echo "===> Cleaning html modules"
	@if test -d ../../../mods/html; then rm -Rf ../../../mods/html; fi
../../../mods/html/./yc_admin.so: yc_admin.cpp
../../../mods/html/./yc_colors.so: yc_colors.cpp
../../../mods/html/./yc_help.so: yc_help.cpp
../../../mods/html/./yc_loggedin.so: yc_loggedin.cpp
../../../mods/html/./yc_options.so: yc_options.cpp
../../../mods/html/./yc_register.so: yc_register.cpp
