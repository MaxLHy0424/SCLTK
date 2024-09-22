msys2=C:\\Software\\MSYS2
CC=g++.exe
args=-C -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Os -flto -o
.PHONY:build clean
arch=x86_64
version=v5.6.4
build:bin/$(version)/main-$(arch)-msvcrt.exe bin/$(version)/main-$(arch)-ucrt.exe
obj=src/*.cpp bin/info.obj
bin/$(version)/main-$(arch)-msvcrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\$(CC) $(obj) $(args) $@
bin/$(version)/main-$(arch)-ucrt.exe:$(obj) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\$(CC) $(obj) $(args) $@
bin/info.obj:info.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-x86-64
bin:bin/$(version)
bin/$(version):
	$(msys2)\\usr\\bin\\mkdir.exe bin/$(version) -p
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf bin
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\touch.exe bin/.gitkeep