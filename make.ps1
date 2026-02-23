param(
    [Parameter(Mandatory)]
    [string]$target,
    [string]$gpg_key = ""
)
if (($target -eq "pack_and_sign") -and ($gpg_key -eq "")) {
    Write-Error -Message "Please provide your gpg key id!"
    exit 1
}
$software_full_name = "Student Computer Lab Toolkit"
$software_short_name = "SCLTK"
$license = "MIT License"
$copyright = "Copyright (C) 2023 - present MaxLHy0424."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_branch = & git branch --show-current
$contains_uncommitted_changes = @(git status --porcelain).Count -ne 0
if (($git_branch -ne "main") -or ($contains_uncommitted_changes -eq $true)) {
    $git_tag = "Insider Preview"
}
else {
    $git_tag = & git describe --tags --abbrev=0
}
if ($contains_uncommitted_changes -eq $false ) {
    $git_hash = & git rev-parse --short HEAD
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
"@ | Out-File -FilePath "build/info.gen" -Encoding UTF8 -NoNewline -Force
$old_info = "src/info.hpp"
$new_info = "build/info.gen"
if (-not (Test-Path $old_info -PathType Leaf) -or -not (Test-Path $new_info -PathType Leaf ) ) {
    Copy-Item -Path $new_info -Destination $old_info
}
elseif ((Get-FileHash $old_info).Hash -ne (Get-FileHash $new_info).Hash ) {
    Copy-Item -Path $new_info -Destination $old_info
}
Remove-Item -Path "build/info.gen"
& make $target -f main.mk -j gpg_key=$gpg_key