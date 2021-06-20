#include "app.h"

#include <ncurses.h>

#include <list>
#include <string>

#include "../repository.h"

App *App::instance = 0;

App *App::getInstance() {
  if (instance == 0) {
    instance = new App();
  }
  return instance;
}

WINDOW *App::TMPgetPromptWinField() { return this->promptWinField; }

App::~App() { endwin(); }

App::App() {
  // TODO: loading animation when waiting for curl
  // TODO: use different mode so that it is nonblocking (halfdelay/timeout)
  // might be needed later on when getting http responses
  initscr();    // init ncurses
  noecho();     // do not echo the button press
  cbreak();     // set mode
  curs_set(0);  // hide cursor
}

bool App::TMPfuzzyFindFilter(Repository resource, std::string filter,
                             bool strict) {
  // empty filter allowes everything
  if (filter.size() == 0) return true;

  // if strict, try finding an exact match
  if (strict) {
    return resource.ssh_url_to_repo.find(filter) != std::string::npos;
  }

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

void App::drawMainWinList(std::string userInput, int &selected,
                          std::list<Repository> &filteredResources,
                          std::list<Repository> resources) {
  std::string filter = userInput;
  // calculate how far the view needs to be shifted, to have selection always
  // in focus this currently results in the cursor sticking to the bottom
  wclear(mainWinField);
  int maxHeight = getmaxy(mainWinField);
  int offset = (selected + 1) - maxHeight;
  if (offset < 0) offset = 0;

  // TODO: highlight the matched part
  filteredResources = {};
  for (auto const &resource : resources) {
    bool valid = this->TMPfuzzyFindFilter(resource, filter, true);
    if (valid) {
      filteredResources.push_back(resource);
    }
  }

  // if selection is bigger than filtered list -> update selected
  if (selected > filteredResources.size() - 1) {
    selected = filteredResources.size() - 1;
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

void App::deleteInPrompt(std::string &userInput) {
  userInput.pop_back();
  mvwprintw(promptWinField, 0, userInput.length(), " ");
  wrefresh(promptWinField);
}

void App::typeInPrompt(std::string userInput) {
  mvwprintw(promptWinField, 0, 0, userInput.c_str());
  wrefresh(promptWinField);
}

void App::drawPromptWin() {
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

void App::drawMainWin() {
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
