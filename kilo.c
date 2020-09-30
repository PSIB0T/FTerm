#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  char c[3];
  while (read(STDIN_FILENO, &c, 3) >= 1 && c[0] != 'q') {
    // if (iscntrl(c)) {
    //   printf("%d\n", c);
    // } else {
    //   printf("%d ('%c')\n", c, c);
    // }
    printf("%lu\n", strlen(c));
    fflush(0);
  }
  return 0;
}