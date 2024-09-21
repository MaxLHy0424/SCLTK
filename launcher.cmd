@echo off
chcp 65001 > nul
set AUTO_LAUNCH=0
set SHOW_VERBOSE_INFO=0
set TITLE=CRCSN Launcher
title %TITLE%
mode con cols=50 lines=25
cd /d "%~dp0"
set version=v5.6.2
set arch=x86_64
set runtime=msvcrt
set args=
%1
if %AUTO_LAUNCH%==0 (
  echo                    ^| Launcher ^|
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All rights reserved.
  echo.
  if %SHOW_VERBOSE_INFO%==0 (
    echo 按任意键启动 CRCSN...
  ) else (
    echo 按任意键启动 CRCSN ^(%arch%, %runtime%^)...
  )
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:launch","","runAs",1)(window.close) & goTo:eof
:launch
  chcp 936 > nul
  start .\bin\%version%\CRCSN-%arch%-%runtime%.exe %args%
  exit /b