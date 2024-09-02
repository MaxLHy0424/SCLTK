msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -C -std=gnu++23 -Wall -Wextra -pipe -m64 -DNDEBUG -static -Ofast -Os -flto -o
.PHONY: build clean
build: bin/x86_64/CRCSN-msvcrt.exe bin/x86_64/CRCSN-ucrt.exe
obj = src/*.cpp bin/info.obj
bin/info.obj: info.rc img/favicon.ico bin
	$(msys2)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-x86-64
bin/x86_64/CRCSN-msvcrt.exe: $(obj) src/*.hpp bin
	$(msys2)\\mingw64\\bin\\g++.exe $(obj) $(argv) $@
bin/x86_64/CRCSN-ucrt.exe: $(obj) src/*.hpp bin
	$(msys2)\\ucrt64\\bin\\g++.exe $(obj) $(argv) $@
bin: bin/x86_64
bin/x86_64:
	$(msys2)\\usr\\bin\\mkdir.exe bin/x86_64 -p
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf\
	 bin/__debug__.exe\
	 bin/info.obj\
	 bin/x86_64