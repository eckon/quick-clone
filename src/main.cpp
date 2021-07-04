#include <iostream>
#include <string>

#include "app/app.h"

int main() {
  App *app = App::getInstance();

  app->drawMainWin();
  app->drawPromptWin();
  app->drawMainWinList();

  bool running = true;

  while (running) {
    int keyPress = app->getKeyPress();

    switch (keyPress) {
      case KEY_UP:
        app->previousItem();
        break;
      case KEY_DOWN:
        app->nextItem();
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

  Resource selectedResource;
  try {
    selectedResource = app->getSelectedRepository();
  } catch (std::string error) {
    delete app;
    std::cout << error << std::endl;
    return 0;
  }

  delete app;

  // Final process -> get selected value and clone the repo in current directory
  std::string selectedRepositoryPath =
      selectedResource.repository.ssh_url_to_repo;

  std::string command = "git clone " + selectedRepositoryPath;
  std::system(command.c_str());

  return 0;
}
