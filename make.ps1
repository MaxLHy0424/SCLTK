param(
    [ValidateSet('Mainline', 'Legacy')]
    [string]$edition,
    [string]$target,
    [string]$gpg_key = ""
)
if ($target -eq 'sign' -and [string]::IsNullOrEmpty($gpg_key)) {
    Write-Error "Please provide your GPG key ID when target is 'pack_and_sign'."
    exit 1
}
function Get-GitInfo {
    $in_repo = & git rev-parse --is-inside-work-tree 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $in_repo) {
        return @{
            Branch = "<Unknown Branch>"
            Tag    = "<Insider Preview>"
            Hash   = "<Work In Progress>"
        }
    }
    $branch = & git branch --show-current 2>$null
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($branch)) {
        $branch = "<Unknown Branch>"
    }
    $status = & git status --porcelain 2>$null
    $has_changes = ($status -split "`n" | Where-Object { $_ -ne "" }).Count -gt 0
    if ($branch -ne 'main' -or $has_changes -or $branch -eq "<Unknown Branch>") {
        $tag = "<Insider Preview>"
    }
    else {
        $tag = & git describe --tags --abbrev=0 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($tag)) {
            $tag = "<No Tag>"
        }
    }
    if ($has_changes) {
        $hash = "<Work In Progress>"
    }
    else {
        $hash = & git rev-parse HEAD 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($hash)) {
            $hash = "<Unknown Hash>"
        }
    }
    return @{
        branch = $branch
        tag    = $tag
        hash   = $hash
    }
}
$software_full_name = "Student Computer Lab Toolkit - $edition Edition"
$software_short_name = "SCLTK-$edition"
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
$git_info = Get-GitInfo
$meta_dir = Join-Path -Path "meta" -ChildPath $edition
$old_info = Join-Path -Path $meta_dir -ChildPath "info.h"
$new_info = Join-Path -Path $meta_dir -ChildPath "info.new.h"
if (-not (Test-Path $meta_dir)) {
    New-Item -Path $meta_dir -ItemType Directory -Force | Out-Null
}
$content = @"
#pragma once
#define INFO_FULL_NAME  "$software_full_name"
#define INFO_SHORT_NAME "$software_short_name"
#define INFO_REPO_URL   "$repo_url"
#define INFO_GIT_BRANCH "$($git_info.branch)"
#define INFO_GIT_TAG    "$($git_info.tag)"
#define INFO_GIT_HASH   "$($git_info.hash)"
"@
Set-Content -Path $new_info -Value $content -Encoding UTF8 -NoNewline
if (-not (Test-Path $old_info) -or (Get-FileHash $old_info).Hash -ne (Get-FileHash $new_info).Hash) {
    Move-Item -Path $new_info -Destination $old_info -Force
}
else {
    Remove-Item -Path $new_info
}
& make $target -f ".\meta\$edition\main.mk" -j "gpg_key=$gpg_key"
exit $LASTEXITCODE