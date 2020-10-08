#include "./../includes/modes.h"
#include "./../includes/listDir.h"
#include "./../includes/misc.h"

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


void createFolder(){
    struct stat fileStat;
    char temp[PATH_MAX] = "";
    if (tokens.size() < 3){
        throw CmdModeException("Atleast 2 arguments must be present");
    }
    char * destination = getAbsPath(tokens.back());
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        free(destination);
        throw CmdModeException("Destination should be a directory!");
    }
    strcpy(temp, destination);
    if(temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    int tempLen = strlen(temp);
    for (int i = 1; i < tokens.size(); i++){
        memset(temp + tempLen, 0, strlen(temp) - tempLen);
        strcat(temp, tokens[i]);
        mkdir(temp, 0774);
    }
}

void createFile(){
    struct stat fileStat;
    int destFile;
    char temp[PATH_MAX] = "";
    if (tokens.size() < 3){
        throw CmdModeException("Atleast 2 arguments must be present");
    }
    char * destination = getAbsPath(tokens.back());
    tokens.pop_back();
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        free(destination);
        throw CmdModeException("Destination should be a directory!");
    }
    strcpy(temp, destination);
    if(temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    int tempLen = strlen(temp);
    for (int i = 1; i < tokens.size(); i++){
        memset(temp + tempLen, 0, strlen(temp) - tempLen);
        strcat(temp, tokens[i]);
        destFile = open(temp, O_WRONLY | O_CREAT, 0776);
    }
}

void deleteFile(){
    struct stat fileStat;
    int status;
    char temp[PATH_MAX] = "";
    if (tokens.size() < 2){
        throw CmdModeException("Atleast 1 argument must be present");
    }
    char * destination;
    for (int i = 1; i < tokens.size(); i++){
        destination = getAbsPath(tokens[i]);
        int status = lstat(destination, &fileStat);
        if (status == -1 || (fileStat.st_mode & S_IFMT) == S_IFDIR)
            continue;
        remove(destination);
    }
}

void deleteDirectory(){
    struct stat fileStat;
    int status;
    char temp[PATH_MAX] = "";
    if (tokens.size() < 2){
        throw CmdModeException("Atleast  argument must be present");
    }
    char * destination;
    for (int i = 1; i < tokens.size(); i++){
        destination = getAbsPath(tokens[i]);
        int status = lstat(destination, &fileStat);
        if (status == -1 || (fileStat.st_mode & S_IFMT) != S_IFDIR)
            continue;
        deleteUtil(destination);
    }
}

void search(){
    if (tokens.size() != 2){
        throw CmdModeException("Number of args must be 1");
    }
    Stack searchStack;
    searchStack.push(currBuff);
    if (searchUtil(tokens[1], searchStack)){
        printf("True");
    }else {
        printf("False");
    }
    getchar();

}

void copy(){
    struct stat fileStat;
    if (tokens.size() < 3){
        throw CmdModeException("Atleast 2 arguments are required for move");
    }
    char * destination = getAbsPath(tokens.back());
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        free(destination);
        throw CmdModeException("Destination should be a directory!");
    }
    tokens.pop_back();
    char * source;
    char temp[PATH_MAX] = "";
    strcat(destination, "/");
    for (int i = 1; i < tokens.size(); i++){
        memset(temp, 0, strlen(temp));
        strcpy(temp, destination);
        strcat(temp, basename(tokens[i]));
        source = getAbsPath(tokens[i]);
        copyUtil(source, temp);

        free(source);
    }
    free(destination);
}

void move(){
    struct stat fileStat;
    if (tokens.size() < 3){
        throw CmdModeException("Atleast 2 arguments are required for move");
    }
    char * destination = getAbsPath(tokens.back());
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        free(destination);
        throw CmdModeException("Destination should be a directory!");
    }
    tokens.pop_back();
    char temp[PATH_MAX] = "";
    char * source;
    strcat(destination, "/");
    for (int i = 1; i < tokens.size(); i++){
        memset(temp, 0, strlen(temp));
        strcpy(temp, destination);
        strcat(temp, basename(tokens[i]));
        source = getAbsPath(tokens[i]);
        copyUtil(source, temp);
        deleteUtil(source);
        free(source);
    }
    free(destination);
}

void gotoFolder(){
    struct stat fileStat;
    if (tokens.size() != 2){
        throw CmdModeException("Incorrect number of args");
    }
    char * destination = getAbsPath(tokens.back());
    lstat(destination, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR){
        free(destination);
        throw CmdModeException("Destination should be a directory!");
    }

    listContents(destination);
    history.push(destination);
}

void evalCommand(){
    char * command = tokens[0];
    if (strcmp(command, RENAME_COMMAND) == 0){
        rename();
    }
    else if (strcmp(command, MOVE_COMMAND) == 0){
        move();
    }
    else if (strcmp(command, COPY_COMMAND) == 0){
        copy();
    }
    else if(strcmp(command, CREATE_FOLDER_COMMAND) == 0){
        createFolder();
    }
    else if(strcmp(command, CREATE_FILE_COMMAND) == 0){
        createFile();
    }
    else if(strcmp(command, DELETE_FILE_COMMAND) == 0){
        deleteFile();
    }
    else if(strcmp(command, DELETE_FOLDER_COMMAND) == 0){
        deleteDirectory();
    }
    else if(strcmp(command, GOTO_COMMAND) == 0){
        gotoFolder();
    }
    else if(strcmp(command, SEARCH_COMMAND) == 0){
        search();
    }

}

std::vector<char *> tokenize(char * tokenStr){
    std::vector<char *>res;
    char temp[MAX_CMD_LENGTH] = "";
    for (int i = 0; i < strlen(tokenStr); i++){
        if (tokenStr[i] == ' '){
            if (i == 0 || tokenStr[i - 1] == ' ')
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
            if (strlen(command) == 0)
                continue;
            printf("\033[1K");
            printf("\033[%d;0H", ws.ws_row );
            tokens = tokenize(command);
            try {
                evalCommand();
            } catch(const CmdModeException &e){
                printf("%s", e.what());
                getchar();
                printf("\033[1K");
                printf("\033[%d;0H", ws.ws_row );
            }
            memset(command, 0, strlen(command));
            tokens.clear();
            i = 0;
            enterDir(currDir, false);
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
    enterDir(currDir, false);
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

void enterDir(char * dir, bool pushToStack){
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
    } else {
        pid_t childProc = fork();
        if (childProc == 0){
            char * cArgs[] = {viPath, temp, NULL};
            execv(cArgs[0], cArgs);
            exit(0);
        } else {
            int child_status;
            pid_t tpid;
            do {
                tpid = wait(&child_status);
            }while(tpid != childProc);
        }
    }
    if (pushToStack)
        history.push(temp);
}

