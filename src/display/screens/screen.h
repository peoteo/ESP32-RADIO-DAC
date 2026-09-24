#ifndef SCREEN_H
#define SCREEN_H
#include <functional>
#include "display/lcd_cursor.h"

enum class ScreenId {BT, FM, Analog, Home, Source};

enum class Command {
  GoToBT,
  GoToFM,
  GoToAnalog,
  GoToHome,
  GoToSource,
  GoBack,
  SetFreq,
  SetMode,
  StopBT,
  StopRadio
};

/**
 * @brief This is the the display screen blueprint 
 */
class Screen {
public:
  virtual ~Screen() = default;

  virtual void render() = 0;
  virtual void dynamicRender() {};

  virtual void setDLabel(const char* label, uint8_t label_id) {};

  virtual void onUp() {};
  virtual void onDown() {};
  virtual void onSelect() {};
  virtual void onExit() {};

  virtual int getVar(uint8_t id) {return -1;}; // to get for example the frequency from the screen

  std::function<void(Command, int)> onCommand = nullptr;

protected:
  bool _isDynamic = false;      // this flag is needed for screens with autoscrolling labels
  bool _isReactive = true;      // this flas is used to tell screenmanager if the screen react to the encoder onup and ondown
  LCD_cursor* _cursor = nullptr;
  friend class ScreenManager;
};

#endif