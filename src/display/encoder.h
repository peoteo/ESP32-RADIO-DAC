#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public: 
  Encoder(uint8_t pinA, uint8_t pinB, uint8_t pinBtn);
  void begin();

  int8_t getDelta();
  bool getButton();

private:
  uint8_t _pinA;
  uint8_t _pinB;
  uint8_t _pinBtn;

  portMUX_TYPE _mux = portMUX_INITIALIZER_UNLOCKED; /* Spinlock for critical section */
  volatile int8_t _delta = 0;
  static void IRAM_ATTR _read_rotation(void* arg);

  bool _last_state = HIGH;
  bool _stableState = HIGH;
  uint32_t _lastPress = 0;
};

#endif