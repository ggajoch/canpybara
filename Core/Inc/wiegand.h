#ifndef WIEGAND_H
#define WIEGAND_H

#define WIEGAND_MAX_LENGTH 30
#define WIEGAND_BUFFER_BIT_LENGTH sizeof(wiegand_buffer)*8
#define WIEGAND_TIMEOUT 100

#define WIEGAND_CARD_LENGTH  26
#define WIEGAND_KEYPRESS_LENGTH 6

#define WIEGAND_ENABLED 

void canpybara_wiegand_reset(void);

void canpybara_wiegand_pin_pulse_interrupt(int bit);
void canpybara_wiegand_systick_interrupt(void);

#endif
