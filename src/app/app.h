#ifndef APP_H
#define APP_H

#include <ncurses.h>

#include <list>
#include <string>

#include "../repository.h"

// Singleton pattern for easy access to the ui part of the app
class App {
 private:
  App();
  static App *instance;
  WINDOW *mainWinBorder;
  WINDOW *mainWinField;
  WINDOW *promptWinBorder;
  WINDOW *promptWinField;

 public:
  ~App();
  static App *getInstance();
  void drawMainWinList(std::string, int, std::list<Repository>,
                       std::list<Repository>);
  void deleteInPrompt(std::string &);
  void typeInPrompt(std::string);
  void drawPromptWin();
  void drawMainWin();
  void stop();

  WINDOW *TMPgetPromptWinField();
};

#endif
