#include <nall/nall.hpp>
#include <nall/beat/patch.hpp>
#include "heuristics/game-boy.hpp"
using namespace nall;

//#include "resource/resource.cpp"
#include "ananke.hpp"
#include "archive.cpp"
#include "patch.cpp"
#include "game-boy.cpp"


//FileDialog *fileDialog = nullptr;

Ananke::Ananke() {
  //TODO: Don't generate Emulation directory
  //Either warn that the configuration file does not exist,
  //or put all data in a .termboy directory
  libraryPath = {configpath(), "termboy/"};
  int dummy = 0;
}

bool Ananke::supported(const string &filename) {
  string extension = nall::extension(filename);

  if(extension == "fc" ) return true;
  if(extension == "nes") return true;
  if(extension == "sfc") return true;
  if(extension == "smc") return true;
  if(extension == "st" ) return true;
  if(extension == "bs" ) return true;
  if(extension == "gb" ) return true;
  if(extension == "gbc") return true;
  if(extension == "gba") return true;
  if(extension == "zip") return true;

  return false;
}

string Ananke::open(string filename) {
  /*if(filename.empty()) {
    if(!fileDialog) {
      fileDialog = new FileDialog;
      fileDialog->setGeometry(config.geometry);
    }
    fileDialog->setPath(config.path);
    filename = fileDialog->open();
    config.geometry = fileDialog->geometry().text();
  }*/

  if(filename.empty()) return "";

  information.path = dir(filename);
  information.name = notdir(filename);
  config.path = information.path;  //remember last used directory

  vector<uint8_t> buffer;
  if(filename.endswith(".zip")) {
    information.archive = filename;
    buffer = extractROM();
  } else {
    buffer = file::read(filename);
  }
  if(buffer.size() == 0) return "";  //failed to read file

  applyBeatPatch(buffer);

  if(information.name.endswith(".gb") || information.name.endswith(".gbc")) return openGameBoy(buffer);
  return "";
}

string Ananke::sync(string pathname) {
  if(pathname.endswith(".gb/")) return syncGameBoy(pathname);
  if(pathname.endswith(".gbc/")) return syncGameBoy(pathname);
  return "";
}

extern "C" string ananke_browse(const string &filename) {
  Ananke ananke;
  return ananke.open();
}

extern "C" string ananke_open(const string &filename) {
  Ananke ananke;
  return ananke.open(filename);
}

extern "C" string ananke_sync(const string &pathname) {
  Ananke ananke;
  return ananke.sync(pathname);
}
