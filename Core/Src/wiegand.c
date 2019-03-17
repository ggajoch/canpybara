#include <stdint.h>

#include "logger.h"
#include "wiegand.h"
#include "can.h"

uint8_t canpybara_wiegand_is_valid(void);
static uint32_t canpybara_wiegand_strip_parity_bits(uint32_t input);

unsigned int wiegand_position;
uint32_t wiegand_buffer;
uint32_t wiegand_timeout;

void canpybara_wiegand_reset(void)
{
	wiegand_position = 0;
	wiegand_buffer = 0;
}

uint32_t uint32_reverse(uint32_t x)
{
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x;
}

void canpybara_wiegand_process_card(void)
{
	static CanTxMsgTypeDef can_tx;
	can_tx.StdId = CANPYBARA_REPORT_SCAN;
	can_tx.ExtId = 0;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;

	can_tx.DLC = 3;
	int i = 0;

	wiegand_buffer = uint32_reverse(wiegand_buffer);

	can_tx.Data[i++] = wiegand_buffer >> 24; 
	can_tx.Data[i++] = wiegand_buffer >> 16;
	can_tx.Data[i++] = wiegand_buffer >> 8;

	canpybara_can_tx(&can_tx);
}

void canpybara_wiegand_process_keypress(void)
{
	LOG("Key ID: %u", wiegand_buffer);

	static CanTxMsgTypeDef can_tx;
	can_tx.StdId = CANPYBARA_REPORT_BTNCLICK;
	can_tx.ExtId = 0;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;

	can_tx.DLC = 1;
	int i = 0;

	wiegand_buffer = uint32_reverse(wiegand_buffer);

	can_tx.Data[i++] = wiegand_buffer >> 28;

	canpybara_can_tx(&can_tx);
}

void canpybara_wiegand_process_scan(void)
{
	LOG("Buff: %"PRIu32", len: %"PRIu32, wiegand_buffer, wiegand_position);

	// Check if makes sense
	if(!canpybara_wiegand_is_valid())
	{
		LOG("Bad wiegand checksum");
		return;
	}

	// Strip parity bits and fix bit order
	wiegand_buffer = canpybara_wiegand_strip_parity_bits(wiegand_buffer);

	switch(wiegand_position)
	{
		case WIEGAND_CARD_LENGTH:
			canpybara_wiegand_process_card();
			break;
		case WIEGAND_KEYPRESS_LENGTH:
			canpybara_wiegand_process_keypress();
			break;
		default:
			LOG("Unknown WIEGAND message");
	}
}

void canpybara_wiegand_pin_pulse_interrupt(int bit)
{
	if(wiegand_position > WIEGAND_MAX_LENGTH)
	{
		return;
	}

	wiegand_buffer |= bit << wiegand_position;
	wiegand_position ++;

	wiegand_timeout = 0;
}

void canpybara_wiegand_systick_interrupt(void)
{
	if(wiegand_timeout > WIEGAND_TIMEOUT)
	{
		if(wiegand_position)
		{
			canpybara_wiegand_process_scan();
		}
		canpybara_wiegand_reset();
	}
	else
	{
		wiegand_timeout ++;
	}
}


uint8_t canpybara_wiegand_parity_calc(uint32_t bitstream, int from, int len)
{
	bitstream >>= from;

	int i = 0;
	uint8_t calc_parity = 0;

	while(i < len)
	{
		calc_parity ^= (uint8_t) bitstream & 1;

		bitstream >>= 1;
		i++;
	}

	return calc_parity;
}


uint8_t canpybara_wiegand_is_valid(void)
{
	// too short...
	// (minimal message length 4 bits (2 bits of data and 2 parity)
	if(wiegand_position < 4)
	{
		return 0;
	}

	uint8_t parity_calc;
	uint8_t bitstream_length = wiegand_position - 2; // length of data bits (len-2 parity bits)
	uint8_t bitstream_length_2 = bitstream_length/2;

	parity_calc = canpybara_wiegand_parity_calc(wiegand_buffer, 0, bitstream_length_2+1);

	if(parity_calc != 0)
	{
		return 0;
	}

	parity_calc = canpybara_wiegand_parity_calc(wiegand_buffer, bitstream_length_2+1, bitstream_length_2+1);

	if(parity_calc != 1)
	{
		return 0;
	}

	return 1;
}

/**
 * This quite magic function will shift 1 bit to right and zero-out
 * last bit in message (removes parity bits from message)
 */
static uint32_t canpybara_wiegand_strip_parity_bits(uint32_t input)
{
	uint32_t mask = (~(1<<(wiegand_position-1)));
	return (wiegand_buffer & mask)>>1;
}