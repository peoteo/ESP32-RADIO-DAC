#include "screen_bt.h"

ScreenBT::ScreenBT() {
  _isDynamic = true;
  _isReactive = false;
}

void ScreenBT::render() {
    _cursor->printAt("      BT  mode      ", 0, 0);
    _cursor->printAt("dev:                ", 1, 0);
  
    _cursor->printAt("[home] <            ", 3, 0);
}

void ScreenBT::dynamicRender() {
  _cursor->printAutoScroll(_dev_name, 1, 5);
}

void ScreenBT::setDLabel(const char* label, uint8_t label_id) {
  if (!label) {
    snprintf(_dev_name, 41, "not connected");
    return;
  }
  snprintf(_dev_name, 41, "%s", label);
}

void ScreenBT::onSelect() {
  if (!onCommand) return;
  onCommand(Command::GoToHome, 0);
}

void ScreenBT::onExit() {
  if (!onCommand) return;
  onCommand(Command::StopBT, 0);
}