#include "bt_module.h"

void BTModule::initialize(int bck_pin, int ws_pin, int dout_pin) {
  i2s_pin_config_t i2s_pcm5201a = {
    .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = bck_pin,
    .ws_io_num = ws_pin,
    .data_out_num = dout_pin,
    .data_in_num = I2S_PIN_NO_CHANGE  
  };

  set_pin_config(i2s_pcm5201a);
}