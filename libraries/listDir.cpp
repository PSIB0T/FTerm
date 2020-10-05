#include "./../includes/listDir.h"

void listContents(const char * cwd){
    DIR *dp;
    struct dirent * dirp;
    if ((dp = opendir(cwd)) == NULL) {
        perror("DError");
        exit(1);
    }
    char * res = realpath(cwd, currBuff);
    dirList.clear();
    dirList.push_back(currDir);
    dirList.push_back(parentDir);
    while ((dirp = readdir(dp)) != NULL){
        if (strcmp(currDir, dirp->d_name) != 0 && strcmp(parentDir, dirp->d_name) != 0){
            dirList.push_back(dirp->d_name);
        }
    }
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int limit = ws.ws_row > dirList.size()?dirList.size():ws.ws_row - 1;
    y = 0;
    x = 0;
    start = 0;
    end = limit;
    updateScreen();
    // printf("\033[2A");
        // printf("%s\t%lu\n", dirp->d_name, dirp->d_ino);
    closedir(dp);
}

void updateScreen(){
    printf("\033[H\033[J");
    for (int i = start; i < end; i++){
        listFile(dirList[i]);
    }
    printf("\033[%d;0H", (y + 1));
}

void listFile(const char * fName){
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    char temp[PATH_MAX] = "";
    struct stat fileStat;
    strcat(temp, currBuff);
    if (temp[strlen(temp) - 1] != '/'){
        strcat(temp, "/");
    }
    strcat(temp, fName);
    lstat(temp, &fileStat);
    switch(fileStat.st_mode & S_IFMT){
        case S_IFBLK:
        printf("b");
        break;
        case S_IFCHR:
        printf("c");
        break;
        case S_IFDIR:
        printf("d");
        break;
        case S_IFIFO:
        printf("p");
        break;
        case S_IFREG:
        printf("-");
        break;
        case S_IFSOCK:
        printf("s");
        break;
        case S_IFLNK:
        printf("l");
        break;

    }
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    printf("\t%s", fName);

    printf("\n");
}