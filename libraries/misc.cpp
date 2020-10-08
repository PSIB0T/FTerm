#include "./../includes/misc.h"

void disableNCMode(){
    printf("\033[H\033[J");
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableNCMode() {
    // printf("\033[H");
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableNCMode);
    currentMode = orig_termios;
    currentMode.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &currentMode);
    // printf("\033[r");
}

void setHomeDir(){
    char * homeDirTemp = getenv("HOME");
    if (homeDirTemp != NULL){
        strcpy(userDir, homeDirTemp);
    } else {
        uid_t uid = getuid();
        struct passwd * pw= getpwuid(uid);
        strcpy(userDir, pw->pw_dir);
    }
    strcat(userDir, "/");
}