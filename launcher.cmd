@Echo Off
Title CRCSN Launcher
CD /D "%~DP0"
%1
Echo              CRCSN Launcher
Echo                  24w27e
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
Echo.
TimeOut /NoBreak /T 3
: MSHTA VBScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
  CLS
  .\bin\main.exe