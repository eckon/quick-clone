#include <ncurses.h>

#include <iostream>
#include <string>

#include "api.h"
#include "repository.h"

WINDOW *mainWinBorder;
WINDOW *mainWinField;
WINDOW *promptWinBorder;
WINDOW *promptWinField;

void drawPromptWin();
void drawMainWin();
void drawMainWinList(std::list<Repository> resources, int selected);
void typeInPrompt(std::string userInput);
void deleteInPrompt(std::string &userInput);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Pass at least one parameter to this command");
    return 1;
  }

  std::string searchValue = "";
  for (int i = 1; i < argc; i++) {
    searchValue = searchValue + argv[i];
  }

  if (searchValue.size() < 3) {
    printf("The passed parameter needs to be at least 3 symbols long");
    return 1;
  }

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
  // TODO: get data from user inside of application not on startup
  // TODO: put these inside of their own struct/class for better readabillity
  // and way of adding more info (like descritpion/maintainer etc)
  // also this should make filtering and highlighting easier
  auto resources = getRepoResources(searchValue);
  // std::list<std::string> resources = {
  //     "git@gitlab.eu:foo/singularity-elasticsearch-curator.git",
  //     "git@gitlab.eu:webteam/singularity-v2/singularity2-frontend.git",
  //     "git@gitlab.eu:composer-registry/singularity-newsletter.git",
  //     "git@gitlab.eu:bar/discontinued/cleverprint.git",
  //     "git@gitlab.eu:webteam/singularity-simple-api.git",
  //     "git@gitlab.eu:composer-registry/singularity-customer.git",
  //     "git@gitlab.eu:composer-registry/singularity-customergroup.git",
  //     "git@gitlab.eu:baz-registry/singularity-newsletterjob.git",
  //     "git@gitlab.eu:composer-registry/singularity-nodeserver.git",
  //     "git@gitlab.eu:composer-registry/singularity-order.git",
  // };

  int selected = 0;
  drawMainWinList(resources, selected);

  // Prompt Data
  std::string userInput;
  int keyPress;

  while (true) {
    // Terminate on ENTER
    if (keyPress == 10) break;

    keyPress = wgetch(promptWinField);
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
          deleteInPrompt(userInput);
        }
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        drawMainWin();
        drawMainWinList(resources, selected);
        drawPromptWin();
        typeInPrompt(userInput.c_str());
      default:
        // if printable, append to the user input
        if (isprint(keyPress)) {
          userInput.push_back(keyPress);
          typeInPrompt(userInput.c_str());
        }
    }
  }

  // End ncurses
  endwin();

  // Return selected value for later usage
  printf("Selected: %s",
         std::next(resources.begin(), selected)->ssh_url_to_repo.c_str());

  return 0;
}

void drawMainWinList(std::list<Repository> resources, int selected) {
  // calculate how far the view needs to be shifted, to have selection always in
  // focus this currently results in the cursor sticking to the bottom
  wclear(mainWinField);
  int maxHeight = getmaxy(mainWinField);
  int offset = (selected + 1) - maxHeight;
  if (offset < 0) offset = 0;

  int i = 0;
  for (auto &resource : resources) {
    if (i == selected) wattron(mainWinField, A_REVERSE);

    // TODO: instead of offset, position highlight in center on scroll
    mvwprintw(mainWinField, i - offset, 0, resource.ssh_url_to_repo.c_str());
    wattroff(mainWinField, A_REVERSE);
    i++;
  }
  wrefresh(mainWinField);
}

void deleteInPrompt(std::string &userInput) {
  // TODO: all logic of inputting data should be inside its own class
  // and not be handled by different parts and functions of a big file
  userInput.pop_back();
  mvwprintw(promptWinField, 0, userInput.length(), " ");
  wrefresh(promptWinField);
}

void typeInPrompt(std::string userInput) {
  mvwprintw(promptWinField, 0, 0, userInput.c_str());
  wrefresh(promptWinField);
}

void drawPromptWin() {
  int yMax, xMax, promptHeight, promptWidth, promptStartY, promptStartX;
  getmaxyx(stdscr, yMax, xMax);
  promptHeight = 3;
  promptWidth = xMax;
  promptStartY = yMax - promptHeight;
  promptStartX = 0;

  // Surrounding box of input field
  promptWinBorder =
      newwin(promptHeight, promptWidth, promptStartY, promptStartX);
  box(promptWinBorder, 0, 0);
  mvwprintw(promptWinBorder, 0, 1, "Prompt");
  wrefresh(promptWinBorder);

  // Info: Ignore scroll, because filter should be shorter than links anyways
  promptWinField =
      newwin(1, promptWidth - 2, promptStartY + 1, promptStartX + 1);
  keypad(promptWinField, true);
  wrefresh(promptWinField);
}

// TODO: add ui class for this whole tui/ui part
void drawMainWin() {
  int yMax, xMax, mainHeight, mainWidth, mainStartY, mainStartX;
  getmaxyx(stdscr, yMax, xMax);
  mainHeight = yMax;
  mainWidth = xMax;
  mainStartX = mainStartY = 0;

  // Surrounding box of input field
  mainWinBorder = newwin(mainHeight - 3, mainWidth, mainStartY, mainStartX);
  box(mainWinBorder, 0, 0);
  mvwprintw(mainWinBorder, 0, 1, "Main");
  wrefresh(mainWinBorder);

  // Input field
  mainWinField =
      newwin(mainHeight - 5, mainWidth - 2, mainStartY + 1, mainStartX + 1);
  wrefresh(mainWinField);
}
