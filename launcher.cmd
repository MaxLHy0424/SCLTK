@echo off
title CRCSN Launcher
set bin="msvcrt64"
set branch="dev"
cd /d "%~dp0"
%1
mode con cols=50 lines=30
echo                  CRCSN Launcher
echo                      24w28b
echo     (C) 2024 MaxLHy0424, All Rights Reserved.
timeOut /noBreak /t 2
: msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:EOF
:RunAs
  cls
  .\bin\%branch%_%bin%.exe