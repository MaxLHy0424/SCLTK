msys2Path = C:\Software\MSYS2
main:
	$(msys2Path)\mingw64\bin\g++.exe main.cpp\
	 -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -static -Ofast -Os -flto -o\
	 .\bin\main_msvcrt64.exe
	$(msys2Path)\ucrt64\bin\g++.exe main.cpp\
	 -fexec-charset=GBK -std=gnu++23 -Wall -Wextra -static -Ofast -Os -flto -o\
	 .\bin\main_ucrt64.exe