#pragma once
#ifndef _BUILD_INFO_
#define WINDOW_TITLE "CRCSN"
#include<stdio.h>
#include<shlobj.h>
#ifdef _NEXT_
#include<fstream>
#endif
#include<vector>
#include<string>
#include<functional>
#include<thread>
#endif
#define INFO_NAME "Computer Room Control Software Nemesis"
#ifdef _NEXT_
#define INFO_VERSION "v6.0.0-dev.241027"
#else
#define INFO_VERSION "v5.10.0"
#endif
#define INFO_REPO_URL "https://github.com/MaxLHy0424/CRCSN"
#define INFO_DEVELOPER "MaxLHy0424"
#define INFO_LICENSE "MIT License"