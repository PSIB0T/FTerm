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
#include <sys/ioctl.h>


extern char currBuff[PATH_MAX];
extern char homeDir[PATH_MAX];
extern char currDir[2];
extern char parentDir[3];
extern std::vector<char *> dirList;
extern struct termios orig_termios;
extern struct termios currentMode;
extern struct winsize ws;
extern char UP_KEY[4];
extern char DOWN_KEY[4];
extern char ENTER_KEY[2];
extern char BACKSPACE[2];
extern char SCROLL_UP[2];
extern char SCROLL_DOWN[2];
extern char HOME_KEY[2];
extern int y;
extern int x;
extern int start;
extern int end;

#endif