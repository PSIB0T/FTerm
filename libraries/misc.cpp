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

char * getAbsPath(char * dir){
    char * temp = (char *)malloc(sizeof(char) * PATH_MAX);
    temp[0] = '\0';
    if (dir[0] != '~' && dir[0] != '/'){
        strcat(temp, currBuff);
        if (temp[strlen(temp) - 1] != '/'){
            strcat(temp, "/");
        }
    } else if(dir[0] == '~'){
        strcat(temp, homeDir);
        if (temp[strlen(temp) - 1] != '/'){
            strcat(temp, "/");
        }
        dir = dir + 1;
    }
    strcat(temp, dir);
    return temp;
}

void rename(){
    if (tokens.size() != 3){
        throw CmdModeException("Rename command requires 2 args");
    }
    char * src = getAbsPath(tokens[1]);
    char * dest = getAbsPath(tokens[2]);
    // printf("%s %s", src, dest);
    int value = rename(src, dest);
    if (value){
        throw CmdModeException("Error changing file name");
        free(src);
        free(dest);
    }
    free(src);
    free(dest);
}

void copyUtil(char * src, char * dest){
    int sourceFile, destFile, status, returnStatus;
    DIR * sourceDir;
    char * sourceFilePath, destFilePath;
    char fileBuffer;
    struct stat fileStat;
    char temp[PATH_MAX] = "";
    int destPathLen, srcPathLen;
    char destBuff[PATH_MAX] = "";
    returnStatus = lstat(src, &fileStat);
    if (returnStatus == -1)
        throw CmdModeException("Source File is invalid");
    if ((fileStat.st_mode & S_IFMT) == S_IFDIR){
        struct dirent * dirp;
        strcpy(destBuff, dest);
        strcpy(temp, src);
        if (destBuff[strlen(destBuff) - 1] != '/')
            strcat(destBuff, "/");
        if (temp[strlen(temp) - 1] != '/')
            strcat(temp, "/");  
        destPathLen = strlen(destBuff);
        srcPathLen = strlen(temp);
        status = mkdir(destBuff, fileStat.st_mode);
        sourceDir = opendir(src);
        while ((dirp = readdir(sourceDir)) != NULL){
            if (strcmp(currDir, dirp->d_name) == 0 || strcmp(parentDir, dirp->d_name) == 0)
                continue;
            memset(temp + srcPathLen, 0, strlen(temp) - srcPathLen);
            memset(destBuff + destPathLen, 0, strlen(destBuff) - destPathLen);
            strcat(temp, dirp->d_name);
            strcat(destBuff, dirp->d_name);
            copyUtil(temp, destBuff);
        }
        closedir(sourceDir);
    } else {
        sourceFile = open(src, O_RDONLY);
        destFile = open(dest, O_WRONLY | O_CREAT, fileStat.st_mode);
        while((status=read(sourceFile, &fileBuffer, 1)) != 0){
            write(destFile, &fileBuffer, 1);
        }
        close(sourceFile);
        close(destFile);
    }
}

void deleteUtil(char * src){
    struct dirent * dirp;
    struct stat fileStat;
    char temp[PATH_MAX] = "";
    strcpy(temp, src);
    lstat(temp, &fileStat);
    if ((fileStat.st_mode & S_IFMT) != S_IFDIR && access(src, W_OK) == 0){
        remove(temp);
        return;
    }
    if (temp[strlen(temp) - 1] != '/')
        strcat(temp, "/");
    int srcPathLen = strlen(temp);
    DIR * sourceDir = opendir(src);
    if (access(src, R_OK | W_OK | X_OK) == 0){
        while((dirp = readdir(sourceDir)) != NULL){
            if (strcmp(currDir, dirp->d_name) == 0 || strcmp(parentDir, dirp->d_name) == 0)
                continue;
            memset(temp + srcPathLen, 0, strlen(temp) - srcPathLen);
            strcat(temp, dirp->d_name);
            lstat(temp, &fileStat);
            deleteUtil(temp);
        }
    }
    closedir(sourceDir);
    if (access(src, W_OK) == 0)
        rmdir(src);
}

bool searchUtil(char * searchStr, Stack &searchStack){
    DIR * sourceDir;
    struct stat fileStat;
    struct dirent * dirp;
    const char * dir = searchStack.getTop();
    sourceDir = opendir(dir);
    char * temp = (char *)malloc(sizeof(char) * PATH_MAX);
    temp[0] = 0;
    strcpy(temp, dir);
    if (temp[strlen(temp) - 1] != '/')
        strcat(temp, "/");
    // printf("%s", temp);
    // getchar();
    int pathLen = strlen(temp);
    while ((dirp = readdir(sourceDir)) != NULL){
        if (strcmp(currDir, dirp->d_name) == 0 || strcmp(parentDir, dirp->d_name) == 0)
            continue;
        memset(temp + pathLen, 0, strlen(temp) - pathLen);
        strcat(temp, dirp->d_name);
        if (strcmp(dirp->d_name, searchStr) == 0)
            return true;
        lstat(temp, &fileStat);
        if ((fileStat.st_mode & S_IFMT) == S_IFDIR && access(temp, R_OK) == 0){
            searchStack.push(temp);
            if (searchUtil(searchStr, searchStack) == true)
                return true;
            searchStack.pop();
        }
    }
    closedir(sourceDir);
    // free(temp);
    return false;
}