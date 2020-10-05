#include "./../includes/modes.h"
#include "./../includes/listDir.h"

void normalMode(){
    char c[3];
    strcpy(homeDir, currBuff);
    // printf("%s", dirList[2]);
    history.push(homeDir);
    fflush(0);
    while (read(STDIN_FILENO, &c, 3) >= 1 && c[0] != 'q') {
        if (strcmp(c, UP_KEY) == 0)
            up();
        else if (strcmp(c, DOWN_KEY) == 0)
            down();
        else if(strcmp(c, ENTER_KEY) == 0){
            // printf("%d", (y + start));
            // printf("%s", dirList[y + start]);
            enterDir(dirList[y + start]);
        }
        else if(strcmp(c, SCROLL_UP) == 0)
            scrollUp();
        else if(strcmp(c, SCROLL_DOWN) == 0)
            scrollDown();
        else if (strcmp(c, HOME_KEY) == 0)
            goHome();
        else if (strcmp(c, LEFT_KEY) == 0)
            goBack();
        else if (strcmp(c, RIGHT_KEY) == 0)
            goForward();
        else if (strcmp(c, BACKSPACE) == 0)
            gotoParent();
        else if (strcmp(c, CMD_KEY) == 0)
            commandMode();
        // printf("%s", c);
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

char * getAbsPath(char * dir){
    char * temp = (char *)malloc(sizeof(char) * PATH_MAX);
    temp[0] = '\0';
    strcat(temp, currBuff);
    if (temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    strcat(temp, dir);
    return temp;
}

void rename(){
    if (tokens.size() != 3){
        printf("Pass 2 arguments for rename");
        getchar();
        return;
    }
    char * src = getAbsPath(tokens[1]);
    char * dest = getAbsPath(tokens[2]);
    printf("%s %s", src, dest);
    int value = rename(src, dest);
    if (value){
        printf("Error changing file name");
        getchar();
    }
}

void move(){
    struct stat fileStat;
    if (tokens.size() < 3){
        printf("Atleast 2 arguments are required for move");
        getchar();
        return;
    }
    char * destination = getAbsPath(tokens.back());
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        printf("Destination should be a directory!");
        getchar();
        return;
    }
    tokens.pop_back();
    char temp[PATH_MAX] = "";
    strcat(destination, "/");
    for (int i = 1; i < tokens.size(); i++){
        memset(temp, 0, strlen(temp));
        strcpy(temp, destination);
        strcat(temp, basename(tokens[i]));
        // printf("%s ", temp);
        rename(getAbsPath(tokens[i]), temp);
    }
    getchar();
}

void evalCommand(){
    char * command = tokens[0];
    if (strcmp(command, RENAME_COMMAND) == 0){
        rename();
    }
    else if (strcmp(command, MOVE_COMMAND) == 0){
        move();
    }

}

std::vector<char *> tokenize(char * tokenStr){
    std::vector<char *>res;
    char temp[MAX_CMD_LENGTH] = "";
    for (int i = 0; i < strlen(tokenStr); i++){
        if (tokenStr[i] == ' '){
            if (i == 0 && tokenStr[i - 1] == ' ')
                continue;
            else{
                res.push_back(strdup(temp));
                memset(temp, 0, strlen(temp));
            }

        } else {
            strncat(temp, &tokenStr[i], 1);
        }
    }
    if (strlen(temp) > 0)
        res.push_back(strdup(temp));

    return res;
}


void commandMode(){
    
    char c[3];
    strcpy(command, "");
    printf("\033[%d;0H", ws.ws_row );
    char * token;
    fflush(0);
    int i = 0;
    tokens.clear();
    while (read(STDIN_FILENO, &c, 3) >= 1 && strcmp(c, ESCAPE) != 0){
        if (strcmp(c, BACKSPACE) == 0){
            if (i > 0){
                i--;
                command[i] = '\0';
                printf("\b \b");
            }
        }
        else if (strcmp(c, ENTER_KEY) == 0){
            printf("\033[1K");
            printf("\033[%d;0H", ws.ws_row );
            tokens = tokenize(command);
            evalCommand();
            memset(command, 0, strlen(command));
            tokens.clear();
            i = 0;
            printf("\033[1K");
            printf("\033[%d;0H", ws.ws_row );
        }
        else if (!iscntrl(c[0])){
            command[i] = c[0];
            printf("%c", c[0]);
            i++;
        }
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        fflush(0);
    }
    updateScreen();
    return;

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

void goBack(){
    try {
        history.back();
        listContents(history.getTop());
    } catch (StackOverflowException e){}

}

void goForward(){
    try {
        history.forward();
        listContents(history.getTop());
    } catch (StackOverflowException e){}

}

void goHome(){
    listContents(homeDir);
    history.push(homeDir);
}

void gotoParent(){
    enterDir(parentDir);
}

void enterDir(char * dir){
    struct stat fileStat;
    char temp[PATH_MAX] = "";
    strcat(temp, currBuff);
    if (temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    strcat(temp, dir);
    lstat(temp, &fileStat);
    if ((fileStat.st_mode & S_IFMT) == S_IFDIR){
        listContents(temp);
        printf("\033[H");
    }
    history.push(temp);
}

