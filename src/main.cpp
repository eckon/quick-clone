#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "data/repository.h"
#include "data/resource.h"

int main() {
  App *app = App::getInstance();

  // Initial draw of windows
  app->drawMainWin();
  app->drawPromptWin();

  std::vector<Repository> repositories = {};

  // Add data for the list (in MainWin) and draw it

  // TODO: the collection should not be handled here, it should be passed once
  // and then handled by the app
  ResourceCollection collection(repositories);
  app->drawMainWinList(collection);

  int keyPress;
  while (true) {
    // TODO: figure out why the wgetch() is needed for my screen to be rendered
    // I want to be able to draw without a sideeffect
    // meaning without while loop i want to see the application
    keyPress = app->getKeyPress();

    // Terminate on ENTER
    // TODO: handle key presses inside the app
    if (keyPress == 10) {
      App::Prompt prompt = app->getSelectedPrompt();
      // if filter, break and continue the program -> clone the selected entry
      if (prompt == App::Prompt::Filter) break;

      if (prompt == App::Prompt::Query) {
        // TODO: totally overhaul this, needs to be in app or other classes
        collection = app->requestResources();
      }
    }

    switch (keyPress) {
      case KEY_UP:
        collection.previous();
        break;
      case KEY_DOWN:
        collection.next();
        break;
      case KEY_BACKSPACE:
        app->deleteInPrompt();
        break;
      case KEY_RESIZE:
        // Redraw whole app when terminal gets resized
        app->drawMainWin();
        app->drawPromptWin();
        app->typeInPrompt();
        break;
      case KEY_LEFT:
        app->previousPrompt();
        break;
      case KEY_RIGHT:
        app->nextPrompt();
        break;
      default:
        if (isprint(keyPress)) {
          app->pushKey(keyPress);
          app->typeInPrompt();
        }
    }

    app->drawMainWinList(collection);
  }

  delete app;

  if (collection.selected == -1 || collection.resources.size() <= 0) {
    std::cout << "Nothing selected, quitting quick-clone" << std::endl;
    return 0;
  }

  // Final process -> get selected value and clone the repo in current directory
  Repository selectedRepository =
      collection.resources[collection.selected].repository;

  std::string command = "git clone " + selectedRepository.ssh_url_to_repo;
  std::system(command.c_str());

  return 0;
}
