#include "includes/fTerm.h"

int main(int argc, char * argv[]){
    char * path = currDir;
    if (argc == 2){
        path = argv[1];
    }
    enableNCMode();
    listContents(path);
    normalMode();
    return 0;
}