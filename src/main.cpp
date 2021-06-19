#include <ncurses.h>
#include <string>
using namespace std;

WINDOW *mainWin;
WINDOW *promptWin;

void drawPromptWin();
void drawMainWin();
void drawMainWinList(int dataSize, int selected, string data[]);

int main(int argc, char **argv) {
  // Start
  initscr();
  noecho();
  cbreak();
  // use different mode so that it is nonblocking (halfdelay/timeout)
  // might be needed later on when getting http responses
  curs_set(0);

  // Setup Main/Prompt Windows
  drawMainWin();
  drawPromptWin();
  keypad(promptWin, true);

  // Data to select
  string data[] = {"foo", "bar", "baz"};
  int dataSize = sizeof(data) / sizeof(*data);
  int selected = 0;

  // Prompt Data
  string userInput;

  while (true) {
    // drawMainWin();
    // drawPromptWin();
    drawMainWinList(dataSize, selected, data);

    // Print user input to prompt
    mvwprintw(promptWin, 1, 1, userInput.c_str());

    // User input (in Prompt Window)
    int keyPress = wgetch(promptWin);
    switch (keyPress) {
    case KEY_UP:
      if (selected > 0)
        selected--;
      break;
    case KEY_DOWN:
      if (selected < dataSize - 1)
        selected++;
      break;
    case KEY_BACKSPACE:
      if (userInput.length() > 0)
        userInput.pop_back();
      break;
    default:
      // if printable, append to the user input
      if (isprint(keyPress))
        userInput.push_back(keyPress);
      // mvaddch(10, 10, (char)keyPress);
      // refresh();
    }

    // On ENTER Press see if i can use KEY_ENTER or something
    if (keyPress == 10)
      break;

    wrefresh(promptWin);
  }

  // End
  endwin();

  // this would call the end function (git clone)
  printf("Selected: %s", data[selected].c_str());

  return 0;
}

void drawMainWinList(int dataSize, int selected, string data[]) {
    for (int i = 0; i < dataSize; i++) {
      if (i == selected)
        wattron(mainWin, A_REVERSE);

      mvwprintw(mainWin, i + 1, 1, data[i].c_str());
      wattroff(mainWin, A_REVERSE);
    }
    wrefresh(mainWin);
}

void drawPromptWin() {
  int yMainMax, xMainMax, yMainBeg, xMainBeg;
  getmaxyx(mainWin, yMainMax, xMainMax);
  getbegyx(mainWin, yMainBeg, xMainBeg);

  int promptHeight, promptWidth, promptStartY, promptStartX;
  promptHeight = 3;
  promptWidth = xMainMax;
  promptStartY = yMainMax - promptHeight;
  promptStartX = xMainBeg;
  promptWin = newwin(promptHeight, promptWidth, promptStartY, promptStartX);

  box(promptWin, 0, 0);
  mvwprintw(promptWin, 0, 1, "Prompt");
  wrefresh(promptWin);
}

void drawMainWin() {
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  int mainHeight, mainWidth, mainStartY, mainStartX;
  mainHeight = yMax;
  mainWidth = xMax;
  mainStartX = mainStartY = 0;
  mainWin = newwin(mainHeight, mainWidth, mainStartY, mainStartX);

  box(mainWin, 0, 0);
  mvwprintw(mainWin, 0, 1, "Main");
  wrefresh(mainWin);
}
