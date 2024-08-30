@echo off
chcp 65001 > nul
set AUTO_LAUNCH=0
set CHANNEL=2
set CUSTOM_TITLE=[CUSTOM] CRCSN Launcher
if %CHANNEL%==0 (
  title CRCSN Launcher
) else if %CHANNEL%==1 (
  title [Preview] CRCSN Launcher
) else if %CHANNEL%==2 (
  title [DevSnap] CRCSN Launcher
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
echo                      v2.0.3
echo     (C) 2023 MaxLHy0424. All Rights Reserved.
echo.
echo   (i) 将要启动: CRCSN (%arch%, %runtime%).
if %AUTO_LAUNCH%==0 (
  echo.
  echo 按任意键继续...
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
  start .\bin\%arch%\main-%runtime%.exe %argv%
  exit