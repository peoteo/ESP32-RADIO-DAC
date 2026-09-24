#include "screen_home.h"

void ScreenHome::render() {
  _cursor->printAt("   ESP32-RadioDAC   ", 0, 0);
  _cursor->printAt("- Source            ", 1, 0);
  _cursor->printAt("- Brightness        ", 2, 0);
  _cursor->printAt("                back", 3, 0);

  switch (_selected) {
    case 0: _cursor->printAt("<",1,9); return;
    case 1: _cursor->printAt("<",2,13); return;
    case 2: _cursor->printAt(">",3,14); return;
    default: break;
  }
}

void ScreenHome::onUp() {
  _selected = (_selected - 1 + _max_select)%_max_select;
}

void ScreenHome::onDown() {
  _selected = (_selected + 1)%_max_select;
}

void ScreenHome::onSelect() {
  if (!onCommand) return;

  switch (_selected) {
    case 0: onCommand(Command::GoToSource, 0); return;
    case 1: /*onCommand(Command::GoToSource, 0);*/ return;
    case 2: onCommand(Command::GoBack, 0); return;
    default: break;
  }
}