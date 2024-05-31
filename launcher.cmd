@Echo Off
Title CRCSN Launcher v1.0.15
CD /D "%~DP0"
%1 %2
: MSHTA VBScript:CreateObject("Shell.application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
.\bin\main.exe