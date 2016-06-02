#include <stdint.h>
#include <stdio.h>
#include "gpio.h"

int is_GPIO_addr(int addr) {

	switch(addr)
	{
		case 0x20200000:
		case 0x20200004:
		case 0x20200008:
		case 0x20200028:
		case 0x2020001c: return 1; 
		default:         return 0;
	}
}

void print_GPIO_addr(int addr)
{
	int grp_of_pins = 0;
	int pin_on      = 0;
	int pin_off     = 0;

	switch(addr)
	{
		case 0x20200000: grp_of_pins =  0; goto accessed;
		case 0x20200004: grp_of_pins = 10; goto accessed;
		case 0x20200008: grp_of_pins = 20; goto accessed;
		case 0x20200028: pin_off     =  1; break;
		case 0x2020001c: pin_on      =  1; break;
		default:         return;
	}

	if (pin_on)  printf("PIN ON\n");
	if (pin_off) printf("PIN OFF\n");

	return;

accessed:
    printf("One GPIO pin from %d to %d has been accessed\n",
           grp_of_pins, grp_of_pins + 9);
}
