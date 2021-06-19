#include <ncurses.h>

#include <iostream>
#include <string>

#include "api.h"

WINDOW *mainWin;
WINDOW *promptWin;

void drawPromptWin();
void drawMainWin();
void drawMainWinList(std::list<std::string> resources, int selected);

int main(int argc, char **argv) {
  // TODO:
  // use different mode so that it is nonblocking (halfdelay/timeout)
  // might be needed later on when getting http responses
  initscr();    // init ncurses
  noecho();     // do not echo the button press
  cbreak();     // set mode
  curs_set(0);  // hide cursor

  // Initial draw of windows
  drawMainWin();
  drawPromptWin();

  // Add data for the list (in MainWin) and draw it
  // auto resources = getRepoResources();
  // for (auto const &route : resources) std::cout << route << std::endl;
  std::list<std::string> resources = {
      "git@gitlab.eu:foo/singularity-elasticsearch-curator.git",
      "git@gitlab.eu:webteam/singularity-v2/singularity2-frontend.git",
      "git@gitlab.eu:composer-registry/singularity-newsletter.git",
      "git@gitlab.eu:bar/discontinued/cleverprint.git",
      "git@gitlab.eu:webteam/singularity-simple-api.git",
      "git@gitlab.eu:composer-registry/singularity-customer.git",
      "git@gitlab.eu:composer-registry/singularity-customergroup.git",
      "git@gitlab.eu:baz-registry/singularity-newsletterjob.git",
      "git@gitlab.eu:composer-registry/singularity-nodeserver.git",
      "git@gitlab.eu:composer-registry/singularity-order.git",
  };

  int selected = 0;
  drawMainWinList(resources, selected);

  // Prompt Data
  std::string userInput;
  int keyPress;

  while (true) {
    // Terminate on ENTER
    if (keyPress == 10) break;

    keyPress = wgetch(promptWin);
    switch (keyPress) {
      case KEY_UP:
        if (selected > 0) {
          selected--;
          drawMainWinList(resources, selected);
        }
        break;
      case KEY_DOWN:
        if (selected < resources.size() - 1) {
          selected++;
          drawMainWinList(resources, selected);
        }
        break;
      case KEY_BACKSPACE:
        if (userInput.length() > 0) {
          userInput.pop_back();
          mvwprintw(promptWin, 1, userInput.length() + 1, " ");
          mvwprintw(promptWin, 1, 1, userInput.c_str());
          wrefresh(promptWin);
        }
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        drawMainWin();
        drawMainWinList(resources, selected);
        drawPromptWin();
        mvwprintw(promptWin, 1, 1, userInput.c_str());
      default:
        // if printable, append to the user input
        if (isprint(keyPress)) {
          userInput.push_back(keyPress);
          mvwprintw(promptWin, 1, 1, userInput.c_str());
          wrefresh(promptWin);
        }
    }
  }

  // End ncurses
  endwin();

  // Return selected value for later usage
  printf("Selected: %s", std::next(resources.begin(), selected)->c_str());

  return 0;
}

void drawMainWinList(std::list<std::string> resources, int selected) {
  int i = 0;
  for (auto &resource : resources) {
    if (i == selected) wattron(mainWin, A_REVERSE);

    mvwprintw(mainWin, i + 1, 1, resource.c_str());
    wattroff(mainWin, A_REVERSE);
    i++;
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

  keypad(promptWin, true);
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
