#pragma once
#ifndef _BUILD_INFO_
#define WINDOW_TITLE "CRCSN"
#include<stdio.h>
#include<windows.h>
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
#define INFO_VERSION "v6.0.0-preview.1"
#else
#define INFO_VERSION "v5.10.1"
#endif
#define INFO_REPO_URL "https://github.com/MaxLHy0424/CRCSN"
#define INFO_DEVELOPER "MaxLHy0424"
#define INFO_LICENSE "MIT License"