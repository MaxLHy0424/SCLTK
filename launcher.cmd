@Echo Off
Title CRCSN Launcher v1.0.11
CD /D "%~DP0"
: SetLocal EnableDelayedExpansion
: %1 %2
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
.\bin\main.exe