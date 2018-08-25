#include "main.h"
#include "logger.h"
#include "can.h"

#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;

CanTxMsgTypeDef hcan_CAN_Tx;

CanRxMsgTypeDef hcan_CAN_Rx0;
CanRxMsgTypeDef hcan_CAN_Rx1;



// struct canpybara_can_pending_message
// {
// 	uint8_t enabled : 1;
// 	uint8_t sending : 1;
	
// 	CanTxMsgTypeDef message;
// };

// #define MESSAGE_QUEUE_LENGTH 8
// struct canpybara_can_pending_message message_queue[MESSAGE_QUEUE_LENGTH];

// void canpybara_mesage_queue_clear(void)
// {
// 	int i;

// 	for (i = 0; i < MESSAGE_QUEUE_LENGTH; ++i)
// 	{
// 		message_queue[i].enabled = 0;
// 		message_queue[i].sending = 0;
// 	}
// }

// void canpybara_send_message(CanTxMsgTypeDef *message)
// {
// 	int i;

// 	for (i = 0; i < MESSAGE_QUEUE_LENGTH; ++i)
// 	{
// 		struct canpybara_can_pending_message *item = message_queue + i;

// 		if(item->enabled == 0 && item->sending == 0)
// 		{
// 			item->message = *message;
// 			item->enabled = 1;

// 			return;
// 		}
// 	}
// }

uint16_t canpybara_can_get_my_address(void)
{
	uint16_t result = 0;

	uint16_t ADDR_PIN[] = {ADDR0_Pin, ADDR1_Pin, ADDR2_Pin, ADDR3_Pin, ADDR4_Pin, ADDR5_Pin, ADDR6_Pin};
	GPIO_TypeDef * ADDR_PORT[] = {ADDR0_GPIO_Port, ADDR1_GPIO_Port, ADDR2_GPIO_Port, ADDR3_GPIO_Port, ADDR4_GPIO_Port, ADDR5_GPIO_Port, ADDR6_GPIO_Port};
	const size_t INPUTS_ADDR_LEN = sizeof(ADDR_PIN) / sizeof(ADDR_PIN[0]);

	int i;
	for (i = 0; i < INPUTS_ADDR_LEN; ++i)
	{
		if(HAL_GPIO_ReadPin(ADDR_PORT[i], ADDR_PIN[i]) == GPIO_PIN_RESET)
		{
			result |= 1 << i;
		}
	}

	if(result == 0)
	{
		LOG("Invalid module address: %d", result);
		_Error_Handler(__FILE__, __LINE__);
	}

	return result;
}

// void canpybara_can_tx_ready(CAN_HandleTypeDef* hcan)
// {
// 	int i;

// 	for (i = 0; i < MESSAGE_QUEUE_LENGTH; ++i)
// 	{
// 		if(message_queue[i].sending)
// 		{
// 			message_queue[i].sending = 0;
// 		}

// 		if(message_queue[i].enabled)
// 		{
// 			hcan->pTxMsg = &message_queue[i].message;

// 			message_queue[i].sending = 1;
// 			message_queue[i].enabled = 0;

// 			HAL_StatusTypeDef result = HAL_CAN_Transmit_IT(hcan);
// 			if(result != HAL_OK)
// 			{
// 				LOG("HAL_CAN_Transmit_IT called status: %d", result);
// 				_Error_Handler(__FILE__, __LINE__);
// 			}
// 		}
// 	}
// }

void canpybara_configure_filters(CAN_HandleTypeDef* hcan)
{
	uint16_t my_address = canpybara_can_get_my_address();

	CAN_FilterConfTypeDef filter_config;
	filter_config.FilterNumber = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;

	// Filter 1 : My addr
	filter_config.FilterIdHigh = my_address << 5;
	filter_config.FilterMaskIdHigh = 0x8FE0;

	// Filter 2 : Addr 0
	filter_config.FilterIdLow = 0x000 << 5;
	filter_config.FilterMaskIdLow = 0xFFFF;

	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter_config.FilterActivation = ENABLE;
	filter_config.BankNumber = 0;

	HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(hcan, &filter_config);

	LOG("Configuring CAN filters my_addr: %d, status: %d", my_address, result);
	if(result != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void canpybara_reload_canrx(CAN_HandleTypeDef* hcan)
{
	HAL_StatusTypeDef result = HAL_CAN_Receive_IT(hcan, CAN_FIFO0);

	if(result != HAL_OK)
	{
		LOG("CAN_Receive_IT called status: %d", result);
		_Error_Handler(__FILE__, __LINE__);
	}
}

void canpybara_can_init(void)
{
	LOG("Initializing CAN");

	// hcan.pTxMsg = &hcan_CAN_Tx;
	hcan.pRxMsg = &hcan_CAN_Rx0;
	hcan.pRx1Msg = &hcan_CAN_Rx1;

	// canpybara_mesage_queue_clear();

	canpybara_configure_filters(&hcan);
	canpybara_reload_canrx(&hcan);
}

void canpybara_can_rx(CAN_HandleTypeDef* hcan)
{
	// hcan->pRxMsg->StdId |= 1<<5;

	// if(hcan->pRxMsg->StdId )
	// {
	// 	LOG("Rcv byte: %d", (int)hcan->pRxMsg->Data[0]);
	// }

	// hcan->pTxMsg->StdId = hcan->pRxMsg->StdId | 1<<10;
	// hcan->pTxMsg->ExtId = 0;
	// hcan->pTxMsg->IDE = CAN_ID_STD;
	// hcan->pTxMsg->RTR = CAN_RTR_DATA;
	// hcan->pTxMsg->DLC = hcan->pRxMsg->DLC;
	// int i;
	// for (i = 0; i < hcan->pRxMsg->DLC; ++i)
	// {
	// 	hcan->pTxMsg->Data[i] = hcan->pRxMsg->Data[i];
	// }

	// HAL_StatusTypeDef result = HAL_CAN_Transmit_IT(hcan);
	// if(result != HAL_OK)
	// {
	// 	LOG("Reply to CAN msg result: %d", result);
	// 	_Error_Handler(__FILE__, __LINE__);
	// }

	int request_id = CANPYBARA_CONTROLLER_REQUESTID(hcan->pRxMsg->StdId);

	// LOG("Request id: %d for %"PRIu32, request_id, hcan->pRxMsg->StdId);

	switch(request_id)
	{
		case CANPYBARA_REPORT_OUTSET:
			if(hcan->pRxMsg->DLC == 1)
			{
				canpybara_gpio_handle_outset(hcan->pRxMsg->Data[0]);
			}
			else if (hcan->pRxMsg->RTR == CAN_RTR_REMOTE)
			{
				canpybara_gpio_handle_outrdreq();
			}
			break;
			default:

			LOG("Unknown request ID: %d", request_id);
	}
	
	canpybara_reload_canrx(hcan);
}
