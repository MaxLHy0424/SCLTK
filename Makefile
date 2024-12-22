msys2        = C:\\Software\\MSYS2
cc           = g++.exe
def          = -D_CRT_SECURE_NO_WARNINGS -DANSI -D_ANSI
args_base    = -fexec-charset=GBK -std=gnu++26 -Wall -Wextra -Weffc++ -pipe
args_release = -DNDEBUG $(args_base) -static -Oz -flto=auto -fno-rtti
args_debug   = $(args_base) -Og -g3
arch         = x86_64
.PHONY: init release debug clean
all: init clean release
init:
	$(msys2)\\usr\\bin\\pacman.exe -Syyu --noconfirm --needed\
	 mingw-w64-i686-toolchain\
	 mingw-w64-ucrt-x86_64-toolchain
release: bin/release/CRCSN-i686-msvcrt.exe\
         bin/release/CRCSN-x86_64-ucrt.exe
debug: src/* bin/debug/.gitkeep
	$(msys2)\\ucrt64\\bin\\$(cc) src/*.cpp $(def) $(args_debug) -o ./bin/debug/__debug__.exe
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf bin
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\touch.exe bin/.gitkeep
bin/release/CRCSN-i686-msvcrt.exe: src/* bin/info-i686.obj bin/release/.gitkeep
	$(msys2)\\mingw32\\bin\\$(cc) src/*.cpp bin/info-i686.obj $(def) $(args_release) -o $@
bin/release/CRCSN-x86_64-ucrt.exe: src/* bin/info-x86_64.obj bin/release/.gitkeep
	$(msys2)\\ucrt64\\bin\\$(cc) src/*.cpp bin/info-x86_64.obj $(def) $(args_release) -o $@
bin/info-i686.obj: info.rc img/favicon.ico bin/.gitkeep
	$(msys2)\\usr\\bin\\windres.exe -i $< $(def) -o $@ -F pe-i386
bin/info-x86_64.obj: info.rc img/favicon.ico bin/.gitkeep
	$(msys2)\\usr\\bin\\windres.exe -i $< $(def) -o $@ -F pe-x86-64
	$(msys2)\\usr\\bin\\mkdir.exe bin -p
	$(msys2)\\usr\\bin\\touch.exe $@
bin/release/.gitkeep: bin/.gitkeep
	$(msys2)\\usr\\bin\\mkdir.exe bin/release -p
	$(msys2)\\usr\\bin\\touch.exe $@
bin/debug/.gitkeep: bin/.gitkeep
	$(msys2)\\usr\\bin\\mkdir.exe bin/debug -p
	$(msys2)\\usr\\bin\\touch.exe $@