@Echo Off
Title CRCSN Launcher
CD /D "%~DP0"
%1
Echo          CRCSN Launcher v1.4.0
Echo.
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 2
: MSHTA VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
  CLS
  .\bin\main.exe