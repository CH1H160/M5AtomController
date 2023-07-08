/**
 * @file Robot.ino
 * @brief A sample of a robot that moves with controller communication.
 * @author Ryota Kobayashi
 * @date July 8, 2023
 */
#include <esp_now.h>
#include <WiFi.h>

bool is_wired = true;

//! controller button
enum button {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  A,
  B,
  C
};

//! ESP-NOW receive callback
void onReceive(const uint8_t* mac_addr, const uint8_t* data, int data_len) {

  is_wired = false;
  action(data[0]);
  delay(100);
  is_wired = true;
}

//! Movement when command is received
void action(uint8_t input_data){
  switch (input_data){
    case button::UP:
      // command response
      break;
    case button::DOWN:
      // command response
      break;
    case button::LEFT:
      // command response
      break;
    case button::RIGHT:
      // command response
      break;
    case button::A:
      // command response
      break;
    case button::B:
      // command response
      break;
    case button::C:
      // command response
      break;
  }
}

/**
* Arduino standard setup function
* put your setup code here, to run once
*/
void setup(){
  Serial1.begin(115200, SERIAL_8N1, 32, 33);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_now_init();
  esp_now_register_recv_cb(onReceive);
}

/**
* Arduino standard loop function
* put your main code here, to run repeatedly
*/
void loop(){
  if(is_wired){
    if ( Serial1.available() ) {
      action(Serial1.read());
    }
  }
}
