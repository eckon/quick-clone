#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "app/prompt.h"

int main() {
  App *app = App::getInstance();

  app->drawMainWin();
  app->drawPromptWin();
  app->drawMainWinList();

  int keyPress;
  bool running = true;
  while (running) {
    keyPress = app->getKeyPress();

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
      case 10: {
        // On ENTER
        switch (app->getSelectedPrompt()) {
          case Prompt::Filter:
            // exit infinite loop to execute git clone
            running = false;
            break;
          case Prompt::Query:
            app->requestResources();
            break;
        }
      }; break;
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
