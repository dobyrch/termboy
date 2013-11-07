#include <signal.h>
#include <stdlib.h>
#include "ethos.hpp"
#include "bootstrap.cpp"
#include "resource/resource.cpp"
#include "../ananke/ananke.hpp"

Program* program = nullptr;
DSP dspaudio;

Emulator::Interface& system() {
  if(program->active == nullptr) throw;
  return *program->active;
}

string Program::path(string name) {
  string path = {basepath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {userpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  path = {sharedpath, name};
  if(file::exists(path) || directory::exists(path)) return path;
  return {userpath, name};
}

void Program::init_curses() {

  setlocale(LC_ALL, "");
  //window = subwin(initscr(), 200, 200, 0, 0);
  window= initscr();
  start_color();

  timeout(0);
  noecho();

  /*if (!has_colors() || ! can_change_color()) {
    //TODO: Add descriptive error message, clean up
    exit(EXIT_FAILURE);
  }*/

  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_BLACK, COLOR_GREEN);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);

  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_RED);
  init_pair(7, COLOR_RED, COLOR_GREEN);
  init_pair(8, COLOR_RED, COLOR_YELLOW);

  init_pair(9, COLOR_GREEN, COLOR_BLACK);
  init_pair(10, COLOR_GREEN, COLOR_RED);
  init_pair(11, COLOR_GREEN, COLOR_GREEN);
  init_pair(12, COLOR_GREEN, COLOR_YELLOW);

  init_pair(13, COLOR_YELLOW, COLOR_BLACK);
  init_pair(14, COLOR_YELLOW, COLOR_RED);
  init_pair(15, COLOR_YELLOW, COLOR_GREEN);
  init_pair(16, COLOR_YELLOW, COLOR_YELLOW);

  init_color(COLOR_WHITE, 0, 0, 0);
  init_color(COLOR_BLACK, 0, 0, 0);
  init_color(COLOR_RED, 333, 333, 333);
  init_color(COLOR_GREEN, 667, 667, 667);
  init_color(COLOR_YELLOW, 1000, 1000, 1000);

  erase();
}

void Program::main() {
  system().run();
}

Program::Program(int argc, char** argv) {
  program = this;

  basepath = dir(realpath(argv[0]));
  userpath = {nall::configpath(), "termboy/"};
  sharedpath = {nall::sharedpath(), "termboy/"};
  directory::create(userpath);

  bootstrap();
  active = nullptr;

  config = new ConfigurationSettings;
  utility = new Utility;

  audio.driver("ALSA");

  if(audio.init() == false) {
    audio.driver("None");
    audio.init();
  }

  init_curses();
  inputManager = new InputManager();
  inputManager->setupKeyboard();

  dspaudio.setPrecision(16);
  dspaudio.setBalance(0.0);
  dspaudio.setFrequency(96000);

  utility->synchronizeRuby();
  utility->updateShader();

  if(argc >= 2)
    utility->loadMedia(argv[1]);

  //TODO:  This is bad! Remove hardcoded string and use appropriate path
  //TODO:  periodically sync RAM in case of crash?
  Ananke ananke;
  ananke.sync("/home/dobyrch/ROMs/Game Boy/pokemon_blue.gb");
  while(true) {
    main();
  }

  utility->unload();
  //config->save();
}

//TODO: Come up with a solution that is guaranteed to work when multiple threads are running
void sighandler(int sig) {
  endwin();
  inputManager->restoreKeyboard();
  utility->unload();
  printf("Caught signal: %d\n", sig);
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  signal(SIGINT, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGSEGV, sighandler);

  audio.clear();

  new Program(argc, argv);
  delete program;
  return 0;
}
