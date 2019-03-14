#include <stdint.h>

#include "logger.h"
#include "wiegand.h"

unsigned int wiegand_position;
uint32_t wiegand_buffer;
uint32_t wiegand_timeout;

void canpybara_wiegand_reset(void)
{
	wiegand_position = 0;
	wiegand_buffer = 0;
}

void canpybara_wiegand_process_card(void)
{

}

void canpybara_wiegand_process_keypress(void)
{

}

void canpybara_wiegand_process_scan(void)
{
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
	wiegand_buffer |= bit << (WIEGAND_BUFFER_BIT_LENGTH -1 - wiegand_position);
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

	if(parity_calc != 1)
	{
		return 0;
	}

	parity_calc = canpybara_wiegand_parity_calc(wiegand_buffer, bitstream_length_2+1, bitstream_length_2+1);

	if(parity_calc != 0)
	{
		return 0;
	}

	return 1;
}
