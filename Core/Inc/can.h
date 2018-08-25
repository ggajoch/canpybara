#ifndef _CAN_H
#define _CAN_H

#include "stm32f1xx_hal.h"

#define CANPYBARA_DEVICE_ADDR_LEN 7

#define CANPYBARA_CONTROLER_ADDR (canpybara_can_get_my_address() | 1<<10)
#define CANPYBARA_CLIENT_REPORT(id) (CANPYBARA_CONTROLER_ADDR | ((id & 0x7) << CANPYBARA_DEVICE_ADDR_LEN))

#define CANPYBARA_REPORT_INRD CANPYBARA_CLIENT_REPORT(0x2)
#define CANPYBARA_REPORT_OUTRD CANPYBARA_CLIENT_REPORT(0x3)

#define CANPYBARA_CONTROLLER_REQUESTID(id) ((id >> CANPYBARA_DEVICE_ADDR_LEN) & 0xF)
#define CANPYBARA_REPORT_OUTSET 0x3

void canpybara_configure_filters(CAN_HandleTypeDef* hcan);
void canpybara_can_init(void);

void canpybara_can_rx(CAN_HandleTypeDef* hcan);
void canpybara_can_tx_ready(CAN_HandleTypeDef* hcan);

uint16_t canpybara_can_get_my_address(void);


#endif
