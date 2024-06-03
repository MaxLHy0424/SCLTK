@Echo Off
Title CRCSN Launcher v1.1.1
CD /D "%~DP0"
%1 %2
: MsHta VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
.\bin\main.exe
GoTo RunAs