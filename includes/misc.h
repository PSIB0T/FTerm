#include "globals.h"

#ifndef _MISC
#define _MISC

void enableNCMode();
void setHomeDir();
bool searchUtil(char * searchStr, Stack &searchStack);
void deleteUtil(char * src);
void copyUtil(char * src, char * dest);
void rename();
char * getAbsPath(char * dir);

#endif