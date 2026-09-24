#include "screen_fm.h"

ScreenFM::ScreenFM() {
  _isDynamic = true;
}

void ScreenFM::render() {
  switch (_state) {
    case state::IDLE:
      /**
       *  |  FM   100.00 MHz   |
       *  |       no data      |       
       *  |       no data      |
       *  |[home] <     [tools]|
       */
      _cursor->printAt(_mode_label[_radio_mode], 0, 2);
      _printFreq();  
      _cursor->printAt("[home]       [tools]", 3, 0);
      _cursor->printAt(" ",0,5); _cursor->printAt(" ",0,18);
      
      switch (_selected) {
        case 0: _cursor->printAt("<",3,7); return;  // home
        case 1: _cursor->printAt("<",0,5); return;  // mode
        case 2: _cursor->printAt("<",0,18); return; // frequency
        case 3: _cursor->printAt(">",3,11); return; // tools
        default: break;
      }
      return;

    case state::FREQ: 
    case state::MODE:
      _cursor->printAt(_mode_label[_radio_mode], 0, 2);
      _printFreq();
      return;  
    
    case state::TOOLS:
      /**
       *  |save/delete <       |  |               > back|
       *  |storage             |       
       *  |AF check            |
       *  |other commands     +|
       */  
      for (int i=0; i<4; i++) {
        _cursor->printPadded(_tools_page_lines[i+_offset], i, 0);
        if((i+_offset) == _selected) {
          size_t line_len = strlen(_tools_page_lines[i+_offset]);
          if(line_len > 19) {
            /* back line case */
            _cursor->printAt(">", i, 14);
          } else {
            _cursor->printAt("<", i, line_len+1);}
        }
      }

      if(_offset != 2) {
        _cursor->printAt("+",3,19);
      }
      return;
      
    case state::STORAGE:
      /**
       *  |102.50 MHz [FM] <   |  |               > back|
       *  |243 KHz    [AM]     |       
       *  |11.205 MHz [AM]     |
       *  |3456 KHz   [AM]    +|
       */
      return;
    default: break;
  }
}

void ScreenFM::dynamicRender() {
  switch (_state) {
    case state::IDLE:
    case state::FREQ: 
    case state::MODE:
      _cursor->printAutoScroll(_station_name, 1, 0);
      _cursor->printAutoScroll(_song_name, 2, 0);
    default: break;
  }
}

void ScreenFM::setDLabel(const char* label, uint8_t label_id) {
  switch (label_id) {
    case 0: 
      if (!label) {
        snprintf(_station_name, 41, "no data");
        return;
      }
      snprintf(_station_name, 41, "%s", label);
    return;

    case 1:
      if (!label) {
        snprintf(_song_name, 41, "no data");
        return;
      }
      snprintf(_song_name, 41, "%s", label);
    return;

    default: return;
  }
}

void ScreenFM::onUp() {
  if (!onCommand) return;

  switch (_state) {
    case state::IDLE:
      _selected = (_selected - 1 + _max_select)%_max_select; 
      return;

    case state::MODE:
      _radio_mode = (_radio_mode - 1 + _max_mode)%_max_mode;
      switch(_radio_mode) {
        case 0: _freq = _old_FM_freq; break;
        case 1: _freq = _old_AM_freq; break;
      }
      return;

    case state::FREQ:
      _freq = _freq + 500;
      onCommand(Command::SetFreq, _freq); 
      return;
    
    case state::TOOLS:
      _selected = (_selected - 1 + _max_select)%_max_select;
      _computeOffset();
      return;

    case state::STORAGE:
      break;

    default: return;
  }
}

void ScreenFM::onDown() {
  if (!onCommand) return;

  switch (_state) {
    case state::IDLE:
      _selected = (_selected + 1)%_max_select; 
      return;

    case state::MODE:
      _radio_mode = (_radio_mode + 1)%_max_mode;
      switch(_radio_mode) {
        case 0: _freq = _old_FM_freq; break;
        case 1: _freq = _old_AM_freq; break;
      }
      return;

    case state::FREQ:
      if (_freq > 0) _freq = _freq - 500; 
      onCommand(Command::SetFreq, _freq); 
      return;

    case state::TOOLS:
      _selected = (_selected + 1)%_max_select;
      _computeOffset();
      return;

    case state::STORAGE:
      break;

    default: return;
  }
}

void ScreenFM::onSelect() {
  if (!onCommand) return;
  // NOTE: the break is used to jump to the render function at the end
  switch (_state) {
    case state::IDLE:
      switch(_selected) {
        case 0: onCommand(Command::GoToHome, 0); return;
        case 1: _state = state::MODE; return;
        case 2: _state = state::FREQ; return;
        case 3: _state = state::TOOLS; _selected = 0; _max_select = 6; _offset = 0; _cursor->clear(); break; 
        default: break;
      }
      break;
    
    case state::MODE:
      onCommand(Command::SetMode, _radio_mode); 
      _state = state::IDLE; break;
      
    case state::FREQ:
      switch(_radio_mode) {
        case 0: _old_FM_freq = _freq; break;
        case 1: _old_AM_freq = _freq; break;
        default: break;
      }      
      _state = state::IDLE; return;
    
    case state::TOOLS:
      switch(_selected) {
        case 5: _state = state::IDLE; _selected = 0; _max_select = 4; _cursor->clear(); break;
        default: break;
      }
      break;
    default: break;
  }
  render();
}

void ScreenFM::onExit() {
  if (!onCommand) return;
  onCommand(Command::StopRadio, 0);
}

int ScreenFM::getVar(uint8_t id) {
  switch (id) {
    case 0: return _freq;
    case 1: return _radio_mode;
    default: return -1;
  }
}

void ScreenFM::_printFreq() {
  char buff[40]; 
  if (_radio_mode == 0) {
    uint64_t whole_part = _freq / 1000;
    Serial.printf("[D] whole_part = %i \n", whole_part);
    uint64_t dec_part = (_freq % 1000) / 10;
    Serial.printf("[D] dec_part = %i \n", dec_part);

    snprintf(buff, 20, "%3i.%02i MHz ", (int)whole_part, (int)dec_part);
    Serial.println(buff);
  } else if (_radio_mode == 1) {
    snprintf(buff, 20, "%6i KHz ", (int)_freq);
  }
  _cursor->printAt(buff,0,7);
}

void ScreenFM::_computeOffset() {
  if(_selected > (_offset + 3)) {
    _offset = _selected - 3;
  }
  if(_selected < _offset) {
    _offset = _selected;
  }
  Serial.printf("[FM screen]    offset = %i     selected = %i \n", _offset, _selected);
}
