#include "globals.h"
#include "listDir.h"

#include <string>

#ifndef _MODES
#define _MODES
    void normalMode();
    void commandMode();
    void up();
    void down();
    void enterDir(char * dir);
    void scrollUp();
    void scrollDown();
    void goHome();
    void goBack();
    void goForward();
    void gotoParent();
    void commandMode();
#endif