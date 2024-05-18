@Echo Off
SetLocal EnableDelayedExpansion
Title CRCSN Launcher
PushD %~DP0&CD /D "%~dp0"
%1 %2
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close)&GoTo:EOF
:RunAs
.\bin\main.exe