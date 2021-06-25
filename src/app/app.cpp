#include "app.h"

#define COLOR_HIGHLIGHT 1

App *App::instance = 0;

App *App::getInstance() {
  if (instance == 0) {
    instance = new App();
  }
  return instance;
}

App::~App() { endwin(); }

App::App() {
  // TODO: loading animation when waiting for curl
  // TODO: use different mode so that it is nonblocking (halfdelay/timeout)
  // might be needed later on when getting http responses
  initscr();  // init ncurses
  noecho();   // do not echo the button press
  cbreak();   // set mode

  // setup colors
  if (!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(COLOR_HIGHLIGHT, COLOR_BLUE, COLOR_BLACK);

  // prepare prompt for user input
  this->userInput = "";
}

int App::getKeyPress() {
  // always move the cursor to the prompt so we can show the cursor to the user
  int cursorXPosition = this->userInput.length();
  wmove(this->promptWinField, 0, cursorXPosition);
  return wgetch(this->promptWinField);
}

void App::pushKey(int key) {
  // small restriction to not allow user input beyond visible field
  int promptWidth = getmaxx(this->promptWinField);
  if (promptWidth <= (int)this->userInput.length() + 1) return;

  this->userInput.push_back(key);
}

void App::drawMainWinList(ResourceCollection &collection) {
  // TODO allow multiple filter words (maybe indicated by SPACE)
  std::string filter = this->userInput;

  werase(this->mainWinField);

  auto filteredResources = collection.getFilteredResources(filter);

  // check if the selected element is hidden, if so, reset selected
  if (collection.resources[collection.selected].hidden) {
    bool previousExists = !collection.previous();
    if (previousExists) collection.next();
  }

  int selectedIndex = collection.getFilteredSelectedIndex(filteredResources);

  // calculate how far the view needs to be shifted, to have selection always
  // in focus this currently results in the cursor sticking to the bottom
  int maxHeight = getmaxy(this->mainWinField);
  int offset = (selectedIndex + 1) - maxHeight;
  if (offset < 0) offset = 0;

  // TODO: instead of offset, position highlight in center on scroll
  // TODO: add good fuzzyfind implementation
  // TODO: combine filtering and showing more closely together to make
  // highlighting easier but not as it is now (merged together) but not as it is
  // now (merged together)
  int row = 0;
  for (auto const &resource : filteredResources) {
    if (row == selectedIndex) wattron(this->mainWinField, A_REVERSE);

    // get the position of the substring to highlight its length
    int highlightPosition = resource.repository.ssh_url_to_repo.find(filter);
    bool hasFilter = filter.size() > 0;

    // instead of printing the whole line with "mvwprintw"
    // I use the char wise method to be able to highlight individual chars
    int maxColumns = (int)resource.repository.ssh_url_to_repo.size();
    for (int column = 0; column < maxColumns; column++) {
      // activate highlight starting when we are at the found position
      if (hasFilter && column >= highlightPosition) {
        wattron(this->mainWinField, COLOR_PAIR(COLOR_HIGHLIGHT));
      }

      // deactivate highlight if we shot over the last position
      if (column >= highlightPosition + (int)filter.size()) {
        wattroff(this->mainWinField, COLOR_PAIR(COLOR_HIGHLIGHT));
      }
      mvwaddch(this->mainWinField, row - offset, column,
               resource.repository.ssh_url_to_repo[column]);
      wattroff(this->mainWinField, COLOR_PAIR(COLOR_HIGHLIGHT));
    }
    wattroff(this->mainWinField, A_REVERSE);
    row++;
  }
  wrefresh(this->mainWinField);
}

void App::deleteInPrompt() {
  if (this->userInput.size() <= 0) return;

  this->userInput.pop_back();
  // overwrite character with a space to simulate deletion
  // mainly to prevent of redrawing everything
  mvwprintw(this->promptWinField, 0, this->userInput.length(), " ");
  wrefresh(this->promptWinField);
}

void App::typeInPrompt() {
  mvwprintw(this->promptWinField, 0, 0, this->userInput.c_str());
  wrefresh(this->promptWinField);
}

void App::drawPromptWin() {
  int yMax, xMax, promptHeight, promptWidth, promptStartY, promptStartX;
  getmaxyx(stdscr, yMax, xMax);
  promptHeight = 3;
  promptWidth = xMax;
  promptStartY = yMax - promptHeight;
  promptStartX = 0;

  // Surrounding box of input field
  this->promptWinBorder =
      newwin(promptHeight, promptWidth, promptStartY, promptStartX);
  box(this->promptWinBorder, 0, 0);
  mvwprintw(this->promptWinBorder, 0, 1, "Prompt");
  wrefresh(this->promptWinBorder);

  // Info: Ignore scroll, because filter should be shorter than links anyways
  this->promptWinField =
      newwin(1, promptWidth - 2, promptStartY + 1, promptStartX + 1);
  keypad(this->promptWinField, true);

  // make the prompt non-blocking, meaning we do not wait on user input
  nodelay(this->promptWinField, true);
  wrefresh(this->promptWinField);
}

void App::drawMainWin() {
  int yMax, xMax, mainHeight, mainWidth, mainStartY, mainStartX;
  getmaxyx(stdscr, yMax, xMax);
  mainHeight = yMax;
  mainWidth = xMax;
  mainStartX = mainStartY = 0;

  // Surrounding box of input field
  this->mainWinBorder =
      newwin(mainHeight - 3, mainWidth, mainStartY, mainStartX);
  box(this->mainWinBorder, 0, 0);
  mvwprintw(this->mainWinBorder, 0, 1, "Main");
  wrefresh(this->mainWinBorder);

  // Input field
  this->mainWinField =
      newwin(mainHeight - 5, mainWidth - 2, mainStartY + 1, mainStartX + 1);
  wrefresh(this->mainWinField);
}
