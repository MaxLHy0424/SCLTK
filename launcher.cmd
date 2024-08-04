@echo off
chcp 65001
set EVALUATE_COPY=true
if "%EVALUATE_COPY%"=="true" (
    title [评估副本] CRCSN Launcher
) else (
    title CRCSN Launcher
)
mode con cols=50 lines=30
set branch=dev
set arch=x86_64
set runtime=msvcrt
set argv=
cd /d "%~dp0"
%1
echo                  CRCSN Launcher
echo                   v2.0.0-beta.1
echo     (C) 2023 MaxLHy0424. All Rights Reserved.
echo.
echo 按任意键启动 CRCSN (%branch%, %arch%, %runtime%)...
pause > nul
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
    .\bin\%branch%-%arch%-%runtime%.exe %argv%
    exit