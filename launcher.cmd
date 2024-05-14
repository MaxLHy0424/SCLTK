@Echo Off
SetLocal EnableDelayedExpansion
Title CRCSN Launcher
PushD %~DP0 & CD /D "%~dp0"
%1 %2
: MsHta Vbscript:CreateObject("Shell.Application").ShellExecute("%~S0","Goto :RunAs","","RunAs",1)(Window.Close)&Goto:EOF
:RunAs
.\bin\main.exe -clr=3