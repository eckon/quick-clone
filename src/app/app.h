#ifndef APP_H
#define APP_H

#include <ncurses.h>

#include <list>
#include <string>

#include "../data/repository.h"
#include "../data/resource.h"
#include "prompt.h"

// Singleton pattern for easy access to the ui part of the app
class App {
 private:
  App();

  static App *instance;

  WINDOW *mainWinBorder;
  WINDOW *mainWinField;
  WINDOW *promptWinBorder;
  WINDOW *promptWinField;
  WINDOW *modalWinBorder;
  WINDOW *modalWinField;

  std::string userInput;
  std::string filterString;
  std::string queryString;

  ResourceCollection collection;

  Prompt selectedPrompt;

 public:
  ~App();

  static App *getInstance();

  void drawMainWin();
  void drawMainWinList();
  void drawPromptWin();
  void drawModal(std::string message);

  int getKeyPress();
  void pushKey(int key);
  void deleteInPrompt();
  void typeInPrompt();

  void nextPrompt();
  void previousPrompt();

  void nextItem();
  void previousItem();

  void requestResources();

  Prompt getSelectedPrompt();
  Repository getSelectedRepository();
};

#endif
