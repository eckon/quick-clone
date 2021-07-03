#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "data/api.h"
#include "data/repository.h"
#include "data/resource.h"

ResourceCollection TMPrequestNewData(ResourceCollection collection) {
  App *app = App::getInstance();
  // on enter if in query -> request new data from api
  auto userInput = app->TMPgetUserInput();
  app->drawModal("Loading with query: " + userInput);

  std::vector<Repository> repositories = {};
  try {
    repositories = getRepoResources(userInput);

    // TODO: handle empty repository (currently it crashes)
    if (repositories.empty()) {
      app->drawModal("Search query \"" + userInput + "\" resulted in no hits.");
      // if we did not find anything, go back to query for next search
      app->previousPrompt();
    }

    // just overwrite the collection with the new data
    collection = ResourceCollection(repositories);
    app->drawMainWinList(collection);

    // QoL: go to the filter prompt
    app->nextPrompt();
  } catch (std::string error) {
    // simple error modal, with inf loop (because we are non blocking)
    app->drawModal(error);
    app->getKeyPress();
    delete app;
    std::exit(1);
  }

  return collection;
}

int main() {
  App *app = App::getInstance();

  // TODO: draw information about highlighted entry
  // TODO: add way of changing parameter etc
  // Initial draw of windows
  app->drawMainWin();
  app->drawPromptWin();

  std::vector<Repository> repositories = {};

  // Add data for the list (in MainWin) and draw it

  // TODO: handle if the collection has no repositories (on init)
  // for now just pass one empty repo as a quick fix

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
    if (keyPress == 10) {
      // TODO: totally overhaul this, needs to be in app or other classes
      App::Prompt prompt = app->TMPgetSelectedPrompt();
      // if filter, break and continue the program -> clone the selected entry
      if (prompt == App::Prompt::Filter) break;

      if (prompt == App::Prompt::Query) {
        collection = TMPrequestNewData(collection);
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
        // TODO: allow better typing, meaning show cursor, let user position it
        // to remove spcific parts, maybe even short hands (C-W) to delete words
        // etc etc. Things that I would consider basic in this part if
        // printable, append to the user input
        // also stuff like copy&paste
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
