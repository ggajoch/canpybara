#include "main.h"
#include "logger.h"

#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;

CanTxMsgTypeDef hcan_CAN_Tx;

CanRxMsgTypeDef hcan_CAN_Rx0;
CanRxMsgTypeDef hcan_CAN_Rx1;

void canpybara_configure_filters(CAN_HandleTypeDef* hcan)
{
	CAN_FilterConfTypeDef filter_config;
	filter_config.FilterNumber = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
	filter_config.FilterIdHigh = 0x3FF;
	filter_config.FilterIdLow = 0;
	filter_config.FilterMaskIdHigh = 0;
	filter_config.FilterMaskIdLow = 0;
	filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter_config.FilterActivation = ENABLE;
	filter_config.BankNumber = 0;

	HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(hcan, &filter_config);

	if(result != HAL_OK)
	{
		LOG("Configuring CAN filters status: %d", result);
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

	hcan.pTxMsg = &hcan_CAN_Tx;
	hcan.pRxMsg = &hcan_CAN_Rx0;
	hcan.pRx1Msg = &hcan_CAN_Rx1;

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

	hcan->pTxMsg->StdId = hcan->pRxMsg->StdId | 1<<10;
	hcan->pTxMsg->ExtId = 0;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = hcan->pRxMsg->DLC;
	int i;
	for (i = 0; i < hcan->pRxMsg->DLC; ++i)
	{
		hcan->pTxMsg->Data[i] = hcan->pRxMsg->Data[i];
	}
	// hcan->pTxMsg->Data[0] = 0x55;

	HAL_StatusTypeDef result = HAL_CAN_Transmit_IT(hcan);
	if(result != HAL_OK)
	{
		LOG("Reply to CAN msg result: %d", result);
		_Error_Handler(__FILE__, __LINE__);
	}
	
	canpybara_reload_canrx(hcan);
}
