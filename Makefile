msys2 = C:\\Software\\MSYS2
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -DNDEBUG -static -Ofast -Os -flto -o
branch = dev
arch = x86_64
binTpl = .\\bin\\$(branch)-$(arch)
rvLegacy = $(binTpl)-msvcrt.exe
rvModern = $(binTpl)-ucrt.exe
build:
	$(msys2)\\mingw64\\bin\\g++.exe main.cpp $(argv) $(rvLegacy)
	$(msys2)\\ucrt64\\bin\\g++.exe main.cpp $(argv) $(rvModern)
clean:
	$(msys2)\\usr\\bin\\rm.exe -rf $(rvLegacy) $(rvModern) .\\bin\\main.debug.exe
.PHONY: build clean