include_path     := include
cpp_utils_path   := $(include_path)/cpp_utils
include $(cpp_utils_path)/all.mk
project_name     := SCLTK-Legacy
compiler         := /mingw32/bin/g++
args_arch        := -march=x86-64 -mtune=generic -msse3 -mfpmath=sse
args_defines     := -D{UNICODE,_UNICODE,SCLTK_LEGACY}
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
                    -fno-ident \
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
                    -fvect-cost-model=dynamic \
                    -fipa-cp-clone \
                    -fipa-bit-cp \
                    -fisolate-erroneous-paths-attribute \
                    -fsched-pressure \
                    -frename-registers \
                    -fmerge-all-constants \
                    -fno-math-errno \
                    -fmodulo-sched \
                    -fmodulo-sched-allow-regmoves \
                    -fgraphite-identity \
                    -floop-nest-optimize \
                    -D_FORTIFY_SOURCE=0
args_include     := -I$(include_path)
args_library     := -ltre -liphlpapi -lsetupapi -lcrypt32 -lwinhttp -lwininet -lversion -ldnsapi
args_extra       := $(cpp_utils_windows_impl_win7_args)
input_charset    := utf-8
output_charset   := gbk
args_base        := -pipe -finput-charset=$(input_charset) -fexec-charset=$(output_charset) \
                    -std=$(args_std) $(args_warning) $(args_defines) $(args_include) \
                    $(args_library) $(args_extra)
args_debug       := -g3 -fuse-ld=lld -DDEBUG $(args_base) $(args_opt_debug) -fstack-protector-all -fstack-clash-protection
args_release     := -DNDEBUG -static $(args_base) $(args_opt_release)
args_ld          := -Wl,-O3,--gc-sections,--strip-all,--as-needed,--build-id=none \
                    -Wl,--no-insert-timestamp,--no-seh,--disable-runtime-pseudo-reloc \
                    -Wl,--disable-auto-import,--dynamicbase,--nxcompat,--tsaware
cmd_echo         := /usr/bin/echo
cmd_upx          := /ucrt64/bin/upx --lzma --best --8-bit --no-align --ultra-brute -qqq
cmd_gpg          := gpg -bs -u $(gpg_key) --yes
dep_test         := src/main.cpp \
                    src/clear_winhttp_proxy.cpp \
                    src/clear_wininet_proxy.cpp \
                    meta/legacy/info.h \
                    $(cpp_utils_all_files)
dep_debug        := src/*.cpp
dep_release      := build/legacy/manifest.o \
                    src/*.cpp
dep_res          := meta/manifest.rc \
                    meta/manifest.xml \
                    meta/favicon.ico
.PHONY: toolchain build debug release pack_and_sign clean
build: debug release
debug: build/legacy/debug/__debug__.exe
release: build/legacy/release/$(project_name).exe
toolchain:
	/usr/bin/pacman -Sy --noconfirm --needed\
     mingw-w64-i686-toolchain\
     mingw-w64-ucrt-x86_64-7zip\
     mingw-w64-ucrt-x86_64-upx\
     base\
     base-devel\
     binutils
pack_and_sign: build
	@$(cmd_echo) "Signing binaries..."
	@$(cmd_gpg) build/legacy/release/$(project_name).exe
	@$(cmd_echo) "Removing old package..."
	@/usr/bin/rm -rf build/legacy/$(project_name).7z
	@$(cmd_echo) "Copying binaries, signatures, and the LICENSE.txt..."
	@/usr/bin/mkdir build/legacy/__temp__ -p
	@/usr/bin/cp build/legacy/release/*.exe build/legacy/__temp__/
	@/usr/bin/cp build/legacy/release/*.sig build/legacy/__temp__/
	@/usr/bin/cp LICENSE.txt build/legacy/__temp__/
	@/usr/bin/cp -r THIRD-PARTY-LICENSES build/mainline/__temp__/
	@$(cmd_echo) "Compressing to '$(project_name).7z'..."
	@/ucrt64/bin/7z a -bso0 -bsp0 -mx9 -m0=LZMA2 -md=64m -mfb=64 -ms=16g -mmt=16 build/legacy/$(project_name).7z ./build/legacy/__temp__/*
	@$(cmd_echo) "Signing '$(project_name).7z'..."
	@$(cmd_gpg) build/legacy/$(project_name).7z
	@$(cmd_echo) "Cleaning 'build/legacy/__temp__'..."
	@/usr/bin/rm -rf build/legacy/__temp__
clean:
	@$(cmd_echo) "Cleaning..."
	@/usr/bin/rm -rf build/legacy
	@/usr/bin/rm -rf meta/legacy/info.h
build/legacy/debug/__debug__.exe: $(dep_test) \
                                  build/legacy/debug/.gitkeep
	@$(cmd_echo) "Compiling '$@'..."
	@$(compiler) $(dep_debug) $(args_debug) -o $@
build/legacy/release/$(project_name).exe: $(dep_test) \
                                          $(dep_release) \
                                          build/legacy/release/.gitkeep
	@$(cmd_echo) "Compiling '$@'..."
	@$(compiler) $(dep_release) $(args_release) $(args_arch) $(args_ld) -o $@
	@$(cmd_echo) "Compressing '$@'..."
	@$(cmd_upx) $@
build/legacy/manifest.o: $(dep_res) \
                         build/legacy/.gitkeep
	@$(cmd_echo) "Generating '$@'..."
	@/usr/bin/windres -i $< -o $@ $(args_defines) -c 65001 -F pe-i386
build/legacy/.gitkeep:
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build/legacy -p
	@/usr/bin/touch $@
build/legacy/debug/.gitkeep: build/legacy/.gitkeep
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build/legacy/debug -p
	@/usr/bin/touch $@
build/legacy/release/.gitkeep: build/legacy/.gitkeep
	@$(cmd_echo) "Creating '$@'..."
	@/usr/bin/mkdir build/legacy/release -p
	@/usr/bin/touch $@