#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <linux/kd.h>
#include <ncursesw/curses.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

namespace input {

struct AbstractInput {
  string name;
  string mapping;
  bool logic;  //0 = OR, 1 = AND
  bool state;

  struct Input {
    enum class Type : unsigned { Button, MouseButton, MouseAxis, HatUp, HatDown, HatLeft, HatRight, Axis, AxisLo, AxisHi } type;
    unsigned scancode;
  };
  vector<Input> inputList;

  void bind();
  bool append(string mapping);
  virtual bool bind(unsigned scancode, int16_t value) = 0;
  virtual int16_t poll() = 0;
  AbstractInput();
};

struct DigitalInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct RelativeInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct AbsoluteInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(unsigned scancode, int16_t value);
  int16_t poll();
};

struct HotkeyInput : DigitalInput {
  function<void ()> press;
  function<void ()> release;
  HotkeyInput();
};

struct InputManager {
  //vector<AbstractInput*> inputMap;
  //vector<HotkeyInput*> hotkeyMap;
  //bool activeScancode;
  termios tty_attr_old;
  int old_keyboard_mode;

  int setupKeyboard();
  void restoreKeyboard();

  /*void bind();
  void poll();
  int16_t poll(unsigned scancode);
  void saveConfiguration();
  void bootstrap();
  */
  InputManager();

  //hotkeys.cpp
  /*void appendHotkeys();
  void pollHotkeys();
  */

/*private:
  Configuration::Document config;
  */
};

extern InputManager* inputManager;
}
