PORTB &= ~(1<<RS_LCD);
	wyslij_do_LCD(0xC0);
	PORTB |= (1<<RS_LCD);

		_delay_us(40);

