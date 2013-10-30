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
  config->save();
}

//Come up with a solution that is guaranteed to work when 
//multiple threads are running
void sighandler(int sig) {
  utility->unload();
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  signal(SIGINT, sighandler);

  audio.clear();

  new Program(argc, argv);
  delete program;
  return 0;
}
