#include "./../includes/globals.h"

struct termios orig_termios;
struct termios currentMode;
std::vector<char *> dirList;
char currBuff[PATH_MAX];
char currDir[2] = ".";
char parentDir[3] = "..";
char UP_KEY[2] = "k";
char DOWN_KEY[2] = "l";
int y = 0;
int x = 0;