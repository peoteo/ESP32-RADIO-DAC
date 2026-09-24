#ifndef SCREEN_HOME_H
#define SCREEN_HOME_H

#include "screen.h"

class ScreenHome: public Screen {
public:
  void render() override;

  void onUp() override;
  void onDown() override;
  void onSelect() override;

private:
  uint8_t _selected = 0;
  uint8_t _max_select = 3;
};

#endif