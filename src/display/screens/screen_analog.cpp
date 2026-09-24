#include "screen_analog.h"

ScreenAnalog::ScreenAnalog() {
  _isReactive = false;
}

void ScreenAnalog::render() {
  _cursor->printAt("    Analog input    ", 0, 0);
  _cursor->printAt("                    ", 1, 0);
  _cursor->printAt("                    ", 2, 0);
  _cursor->printAt("[home] <            ", 3, 0);

}

void ScreenAnalog::onSelect() {
  if (!onCommand) return;
  onCommand(Command::GoToHome, 0);
}