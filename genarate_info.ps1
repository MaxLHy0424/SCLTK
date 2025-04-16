$software_full_name = "Student Computer Lab Toolkit"
$software_short_name = "SCLTK"
$license = "MIT License"
$copyright = "Copyright (C) 2023 - present MaxLHy0424."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_branch = git branch --show-current
$git_tag = git describe --tags --abbrev=0
$git_hash = git rev-parse --short HEAD
$git_date = git log -1 --format=%cd --date=format:"%Y/%m/%d %H:%M:%S"
$build_time = Get-Date -Format "yyyy/MM/dd HH:mm:ss"
$time_zone = Get-TimeZone | Select-Object -ExpandProperty Id
@"
#pragma once
#define INFO_FULL_NAME  "$software_full_name"
#define INFO_SHORT_NAME "$software_short_name"
#define INFO_LICENSE    "$license"
#define INFO_COPYRIGHT  "$copyright"
#define INFO_REPO_URL   "$repo_url"
#define INFO_GIT_BRANCH "$git_branch"
#define INFO_GIT_TAG    "$git_tag"
#define INFO_GIT_HASH   "$git_hash"
#define INFO_GIT_DATE   "$git_date"
#define INFO_BUILD_TIME "$build_time"
#define INFO_TIME_ZONE  "$time_zone"
#define INFO_VERSION    INFO_GIT_TAG "-" INFO_GIT_BRANCH "-" INFO_GIT_HASH
"@ | Out-File -FilePath "src/info.hpp" -Encoding UTF8 -NoNewline