msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -C -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Ofast -Os -flto -o
build: bin/x86_64/main-msvcrt.exe bin/x86_64/main-ucrt.exe
bin/x86_64:
	$(msys2)\\usr\\bin\\mkdir.exe bin/x86_64 -p
bin: bin/x86_64
usedFile = src/main.cpp bin/favicon.o
bin/x86_64/main-msvcrt.exe: $(usedFile) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\g++.exe $(usedFile) $(argv) $@
bin/x86_64/main-ucrt.exe: $(usedFile) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\g++.exe $(usedFile) $(argv) $@
bin/favicon.o: favicon.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-x86-64
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/__debug__.exe\
	 bin/favicon.o\
	 bin/x86_64
.PHONY: build clean