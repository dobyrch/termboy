struct Ananke {
  #include "configuration.cpp"
  string libraryPath;

  Ananke();

  struct Information {
    string path;      //path to selected file
    string name;      //name of selected file (inside of archive if .zip)
    string archive;   //pathname of archive
    string manifest;  //manifest from successfully applied patch
  } information;

  //archive.cpp
  vector<uint8_t> extractROM();
  vector<uint8_t> extractFile(const string &filename);

  //patch.cpp
  void applyBeatPatch(vector<uint8_t> &buffer);

  //game-boy.cpp
  void copyGameBoySaves(const string &pathname);
  string createGameBoyHeuristic(vector<uint8_t> &buffer);
  string openGameBoy(vector<uint8_t> &buffer);
  string syncGameBoy(const string &pathname);

  static bool supported(const string &filename);
  string open(string filename = "");
  string sync(string pathname);
};
