#include <ncurses.h>

#include <iostream>
#include <string>

#include "api.h"
#include "repository.h"

#define DEV false

WINDOW *mainWinBorder;
WINDOW *mainWinField;
WINDOW *promptWinBorder;
WINDOW *promptWinField;

void drawPromptWin();
void drawMainWin();
void drawMainWinList(std::list<Repository> resources, int selected,
                     std::string filter);
bool fuzzyFindFilter(Repository resource, std::string filter);
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

  // TODO: draw information about highlighted entry
  // TODO: add way of changing parameter etc
  // Initial draw of windows
  drawMainWin();
  drawPromptWin();

  // Add data for the list (in MainWin) and draw it
  // TODO: get data from user inside of application not on startup
  // TODO: the own class should allow things like selected etc
  // this should make filtering and highlighting easier
  std::list<Repository> resources = {};
  if (DEV) {
    for (int i = 0; i < 100; i++) {
      Repository repo("Example Number " + std::to_string(i), "", "", "", "");
      resources.push_back(repo);
    }
  } else {
    resources = getRepoResources(searchValue);
  }

  int selected = 0;
  drawMainWinList(resources, selected, "");

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
          drawMainWinList(resources, selected, userInput);
        }
        break;
      case KEY_DOWN:
        if (selected < resources.size() - 1) {
          selected++;
          drawMainWinList(resources, selected, userInput);
        }
        break;
      case KEY_BACKSPACE:
        if (userInput.length() > 0) {
          deleteInPrompt(userInput);
          drawMainWinList(resources, selected, userInput);
        }
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        drawMainWin();
        drawMainWinList(resources, selected, userInput);
        drawPromptWin();
        typeInPrompt(userInput.c_str());
      default:
        // if printable, append to the user input
        if (isprint(keyPress)) {
          userInput.push_back(keyPress);
          typeInPrompt(userInput.c_str());
          drawMainWinList(resources, selected, userInput);
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

bool fuzzyFindFilter(Repository resource, std::string filter) {
  // empty filter allowes everything
  if (filter.size() == 0) return true;

  // check if every char in filter exists in the resource (but in order)
  // "abc" will be okay with "a1b2c" but not "bac"
  int filterPosition = 0;
  for (auto const &r : resource.ssh_url_to_repo) {
    char filterChar = char(tolower(filter[filterPosition]));
    if (filterChar == char(tolower(r))) {
      filterPosition++;
      if (filterPosition >= filter.size()) return true;
    }
  }

  return false;
}

void drawMainWinList(std::list<Repository> resources, int selected,
                     std::string filter) {
  // calculate how far the view needs to be shifted, to have selection always
  // in focus this currently results in the cursor sticking to the bottom
  wclear(mainWinField);
  int maxHeight = getmaxy(mainWinField);
  int offset = (selected + 1) - maxHeight;
  if (offset < 0) offset = 0;

  // TODO: highlight the matched part
  std::list<Repository> filteredResources = {};
  for (auto const &resource : resources) {
    bool valid = fuzzyFindFilter(resource, filter);
    if (valid) {
      filteredResources.push_back(resource);
    }
  }

  int i = 0;
  for (auto const &resource : filteredResources) {
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
