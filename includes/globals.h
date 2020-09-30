#ifndef _GLOBALS
#define _GLOBALS

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <algorithm>


extern char currBuff[PATH_MAX];
extern char currDir[2];
extern char parentDir[3];
extern std::vector<char *> dirList;
extern struct termios orig_termios;
extern struct termios currentMode;
extern char UP_KEY[2];
extern char DOWN_KEY[2];
extern int y;
extern int x;

#endif