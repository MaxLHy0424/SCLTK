@Echo Off
Title CRCSN Launcher v1.2.1
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
PushD "%~DP0"
%1
TimeOut /NoBreak /T 3
: MsHta VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
.\bin\main.exe
GoTo RunAs