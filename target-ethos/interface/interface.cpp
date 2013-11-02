#include <ncursesw/curses.h>
#include "../ethos.hpp"
Interface* interface = nullptr;

void Interface::loadRequest(unsigned id, string name, string type) {
  return utility->loadRequest(id, name, type);
}

void Interface::loadRequest(unsigned id, string path) {
  return utility->loadRequest(id, path);
}

void Interface::saveRequest(unsigned id, string path) {
  return utility->saveRequest(id, path);
}

uint32_t Interface::videoColor(unsigned source, uint16_t r, uint16_t g, uint16_t b) {
  if(config->video.saturation != 100) {
    uint16_t grayscale = uclamp<16>((r + g + b) / 3);
    double saturation = config->video.saturation * 0.01;
    double inverse = max(0.0, 1.0 - saturation);
    r = uclamp<16>(r * saturation + grayscale * inverse);
    g = uclamp<16>(g * saturation + grayscale * inverse);
    b = uclamp<16>(b * saturation + grayscale * inverse);
  }

  if(config->video.gamma != 100) {
    double exponent = config->video.gamma * 0.01;
    double reciprocal = 1.0 / 32767.0;
    r = r > 32767 ? r : 32767 * pow(r * reciprocal, exponent);
    g = g > 32767 ? g : 32767 * pow(g * reciprocal, exponent);
    b = b > 32767 ? b : 32767 * pow(b * reciprocal, exponent);
  }

  if(config->video.luminance != 100) {
    double luminance = config->video.luminance * 0.01;
    r = r * luminance;
    g = g * luminance;
    b = b * luminance;
  }

  if(program->depth == 30) {
    r >>= 6, g >>= 6, b >>= 6;
    return r << 20 | g << 10 | b << 0;
  }

  //TODO: find where color depth gets set
  if(program->depth == 0) {
    r >>= 8, g >>= 8, b >>= 8;
    return r << 16 | g << 8 | b << 0;
  }

  return 0u;
}

void Interface::videoRefresh(const uint32_t* data, unsigned pitch, unsigned width, unsigned height) {
  static int black = 0;
  static int red = 1;
  static int green = 2;
  static int yellow = 3;
  int ymax, xmax, yout, xout;
  int cpair = 0;

  getmaxyx(program->window, ymax, xmax);


  //Unicode LEFT HALF BLOCK
  wchar_t block[] = {L'\u258c', L'\0'};

  //TODO: draw on separate thread to help with audio crackling
  for (int x = 0; x < width; x += 2) {
    for (int y = 0; y < height; ++y) {
        switch (data[x + width*y]) {
          case 0x052505:
            cpair = black;
            break;
          case 0x1d551d:
            cpair = red;
            break;
          case 0x8bac05:
            cpair = green;
            break;
          case 0x9abb05:
            cpair = yellow;
            break;
        }

        cpair = cpair * 4;

        switch (data[x + width*y + 1]) {
          case 0x052505:
            cpair += black;
            break;
          case 0x1d551d:
            cpair += red;
            break;
          case 0x8bac05:
            cpair += green;
            break;
          case 0x9abb05:
            cpair += yellow;
            break;
        }

        cpair += 1;

        yout = y;
        xout = x/2;

        if (height < ymax)
          yout += ymax/2 - height/2;
        if (width/2 < xmax)
          xout += xmax/2 - width/4;

        attron(COLOR_PAIR(cpair));
        mvaddwstr(yout, xout, block);
        attroff(COLOR_PAIR(cpair));
    }
  }

  refresh();

  /*static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    utility->setStatusText({"FPS: ", frameCounter});
    frameCounter = 0;
  } */
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = {lsample, rsample};
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

int16_t Interface::inputPoll(unsigned port, unsigned device, unsigned input) {
  //unsigned guid = system().port[port].device[device].input[input].guid;
  return 0;//inputManager->inputMap[guid]->poll();
}

unsigned Interface::dipSettings(const Markup::Node& node) {
  //return dipSwitches->run(node);
  return 0;
}

string Interface::path(unsigned group) {
  return utility->path(group);
}

string Interface::server() {
  return {
    config->server.username, ":",
    config->server.password, "@",
    config->server.hostname
  };
}

void Interface::notify(string text) {
  //MessageWindow().setParent(*presentation).setText(text).information();
}
