#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "data/api.h"
#include "data/repository.h"
#include "data/resource.h"

#define DEV true

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

  App *app = App::getInstance();

  // TODO: draw information about highlighted entry
  // TODO: add way of changing parameter etc
  // Initial draw of windows
  app->drawMainWin();
  app->drawPromptWin();

  std::vector<Repository> repositories = {};

  // Add data for the list (in MainWin) and draw it
  // TODO: get data from user inside of application not on startup
  if (DEV) {
    for (int i = 0; i < 100; i++) {
      Repository repo("Example Number " + std::to_string(i), "", "", "", "");
      repositories.push_back(repo);
    }
  } else {
    try {
      repositories = getRepoResources(searchValue);
    } catch (std::string error) {
      // simple error modal, with inf loop (because we are non blocking)
      app->drawModal(error);
      app->getKeyPress();
      delete app;
      return 1;
    }
  }

  ResourceCollection collection(repositories);
  app->drawMainWinList(collection);

  int keyPress;
  while (true) {
    // TODO: figure out why the wgetch() is needed for my screen to be rendered
    // I want to be able to draw without a sideeffect
    // meaning without while loop i want to see the application
    keyPress = app->getKeyPress();

    // Terminate on ENTER
    if (keyPress == 10) break;

    switch (keyPress) {
      case KEY_UP:
        collection.previous();
        app->drawMainWinList(collection);
        break;
      case KEY_DOWN:
        collection.next();
        app->drawMainWinList(collection);
        break;
      case KEY_BACKSPACE:
        app->deleteInPrompt();
        app->drawMainWinList(collection);
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        app->drawMainWin();
        app->drawMainWinList(collection);
        app->drawPromptWin();
        app->typeInPrompt();
        break;
      default:
        // TODO: allow better typing, meaning show cursor, let user position it
        // to remove spcific parts, maybe even short hands (C-W) to delete words
        // etc etc. Things that I would consider basic in this part if
        // printable, append to the user input
        // also stuff like copy&paste
        if (isprint(keyPress)) {
          app->pushKey(keyPress);
          app->typeInPrompt();
          app->drawMainWinList(collection);
        }
    }
  }

  delete app;

  // Final process -> get selected value and clone the repo in current directory
  Repository selectedRepository =
      collection.resources[collection.selected].repository;

  std::string command = "git clone " + selectedRepository.ssh_url_to_repo;
  std::system(command.c_str());

  return 0;
}
