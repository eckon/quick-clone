#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "app/prompt.h"

int main() {
  App *app = App::getInstance();

  // Initial draw of windows
  app->drawMainWin();
  app->drawPromptWin();

  // Add data for the list (in MainWin) and draw it
  app->drawMainWinList();

  int keyPress;
  while (true) {
    // TODO: figure out why the wgetch() is needed for my screen to be rendered
    // I want to be able to draw without a sideeffect
    // meaning without while loop i want to see the application
    keyPress = app->getKeyPress();

    // Terminate on ENTER
    // TODO: handle key presses inside the app
    if (keyPress == 10) {
      Prompt prompt = app->getSelectedPrompt();
      // if filter, break and continue the program -> clone the selected entry
      if (prompt == Prompt::Filter) break;

      if (prompt == Prompt::Query) {
        app->requestResources();
      }
    }

    switch (keyPress) {
      case KEY_UP:
        app->TMPgetCollection()->previous();
        break;
      case KEY_DOWN:
        app->TMPgetCollection()->next();
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

    app->drawMainWinList();
  }

  // TODO: this whole part should not be done here, refactor later on
  if (app->TMPgetCollection()->selected == -1 ||
      app->TMPgetCollection()->resources.size() <= 0) {
    delete app;
    std::cout << "Nothing selected, quitting quick-clone" << std::endl;
    return 0;
  }

  // Final process -> get selected value and clone the repo in current directory
  std::string selectedRepositoryPath =
      app->TMPgetCollection()
          ->resources[app->TMPgetCollection()->selected]
          .repository.ssh_url_to_repo.c_str();

  delete app;

  std::string command = "git clone " + selectedRepositoryPath;
  std::system(command.c_str());

  return 0;
}
