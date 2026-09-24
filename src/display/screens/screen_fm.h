#ifndef SCREEN_FM_H
#define SCREEN_FM_H

#include "screen.h"

class ScreenFM: public Screen {
public:
  ScreenFM();
  void render() override;
  void dynamicRender() override;
  void setDLabel(const char* label, uint8_t label_id) override;

  void onUp() override;
  void onDown() override;
  void onSelect() override;
  void onExit() override;

  int getVar(uint8_t id) override;

private:
  enum class state {IDLE, MODE, FREQ, TOOLS, STORAGE};

  state _state = state::IDLE;

  char _station_name[41] = "      no  data      ";
  char _song_name[41] = "      no  data      ";

  /* dynamic menus */
  uint8_t _offset = 0;
  void _computeOffset();

  /* IDLE (shared varibles with TOOLS and STORAGE) */
  uint8_t _selected = 0;
  uint8_t _max_select = 4;

  /* FREQ */
  uint64_t _freq = 100000; // kHz
  uint64_t _old_FM_freq = 100000;
  uint64_t _old_AM_freq = 500;
  void _printFreq();

  /* MODE */
  uint8_t _radio_mode = 0; // 0 = FM  1 = AM
  static const uint8_t _max_mode = 2;
  const char* _mode_label[2] = {"FM ", "AM "};

  /* TOOLS */
  const char* _tools_page_lines[6] = {
    "- save/delete",
    "- storage", 
    "- AF check",
    "- config 1",
    "- config 2",
    "                back",
  };

  /* STORAGE */
  struct station{
    uint64_t freq;
    uint8_t mode;
  };
  station saved_stat[10];
};

#endif