#include <string>
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
			printf("Unexpected address: 0x%4x", addr);
			break;
	}
}


int main(){

}

