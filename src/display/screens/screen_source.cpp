#include "screen_source.h"

void ScreenSource::render() {
    _cursor->printAt("- BT mode           ", 0, 0);
    _cursor->printAt("- FM radio mode     ", 1, 0);
    _cursor->printAt("- Analog input      ", 2, 0);
    _cursor->printAt("                back", 3, 0);

  switch (_selected) {
    case 0: _cursor->printAt("<",0,10); return;
    case 1: _cursor->printAt("<",1,16); return;
    case 2: _cursor->printAt("<",2,15); return;
    case 3: _cursor->printAt(">",3,14); return;
    default: break;
  }
}

void ScreenSource::onUp() {
  _selected = (_selected - 1 + _max_select)%_max_select;
}

void ScreenSource::onDown() {
  _selected = (_selected + 1)%_max_select;
}

void ScreenSource::onSelect() {
  if (!onCommand) return;

  switch (_selected) {
    case 0: onCommand(Command::GoToBT, 0); return;
    case 1: onCommand(Command::GoToFM, 0); return;
    case 2: onCommand(Command::GoToAnalog, 0); return;
    case 3: onCommand(Command::GoBack, 0); return;
    default: break;
  }
}