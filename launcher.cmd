@Echo Off
CD /D "%~DP0"
%1
Title CRCSN Launcher
Echo          CRCSN Launcher v1.3.8
Echo.
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 2
: MsHta VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) && GoTo:EOF
:RunAs
  CLS
  .\bin\main.exe
GoTo RunAs