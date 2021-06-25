#ifndef APP_H
#define APP_H

#include <ncurses.h>

#include <list>
#include <string>

#include "../data/repository.h"
#include "../data/resource.h"

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

  int selectedPrompt;

  std::string userInput;
  std::string searchString;
  std::string queryString;

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

  int TMPgetSelectedPrompt();
  std::string TMPgetUserInput();
};

#endif
