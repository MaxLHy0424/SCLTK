@echo off
chcp 65001 > nul
set AUTO_LAUNCH=0
set CHANNEL=3
set CUSTOM_TITLE=[<CUSTOM>] CRCSN Launcher
if %CHANNEL%==1 (
  title CRCSN Launcher
) else if %CHANNEL%==2 (
  title [Evaluate] CRCSN Launcher
) else if %CHANNEL%==3 (
  title [Snapshot] CRCSN Launcher
) else (
  title %CUSTOM_TITLE%
)
mode con cols=50 lines=25
set arch=x86_64
set runtime=msvcrt
set argv=
cd /d "%~dp0"
%1
if %AUTO_LAUNCH%==0 (
  echo                  CRCSN Launcher
  echo.
  echo     (C) 2023 MaxLHy0424, All Rights Reserved.
  echo.
  echo 按任意键启动 CRCSN ^(v5.3.1, %arch%, %runtime%^)...
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
  start .\bin\%arch%\CRCSN-%runtime%.exe %argv%
  exit /b %CHANNEL%