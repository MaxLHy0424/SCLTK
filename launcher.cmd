@Echo Off
CD /D "%~DP0"
Title CRCSN Launcher
Echo         CRCSN Launcher v1.3.1
Echo.
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 3
%1
: MsHta VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
    .\bin\main.exe
GoTo RunAs