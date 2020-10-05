#include "./../includes/globals.h"

struct termios orig_termios;
struct termios currentMode;
struct winsize ws;
std::vector<char *> dirList;
char currBuff[PATH_MAX];
char homeDir[PATH_MAX] = "";
char currDir[2] = ".";
char parentDir[3] = "..";
char UP_KEY[4] = "\033[A";
char DOWN_KEY[4] = "\033[B";
char ENTER_KEY[2] = "\012";
char BACKSPACE[2] = "\010";
char SCROLL_UP[2] = "k";
char SCROLL_DOWN[2] = "l";
char HOME_KEY[2] = "h";
int y = 0;
int x = 0;
int start = 0;
int end = 0;