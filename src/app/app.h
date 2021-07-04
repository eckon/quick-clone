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

  Prompt selectedPrompt;

 public:
  ~App();

  static App *getInstance();

  void drawMainWinList(ResourceCollection &collection);
  void deleteInPrompt();
  void typeInPrompt();
  void drawPromptWin();
  void drawMainWin();
  void drawModal(std::string message);
  void stop();
  int getKeyPress();
  void pushKey(int);
  void nextPrompt();
  void previousPrompt();
  ResourceCollection requestResources();

  Prompt getSelectedPrompt();
  std::string TMPgetUserInput();
};

#endif
