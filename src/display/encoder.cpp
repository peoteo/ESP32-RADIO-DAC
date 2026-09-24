#include "encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t pinBtn)
: _pinA(pinA), _pinB(pinB), _pinBtn(pinBtn) {}

/**
 * @brief Set the pins mode and attach the interrupts
 */
void Encoder::begin() {
  pinMode(_pinA, INPUT);
  pinMode(_pinB, INPUT);
  pinMode(_pinBtn, INPUT);

  attachInterruptArg(digitalPinToInterrupt(_pinA), _read_rotation, this, CHANGE);
  attachInterruptArg(digitalPinToInterrupt(_pinB), _read_rotation, this, CHANGE);
}

/**
 * @brief Returns the rotations steps
 */
int8_t Encoder::getDelta() {
  taskENTER_CRITICAL(&_mux);
  int8_t d = _delta;
  _delta = 0;
  taskEXIT_CRITICAL(&_mux);
  return d;
}

/**
 * @brief Returns the state of the rotary encoder button
 * Needs to be called in the loop for pooling
 */
bool Encoder::getButton() { 
  bool state = digitalRead(_pinBtn);

  if (state !=  _last_state) {
    _lastPress = millis();
    _last_state = state;
  }

  if (millis() - _lastPress > 30) {
    if (state != _stableState) {
      _stableState = state;
      if (_stableState == LOW) return true;
    }
  }
  return false;
}

void IRAM_ATTR Encoder::_read_rotation(void* arg) {
  /* lookuptable for encoder's states: 0 if state_not_valide, 1 if state_clockwise, -1 if state_counterclockwise*/
  static const int8_t encoder_states[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0}; 
  static uint8_t AB_register = 3;  // the rest state is 11 (pinB HIGH, pinA HIGH)
  static int8_t encoder_val = 0;

  Encoder* encoder_inst = static_cast<Encoder*>(arg);

  uint8_t new_state = (digitalRead(encoder_inst->_pinB) << 1) | digitalRead(encoder_inst->_pinA);  // two bit: pinB pinA

  AB_register = (AB_register << 2) | (new_state & 0x03);  // update the states register
  encoder_val += encoder_states[(AB_register & 0x0f)]; // if this variable reaches +-4 there is a valid rotation tick

  if (encoder_val > 3) {
    taskENTER_CRITICAL_ISR(&encoder_inst->_mux);
    encoder_inst->_delta = 1;
    taskEXIT_CRITICAL_ISR(&encoder_inst->_mux);
    encoder_val = 0;
  } else if (encoder_val < -3) {
    taskENTER_CRITICAL_ISR(&encoder_inst->_mux);
    encoder_inst->_delta = -1;
    taskEXIT_CRITICAL_ISR(&encoder_inst->_mux);
    encoder_val = 0;
  }
}
