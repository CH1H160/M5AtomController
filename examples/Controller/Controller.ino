/**
 * @file Controller.ino
 * @brief Controller capable of wireless communication by ESP-NOW and wired communication by USB serial
 * @author Ryota Kobayashi
 * @date May 16, 2023
 * @details Numeric data is sent according to the input of the controller. 
 *          During ESP-NOW, the display turns blue upon successful transmission.
 * @note When using ESP-NOW, change the destination Mac address. 
 *       When using USB serial, match the baud rate.
 */
#include <M5AtomController.h>

//! ESP-NOW Destination address
const uint8_t SLAVE_ADDRESS[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; 

//! Wired communication baud rate
const uint32_t BAUDRATE = 115200;

//! Period of loop function
const uint16_t DELAY_TIME = 100;

/**
* Arduino standard setup function
* put your setup code here, to run once
*/
void setup(){
  if(!M5AC.begin(SLAVE_ADDRESS, BAUDRATE)){
    return;
  }
}

/**
* Arduino standard loop function
* put your main code here, to run repeatedly
*/
void loop(){
  M5AC.update();
  delay(DELAY_TIME);
}
