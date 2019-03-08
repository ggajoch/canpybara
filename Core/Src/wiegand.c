int wiegand_position;
uint32_t wiegand_buffer;

#define WIEGAND_BUFFER_BIT_LENGTH sizeof(wiegand_buffer)*8

void canpybara_wiegand_reset(void)
{
	wiegand_position = 0;
	wiegand_buffer = 0;
}


void canpybara_wiegand_pin_pulse(int bit)
{
	wiegand_buffer |= bit << (WIEGAND_BUFFER_BIT_LENGTH -1 - wiegand_position);
	wiegand_position ++;
}


