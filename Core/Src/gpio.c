#include "stm32f1xx_hal.h"

#include "can.h"

extern CAN_HandleTypeDef hcan;

void canpybara_gpio_report_input(void)
{

	uint8_t port_status = 0;

	uint16_t INPUTS[] = {IN0_Pin, IN1_Pin, IN2_Pin, IN3_Pin, IN4_Pin, IN5_Pin};
	GPIO_TypeDef * INPUTS_PORT[] = {IN0_GPIO_Port, IN1_GPIO_Port, IN2_GPIO_Port, IN3_GPIO_Port, IN4_GPIO_Port, IN5_GPIO_Port};
	const size_t INPUTS_LEN = sizeof(INPUTS) / sizeof(INPUTS[0]);

	int i;
	for (i = 0; i < INPUTS_LEN; ++i)
	{
		if(HAL_GPIO_ReadPin(INPUTS_PORT[i], INPUTS[i]) == GPIO_PIN_SET)
		{
			port_status |= 1 << i;
		}
	}

	CanTxMsgTypeDef CanTx;
	CanTx.StdId = CANPYBARA_REPORT_INRD;
	CanTx.ExtId = 0;
	CanTx.IDE = CAN_ID_STD;
	CanTx.RTR = CAN_RTR_DATA;
	CanTx.DLC = 1;
	CanTx.Data[0] = port_status;

	hcan.pTxMsg = &CanTx;
	HAL_CAN_Transmit(&hcan, 50);
}

static uint16_t OUTPUTS[] = { OUT0_Pin, OUT1_Pin, OUT2_Pin, OUT3_Pin, OUT4_Pin, OUT5_Pin, RELAY0_Pin, RELAY1_Pin };
static GPIO_TypeDef * OUTPUTS_PORT[] = { OUT0_GPIO_Port, OUT1_GPIO_Port, OUT2_GPIO_Port, OUT3_GPIO_Port, OUT4_GPIO_Port, OUT5_GPIO_Port, RELAY0_GPIO_Port, RELAY1_GPIO_Port};
static const size_t OUTPUTS_LEN = sizeof(OUTPUTS) / sizeof(OUTPUTS[0]);
void canpybara_gpio_handle_outset(uint8_t new_port_state)
{
	int i;
	for (i = 0; i < OUTPUTS_LEN; ++i)
	{
		HAL_GPIO_WritePin(OUTPUTS_PORT[i], OUTPUTS[i], (new_port_state & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

void canpybara_gpio_handle_outrdreq(void)
{
	uint8_t port_status = 0;

	int i;
	for (i = 0; i < OUTPUTS_LEN; ++i)
	{
		if(HAL_GPIO_ReadPin(OUTPUTS_PORT[i], OUTPUTS[i]) == GPIO_PIN_SET)
		{
			port_status |= 1 << i;
		}
	}

	CanTxMsgTypeDef CanTx;
	CanTx.StdId = CANPYBARA_REPORT_OUTRD;
	CanTx.ExtId = 0;
	CanTx.IDE = CAN_ID_STD;
	CanTx.RTR = CAN_RTR_DATA;
	CanTx.DLC = 1;
	CanTx.Data[0] = port_status;

	hcan.pTxMsg = &CanTx;
	HAL_CAN_Transmit(&hcan, 50);
}