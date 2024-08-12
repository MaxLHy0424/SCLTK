msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Ofast -Os -flto -o
build: bin/x86_64 bin/x86_64/main-msvcrt.exe bin/x86_64/main-ucrt.exe
bin/x86_64:
	$(msys2)\\usr\\bin\\mkdir.exe bin/x86_64 -p
mainFile = src/main.cpp bin/favicon.o
bin/x86_64/main-msvcrt.exe: $(mainFile) src/*.hpp bin/x86_64
	$(msys2)\\mingw64\\bin\\g++.exe $(mainFile) $(argv) $@
bin/x86_64/main-ucrt.exe: $(mainFile) src/*.hpp bin/x86_64
	$(msys2)\\ucrt64\\bin\\g++.exe $(mainFile) $(argv) $@
bin/favicon.o: favicon.rc img/favicon.ico bin/x86_64
	$(msys2)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-x86-64
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/__debug.exe\
	 bin/favicon.o\
	 bin/x86_64
.PHONY: build clean