include_path     := include
cpp_utils_path   := $(include_path)/cpp_utils
include $(cpp_utils_path)/all.mk
project_name     := SCLTK
compiler         := /ucrt64/bin/g++
args_arch        := -march=x86-64 -mtune=generic -msse3 -mfpmath=sse
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
                    -ffast-math \
                    -freciprocal-math \
                    -fmerge-all-constants \
                    -fno-math-errno \
                    -fmodulo-sched \
                    -fmodulo-sched-allow-regmoves \
                    -fgraphite-identity \
                    -floop-nest-optimize \
                    -D_FORTIFY_SOURCE=0
args_include     := -I$(include_path)
args_library     := -liphlpapi -lsetupapi -lcrypt32 -lwinhttp -lwininet
args_extra       :=
input_charset    := utf-8
output_charset   := gbk
args_base        := -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) \
                    -std=$(args_std) $(args_warning) $(args_defines) $(args_include) \
                    $(args_library) $(args_extra)
args_debug       := -g3 -fuse-ld=lld -DDEBUG $(args_base) $(args_opt_debug) -fstack-protector-all -fstack-clash-protection
args_release     := -DNDEBUG -static $(args_base) $(args_opt_release)
args_ld          := -fuse-ld=lld \
                    -Wl,-O3,--lto-O3,--lto-CGO3,--gc-sections,--strip-all,--as-needed \
                    -Wl,--no-insert-timestamp,--no-seh,--disable-runtime-pseudo-reloc \
                    -Wl,--disable-auto-import,--dynamicbase,--nxcompat,--high-entropy-va,--tsaware \
                    -Wl,--icf=all,--build-id=none
cmd_echo         := /usr/bin/echo
cmd_upx          := /ucrt64/bin/upx --lzma --best --8-bit --no-align --ultra-brute -qqq
cmd_gpg          := gpg -bs -u $(gpg_key) --yes
dep_test         := src/main.cpp \
                    src/clear_winhttp_proxy.cpp \
                    src/clear_wininet_proxy.cpp \
                    meta/info.h \
                    $(cpp_utils_all_files)
dep_debug        := src/*.cpp
dep_release      := build/manifest.o \
                    src/*.cpp
dep_res          := meta/manifest.rc \
                    meta/SCLTK.ico \
                    meta/manifest.xml \
                    meta/info.h
.PHONY: toolchain build debug release pack_and_sign clean
build: debug release
debug: build/debug/__debug__.exe
release: build/release/$(project_name).exe
toolchain:
	/usr/bin/pacman -Sy --noconfirm --needed\
     mingw-w64-ucrt-x86_64-toolchain\
     mingw-w64-ucrt-x86_64-lld\
     mingw-w64-ucrt-x86_64-7zip\
     mingw-w64-ucrt-x86_64-upx\
     base\
     base-devel\
     binutils
pack_and_sign: build
	@$(cmd_echo) "Signing binaries..."
	@$(cmd_gpg) build/release/$(project_name).exe
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
	@$(compiler) $(dep_debug) $(args_debug) -o $@
build/release/$(project_name).exe: $(dep_test) \
                                   $(dep_release) \
                                   build/release/.nothing
	@$(cmd_echo) "Compiling '$@'..."
	@$(compiler) $(dep_release) $(args_release) $(args_arch) $(args_ld) -o $@
	@$(cmd_echo) "Compressing '$@'..."
	@$(cmd_upx) $@
build/manifest.o: $(dep_res) \
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