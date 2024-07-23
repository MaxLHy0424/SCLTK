pathMsys2 = C:\Software\MSYS2
compiler = g++.exe
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -static -Ofast -Os -flto -o
branch = dev
arch = x86_64
compile:
	$(pathMsys2)\mingw64\bin\$(compiler) main.cpp $(argv) .\bin\$(branch)-$(arch)-msvcrt.exe
	$(pathMsys2)\ucrt64\bin\$(compiler) main.cpp $(argv) .\bin\$(branch)-$(arch)-ucrt.exe