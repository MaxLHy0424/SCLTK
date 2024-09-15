@echo off
chcp 65001 > nul
set arch=x86_64
set runtime=msvcrt
set argv=
set AUTO_LAUNCH=0
set CHANNEL=0
set CUSTOM_TITLE=[#CUSTOM#] CRCSN Launcher
if %CHANNEL%==0 (
  title CRCSN Launcher
) else if %CHANNEL%==1 (
  title [Preview] CRCSN Launcher
) else (
  title %CUSTOM_TITLE%
)
mode con cols=50 lines=25
cd /d "%~dp0"
%1
if %AUTO_LAUNCH%==0 (
  echo                ^< CRCSN Launcher ^>
  echo.
  echo     ^(C^) 2023 MaxLHy0424. All rights reserved.
  echo.
  echo 按任意键启动 CRCSN ^(%arch%, %runtime%^)...
  pause > nul
)
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
  chcp 936 > nul
  start .\bin\%arch%\CRCSN-%runtime%.exe %argv%
  exit /b