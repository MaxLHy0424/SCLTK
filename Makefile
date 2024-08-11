msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Ofast -Os -flto -o
build: bin bin/CRCSN-x86_64-msvcrt.exe bin/CRCSN-x86_64-ucrt.exe
	$(msys2)\\usr\\bin\\rm.exe -rf bin/favicon.o
bin:
	$(msys2)\\usr\\bin\\mkdir.exe bin
bin/CRCSN-x86_64-msvcrt.exe: bin/favicon.o
	$(msys2)\\mingw64\\bin\\g++.exe src/main.cpp $^ $(argv) $@
bin/CRCSN-x86_64-ucrt.exe: bin/favicon.o
	$(msys2)\\ucrt64\\bin\\g++.exe src/main.cpp $^ $(argv) $@
bin/favicon.o:
	$(msys2)\\usr\\bin\\windres.exe -i favicon.rc -o bin/favicon.o
clean_std:
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/__debug.exe\
	 bin/favicon.o
clean_full: clean_std
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/CRCSN-x86_64-msvcrt.exe\
	 bin/CRCSN-x86_64-ucrt.exe
.PHONY: build clean