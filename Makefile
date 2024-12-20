msys2        = C:\\Software\\MSYS2
cc           = g++.exe
def          = -D_CRT_SECURE_NO_WARNINGS -DANSI -D_ANSI
args_base    = -fexec-charset=GBK -std=gnu++26 -Wall -Wextra -Weffc++ -pipe -m64
args_release = -DNDEBUG $(args_base) -static -Oz -flto=auto -fno-rtti
args_debug   = $(args_base) -Og -g3
label        = std
arch         = x86_64
version      = v5.11.1
main_src     = src/*.cpp bin/info.obj
.PHONY: init build debug clean
all: init clean build
init:
	$(msys2)\\usr\\bin\\pacman.exe -Syyu --noconfirm --needed\
	 mingw-w64-ucrt-x86_64-toolchain\
	 mingw-w64-x86_64-toolchain
build: bin/$(version)/$(label)-$(arch)-msvcrt.exe\
       bin/$(version)/$(label)-$(arch)-ucrt.exe
debug: $(main_src) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(cc) $(main_src) $(def) $(args_debug) -o ./bin/debug/__debug__.exe
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf bin
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\touch.exe bin/.gitkeep
bin/$(version)/$(label)-$(arch)-msvcrt.exe: $(main_src) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\$(cc) $(main_src) $(def) $(args_release) -o $@
bin/$(version)/$(label)-$(arch)-ucrt.exe: $(main_src) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(cc) $(main_src) $(def) $(args_release) -o $@
bin/info.obj: info.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< $(def) -o $@ -F pe-x86-64
bin: bin/$(version) bin/debug
bin/$(version):
	$(msys2)\\usr\\bin\\mkdir.exe $@ -p
bin/debug:
	$(msys2)\\usr\\bin\\mkdir.exe $@ -p