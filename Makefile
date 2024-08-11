msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Ofast -Os -flto -o
build: bin/x86_64 bin/x86_64/main-msvcrt.exe bin/x86_64/main-ucrt.exe
	$(msys2)\\usr\\bin\\rm.exe -rf bin/favicon.o
bin/x86_64:
	$(msys2)\\usr\\bin\\mkdir.exe bin
	$(msys2)\\usr\\bin\\mkdir.exe bin/x86_64
bin/x86_64/main-msvcrt.exe: bin/favicon.o
	$(msys2)\\mingw64\\bin\\g++.exe src/main.cpp $^ $(argv) $@
bin/x86_64/main-ucrt.exe: bin/favicon.o
	$(msys2)\\ucrt64\\bin\\g++.exe src/main.cpp $^ $(argv) $@
bin/favicon.o:
	$(msys2)\\usr\\bin\\windres.exe -i favicon.rc -o bin/favicon.o
clean_std:
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/__debug.exe\
	 bin/favicon.o
clean_full: clean_std
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/x86_64/main-msvcrt.exe\
	 bin/x86_64/main-ucrt.exe
.PHONY: build clean_std clean_full