@Echo Off
Title CRCSN Launcher
CD /D "%~DP0"
%1
Echo         CRCSN Launcher v1.3.2
Echo.
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 3
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
    .\bin\main.exe
GoTo RunAs