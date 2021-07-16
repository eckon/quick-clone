#include <iostream>
#include <string>

#include "app/app.h"

int running = true;

void handlePromptSpecificKeyPress(int keyPress);

int main() {
  ApiConfigCollection apiConfigs;

  try {
    apiConfigs.init();
  } catch (std::string error) {
    std::cout << error << std::endl;
    return 1;
  }

  App *app = App::getInstance();

  app->setApiConfigs(apiConfigs);
  app->drawMainWin();
  app->drawPromptWin();
  app->drawMainWinList();

  while (running) {
    int keyPress = app->getKeyPress();

    switch (keyPress) {
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
      case KEY_UP:
      case KEY_DOWN:
      case 10:  // On ENTER
        handlePromptSpecificKeyPress(keyPress);
        break;
      default:
        if (isprint(keyPress)) app->pushKey(keyPress);
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

void handlePromptSpecificKeyPress(int keyPress) {
  App *app = App::getInstance();
  Prompt selectedPrompt = app->getSelectedPrompt();

  if (selectedPrompt == Prompt::Filter) {
    switch (keyPress) {
      case KEY_UP:
        app->previousItem();
        break;
      case KEY_DOWN:
        app->nextItem();
        break;
      case 10:  // On ENTER
        // exit infinite loop to execute git clone
        running = false;
        break;
    }
  }

  if (selectedPrompt == Prompt::Query) {
    switch (keyPress) {
      case KEY_UP:
        app->previousApiConfigItem();
        break;
      case KEY_DOWN:
        app->nextApiConfigItem();
        break;
      case 10:  // On ENTER
        app->requestResources();
        break;
    }
  }
}
