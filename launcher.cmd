@echo off
chcp 65001 > nul
set EVALUATE_COPY=1
if %EVALUATE_COPY%==0 (
    title CRCSN Launcher
) else (
    title [评估副本] CRCSN Launcher
)
mode con cols=50 lines=25
set arch=x86_64
set runtime=msvcrt
set argv=
cd /d "%~dp0"
%1
echo                  CRCSN Launcher
echo                   v2.0.0-beta.2
echo     (C) 2023 MaxLHy0424. All Rights Reserved.
echo.
echo 按任意键启动 CRCSN (%arch%, %runtime%)...
pause > nul
msHta vbScript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
    start .\bin\CRCSN-%arch%-%runtime%.exe %argv% /command
    exit /b 0