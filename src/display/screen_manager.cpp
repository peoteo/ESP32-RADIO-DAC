#include "screen_manager.h"

ScreenManager::ScreenManager(LCD_cursor& cursor) : _cursor(cursor) {
  _screens[0] = new ScreenBT();
  _screens[1] = new ScreenFM();
  _screens[2] = new ScreenAnalog();
  _screens[3] = new ScreenHome();
  _screens[4] = new ScreenSource();
}

ScreenManager::~ScreenManager() {
  for (int i = 0; i < 5; i++) delete _screens[i];
}

void ScreenManager::begin(ScreenId id) {
  _setCurrentScr(id);
}

void ScreenManager::setScreen(ScreenId id) {
  /* run the exit script of the screen */
  _currentScr->onExit(); 
  
  _chronologyHandler();
  _setCurrentScr(id);
}

/**
 * @brief Updates the dynamic labels, if there are.
 * 
 * It needs to be call in the loop (the timer is managed internally inside this function)
 */
void ScreenManager::scroolUpdate() {
  static uint32_t lastRender = 0;
  if (_currentScr && _currentScr->_isDynamic && (millis() - lastRender > 1000)) {
    _currentScr->dynamicRender();
    lastRender = millis();
  }
}

/**
 * @brief This function is needed to pass char array to the screen (it cuts array longer than 40)
 * 
 * Screen_BT: device name ->Label_id=0 (default)
 * Screen_FM: station name->label_id=0 (default)
 *            song name   ->label_id=1
 */
void ScreenManager::setDynamicLabel(const char* text, uint8_t label_id) {
  if (_currentScr) {
    _currentScr->setDLabel(text, label_id);
  }
}

/**
 * @brief Sets an handler function that links the user interface to the hardware
 * 
 * The onCommand function is used internally inside the implementation of every screen
 * (GoToSetting, GoToSource and GoBAck are already implemented)
 */
void ScreenManager::setCommandHandler(std::function<void(Command, int)> handler) {
  for (int i = 0; i < 5; i++) {
    if (_screens[i]) {
      _screens[i]->onCommand = [this, handler](Command cmd, int value) {
        
        /* navigation between the screens */
        switch(cmd) {
          // case Command::GoToBT:       setScreen(ScreenId::BT); return;
          // case Command::GoToFM:       setScreen(ScreenId::FM); return;
          // case Command::GoToAnalog:   setScreen(ScreenId::Analog); return; // per gli screen che richiedono un cambio sorgente impostare la callback nel main
          case Command::GoToHome:  setScreen(ScreenId::Home); return;
          case Command::GoToSource:   setScreen(ScreenId::Source); return;
          case Command::GoBack:
            if (_chronCounter > 0) {
              _isGoBack = true;     
              _chronCounter--;          
              setScreen( _chronology[_chronCounter]); 
            }          
            return;
          default: break;
        }

        /* hardware commands */
        if (handler) handler(cmd, value);
      };
    }
  }
}

/**
 * @brief Returns the id of the current screen
 */
ScreenId ScreenManager::getCurrentScrID() {
  return _currentScrID;
}

/**
 * @brief To get private var from the screens 
 * 
 * Screen_FM: frequency-> id=0
 *            mode     -> id=1
 */
int ScreenManager::getVar(uint8_t id) {
  return _currentScr->getVar(id);
}

/**
 * @brief When the encoder rotates in a direction do this
 */
void ScreenManager::onUp() {
  if (_currentScr  && _currentScr->_isReactive) {
    _currentScr->onUp();
    _currentScr->render();
  }
}

/**
 * @brief When the encoder rotates in the opposite direction do this
 */
void ScreenManager::onDown() {
  if (_currentScr && _currentScr->_isReactive) {
    _currentScr->onDown();
    _currentScr->render();
  }
}

/**
 * @brief When the encoder button is pressed do this
 */
void ScreenManager::onSelect() {
  if (_currentScr) {
    _currentScr->onSelect();
  }
}

/**
 * @brief Manage the screen chronology
 */
void ScreenManager::_chronologyHandler() {
  if (!_isGoBack) {
      if (_chronCounter < 5) {
        _chronology[_chronCounter] = _currentScrID;
        _chronCounter++;
      } else {
        for (int i = 0; i < 4; i++) {
          _chronology[i] = _chronology[i+1];
        }
        _chronology[4] = _currentScrID;
      }
  } else {
    _isGoBack = false;
  }
}

/**
 * @brief Set the current screen to the specified id
 */
void ScreenManager::_setCurrentScr(ScreenId id) {
  _currentScrID = id;
  _currentScr = _screens[(int)id];

  if(_currentScr) {
    _currentScr->_cursor = &_cursor;
    _cursor.clear();  // potresti anche riempire di spazi in render e togliere questo
    _currentScr->render();
  }
}
