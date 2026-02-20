include_path   := include
cpp_utils_path := $(include_path)/cpp_utils
include env.mk
include $(cpp_utils_path)/all.mk
project_name     := SCLTK
x86_64_compiler  := $(msys2_path)/ucrt64/bin/g++.exe
i686_compiler    := $(msys2_path)/mingw32/bin/g++.exe
args_arch_i686   := -march=pentium4 -mtune=generic -msse2 -mfpmath=sse
args_arch_x86_64 := -march=x86-64 -mtune=generic -msse3 -mfpmath=sse
args_defines     := -D{UNICODE,_UNICODE}
args_std         := gnu++26
args_warning     := -W{all,extra,effc++,pedantic,cast-align,logical-op,redundant-decls,shadow,strict-null-sentinel}
args_opt_debug   := -Og -fno-omit-frame-pointer
args_opt_release := -Ofast \
                   -flto=auto \
                   -fno-use-linker-plugin \
                   -fwhole-program \
                   -s \
                   -fvisibility=hidden \
                   -fvisibility-inlines-hidden \
                   -fno-rtti \
                   -fno-exceptions \
                   -fno-unwind-tables \
                   -fno-asynchronous-unwind-tables \
                   -fno-common \
                   -ffunction-sections \
                   -fdata-sections \
                   -fno-semantic-interposition \
                   -fdevirtualize-at-ltrans \
                   -fdevirtualize-speculatively \
                   -fipa-pta \
                   -fipa-ra \
                   -fipa-icf \
                   -fomit-frame-pointer \
                   -fno-plt
args_include     := -I$(include_path)
args_library     :=
args_extra       :=
input_charset    := utf-8
output_charset   := gbk
args_base        := -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) \
                    -std=$(args_std) $(args_warning) $(args_defines) $(args_include) \
                    $(args_library) $(args_extra)
args_debug       := -g3 -fuse-ld=lld -DDEBUG $(args_base) $(args_opt_debug) -fstack-protector-strong
args_release     := -DNDEBUG -static $(args_base) $(args_opt_release)
args_ld_base     := -fuse-ld=lld -Wl,-O3,--lto-O3,--lto-CGO3,--gc-sections,--strip-all,--as-needed,--no-insert-timestamp,--no-seh,--disable-runtime-pseudo-reloc,--disable-auto-import,--dynamicbase,--nxcompat,--high-entropy-va,--tsaware,--icf=all
args_ld_i686     := $(args_ld_base)
args_ld_x86_64   := $(args_ld_base)
args_upx         := --lzma --best --8-bit --no-align --ultra-brute -qq
.PHONY: toolchain all build debug release pack_and_sign clean
.NOTPARALLEL: all
dependencies_base := src/* src/info.hpp $(cpp_utils_all_files)
all: toolchain build pack_and_sign
build: debug release
gpg_command := $(gpg_path) -bs -u $(gpg_key) --yes
pack_and_sign:
	$(gpg_command) build/release/$(project_name)-i686-msvcrt.exe
	$(gpg_command) build/release/$(project_name)-x86_64-ucrt.exe
	$(msys2_path)/usr/bin/rm.exe -rf build/$(project_name).7z
	$(msys2_path)/usr/bin/mkdir.exe build/__temp__ -p
	$(msys2_path)/usr/bin/cp.exe build/release/*.exe build/__temp__/
	$(msys2_path)/usr/bin/cp.exe build/release/*.sig build/__temp__/
	$(msys2_path)/usr/bin/cp.exe LICENSE.txt build/__temp__/
	$(msys2_path)/ucrt64/bin/7z.exe a -mx9 -m0=LZMA2 -md=64m -mfb=64 -ms=16g -mmt=16 build/$(project_name).7z ./build/__temp__/*
	$(gpg_command) build/SCLTK.7z
	$(msys2_path)/usr/bin/rm.exe -rf build/__temp__
toolchain:
	$(msys2_path)/usr/bin/pacman.exe -Sy --noconfirm --needed\
     mingw-w64-i686-toolchain\
     mingw-w64-ucrt-x86_64-toolchain\
     mingw-w64-ucrt-x86_64-lld\
     mingw-w64-i686-lld\
     mingw-w64-ucrt-x86_64-7zip\
     mingw-w64-ucrt-x86_64-upx\
     base\
     base-devel\
     binutils
debug: build/debug/__debug__.exe
release: build/release/$(project_name)-i686-msvcrt.exe \
         build/release/$(project_name)-x86_64-ucrt.exe
clean:
	$(msys2_path)/usr/bin/rm.exe -rf build
	$(msys2_path)/usr/bin/rm.exe -rf src/info.hpp
	$(msys2_path)/usr/bin/mkdir.exe build
	$(msys2_path)/usr/bin/touch.exe build/.nothing
dependencies_debug := src/*.cpp
build/debug/__debug__.exe: $(dependencies_base) \
                           build/debug/.nothing
	$(x86_64_compiler) $(dependencies_debug) $(args_debug) -o $@
dependencies_release_32bit := build/manifest-i686.o \
                              src/*.cpp
build/release/$(project_name)-i686-msvcrt.exe: $(dependencies_base) \
                                               $(dependencies_release_32bit) \
                                               build/release/.nothing
	$(i686_compiler) $(dependencies_release_32bit) $(args_release) $(args_arch_i686) $(args_ld_i686) -o $@
	$(msys2_path)/ucrt64/bin/upx.exe $@ $(args_upx)
dependencies_release_64bit := build/manifest-x86_64.o \
                              src/*.cpp
build/release/$(project_name)-x86_64-ucrt.exe: $(dependencies_base) \
                                               $(dependencies_release_64bit) \
                                               build/release/.nothing
	$(x86_64_compiler) $(dependencies_release_64bit) $(args_release) $(args_arch_x86_64) $(args_ld_x86_64) -o $@
	$(msys2_path)/ucrt64/bin/upx.exe $@ $(args_upx)
dependencies_info := manifest.rc \
                     img/favicon.ico \
                     manifest.xml \
                     src/info.hpp
build/manifest-i686.o: $(dependencies_info) \
                       src/info.hpp \
                       build/.nothing
	$(msys2_path)/usr/bin/windres.exe -i $< -o $@ $(args_defines) -c 65001 -F pe-i386
build/manifest-x86_64.o: $(dependencies_info) \
                         src/info.hpp \
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