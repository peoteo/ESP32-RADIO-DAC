#ifndef SCREEN_SOURCE_H
#define SCREEN_SOURCE_H

#include "screen.h"

class ScreenSource: public Screen {
public:
  void render() override;

  void onUp() override;
  void onDown() override;
  void onSelect() override;

private:
  uint8_t _selected = 0;
  uint8_t _max_select = 4;
};

#endif