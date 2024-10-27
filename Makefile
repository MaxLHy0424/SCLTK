msys2=C:\\Software\\MSYS2
CC=g++.exe
def=-D_CRT_SECURE_NO_WARNINGS -DANSI -D_ANSI
argsBase=-fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64
argsRelease=-DNDEBUG $(argsBase) -static -Os -flto -o
argsDebug=$(argsBase) -O0 -g3 -o
tag=std
arch=x86_64
version=v5.10.0
.PHONY:init build debug clean
init:
	$(msys2)\\usr\\bin\\pacman.exe -Syyu --noconfirm --needed\
	 mingw-w64-ucrt-x86_64-toolchain\
	 mingw-w64-x86_64-toolchain
build:bin/$(version)/$(tag)-$(arch)-msvcrt.exe\
      bin/$(version)/$(tag)-$(arch)-ucrt.exe
debug:
	$(msys2)\\ucrt64\\bin\\$(CC) $(obj) $(def) $(argsDebug) ./bin/__debug__.exe
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf bin
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\touch.exe bin/.gitkeep
obj=src/*.cpp bin/info.obj
bin/$(version)/$(tag)-$(arch)-msvcrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\$(CC) $(obj) $(def) $(argsRelease) $@
bin/$(version)/$(tag)-$(arch)-ucrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(CC) $(obj) $(def) $(argsRelease) $@
bin/info.obj:info.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< $(def) -o $@ -F pe-x86-64
bin:bin/$(version)
bin/$(version):
	$(msys2)\\usr\\bin\\mkdir.exe bin/$(version) -p