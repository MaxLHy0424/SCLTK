$is_insider_build = $true
$software_full_name = "Student Computer Lab Toolkit"
$software_short_name = "SCLTK"
$license = "MIT License"
$copyright = "Copyright (C) 2023 - present MaxLHy0424."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_branch = git branch --show-current
$git_hash = git rev-parse --short HEAD
$build_time = Get-Date -AsUTC -Format "yyyy/MM/dd HH:mm:ss"
if ( $is_insider_build -eq $true ) {
    $git_tag = "evaluation_copy"  
}
else {
    $git_tag = git describe --tags --abbrev=0
}
@"
#pragma once
#define STRINGIFY( x ) #x
#define TO_STRING( x ) STRINGIFY( x )
#define INFO_FULL_NAME  "$software_full_name"
#define INFO_SHORT_NAME "$software_short_name"
#define INFO_LICENSE    "$license"
#define INFO_COPYRIGHT  "$copyright"
#define INFO_REPO_URL   "$repo_url"
#define INFO_GIT_BRANCH "$git_branch"
#define INFO_GIT_TAG    "$git_tag"
#define INFO_GIT_HASH   "$git_hash"
#define INFO_BUILD_TIME "UTC " "$build_time"
#define INFO_VERSION    INFO_GIT_TAG " (" INFO_GIT_BRANCH " " INFO_GIT_HASH ")"
#if defined( __LP64__ ) || defined( _WIN64 )
# define INFO_ARCH "x86_64"
#else
# define INFO_ARCH "i686"
#endif
#if defined( __GNUC__ )
# define INFO_COMPILER "gcc " TO_STRING( __GNUC__ ) "." TO_STRING( __GNUC_MINOR__ ) "." TO_STRING( __GNUC_PATCHLEVEL__ )
#elif defined( __clang__ )
# define INFO_COMPILER "clang " TO_STRING( __clang_major__ ) "." TO_STRING( __clang_minor__ ) "." TO_STRING( __clang_patchlevel__ )
#elif defined( _MSC_VER )
# define INFO_COMPILER "msvc " TO_STRING( _MSC_VER )
#else
# define INFO_COMPILER "unknow"
#endif
"@ | Out-File -FilePath "src/info.hpp" -Encoding UTF8 -NoNewline -Force