#include <ncurses.h>

#include <iostream>
#include <string>

#include "api.h"
#include "app/app.h"
#include "repository.h"

#define DEV false

// TODO: these global ones should be put with the ui into its own class
// TODO: make one construct out of these two resources, use one and toggle with
// something like "hide" their state
std::list<Repository> resources = {};
std::list<Repository> filteredResources = {};
std::string userInput = "";
int selected = 0;

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

  App* app = App::getInstance();

  // TODO: draw information about highlighted entry
  // TODO: add way of changing parameter etc
  // Initial draw of windows
  app->drawMainWin();
  app->drawPromptWin();

  // Add data for the list (in MainWin) and draw it
  // TODO: get data from user inside of application not on startup
  if (DEV) {
    for (int i = 0; i < 100; i++) {
      Repository repo("Example Number " + std::to_string(i), "", "", "", "");
      resources.push_back(repo);
      filteredResources.push_back(repo);
    }
  } else {
    try {
      resources = getRepoResources(searchValue);
      filteredResources = getRepoResources(searchValue);
    } catch (std::string error) {
      // TODO: Print error somewhere where it can be seen
      // mvwprintw(mainWinField, 0, 0, error.c_str());
      // wrefresh(mainWinField);
      // wgetch(mainWinField);
      delete app;
      return 1;
    }
  }

  app->drawMainWinList(userInput, selected, filteredResources, resources);

  // Prompt Data
  int keyPress;

  while (true) {
    // Terminate on ENTER
    if (keyPress == 10) break;

    keyPress = wgetch(app->TMPgetPromptWinField());
    switch (keyPress) {
      case KEY_UP:
        if (selected > 0) {
          selected--;
          app->drawMainWinList(userInput, selected, filteredResources, resources);
        }
        break;
      case KEY_DOWN:
        if (selected < filteredResources.size() - 1) {
          selected++;
          app->drawMainWinList(userInput, selected, filteredResources, resources);
        }
        break;
      case KEY_BACKSPACE:
        if (userInput.length() > 0) {
          app->deleteInPrompt(userInput);
          app->drawMainWinList(userInput, selected, filteredResources, resources);
        }
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        app->drawMainWin();
        app->drawMainWinList(userInput, selected, filteredResources, resources);
        app->drawPromptWin();
        app->typeInPrompt(userInput);
      default:
        // TODO: allow better typing, meaning show cursor, let user position it to remove spcific parts, maybe even short hands (C-W) to delete words etc etc. Things that I would consider basic in this part
        // if printable, append to the user input
        if (isprint(keyPress)) {
          userInput.push_back(keyPress);
          app->typeInPrompt(userInput);
          app->drawMainWinList(userInput, selected, filteredResources, resources);
        }
    }
  }

  delete app;

  // Final process -> get selected value and clone the repo in current directory
  Repository selectedRepository =
      *std::next(filteredResources.begin(), selected);

  std::string command = "git clone " + selectedRepository.ssh_url_to_repo;
  std::system(command.c_str());

  return 0;
}
