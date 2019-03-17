#ifndef WIEGAND_H
#define WIEGAND_H

#define WIEGAND_MAX_LENGTH 30
#define WIEGAND_BUFFER_BIT_LENGTH sizeof(wiegand_buffer)*8
#define WIEGAND_TIMEOUT 50

#define WIEGAND_CARD_LENGTH  26
#define WIEGAND_KEYPRESS_LENGTH 6

#define WIEGAND_RELAY_PORT RELAY1_GPIO_Port
#define WIEGAND_RELAY_PIN RELAY1_Pin

#define WIEGAND_BUZZER_PORT OUT0_GPIO_Port
#define WIEGAND_BUZZER_PIN OUT0_Pin

#define WIEGAND_ENABLED 

void canpybara_wiegand_reset(void);

void canpybara_wiegand_pin_pulse_interrupt(int bit);
void canpybara_wiegand_systick_interrupt(void);
void canpybara_wiegand_zone_response(uint8_t response);
void canpybara_wiegand_zone_timeout(void);

#endif
