msys2=C:\\Software\\MSYS2
cc=g++.exe
def=-D_CRT_SECURE_NO_WARNINGS -DANSI -D_ANSI
args_base=-fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64
args_release=-DNDEBUG $(args_base) -static -Oz -flto=auto -fno-rtti
args_debug=$(args_base) -Og -g3
tag=std
arch=x86_64
version=v5.10.3
obj=src/*.cpp bin/info.obj
.PHONY:init build debug clean
all:init clean build
init:
	$(msys2)\\usr\\bin\\pacman.exe -Syyu --noconfirm --needed\
	 mingw-w64-ucrt-x86_64-toolchain\
	 mingw-w64-x86_64-toolchain
build:bin/$(version)/$(tag)-$(arch)-msvcrt.exe\
      bin/$(version)/$(tag)-$(arch)-ucrt.exe
debug:$(obj) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(cc) $(obj) $(def) $(args_debug) -o ./bin/__debug__.exe
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf bin
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\touch.exe bin/.gitkeep
bin/$(version)/$(tag)-$(arch)-msvcrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\$(cc) $(obj) $(def) $(args_release) -o $@
bin/$(version)/$(tag)-$(arch)-ucrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(cc) $(obj) $(def) $(args_release) -o $@
bin/info.obj:info.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< $(def) -o $@ -F pe-x86-64
bin:bin/$(version)
bin/$(version):
	$(msys2)\\usr\\bin\\mkdir.exe bin/$(version) -p