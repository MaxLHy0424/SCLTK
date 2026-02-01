param(
    [string]$gpg_key_id = $(throw "Parameter missing: -gpg_key_id key-id")
)
$software_full_name = "Student Computer Lab Toolkit"
$software_short_name = "SCLTK"
$license = "MIT License"
$copyright = "Copyright (C) 2023 - present MaxLHy0424."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_branch = git branch --show-current
$contains_uncommitted_changes = @(git status --porcelain).Count -eq 0
if ( ($git_branch -ne "main") -or ($contains_uncommitted_changes -eq $true)) {
    $git_tag = "Insider Preview"
}
else {
    $git_tag = git describe --tags --abbrev=0
}
if ($contains_uncommitted_changes -eq $true ) {
    $git_hash = git rev-parse --short HEAD
}
else {
    $git_hash = "<work in progress>"
}
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
#define INFO_VERSION    INFO_GIT_TAG " (" INFO_GIT_BRANCH " " INFO_GIT_HASH ")"
#define INFO_GPG_KEY    "$gpg_key_id"
"@ | Out-File -FilePath "src/info.hpp" -Encoding UTF8 -NoNewline -Force