@echo off
chcp 65001
set os="x86_64"
set bin="msvcrt64"
cd /d "%~dp0"
%1
mode con cols=50 lines=30
title CRCSN Launcher
echo                 CRCSN Launcher
echo                     24w28d
echo    (C) 2024 MaxLHy0424, All Rights Reserved.
echo.
echo 按任意键启动 CRCSN... & pause > nul
: msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
  chcp 936
  cls
  .\bin\%os%-%bin%.exe