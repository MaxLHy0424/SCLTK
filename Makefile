include env
name             = SCLTK
compiler         = g++.exe
args_defines     = -D{ANSI,_ANSI}
args_std         = gnu++26
args_warning     = -W{all,extra,effc++,pedantic}
args_opt_debug   = -Og
args_opt_release = -O3 -flto=auto -fuse-linker-plugin -s -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Wl,--gc-sections
args_include     = -I./include
args_library     =
args_extra       =
input_charset    = utf-8
output_charset   = gbk
args_base        = -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) -std=$(args_std) $(args_warning) $(args_defines) $(args_include) $(args_library) $ $(args_extra)
args_debug       = -g3 -DDEBUG $(args_base) $(args_opt_debug)
args_release     = -DNDEBUG -static $(args_base) $(args_opt_release)
.PHONY: toolchain all build debug release pack clean make_info
dependencies_testing = src/* include*
all: toolchain build pack
build: debug release
pack:
	$(msys2_path)/usr/bin/rm.exe -rf build/$(name).7z
	$(msys2_path)/usr/bin/mkdir.exe build/__temp__ -p
	$(msys2_path)/usr/bin/cp.exe build/release/*.exe build/__temp__/
	$(msys2_path)/usr/bin/cp.exe LICENSE build/__temp__/
	$(msys2_path)/ucrt64/bin/7z.exe a -mx=9 build/$(name).7z ./build/__temp__/*
	$(msys2_path)/usr/bin/rm.exe -rf build/__temp__
toolchain:
	$(msys2_path)/usr/bin/pacman.exe -Sy --noconfirm --needed\
     mingw-w64-i686-toolchain\
     mingw-w64-ucrt-x86_64-toolchain\
     make\
	 mingw-w64-ucrt-x86_64-7zip\
     git\
     base\
     base-devel\
     binutils
debug: build/debug/__debug__.exe
release: build/release/$(name)-i686-msvcrt.exe\
         build/release/$(name)-x86_64-ucrt.exe
clean:
	$(msys2_path)/usr/bin/rm.exe -rf build
	$(msys2_path)/usr/bin/rm.exe -rf src/info.hpp
	$(msys2_path)/usr/bin/mkdir.exe build
	$(msys2_path)/usr/bin/touch.exe build/.nothing
make_info:
	$(pwsh_path) -ExecutionPolicy Bypass -File make_info.ps1
src/info.hpp: make_info
dependencies_debug = src/*.cpp
build/debug/__debug__.exe: $(dependencies_testing) \
                           $(dependencies_debug) \
                           make_info \
                           build/debug/.nothing
	$(msys2_path)/ucrt64/bin/$(compiler) $(dependencies_debug) $(args_debug) -o $@
dependencies_release_32bit = build/manifest-i686.o \
                             src/*.cpp
build/release/$(name)-i686-msvcrt.exe: $(dependencies_testing) \
                                     $(dependencies_release_32bit) \
                                     make_info \
                                     build/release/.nothing
	$(msys2_path)/mingw32/bin/$(compiler) $(dependencies_release_32bit) $(args_release) -o $@
dependencies_release_64bit = build/manifest-x86_64.o \
                             src/*.cpp
build/release/$(name)-x86_64-ucrt.exe: $(dependencies_testing) \
                                     $(dependencies_release_64bit) \
                                     make_info \
                                     build/release/.nothing
	$(msys2_path)/ucrt64/bin/$(compiler) $(dependencies_release_64bit) $(args_release) -o $@
dependencies_info = manifest.rc \
                    img/favicon.ico \
					manifest.xml \
                    src/info.hpp
build/manifest-i686.o: $(dependencies_info) \
                       make_info \
                       build/.nothing
	$(msys2_path)/usr/bin/windres.exe -i $< -o $@ $(args_defines) -c 65001 -F pe-i386
build/manifest-x86_64.o: $(dependencies_info) \
                         make_info \
                         build/.nothing
	$(msys2_path)/usr/bin/windres.exe -i $< -o $@ $(args_defines) -c 65001 -F pe-x86-64
build/.nothing:
	$(msys2_path)/usr/bin/mkdir.exe build -p
	$(msys2_path)/usr/bin/touch.exe $@
build/debug/.nothing: build/.nothing
	$(msys2_path)/usr/bin/mkdir.exe build/debug -p
	$(msys2_path)/usr/bin/touch.exe $@
build/release/.nothing: build/.nothing
	$(msys2_path)/usr/bin/mkdir.exe build/release -p
	$(msys2_path)/usr/bin/touch.exe $@