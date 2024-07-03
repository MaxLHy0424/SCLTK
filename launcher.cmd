@echo off
chcp 65001
set bin="msvcrt64"
set branch="dev"
cd /d "%~dp0"
%1
mode con cols=50 lines=30
title CRCSN Launcher
echo                 CRCSN Launcher
echo                     24w28c
echo    (C) 2024 MaxLHy0424, All Rights Reserved.
echo.
echo (i) 按任意键启动 CRCSN... & pause > nul
: msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:EOF
:runAs
  chcp 936
  cls
  .\bin\%branch%_%bin%.exe