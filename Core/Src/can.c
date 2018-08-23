#include "main.h"
#include "logger.h"

#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;

CanTxMsgTypeDef hcan_CAN_Tx;

CanRxMsgTypeDef hcan_CAN_Rx0;
CanRxMsgTypeDef hcan_CAN_Rx1;

void canpybara_configure_filters(void)
{
	LOG("Configuring CAN filters");

	CAN_FilterConfTypeDef filter_config;
	filter_config.FilterIdHigh = 1;
	filter_config.FilterIdLow = 0;
	filter_config.FilterMaskIdHigh = 0;
	filter_config.FilterMaskIdLow = 0;
	filter_config.FilterFIFOAssignment = 0;
	filter_config.FilterNumber = 0;
	filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
	filter_config.FilterScale = CAN_FILTERSCALE_16BIT;
	filter_config.FilterActivation = CAN_FILTERSCALE_16BIT;
	filter_config.BankNumber = 0;


	HAL_CAN_ConfigFilter(&hcan, &filter_config);

	// __HAL_CAN_ENABLE_IT(&hcan, CAN_IT_FMP0 | CAN_IT_ERR);
}

void canpybara_can_init(void)
{
	LOG("Initializing CAN");

	hcan.pTxMsg = &hcan_CAN_Tx;
	hcan.pRxMsg = &hcan_CAN_Rx0;
	hcan.pRx1Msg = &hcan_CAN_Rx1;

	canpybara_configure_filters();

	LOG("Receive_IT: %d", HAL_CAN_Receive_IT(&hcan, 0));
}