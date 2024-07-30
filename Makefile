pathMsys2 = C:\Software\MSYS2
compiler = g++.exe
argv = -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -pipe -DNDEBUG -static -Ofast -Os -flto -o
arch = x86_64
main:
	$(pathMsys2)\mingw64\bin\$(compiler) main.cpp $(argv) .\bin\CRCSN-$(arch)-msvcrt.exe
	$(pathMsys2)\ucrt64\bin\$(compiler) main.cpp $(argv) .\bin\CRCSN-$(arch)-ucrt.exe