#include "stm32f1xx_hal.h"

#include "can.h"
#include "logger.h"

#include <inttypes.h>

extern CAN_HandleTypeDef hcan;

void canpybara_gpio_report(void)
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

	static CanTxMsgTypeDef can_tx;
	can_tx.StdId = CANPYBARA_REPORT_INRD;
	can_tx.ExtId = 0;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;
	can_tx.DLC = 1;
	can_tx.Data[0] = port_status;

	canpybara_can_tx(&can_tx);
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
		if(OUTPUTS_PORT[i]->ODR & OUTPUTS[i])
		{
			port_status |= 1 << i;
		}
	}

	static CanTxMsgTypeDef can_tx;
	can_tx.StdId = CANPYBARA_REPORT_OUTRD;
	can_tx.ExtId = 0;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;
	can_tx.DLC = 1;
	can_tx.Data[0] = port_status;

	canpybara_can_tx(&can_tx);
}

void canpybara_gpio_interrupt(uint16_t GPIO_Pin)
{
	LOG("GPIO Interrupt: %"PRIu16, GPIO_Pin);
}