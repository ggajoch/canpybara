#ifndef _CAN_H
#define _CAN_H

#include "stm32f1xx_hal.h"
/*
 
CONTROLLER -> ALLREADER
 0 T T T T 0 0 0 0 0 0

 
CONTROLLER -> READER
 0 T T T T X X X X X X


CONTROLLER <- READER
 1 T T T T X X X X X X
 
 1 0 0 0 0 1 1 1 1 1 1

T T T T
-------
0 0 0 0
       <-  VERSIONINFO
           Wersja, info, bardziej dla ludzi, 8 znakow
 
0 0 0 1
        -> STATUSREQ [RTR]
       <-  STATUS
           00   00 00 00 00 00 00
           ^       TX    RX    ERR
           | 
            - mode (00 - normal, 01 - bootloader)
 
0 0 1 0
        -> INRDREQ [RTR]
       <-  INRD
           00
           ^- stan portu (8 bitow)
0 0 1 1
        -> OUTRDREQ [RTR]
        -> OUTSET
           00 
           ^- nowy stan portu
       <-  OUTRD
           00 
           ^- stan portu
 
0 1 0 0
        -> SCANRESP
           00 - OK
           01 - REJECT

       <-  SCAN
           00 00 00 00
           ^- id breloczka
0 1 0 1
       <- BTNCLICK
          00
          ^- id przycisku

0 1 1 0
        -> REBOOT (watchdog)

*/

// Matches bit 11 (direction) and device ID (X part of addr)
#define CANPYBARA_DEVICE_ADDR_BITMASK 0x87E0

#define CANPYBARA_DEVICE_ADDR_LEN 6

#define CANPYBARA_CONTROLER_ADDR (canpybara_can_get_my_address() | 1<<10)
#define CANPYBARA_CLIENT_REPORT(id) (CANPYBARA_CONTROLER_ADDR | ((id & 0xF) << CANPYBARA_DEVICE_ADDR_LEN))

#define CANPYBARA_REQUEST_VERSIONINFO 0x0
#define CANPYBARA_REQUEST_STATUS 0x01
#define CANPYBARA_REQUEST_INRD 0x02
#define CANPYBARA_REQUEST_OUTSET 0x3
#define CANPYBARA_REQUEST_SCANRESP 0x4
#define CANPYBARA_REQUEST_REBOOT 0x6

#define CANPYBARA_REPORT_VERSIONINFO CANPYBARA_CLIENT_REPORT(CANPYBARA_REQUEST_VERSIONINFO)
#define CANPYBARA_REPORT_STATUS CANPYBARA_CLIENT_REPORT(CANPYBARA_REQUEST_STATUS)
#define CANPYBARA_REPORT_INRD CANPYBARA_CLIENT_REPORT(CANPYBARA_REQUEST_INRD)
#define CANPYBARA_REPORT_OUTRD CANPYBARA_CLIENT_REPORT(CANPYBARA_REQUEST_OUTSET)
#define CANPYBARA_REPORT_SCAN CANPYBARA_CLIENT_REPORT(CANPYBARA_REQUEST_SCANRESP)
#define CANPYBARA_REPORT_BTNCLICK CANPYBARA_CLIENT_REPORT(0x05)

#define CANPYBARA_CONTROLLER_REQUESTID(id) ((id >> CANPYBARA_DEVICE_ADDR_LEN) & 0xF)


void canpybara_configure_filters(CAN_HandleTypeDef* hcan);
void canpybara_can_init(void);

void canpybara_can_rx(CAN_HandleTypeDef* hcan);
void canpybara_can_tx_ready(CAN_HandleTypeDef* hcan);
void canpybara_can_tx(CanTxMsgTypeDef *can_tx);

void canpybara_can_tx_complete(void);
void canpybara_can_error(void);

uint16_t canpybara_can_get_my_address(void);


#endif
