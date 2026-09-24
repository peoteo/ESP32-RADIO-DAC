#ifndef SCREEN_ANALOG_H
#define SCREEN_ANALOG_H

#include "screen.h"

class ScreenAnalog: public Screen {
public:
  ScreenAnalog();
  void render() override;
  void onSelect() override;
};

#endif