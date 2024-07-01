@Echo Off
Title CRCSN Launcher
Set bin="msvcrt64"
CD /D "%~DP0"
%1
mode con cols=50 lines=30
Echo              CRCSN Launcher
Echo                  24w28a
Echo (C) 2024 MaxLHy0424, All Rights Reserved.
TimeOut /NoBreak /T 2
: MsHta VbScript:CreateObject("Shell.Application").ShellExecute("%~S0","GoTo:RunAs","","RunAs",1)(Window.Close) & GoTo:EOF
:RunAs
  CLS
  .\bin\main_%bin%.exe