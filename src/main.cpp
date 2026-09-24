#include <Arduino.h>
#include <Wire.h>
#include "BluetoothA2DPSink.h"

#include "config.h"
#include "display/lcd_cursor.h"
#include "display/encoder.h"
#include "display/screen_manager.h"


LiquidCrystal_PCF8574 lcd(LCD_ADR);
LCD_cursor cursor(lcd, 4, 20);
Encoder encoder(ROTARY_PIN_A, ROTARY_PIN_B, ROTARY_BTN);
ScreenManager screen(cursor);

BluetoothA2DPSink bt_sink;

void setup() {
  Serial.begin(115200);
  Serial.println("test 5 ---> general screen classes test");
  delay(2000);
  Serial.print("[START TEST 5]\n");

  Wire.begin(I2C_SDA,I2C_SCL);

  lcd.begin(20,4,Wire);
  lcd.setBacklight(255);

  screen.setCommandHandler([](Command cmd, int value) {
    switch(cmd) {
      case Command::GoToBT:
        screen.setScreen(ScreenId::BT);
        Serial.println("[Home] output change to BT");
        Serial.println("[BT screen] starting the bluetooth");
        return;
      case Command::GoToFM:
        screen.setScreen(ScreenId::FM);
        Serial.println("[Home] Output change to FM");
        return;
      case Command::GoToAnalog:
        screen.setScreen(ScreenId::Analog);
        Serial.println("[Home] Output change to Analog");
        return;
  
      case Command::SetFreq:
        Serial.printf("[FM screen] Frequency set to: %i KHz \n", screen.getVar(0));
        return;
      case Command::SetMode:
        Serial.printf("[FM screen] Radio mode change to: %i (0=FM 1=AM) \n", screen.getVar(1));
        return;

      case Command::StopBT:
        Serial.println("[BT screen] stopping the bluetooth");
        return;
      case Command::StopRadio:
        Serial.println("[FM screen] stopping the radio");
        return;
      default: Serial.println("no implementation found"); return;
    }

  });
  screen.begin(ScreenId::Home);  
  encoder.begin();

  /* ============ BT test ============ */
  i2s_pin_config_t i2s_pcm5201a = {
    .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = I2S_BCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE  
  };

  bt_sink.set_pin_config(i2s_pcm5201a);
  // function that is called after receiving the remote device name
  // bt_sink.set_peer_name_callback([](char* device_name){
  //   if (screen.getCurrentScrID() == ScreenId::BT) {
  //     screen.setDynamicLabel(device_name); // WARNING! this only works because screen is already on BT
  //   }
  // });
  // bt_sink.start("ESP32-RADIODAC");

  Serial.println("ESP32 ready!");
}

void loop() {
  screen.scroolUpdate();
  // static int counter = 0;
  // static int last_counter = 0;

  static int8_t lastDelta = 0; 
  static bool toDo = false;
  int8_t delta = encoder.getDelta();

  // counter += delta;

  // if (counter != last_counter) {
  //   Serial.printf("The encoder moved by: %d  Counter = %d \n",delta,counter);
  //   last_counter = counter;
  // }

  if (delta != lastDelta) {
    lastDelta = delta;
    if (delta != 0) toDo = true;
  }

  if (toDo) {
    switch (lastDelta) {
      case 1: 
        screen.onUp();
        Serial.println("on up");
        break;
      case -1:
        screen.onDown();
        Serial.println("on down");
        break;
      default:
        Serial.println("default");
        break;
    }
    toDo = false;
  }
  
  if (encoder.getButton()) {
    Serial.println("The encoder button was pressed");
    screen.onSelect();
  }

  delay(5);
}

