/**
 * @file M5AtomController.cpp
 * @brief Controller capable of wireless communication by ESP-NOW and wired communication by USB serial
 * @author Ryota Kobayashi
 * @version  V0.1.0
 * @date May 16, 2023
 * @details Numeric data is sent according to the input of the controller. 
 *          During ESP-NOW, the display turns blue upon successful transmission.
 * @note When using ESP-NOW, change the destination Mac address. 
 *       When using USB serial, match the baud rate.
 */

#include "M5AtomController.h"
#include <WiFi.h>
#include <esp_now.h>

//! Pin setting
const uint8_t PIN_UP = 33;
const uint8_t PIN_DOWN = 22;
const uint8_t PIN_LEFT = 19;
const uint8_t PIN_RIGHT = 23;
const uint8_t PIN_A = 21;
const uint8_t PIN_B = 25;
const uint8_t PIN_C = 39;

const uint8_t PIN_COUNT = 7;
const uint8_t PIN_ARRAY[PIN_COUNT] = { PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT, PIN_A, PIN_B, PIN_C }; 

//! Led color
CRGB led_color = CRGB::Red;

/**
* @brief ESP-NOW send callback function
* @param[in] mac_addr Mac address
* @param[in] status Send result
* @details Change led color on successful transmission
*/
static void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  led_color = (status == ESP_NOW_SEND_SUCCESS) ? CRGB::Blue : CRGB::Red;
}

/**
* @brief Constructor
*
* @param[in] mac_addr Mac address
*/
M5AtomController::M5AtomController(const uint8_t *mac_addr){

  memcpy(slave_address, mac_addr, 6);
}

/**
* @brief Start connection
* @param[in] baudrate USB serial baudrate
* @return Success or failure of connection
*/
bool M5AtomController::begin(const uint32_t baudrate){

  M5.begin(true, false, true);

  // Make the display a red square
  led_square(CRGB::Green);

  // Pin setting
  for(int i=0; i<PIN_COUNT; i++){
    pinMode(PIN_ARRAY[i], INPUT_PULLUP);
  }
  Serial.begin(baudrate);

  // WiFi setting
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // ESP-NOW setting
  esp_now_init();
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, slave_address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_register_send_cb(on_data_sent);

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return false;
  }
  return true;
}

/**
* @brief Data transmission
* @param[in] value USB serial baudrate
*/
void M5AtomController::send(uint8_t value){

  uint8_t data[1] = {value};

  // USB serial wired transmission
  Serial.println(value);

  // ESP-NOW wireless transmission
  esp_now_send(slave_address, data, sizeof(data));
}

/**
* @brief Update state
*
*/
void M5AtomController::update(void){

  uint8_t transition_count = 0;

  // Check the input one by one and send the data
  for(uint8_t i=0; i<PIN_COUNT; i++){
    if (!digitalRead(PIN_ARRAY[i])){
      // transmission
      send(i);

      // Turn on the LED and show the display
      show_display(i, led_color);

      transition_count++;
    }
  }

  if(transition_count == 0){
    // Show green square when idle
    led_square(CRGB::Green);
    
    transition_count = 0;
  }
  
}

/**
* @brief Show display according to mode
* @param[in] led_mode Button pressed state
* @param[in] color display led color
*/
void M5AtomController::show_display(uint8_t led_mode, CRGB color){
  
  switch (led_mode){
    case 0:
      led_arrow_up(color);
      break;
    case 1:
      led_arrow_down(color);
      break;
    case 2:
      led_arrow_left(color);
      break;
    case 3:
      led_arrow_right(color);
      break;
    case 4:
      led_text_a(color);
      break;
    case 5:
      led_text_b(color);
      break;
    case 6:
      led_text_c(color);
      break;
  }
  M5.update();
}

/**
* @brief Show up arrow on display
* @param[in] color led color
*/
void M5AtomController::led_arrow_up(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(7, color);
  M5.dis.drawpix(10, color);
  M5.dis.drawpix(12, color);
  M5.dis.drawpix(14, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(17, color);
  M5.dis.drawpix(18, color);
  M5.dis.drawpix(22, color);
  
}

/**
* @brief Show up down on display
* @param[in] color led color
*/
void M5AtomController::led_arrow_down(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(7, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(10, color);
  M5.dis.drawpix(12, color);
  M5.dis.drawpix(14, color);
  M5.dis.drawpix(17, color);
  M5.dis.drawpix(22, color);
  
}

/**
* @brief Show left arrow on display
* @param[in] color led color
*/
void M5AtomController::led_arrow_left(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(10, color);
  M5.dis.drawpix(11, color);
  M5.dis.drawpix(12, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(14, color);
  M5.dis.drawpix(18, color);
  M5.dis.drawpix(22, color);
  
}

/**
* @brief Show right arrow on display
* @param[in] color led color
*/
void M5AtomController::led_arrow_right(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(10, color);
  M5.dis.drawpix(11, color);
  M5.dis.drawpix(12, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(14, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(22, color);
  
}

/**
* @brief Show a square on the display
* @param[in] color led color
*/
void M5AtomController::led_square(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(7, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(11, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(17, color);
  M5.dis.drawpix(18, color);
  
}

/**
* @brief Show the text A on the display
* @param[in] color led color
*/
void M5AtomController::led_text_a(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(1, color);
  M5.dis.drawpix(3, color);
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(7, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(11, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(17, color);
  M5.dis.drawpix(18, color);
  M5.dis.drawpix(22, color);
  
}

/**
* @brief Show the text B on the display
* @param[in] color led color
*/
void M5AtomController::led_text_b(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(3, color);
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(12, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(18, color);
  M5.dis.drawpix(22, color);
  M5.dis.drawpix(23, color);
  
}

/**
* @brief Show the text C on the display
* @param[in] color led color
*/
void M5AtomController::led_text_c(CRGB color){
  
  M5.dis.clear();
  M5.dis.drawpix(1, color);
  M5.dis.drawpix(2, color);
  M5.dis.drawpix(3, color);
  M5.dis.drawpix(6, color);
  M5.dis.drawpix(8, color);
  M5.dis.drawpix(13, color);
  M5.dis.drawpix(16, color);
  M5.dis.drawpix(18, color);
  M5.dis.drawpix(21, color);
  M5.dis.drawpix(22, color);
  M5.dis.drawpix(23, color);
  
}

