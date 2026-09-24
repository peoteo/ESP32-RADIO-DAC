#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "display/lcd_cursor.h"
#include "display/screens/screen.h"
#include "display/screens/screen_bt.h"
#include "display/screens/screen_fm.h"
#include "display/screens/screen_analog.h"
#include "display/screens/screen_home.h"
#include "display/screens/screen_source.h"

class ScreenManager {
public:
  ScreenManager(LCD_cursor& cursor);
  ~ScreenManager();

  void begin(ScreenId id);
  void setScreen(ScreenId id);
  void scroolUpdate();
  void setDynamicLabel(const char* text, uint8_t label_id = 0);
  void setCommandHandler(std::function<void(Command, int)> handler);

  ScreenId getCurrentScrID();
  int getVar(uint8_t id);

  void onUp();
  void onDown();
  void onSelect();

private:
  LCD_cursor& _cursor;
  Screen* _currentScr = nullptr;

  /* chronology logic */
  void _chronologyHandler();
  ScreenId _chronology[5];
  uint8_t _chronCounter = 0;
  bool _isGoBack = false;    
  ScreenId _currentScrID;

  void _setCurrentScr(ScreenId id);
  Screen* _screens[5] = {nullptr};
};

#endif