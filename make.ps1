param(
    [Parameter(Mandatory)]
    [string]$target
)
$env_file = "env.mk"
$env_target_value = "msys2_path"
if (-not (Test-Path $env_file)) {
    Write-Error "Please create $env_file by make_env.ps1"
    exit 1
}
$pattern = "^${env_target_value}\s*=\s*(.*)$"
$match = Select-String -Path $env_file -Pattern $pattern
if ($match) {
    $msys2_path = $match.Matches.Groups[1].Value.Trim()
    $msys2_path = $msys2_path -replace "^[`"']|[`"']$", ""
}
else {
    Write-Error "Unable to find $env_target_value"
    exit 1
}
$software_full_name = "Student Computer Lab Toolkit"
$software_short_name = "SCLTK"
$license = "MIT License"
$copyright = "Copyright (C) 2023 - present MaxLHy0424."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_branch = & "$msys2_path/usr/bin/git.exe" branch --show-current
$contains_uncommitted_changes = @(git status --porcelain).Count -ne 0
if (($git_branch -ne "main") -or ($contains_uncommitted_changes -eq $true)) {
    $git_tag = "Insider Preview"
}
else {
    $git_tag = & "$msys2_path/usr/bin/git.exe" describe --tags --abbrev=0
}
if ($contains_uncommitted_changes -eq $false ) {
    $git_hash = & "$msys2_path/usr/bin/git.exe" rev-parse --short HEAD
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
if ((Get-FileHash $old_info).Hash -ne (Get-FileHash $new_info).Hash ) {
    Copy-Item -Path $new_info -Destination $old_info
}
Remove-Item -Path "build/info.gen"
& "$msys2_path/usr/bin/make.exe" $target -f main.mk -j