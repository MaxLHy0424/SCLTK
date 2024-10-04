@echo off
chcp 65001 > nul
set settings.autoLaunch=0
set settings.showVerboseInfo=0
set settings.windowTitle=CRCSN Launcher
title %settings.windowTitle%
mode con cols=50 lines=25
cd /d "%~dp0"
set item.flag=std
set item.version=v5.8.0
set item.arch=x86_64
set item.runtime=msvcrt
set item.args=
%1
if %settings.autoLaunch%==0 (
  echo                 ^< CRCSN Launcher ^>
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All Rights Reserved.
  echo.
  if %settings.showVerboseInfo%==0 (
    echo 按任意键启动 CRCSN.
  ) else (
    echo 按任意键启动 CRCSN ^(%item.flag%, %item.version%, %item.arch%, %item.runtime%^).
  )
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:launch","","runAs",1)(window.close) & goTo:eof
:launch
  start .\bin\%item.version%\%item.flag%-%item.arch%-%item.runtime%.exe %item.args%
  exit /b