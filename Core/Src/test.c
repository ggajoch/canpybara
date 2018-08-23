#include "main.h"
#include "logger.h"
#include "can.h"

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
extern IWDG_HandleTypeDef hiwdg;
extern CAN_HandleTypeDef hcan;


uint16_t INPUTS[] = {IN0_Pin, IN1_Pin, IN2_Pin, IN3_Pin, IN4_Pin, IN5_Pin};
GPIO_TypeDef * INPUTS_PORT[] = {IN0_GPIO_Port, IN1_GPIO_Port, IN2_GPIO_Port, IN3_GPIO_Port, IN4_GPIO_Port, IN5_GPIO_Port};
const size_t INPUTS_LEN = sizeof(INPUTS) / sizeof(INPUTS[0]);

uint16_t INPUTS_ADDR[] = {ADDR0_Pin, ADDR1_Pin, ADDR2_Pin, ADDR3_Pin, ADDR4_Pin, ADDR5_Pin, ADDR6_Pin};
GPIO_TypeDef * INPUTS_ADDR_PORT[] = {ADDR0_GPIO_Port, ADDR1_GPIO_Port, ADDR2_GPIO_Port, ADDR3_GPIO_Port, ADDR4_GPIO_Port, ADDR5_GPIO_Port, ADDR6_GPIO_Port};
const size_t INPUTS_ADDR_LEN = sizeof(INPUTS_ADDR) / sizeof(INPUTS_ADDR[0]);

uint16_t OUTPUTS[] = { OUT0_Pin, OUT1_Pin, OUT2_Pin, OUT3_Pin, OUT4_Pin, OUT5_Pin, RELAY0_Pin, RELAY1_Pin };
GPIO_TypeDef * OUTPUTS_PORT[] = { OUT0_GPIO_Port, OUT1_GPIO_Port, OUT2_GPIO_Port, OUT3_GPIO_Port, OUT4_GPIO_Port, OUT5_GPIO_Port, RELAY0_GPIO_Port, RELAY1_GPIO_Port};
const size_t OUTPUTS_LEN = sizeof(OUTPUTS) / sizeof(OUTPUTS[0]);


void canpybara_test_zeroout(void)
{
	int i;


	for (i = 0; i < OUTPUTS_LEN; ++i)
	{
		HAL_GPIO_WritePin(OUTPUTS_PORT[i], OUTPUTS[i], GPIO_PIN_RESET);
	}

}

void canpybara_test_sequence(void)
{
	size_t i;

	int q = 0;
	int p = 0;
	
	// LOG("Animation 0->full full->0");
	
	// HAL_IWDG_Refresh(&hiwdg);

	// canpybara_test_zeroout();
	// for(p = 0; p < 3; p++)
	// {
	// 	for (i = 0; i < OUTPUTS_LEN; ++i)
	// 	{
	// 		HAL_GPIO_TogglePin(OUTPUTS_PORT[i], OUTPUTS[i]);

	// 		HAL_Delay(100);
	// 	}

	// 	HAL_IWDG_Refresh(&hiwdg);
	// 	for (i = 0; i < OUTPUTS_LEN; ++i)
	// 	{
	// 		HAL_GPIO_TogglePin(OUTPUTS_PORT[i], OUTPUTS[i]);

	// 		HAL_Delay(100);
	// 	}
	// 	HAL_IWDG_Refresh(&hiwdg);
	// }

	// LOG("IN -> OUT");
	// canpybara_test_zeroout();
	// HAL_GPIO_WritePin(RELAY1_GPIO_Port, RELAY1_Pin, GPIO_PIN_SET);
	// for(p = 0; p < 500; p++) 
	// {
	// 	for (i = 0; i < INPUTS_LEN; ++i)
	// 	{
	// 		HAL_GPIO_WritePin(OUTPUTS_PORT[i], OUTPUTS[i], HAL_GPIO_ReadPin(INPUTS_PORT[i], INPUTS[i]));

	// 		LOG("IN %d -> %d", i, HAL_GPIO_ReadPin(INPUTS_PORT[i], INPUTS[i]));
	// 	}

	// 	HAL_Delay(10);
	// 	HAL_IWDG_Refresh(&hiwdg);
	// }

	// LOG("ADDR");
	// canpybara_test_zeroout();
	// for(p = 0; p < 100; p++) 
	// {
	// 	for (i = 0; i < INPUTS_ADDR_LEN; ++i)
	// 	{
	// 		HAL_GPIO_WritePin(OUTPUTS_PORT[i], OUTPUTS[i], HAL_GPIO_ReadPin(INPUTS_ADDR_PORT[i], INPUTS_ADDR[i]));

	// 		LOG("ADDR %d -> %d", i, HAL_GPIO_ReadPin(INPUTS_ADDR_PORT[i], INPUTS_ADDR[i]));
	// 	}

	// 	HAL_Delay(100);
	// 	HAL_IWDG_Refresh(&hiwdg);
	// }

	// HAL_Delay(100);

	LOG("Testing sending CAN message");

	CanTxMsgTypeDef* CanTx = hcan.pTxMsg;
	CanTx->StdId = 0x01;
	CanTx->ExtId = 0;
	CanTx->IDE = CAN_ID_STD;
	CanTx->RTR = CAN_RTR_DATA;
	CanTx->DLC = 1;
	CanTx->Data[0] = 0x55;


	for(;;)
	{
		HAL_StatusTypeDef result = HAL_CAN_Transmit(&hcan, 500);
		LOG("Transmitting CAN: %s, status: %d", result == HAL_OK ? "OK" : "FAILED", result);

		// for (i = 0; i < 10; ++i)
		// {
			HAL_IWDG_Refresh(&hiwdg);
			HAL_Delay(300);
		// }
	}

	HAL_Delay(100);

	HAL_IWDG_Refresh(&hiwdg);

	LOG("Testing IWDG (should reboot)");

	for(;;);


	// sleep(1000);

}
