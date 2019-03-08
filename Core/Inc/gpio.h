#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void canpybara_gpio_report(void);
void canpybara_gpio_handle_outset(uint8_t new_port_state);
void canpybara_gpio_handle_outrdreq(void);

void canpybara_gpio_interrupt(uint16_t GPIO_Pin);

#endif
