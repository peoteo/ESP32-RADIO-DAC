#ifndef BT_MODULE_H
#define BT_MODULE_H

#include "BluetoothA2DPSink.h"

class BTModule: public BluetoothA2DPSink {
public:
void initialize(int bck_pin, int ws_pin, int dout_pin);
void start();
void stop();

};

#endif 