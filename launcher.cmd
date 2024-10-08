@echo off
chcp 65001 > nul
set config.autoLaunch=0
set config.showVerboseInfo=0
set config.windowTitle=CRCSN Launcher
title %config.windowTitle%
mode con cols=50 lines=25
cd /d "%~dp0"
set bootItem.tag=std
set bootItem.version=v5.9.1
set bootItem.arch=x86_64
set bootItem.runtime=ucrt
set bootItem.args=
%1
if %config.autoLaunch%==0 (
  echo                 ^< CRCSN Launcher ^>
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All Rights Reserved.
  echo.
  if %config.showVerboseInfo%==0 (
    echo 按任意键启动 CRCSN.
  ) else (
    echo 按任意键启动 CRCSN ^(%bootItem.tag%, %bootItem.version%, %bootItem.arch%, %bootItem.runtime%^).
  )
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:launch","","runAs",1)(window.close) & goTo:eof
:launch
  start .\bin\%bootItem.version%\%bootItem.tag%-%bootItem.arch%-%bootItem.runtime%.exe %bootItem.args%
  exit /b