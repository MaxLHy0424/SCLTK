@"
msys2_path = /path/to/msys2
pwsh_path  = /path/to/pwsh
gpg_path   = /path/to/gpg
gpg_key    = key-id
"@ | Out-File -FilePath "env.mk" -Encoding UTF8 -NoNewline -Force