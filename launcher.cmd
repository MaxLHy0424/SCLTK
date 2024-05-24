@Echo Off
Title CRCSN Launcher
Echo Launcher v1.0.8
SetLocal EnableDelayedExpansion
PushD %~DP0 & CD /D "%~DP0"
%1 %2
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
CLS
.\bin\main.exe