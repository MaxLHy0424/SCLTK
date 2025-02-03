include msys2
compiler         = g++.exe
args_defines     = -D_CRT_SECURE_NO_WARNINGS -DANSI -D_ANSI
args_link        =
args_std         = gnu++26
args_warning     = -Wall -Wextra -Weffc++
args_opt_debug   = -Og
args_opt_release = -Ofast -flto=auto -fno-rtti
input_charset    = utf-8
output_charset   = gb18030
args_base        = -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) -std=$(args_std) $(args_link) $(args_warning) $(args_defines)
args_debug       = -g3 $(args_base) $(args_opt_debug)
args_release     = -DNDEBUG -static $(args_base) $(args_opt_release)
.PHONY: init release debug clean
all: init clean release debug
init:
	$(msys2_path)\\usr\\bin\\pacman.exe -Syyu --noconfirm --needed\
	 mingw-w64-i686-toolchain\
	 mingw-w64-ucrt-x86_64-toolchain\
	 make\
	 git\
	 base\
	 base-devel\
	 binutils
release: bin/release/CRCSN-i686-msvcrt.exe\
         bin/release/CRCSN-x86_64-ucrt.exe
debug: src/* bin/debug/.gitkeep
	$(msys2_path)\\ucrt64\\bin\\$(compiler) src/*.cpp $(args_defines) $(args_debug) -o ./bin/debug/__debug__.exe
clean:
	$(msys2_path)\\usr\\bin\\rm.exe -rf bin
	$(msys2_path)\\usr\\bin\\mkdir.exe bin
	$(msys2_path)\\usr\\bin\\touch.exe bin/.gitkeep
bit32_executable_dependent_items = src/* bin/info-i686.obj
bit64_executable_dependent_items = src/* bin/info-x86_64.obj
bin/release/CRCSN-i686-msvcrt.exe: $(bit32_executable_dependent_items) bin/release/.gitkeep
	$(msys2_path)\\mingw32\\bin\\$(compiler) $(bit32_executable_dependent_items) $(args_release) -o $@
bin/release/CRCSN-x86_64-ucrt.exe: $(bit64_executable_dependent_items) bin/release/.gitkeep
	$(msys2_path)\\ucrt64\\bin\\$(compiler) $(bit64_executable_dependent_items) $(args_release) -o $@
info_obj_dependent_items = info.rc img/favicon.ico src/info.hpp
bin/info-i686.obj: $(info_obj_dependent_items) bin/.gitkeep
	$(msys2_path)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-i386
bin/info-x86_64.obj: $(info_obj_dependent_items) bin/.gitkeep
	$(msys2_path)\\usr\\bin\\windres.exe -i $< -o $@ -F pe-x86-64
bin/.gitkeep:
	$(msys2_path)\\usr\\bin\\mkdir.exe bin -p
	$(msys2_path)\\usr\\bin\\touch.exe $@
bin/release/.gitkeep: bin/.gitkeep
	$(msys2_path)\\usr\\bin\\mkdir.exe bin/release -p
	$(msys2_path)\\usr\\bin\\touch.exe $@
bin/debug/.gitkeep: bin/.gitkeep
	$(msys2_path)\\usr\\bin\\mkdir.exe bin/debug -p
	$(msys2_path)\\usr\\bin\\touch.exe $@