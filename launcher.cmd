@Echo Off
PushD "%~DP0"
%1
Title CRCSN Launcher
Echo         CRCSN Launcher v1.3.0
Echo.
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 3
: MsHta VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
    .\bin\main.exe
GoTo RunAs