/**
 * @file M5AtomController.h
 * @brief Controller capable of wireless communication by ESP-NOW and wired communication by USB serial
 * @author Ryota Kobayashi
 * @version  V0.1.0
 * @date May 13, 2023
 * @details Numeric data is sent according to the input of the controller. 
 *          During ESP-NOW, the display turns blue upon successful transmission.
 * @note When using ESP-NOW, change the destination Mac address. 
 *       When using USB serial, match the baud rate.
 */

#ifndef M5ATOMCONTROLLER_h
#define M5ATOMCONTROLLER_h

#include <Arduino.h>
#include <M5Atom.h>

/**
* @class M5AtomController
* @brief Controller using M5Atom
*/
class M5AtomController{

public :
    M5AtomController();
    bool begin(const uint8_t *mac_addr, const uint32_t baudrate);
    void send(uint8_t value);
    void update(void);
    void show_display(uint8_t led_mode, CRGB color);

private :
    uint8_t slave_address[6];
    void led_arrow_up(CRGB color);
    void led_arrow_down(CRGB color);
    void led_arrow_left(CRGB color);
    void led_arrow_right(CRGB color);
    void led_square(CRGB color);
    void led_text_a(CRGB color);
    void led_text_b(CRGB color);
    void led_text_c(CRGB color);
};

#if !defined(NO_GLOBAL_INSTANCES)
extern M5AtomController M5AC;
#endif

#endif
