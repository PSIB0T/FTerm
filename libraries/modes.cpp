#include "./../includes/modes.h"

void normalMode(){
    char c[3];
    while (read(STDIN_FILENO, &c, 3) >= 1 && c[0] != 'q') {
        if (strcmp(c, UP_KEY) == 0)
            up();
        else if (strcmp(c, DOWN_KEY) == 0)
            down();
        fflush(0);
    }
}

void up(){
    if (y > 0){
        printf("\033[1A");
        y--;
    }

}

void down(){
    if (y < dirList.size() - 1){
        printf("\033[1B");
        y++;
    }

}

