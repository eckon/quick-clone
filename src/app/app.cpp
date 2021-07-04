#include "app.h"

#include "../data/api.h"

#define COLOR_HIGHLIGHT 1
#define COLOR_MODAL_BORDER 2

App *App::instance = 0;

App *App::getInstance() {
  if (instance == 0) {
    instance = new App();
  }
  return instance;
}

App::~App() { endwin(); }

App::App() {
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
  init_pair(COLOR_MODAL_BORDER, COLOR_RED, COLOR_BLACK);

  this->selectedPrompt = Prompt::Query;

  this->collection = ResourceCollection();
  // prepare prompt for user input
  this->userInput = "";

  // save string of specific user input
  this->filterString = "";
  this->queryString = "";
}

int App::getKeyPress() {
  // always move the cursor to the prompt so we can show the cursor to the user
  int cursorXPosition = this->userInput.length();
  wmove(this->promptWinField, 0, cursorXPosition);

  // TODO: figure out why the wgetch() is needed for my screen to be rendered
  // I want to be able to draw without a sideeffect
  // meaning without while loop i want to see the application
  return wgetch(this->promptWinField);
}

void App::pushKey(int key) {
  // small restriction to not allow user input beyond visible field
  int promptWidth = getmaxx(this->promptWinField);
  if (promptWidth <= (int)this->userInput.length() + 1) return;

  this->userInput.push_back(key);
}

void App::drawMainWinList() {
  if (this->collection.resources.size() <= 0) return;

  // TODO: do this differently, like this selection is still changeable
  // QUICKFIX: if query, only print collection, nothing else
  if (this->selectedPrompt == Prompt::Query) {
    int row = 0;
    for (auto const &resource : this->collection.resources) {
      mvwprintw(this->mainWinField, row, 0,
                resource.repository.ssh_url_to_repo.c_str());
      row++;
    }

    wrefresh(this->mainWinField);
    return;
  }

  std::string filter = this->userInput;

  werase(this->mainWinField);

  auto filteredResources = this->collection.getFilteredResources(filter);

  // check if the selected element is hidden, if so, reset selected
  if (this->collection.resources[this->collection.selected].hidden) {
    bool previousExists = !this->collection.previous();
    if (previousExists) this->collection.next();
  }

  int selectedIndex =
      this->collection.getFilteredSelectedIndex(filteredResources);

  // calculate how far the view needs to be shifted, to have selection always
  // in focus this currently results in the cursor sticking to the bottom
  int maxHeight = getmaxy(this->mainWinField);
  int offset = (selectedIndex + 1) - maxHeight;
  if (offset < 0) offset = 0;

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

  // Add highlight to prompt title
  if (this->selectedPrompt == Prompt::Query)
    wattron(this->promptWinBorder, COLOR_PAIR(COLOR_HIGHLIGHT));
  mvwprintw(this->promptWinBorder, 0, 1, "Query");
  wattroff(this->promptWinBorder, COLOR_PAIR(COLOR_HIGHLIGHT));

  if (this->selectedPrompt == Prompt::Filter)
    wattron(this->promptWinBorder, COLOR_PAIR(COLOR_HIGHLIGHT));
  mvwprintw(this->promptWinBorder, 0, 7, "Filter");
  wattroff(this->promptWinBorder, COLOR_PAIR(COLOR_HIGHLIGHT));

  wrefresh(this->promptWinBorder);

  // Info: Ignore scroll, because filter should be shorter than links anyways
  this->promptWinField =
      newwin(1, promptWidth - 2, promptStartY + 1, promptStartX + 1);
  keypad(this->promptWinField, true);

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
  mvwprintw(this->mainWinBorder, 0, 1, "Repository List");
  wrefresh(this->mainWinBorder);

  // Input field
  this->mainWinField =
      newwin(mainHeight - 5, mainWidth - 2, mainStartY + 1, mainStartX + 1);
  wrefresh(this->mainWinField);
}

void App::drawModal(std::string message) {
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);

  int padding = 3;
  int padded = xMax - 2 * padding;
  int modalStartX = padding;
  int modalStartY = 5;
  int modalWidth = padded;
  int modalHeight = 10;

  this->modalWinBorder =
      newwin(modalHeight, modalWidth, modalStartY, modalStartX);
  wattron(this->modalWinBorder, COLOR_PAIR(COLOR_MODAL_BORDER));
  box(this->modalWinBorder, 0, 0);
  wattron(this->modalWinBorder, COLOR_PAIR(COLOR_MODAL_BORDER));
  mvwprintw(this->modalWinBorder, 0, 1, "Modal");
  wrefresh(this->modalWinBorder);

  int yMaxModal, xMaxModal;
  getmaxyx(this->modalWinBorder, yMaxModal, xMaxModal);

  this->modalWinField =
      newwin(yMaxModal - 5, xMaxModal - 4, modalStartY + 2, modalStartX + 2);
  mvwprintw(this->modalWinField, 0, 0, message.c_str());
  wrefresh(this->modalWinField);
}

Prompt App::getSelectedPrompt() { return this->selectedPrompt; }

void App::previousPrompt() {
  // have only 2 prompts, so next/previous are the same
  this->nextPrompt();
}

void App::nextPrompt() {
  switch (this->selectedPrompt) {
      // switch from query to prompt
    case Prompt::Query:
      this->selectedPrompt = Prompt::Filter;
      this->queryString = this->userInput.c_str();
      this->userInput = this->filterString.c_str();
      break;

      // switch from prompt to query
    case Prompt::Filter:
      this->selectedPrompt = Prompt::Query;
      this->filterString = this->userInput.c_str();
      this->userInput = this->queryString.c_str();
      break;
  }

  werase(this->promptWinField);
  this->drawPromptWin();
  this->typeInPrompt();
  wrefresh(this->promptWinField);
}

void App::requestResources() {
  // on enter if in query -> request new data from api
  this->drawModal("Loading with query: " + this->userInput);

  ResourceCollection collection;
  try {
    std::vector<Repository> repositories = getRepoResources(this->userInput);

    if (repositories.empty()) {
      this->drawModal("Search query \"" + this->userInput +
                      "\" resulted in no hits.");
      this->collection = collection;
      return;
    }

    // just overwrite the collection with the new data
    this->collection = ResourceCollection(repositories);
    this->drawMainWinList();

    // QoL: go to the filter prompt
    this->nextPrompt();
  } catch (std::string error) {
    // simple error modal, with inf loop (because we are non blocking)
    this->drawModal(error);
    this->getKeyPress();
    delete this;
    std::exit(1);
  }

  return;
}

void App::nextItem() { this->collection.next(); }
void App::previousItem() { this->collection.previous(); }

Repository App::getSelectedRepository() {
  bool noItemSelected = this->collection.selected == -1;
  bool emptyCollection = this->collection.resources.size() <= 0;
  if (noItemSelected || emptyCollection)
    throw std::string("No Repository was selected.");

  Repository selectedResource =
      this->collection.resources[this->collection.selected].repository;

  return selectedResource;
}
