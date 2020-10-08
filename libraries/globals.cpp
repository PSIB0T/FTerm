#include "./../includes/globals.h"

Node::Node(char * path){
    this->path = path;
    this->front = NULL;
    this->back = NULL;
}


const char * StackOverflowException::what() const throw (){
    return "Stackoverflow!";
}

CmdModeException::CmdModeException(char const* const message): std::runtime_error(message){}

Stack::Stack(){
    this->top = NULL;
}

void Stack::push(char * path){
    if (this->top == NULL){
        // printf("Inside new node");
        this->top = new Node(path);
        // printf("%s", dirList[2]);
    }

    else {
        Node * temp = this->top->front;
        Node * prev = temp;
        while(temp){
            temp = temp->front;
            delete prev;
            prev = temp;
        }
        this->top->front = new Node(path);
        this->top->front->back = this->top;
        this->top = this->top->front;
    }
}

void Stack::pop(){
    if (this->top != NULL){
        Node * temp = this->top;
        this->top = this->top->back;
        if (this->top)
            this->top->front = NULL;
        free(temp);
    }
}

void Stack::forward(){
    if (this->top->front){
        this->top = this->top->front;
    } else {
        throw StackOverflowException();
    }
}

void Stack::back(){
    if(this->top->back){
        this->top = this->top->back;
    } else {
        throw StackOverflowException();
    }
}

const char * Stack::getTop(){
    return this->top->path;
}

Stack history;
struct termios orig_termios;
struct termios currentMode;
struct winsize ws;
std::vector<char *> dirList;
std::vector<char *> tokens;
char currBuff[PATH_MAX];
char homeDir[PATH_MAX] = "";
char command[MAX_CMD_LENGTH] = "";
char currDir[2] = ".";
char parentDir[3] = "..";
char UP_KEY[4] = "\033[A";
char DOWN_KEY[4] = "\033[B";
char LEFT_KEY[4] = "\033[D";
char RIGHT_KEY[4] = "\033[C";
char ENTER_KEY[2] = "\012";
char BACKSPACE[2] = "\177";
char CMD_KEY[2] = "\072";
char ESCAPE[2] = "\033";
char SCROLL_UP[2] = "k";
char SCROLL_DOWN[2] = "l";
char HOME_KEY[2] = "h";
char RENAME_COMMAND[7] = "rename";
char MOVE_COMMAND[5] = "move";
char COPY_COMMAND[5] = "copy";
char CREATE_FOLDER_COMMAND[11] = "create_dir";
char CREATE_FILE_COMMAND[12] = "create_file";
char DELETE_FOLDER_COMMAND[11] = "delete_dir";
char DELETE_FILE_COMMAND[12] = "delete_file";
char GOTO_COMMAND[5] = "goto";
char SEARCH_COMMAND[7] = "search";
char userDir[PATH_MAX] = "";
char viPath[20] = "/usr/bin/vi";
int y = 0;
int x = 0;
int start = 0;
int end = 0;