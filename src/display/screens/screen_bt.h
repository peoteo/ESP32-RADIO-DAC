#ifndef SCREEN_BT_H
#define SCREEN_BT_H

#include "screen.h"

class ScreenBT: public Screen {
public:
  ScreenBT();
  void render() override;
  void dynamicRender() override;
  void setDLabel(const char* label, uint8_t label_id) override;

  void onSelect() override;
  void onExit() override;

private:
  char _dev_name[41] = "not connected";
};

#endif