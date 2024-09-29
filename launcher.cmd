@echo off
chcp 65001 > nul
set opt.autoLaunch=0
set opt.showVerboseInfo=0
set opt.windowTitle=CRCSN Launcher
title %opt.windowTitle%
mode con cols=50 lines=25
cd /d "%~dp0"
set version=v5.6.7
set arch=x86_64
set runtime=msvcrt
set args=
%1
if %opt.autoLaunch%==0 (
  echo                   CRCSN Launcher
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All Rights Reserved.
  echo.
  if %opt.showVerboseInfo%==0 (
    echo 按任意键启动 CRCSN...
  ) else (
    echo 按任意键启动 CRCSN ^(%version%, %arch%, %runtime%^)...
  )
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:launch","","runAs",1)(window.close) & goTo:eof
:launch
  chcp 936 > nul
  start .\bin\%version%\std-%arch%-%runtime%.exe %args%
  exit /b