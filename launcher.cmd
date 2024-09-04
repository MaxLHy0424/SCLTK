@echo off
chcp 65001 > nul
set AUTO_LAUNCH=0
set CHANNEL=0
set CUSTOM_TITLE=[<CUSTOM>] CRCSN Launcher
if %CHANNEL%==0 (
  title CRCSN Launcher
) else if %CHANNEL%==1 (
  title [Evaluate] CRCSN Launcher
) else if %CHANNEL%==2 (
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
echo                  CRCSN Launcher
echo                      v2.2.2
echo     (C) 2023 MaxLHy0424, All Rights Reserved.
if %AUTO_LAUNCH%==0 (
  echo.
  echo 按任意键启动 CRCSN (%arch%, %runtime%)...
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
  start .\bin\%arch%\CRCSN-%runtime%.exe %argv%
  exit