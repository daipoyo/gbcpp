#include <string>
#include <iostream>
#include <windows.h>
#include "io_device.cpp"
#include "timer_h.cpp"

void timer::write(unsigned int addr, unsigned short val) {
	switch(addr){
		//DIV
		case 0xff04:
			counter = 0;
			break;
		//TIMA
		case 0xff05:
			tima = val;
			break;
		//TMA
		case 0xff06:
			tma = val;
			break;
		//TAC
		case 0xff07:
			tac = val & 0x7;
			break;
		default:
			printf("Unexpected address: 0x%4x\n", addr);
			break;
	}
}


int main(){
	timer *call_timer;
	printf("%d\n", call_timer->counter);
	call_timer->write(0xff04, 100);
	printf("%d\n", call_timer->counter);

	//std::cout << std::string("Hello World").c_str() << std::endl;
}

