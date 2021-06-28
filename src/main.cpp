#include <ncurses.h>

#include <iostream>
#include <string>

#include "app/app.h"
#include "data/api.h"
#include "data/repository.h"
#include "data/resource.h"

#define DEV false

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
      Repository repo("Nothing found with: " + userInput, "", "", "", "");
      repositories.push_back(repo);
      // QUICKFIX: return to the current one (because it will go to next)
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
  // TODO: get data from user inside of application not on startup
  if (DEV) {
    for (int i = 0; i < 100; i++) {
      Repository repo("Example Number " + std::to_string(i), "", "", "", "");
      repositories.push_back(repo);
    }
  }

  // TODO: handle if the collection has no repositories (on init)
  // for now just pass one empty repo as a quick fix
  Repository tut1("Type in 'Query' to request API data", "", "", "", "");
  Repository tut2(
      "Submit in 'Query' with ENTER (this will add data to the List)", "", "",
      "", "");
  Repository tut3(
      "Type in 'Filter' to filter list (returned after 'Query' Step)", "", "",
      "", "");
  Repository tut4("Up and DOWN lets you select repository", "", "", "", "");
  Repository tut5("Clone repository in 'Filter' with ENTER", "", "", "", "");
  Repository tut6("LEFT and RIGHT lets you switch between 'Query' and 'Filter'",
                  "", "", "", "");
  Repository emptyLine("", "", "", "", "");
  repositories.push_back(tut1);
  repositories.push_back(tut2);
  repositories.push_back(emptyLine);
  repositories.push_back(tut3);
  repositories.push_back(tut4);
  repositories.push_back(tut5);
  repositories.push_back(emptyLine);
  repositories.push_back(tut6);

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

  // Final process -> get selected value and clone the repo in current directory
  Repository selectedRepository =
      collection.resources[collection.selected].repository;

  std::string command = "git clone " + selectedRepository.ssh_url_to_repo;
  std::system(command.c_str());

  return 0;
}
