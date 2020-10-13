#include "./../includes/listDir.h"

void clearDirList(){
    for (char * a: dirList){
        free(a);
    }
    dirList.clear();
}

void listContents(const char * cwd){
    DIR *dp;
    struct dirent * dirp;
    if ((dp = opendir(cwd)) == NULL) {
        perror("DError");
        exit(1);
    }
    char * res = realpath(cwd, currBuff);
    clearDirList();
    dirList.push_back(strdup(currDir));
    dirList.push_back(strdup(parentDir));
    while ((dirp = readdir(dp)) != NULL){
        if (strcmp(currDir, dirp->d_name) != 0 && strcmp(parentDir, dirp->d_name) != 0){
            dirList.push_back(strdup(dirp->d_name));
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
        // printf("Path is %s\t", dirList[i]);
        listFile(dirList[i]);
        // printf("%lu", strlen(dirList[i]));
        // getchar();
    }
    printf("\033[%d;0H", (y + 1));
}

void listFile(const char * fName){
    char timeBuff[25] = "";
    struct passwd *userInfo;
    struct group *groupInfo;
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

    // Convert time to string format
    strftime(timeBuff, 25, "%Y/%m/%d %H:%M:%S", localtime(&fileStat.st_mtim.tv_sec));

    // Extract username and group name from their respective ids
    userInfo = getpwuid(fileStat.st_uid);
    groupInfo = getgrgid(fileStat.st_gid);

    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    printf("\t%ld", fileStat.st_size);
    printf("\t%s", userInfo->pw_name);
    printf("\t%s", groupInfo->gr_name);
    printf("\t%s", timeBuff);
    printf("\t%s", fName);

    printf("\n");
}