@echo off
chcp 65001 > nul
set CHANNEL=2
if %CHANNEL%==0 (
    title CRCSN Launcher
) else if %CHANNEL%==1 (
    title [Beta] CRCSN Launcher
) else if %CHANNEL%==2 (
    title [Dev] CRCSN Launcher
) else (
    title [N/A] CRCSN Launcher
)
mode con cols=50 lines=25
set arch=x86_64
set runtime=msvcrt
set argv=
cd /d "%~dp0"
%1
echo                  CRCSN Launcher
echo                      v2.0.0
echo     (C) 2023 MaxLHy0424. All Rights Reserved.
echo.
echo 按任意键启动 CRCSN (%arch%, %runtime%)...
pause > nul
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
    echo 请稍候...
    start .\bin\%arch%\main-%runtime%.exe %argv% /command
    exit