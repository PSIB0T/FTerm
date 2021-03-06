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
#include <ctype.h>
#include <stdexcept>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h> 

#define MAX_CMD_LENGTH 32768

class CmdModeException: public std::runtime_error{
    public:
        CmdModeException(char const* const message);
};

struct StackOverflowException : public std::exception
{
	const char * what () const throw ();
};

class Node {
    public:
        char * path;
        Node * front;
        Node * back;
        Node(char * path);
};

class Stack {
    Node * top;
    public:
        Stack();
        void push(char * path);
        void pop();
        void forward();
        void back();
        const char * getTop();
};

extern Stack history;
extern char currBuff[PATH_MAX];
extern char homeDir[PATH_MAX];
extern char command[MAX_CMD_LENGTH];
extern char currDir[2];
extern char parentDir[3];
extern std::vector<char *> dirList;
extern std::vector<char *> tokens;
extern struct termios orig_termios;
extern struct termios currentMode;
extern struct winsize ws;
extern char UP_KEY[4];
extern char DOWN_KEY[4];
extern char LEFT_KEY[4];
extern char RIGHT_KEY[4];
extern char ENTER_KEY[2];
extern char BACKSPACE[2];
extern char SCROLL_UP[2];
extern char SCROLL_DOWN[2];
extern char HOME_KEY[2];
extern char CMD_KEY[2];
extern char ESCAPE[2];
extern char RENAME_COMMAND[7];
extern char MOVE_COMMAND[5];
extern char COPY_COMMAND[5];
extern char userDir[PATH_MAX];
extern char CREATE_FOLDER_COMMAND[11];
extern char CREATE_FILE_COMMAND[12];
extern char DELETE_FILE_COMMAND[12];
extern char DELETE_FOLDER_COMMAND[11];
extern char GOTO_COMMAND[5];
extern char SEARCH_COMMAND[7];
extern char viPath[20];
extern int y;
extern int x;
extern int start;
extern int end;

#endif