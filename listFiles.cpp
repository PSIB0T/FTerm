#include "includes/fTerm.h"

int main(int argc, char * argv[]){
    if (argc != 2){
        printf("%s \n", "Insufficient number of args");
        return -1;
    }
    listContents(argv[1]);
    return 0;
}