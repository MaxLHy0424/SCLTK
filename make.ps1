param(
    [string]$edition,
    [string]$target,
    [string]$gpg_key = ""
)
function Get-GitInfo {
    $in_repo = & git rev-parse --is-inside-work-tree 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $in_repo) {
        return @{
            branch = "<Unknown Branch>"
            commit = "<Work In Progress>"
        }
    }
    $branch = & git branch --show-current 2>$null
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($branch)) {
        $branch = "<Unknown Branch>"
    }
    $status = & git status --porcelain 2>$null
    $has_changes = ($status -split "`n" | Where-Object { $_ -ne "" }).Count -gt 0
    if ($has_changes) {
        $commit = "<Work In Progress>"
    }
    else {
        $commit = & git rev-parse HEAD 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrEmpty($commit)) {
            $commit = "<Unknown Hash>"
        }
    }
    return @{
        branch = $branch
        commit = $commit
    }
}
if ($target -eq 'sign' -and [string]::IsNullOrEmpty($gpg_key)) {
    Write-Error "Please provide your GPG key ID when target is 'sign'."
    exit 1
}
$meta_info = Get-Content -Raw -Path ".\meta\info.json" | ConvertFrom-Json
$current_edition = $meta_info.editions | Where-Object { $_.key -eq $edition }
if (-not $current_edition) {
    Write-Error "Unknown edition!"
    exit 1
}
$git_info = Get-GitInfo
$meta_dir = Join-Path -Path "meta" -ChildPath $edition
$old_info = Join-Path -Path $meta_dir -ChildPath "info.h"
$new_info = Join-Path -Path $meta_dir -ChildPath "info.new.h"
if (-not (Test-Path $meta_dir)) {
    New-Item -Path $meta_dir -ItemType Directory -Force | Out-Null
}
$content = @"
#pragma once
#define INFO_FULL_NAME  "$($current_edition.full_name)"
#define INFO_SHORT_NAME "$($current_edition.short_name)"
#define INFO_REPO_URL   "$($meta_info.repo_url)"
#define INFO_VERSION    "$($meta_info.version)"
#define INFO_GIT_BRANCH "$($git_info.branch)"
#define INFO_GIT_COMMIT "$($git_info.commit)"
"@
Set-Content -Path $new_info -Value $content -Encoding UTF8 -NoNewline
if (-not (Test-Path $old_info) -or (Get-FileHash $old_info).Hash -ne (Get-FileHash $new_info).Hash) {
    Move-Item -Path $new_info -Destination $old_info -Force
}
else {
    Remove-Item -Path $new_info
}
& make $target -f ".\meta\$($current_edition.makefile)" -j "gpg_key=$gpg_key"
exit $LASTEXITCODE