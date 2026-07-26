param(
    [ValidateSet('mainline', 'legacy')]
    [string]$edition,
    [string]$target,
    [string]$gpg_key = ""
)
if ($target -eq 'pack_and_sign' -and [string]::IsNullOrEmpty($gpg_key)) {
    Write-Error "Please provide your GPG key ID when target is 'pack_and_sign'."
    exit 1
}
$converted_edition_string = $edition.ToLower().Substring(0, 1).ToUpper() + $edition.Substring(1)
$software_full_name = "Student Computer Lab Toolkit - $converted_edition_string Edition"
$software_short_name = "SCLTK-$converted_edition_string"
$copyright = "Copyright (C) 2023 MaxLHy0424.\n libtre: Copyright (c) 2001-2009 Ville Laurikari."
$repo_url = "https://github.com/MaxLHy0424/SCLTK"
function Get-GitInfo {
    $inRepo = & git rev-parse --is-inside-work-tree 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $inRepo) {
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
    $hasChanges = ($status -split "`n" | Where-Object { $_ -ne "" }).Count -gt 0
    if ($branch -ne 'main' -or $hasChanges -or $branch -eq "<Unknown Branch>") {
        $tag = "<Insider Preview>"
    }
    else {
        $tag = & git describe --tags --abbrev=0 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($tag)) {
            $tag = "<No Tag>"
        }
    }
    if ($hasChanges) {
        $hash = "<Work In Progress>"
    }
    else {
        $hash = & git rev-parse HEAD 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($hash)) {
            $hash = "<Unknown Hash>"
        }
    }
    return @{
        Branch = $branch
        Tag    = $tag
        Hash   = $hash
    }
}
$gitInfo = Get-GitInfo
$metaDir = Join-Path -Path "meta" -ChildPath $edition
$oldInfo = Join-Path -Path $metaDir -ChildPath "info.h"
$newInfo = Join-Path -Path $metaDir -ChildPath "info.new.h"
if (-not (Test-Path $metaDir)) {
    New-Item -Path $metaDir -ItemType Directory -Force | Out-Null
}
$content = @"
#pragma once
#define INFO_FULL_NAME  "$software_full_name"
#define INFO_SHORT_NAME "$software_short_name"
#define INFO_COPYRIGHT  "$copyright"
#define INFO_REPO_URL   "$repo_url"
#define INFO_GIT_BRANCH "$($gitInfo.Branch)"
#define INFO_GIT_TAG    "$($gitInfo.Tag)"
#define INFO_GIT_HASH   "$($gitInfo.Hash)"
"@
Set-Content -Path $newInfo -Value $content -Encoding UTF8 -NoNewline
if (-not (Test-Path $oldInfo) -or (Get-FileHash $oldInfo).Hash -ne (Get-FileHash $newInfo).Hash) {
    Move-Item -Path $newInfo -Destination $oldInfo -Force
}
else {
    Remove-Item -Path $newInfo
}
& make $target -f ".\meta\$edition\main.mk" -j "gpg_key=$gpg_key"
exit $LASTEXITCODE