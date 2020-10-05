#include "./../includes/modes.h"
#include "./../includes/listDir.h"

void normalMode(){
    fflush(0);
    char c[3];
    strcpy(homeDir, currBuff);
    while (read(STDIN_FILENO, &c, 3) >= 1 && c[0] != 'q') {
        if (strcmp(c, UP_KEY) == 0)
            up();
        else if (strcmp(c, DOWN_KEY) == 0)
            down();
        else if(strcmp(c, ENTER_KEY) == 0)
            enterDir();
        else if(strcmp(c, SCROLL_UP) == 0)
            scrollUp();
        else if(strcmp(c, SCROLL_DOWN) == 0)
            scrollDown();
        else if (strcmp(c, HOME_KEY) == 0)
            goHome();
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        fflush(0);
    }
}

void scrollUp(){
    if (start > 0){
        start--;
        end--;
        updateScreen();
    }
}

void scrollDown(){
    if (end < dirList.size()){
        start++;
        end++;
        updateScreen();
    }
}

void up(){
    if (y > 0){
        printf("\033[1A");
        y--;
    }

}

void down(){
    if (y < end - start - 1){
        printf("\033[1B");
        y++;
    }

}

void goHome(){
    listContents(homeDir);
}

void enterDir(){
    // printf("%d", y);
    struct stat fileStat;
    char temp[PATH_MAX] = "";
    strcat(temp, currBuff);
    if (temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    strcat(temp, dirList[y + start]);
    lstat(temp, &fileStat);
    if ((fileStat.st_mode & S_IFMT) == S_IFDIR){
        listContents(temp);
        printf("\033[H");
    }
}

