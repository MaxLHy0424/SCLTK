@echo off
set branch=dev
set arch=x86_64
set bin=msvcrt
cd /d "%~dp0"
chcp 65001
%1
mode con cols=50 lines=30
title CRCSN Launcher
echo                  CRCSN Launcher
echo                      24w31c
echo     (C) 2024 MaxLHy0424, All Rights Reserved.
echo.
echo 按任意键启动 CRCSN.
echo.
pause > nul
: msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
.\bin\%branch%-%arch%-%bin%.exe