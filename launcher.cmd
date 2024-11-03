@echo off
chcp 65001 > nul
set config.auto_launch=0
set config.show_verbose_info=0
set config.window_title=CRCSN Launcher
set item.tag=std
set item.version=v5.10.1
set item.arch=x86_64
set item.runtime=ucrt
set item.args=
title %config.window_title%
mode con cols=50 lines=25
cd /d "%~dp0"
%1
if %config.auto_launch%==0 (
  echo                 [ CRCSN Launcher ]
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All Rights Reserved.
  echo.
  if %config.show_verbose_info%==0 (
    echo 按任意键启动 CRCSN.
  ) else (
    echo 按任意键启动 CRCSN ^(%item.tag%, %item.version%, %item.arch%, %item.runtime%^).
  )
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:launch","","runAs",1)(window.close) & goTo:eof
:launch
  start .\bin\%item.version%\%item.tag%-%item.arch%-%item.runtime%.exe %item.args%
  exit /b