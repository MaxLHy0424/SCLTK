include_path          := include
cpp_utils_path        := $(include_path)/cpp_utils
include $(cpp_utils_path)/all.mk
project_name          := SCLTK
x86_64_compiler       := /ucrt64/bin/g++
i686_compiler         := /mingw32/bin/g++
args_arch_32          := -march=pentium4 -mtune=generic -msse2 -mfpmath=sse
args_arch_64          := -march=x86-64 -mtune=generic -msse3 -mfpmath=sse
args_defines          := -D{UNICODE,_UNICODE}
args_std              := gnu++26
args_warning          := -W{all,extra,effc++,pedantic,cast-align,logical-op,redundant-decls,shadow,strict-null-sentinel}
args_opt_debug        := -Og -fno-omit-frame-pointer
args_opt_release_base := -Ofast \
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
                         -fno-stack-protector \
                         -fno-stack-clash-protection \
                         -fno-semantic-interposition \
                         -fdevirtualize-at-ltrans \
                         -fdevirtualize-speculatively \
                         -fipa-pta \
                         -fipa-ra \
                         -fipa-icf \
                         -fomit-frame-pointer \
                         -fno-plt \
                         -fstrict-aliasing \
						 -fassociative-math \
						 -freciprocal-math \
                         -fmerge-all-constants \
                         -fno-math-errno \
                         -fmodulo-sched \
                         -fmodulo-sched-allow-regmoves \
						 -fgraphite-identity \
						 -floop-nest-optimize \
                         -D_FORTIFY_SOURCE=0
args_opt_release_32   := $(args_opt_release_base)
args_opt_release_64   := $(args_opt_release_base) -flto=auto
args_include          := -I$(include_path)
args_library          := -liphlpapi -lsetupapi -lcrypt32
args_extra            :=
input_charset         := utf-8
output_charset        := gbk
args_base             := -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) \
                         -std=$(args_std) $(args_warning) $(args_defines) $(args_include) \
                         $(args_library) $(args_extra)
args_debug            := -g3 -fuse-ld=lld -DDEBUG $(args_base) $(args_opt_debug) -fstack-protector-all -fstack-clash-protection
args_release_32       := -DNDEBUG -static $(args_base) $(args_opt_release_32)
args_release_64       := -DNDEBUG -static $(args_base) $(args_opt_release_64)
args_ld_base          := -fuse-ld=lld \
                         -Wl,-O3,--lto-O3,--lto-CGO3,--gc-sections,--strip-all,--as-needed \
                         -Wl,--no-insert-timestamp,--no-seh,--disable-runtime-pseudo-reloc \
                         -Wl,--disable-auto-import,--dynamicbase,--nxcompat,--high-entropy-va,--tsaware \
                         -Wl,--icf=all,--build-id=none
args_ld_32            := $(args_ld_base)
args_ld_64            := $(args_ld_base)
cmd_echo              := /usr/bin/echo
cmd_upx               := /ucrt64/bin/upx --lzma --best --8-bit --no-align --ultra-brute -qqq
cmd_gpg               := gpg -bs -u $(gpg_key) --yes
dep_test              := src/main.cpp meta/info.h $(cpp_utils_all_files)
dep_debug             := src/*.cpp
dep_release_32        := build/manifest-i686.o \
                         src/*.cpp
dep_release_64        := build/manifest-x86_64.o \
                         src/*.cpp
dep_res               := meta/manifest.rc \
                         meta/SCLTK.ico \
                         meta/manifest.xml \
                         meta/info.h
.PHONY: toolchain all build debug release release-32 release-64 pack_and_sign clean
.NOTPARALLEL: all
all: toolchain build pack_and_sign
build: debug release
debug: build/debug/__debug__.exe
release: release-32 release-64
release-32: build/release/$(project_name)-i686-msvcrt.exe
release-64: build/release/$(project_name)-x86_64-ucrt.exe
toolchain:
	/usr/bin/pacman -Sy --noconfirm --needed\
     mingw-w64-i686-toolchain\
     mingw-w64-ucrt-x86_64-toolchain\
     mingw-w64-ucrt-x86_64-lld\
     mingw-w64-i686-lld\
     mingw-w64-ucrt-x86_64-7zip\
     mingw-w64-ucrt-x86_64-upx\
     base\
     base-devel\
     binutils
pack_and_sign: build
	@$(cmd_echo) "Signing binaries..."
	@$(cmd_gpg) build/release/$(project_name)-i686-msvcrt.exe
	@$(cmd_gpg) build/release/$(project_name)-x86_64-ucrt.exe
	@$(cmd_echo) "Removing old package..."
	@/usr/bin/rm -rf build/$(project_name).7z
	@$(cmd_echo) "Copying binaries, signatures, and the LICENSE.txt..."
	@/usr/bin/mkdir build/__temp__ -p
	@/usr/bin/cp build/release/*.exe build/__temp__/
	@/usr/bin/cp build/release/*.sig build/__temp__/
	@/usr/bin/cp LICENSE.txt build/__temp__/
	@$(cmd_echo) "Compressing to '$(project_name).7z'..."
	@/ucrt64/bin/7z a -bso0 -bsp0 -mx9 -m0=LZMA2 -md=64m -mfb=64 -ms=16g -mmt=16 build/$(project_name).7z ./build/__temp__/*
	@$(cmd_echo) "Signing '$(project_name).7z'..."
	@$(cmd_gpg) build/$(project_name).7z
	@$(cmd_echo) "Cleaning 'build/__temp__'..."
	@/usr/bin/rm -rf build/__temp__
clean:
	@$(cmd_echo) "Cleaning..."
	@/usr/bin/rm -rf build
	@/usr/bin/rm -rf meta/info.h
build/debug/__debug__.exe: $(dep_test) \
                           build/debug/.nothing
	@$(cmd_echo) "Compiling '$@'..."
	@$(x86_64_compiler) $(dep_debug) $(args_debug) -o $@
build/release/$(project_name)-i686-msvcrt.exe: $(dep_test) \
                                               $(dep_release_32) \
                                               build/release/.nothing
	@$(cmd_echo) "Compiling '$@'..."
	@$(i686_compiler) $(dep_release_32) $(args_release_32) $(args_arch_32) $(args_ld_32) -o $@
	@$(cmd_echo) "Compressing '$@'..."
	@$(cmd_upx) $@
build/release/$(project_name)-x86_64-ucrt.exe: $(dep_test) \
                                               $(dep_release_64) \
                                               build/release/.nothing
	@$(cmd_echo) "Compiling '$@'..."
	@$(x86_64_compiler) $(dep_release_64) $(args_release_64) $(args_arch_64) $(args_ld_64) -o $@
	@$(cmd_echo) "Compressing '$@'..."
	@$(cmd_upx) $@
build/manifest-i686.o: $(dep_res) \
                       build/.nothing
	@$(cmd_echo) "Generating '$@'..."
	@/usr/bin/windres -i $< -o $@ $(args_defines) -c 65001 -F pe-i386
build/manifest-x86_64.o: $(dep_res) \
                         build/.nothing
	@$(cmd_echo) "Generating '$@'..."
	@/usr/bin/windres -i $< -o $@ $(args_defines) -c 65001 -F pe-x86-64
build/.nothing:
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build -p
	@/usr/bin/touch $@
build/debug/.nothing: build/.nothing
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build/debug -p
	@/usr/bin/touch $@
build/release/.nothing: build/.nothing
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build/release -p
	@/usr/bin/touch $@