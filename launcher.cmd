@echo off
title CRCSN Launcher v1.0.13
cd /d "%~dp0"
: setLocal enableDelayedExpansion
: %1 %2
: msHta vbscript:createObject("shell.application").shellExecute("%~s0","goTo:runAs","","runAs",1)(window.close) & goTo:eof
:runAs
.\bin\main.exe