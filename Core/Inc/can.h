#ifndef _CAN_H
#define _CAN_H

#include "stm32f1xx_hal.h"

void canpybara_configure_filters(void);
void canpybara_can_init(void);
void canpybara_can_rx(CAN_HandleTypeDef* hcan);

#endif
