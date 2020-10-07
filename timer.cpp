#include <string>
#include <iostream>
#include <windows.h>
#include "timer_h.cpp"

void timer::write(unsigned int addr, unsigned short val){
	switch(addr){
		//DIV
		case 0xff04:
			this->counter = 0;
			break;
		//TIMA
		case 0xff05:
			this->tima = val;
			break;
		//TMA
		case 0xff06:
			this->tma = val;
			break;
		//TAC
		case 0xff07:
			this->tac = val & 0x7;
			break;
		default:
			printf("Unexpected address: 0x%4x\n", addr);
			break;
	}
}

unsigned int timer::read(unsigned int addr, unsigned short val){
	
	unsigned int temp = 0;

	switch(addr){
		//DIV
		case 0xff04:
			temp = this->counter >> 8;
			break;
		//TIMA
		case 0xff05:
			temp = this->tima;
			break;
		//TMA
		case 0xff06:
			temp = this->tma;
			break;
		//TAC
		case 0xff07:
			temp = this->tac;
			break;
		default:
			printf("Unexpexted address: 0x%4x\n", addr);
			break;
	}
	return temp;
}

void timer::update(unsigned int tick){

	unsigned int counter = this->counter;
	unsigned int counter_prev = this->counter;

	//wrappng_add
	if(counter + tick > 0xffff){
		counter = counter + tick - 0xffff;
	}else{
		counter = counter + tick;
	}

	unsigned short divider = 0;

	if((this->tac & 4) > 0){
		switch(this->tac & 3){
			case 0:
				divider = 10;
				break;
			case 1:
				divider = 4;
				break;
			case 2:
				divider = 6;
				break;
			case 3:
			default:
				divider = 8;
				break;
		}
	}

	unsigned int x = counter >> divider;
	unsigned int y = counter_prev >> divider;
	unsigned int mask = (1 << (16 - divider)) - 1;
	unsigned int diff;

	//wrapping_sub
	if((x - y) < 0){
		diff = (x - y + 0xffff) & mask;
	}else{
		diff = (x - y) & mask;
	}

	bool overflow;
	unsigned short res;
	
	if(diff > 0){
		//overflowing_add
		if(this->tima + diff > 0xff){
			overflow = true;
			res = this->tima + diff - 0xff;
		}else{
			overflow = false;
			res = this->tima + diff;
		}

		if(overflow){
			this->tima = this->tma + (diff - 1);
			this->irq = true;
		}else{
			this->tima = res;
		}
	}
	
	this->counter = counter;
	
}




// int main(){
// 	timer *call_timer;
// 	printf("%d\n", call_timer->counter);
// 	call_timer->write(0xff04, 100);
// 	printf("%d\n", call_timer->counter);

// 	//std::cout << std::string("Hello World").c_str() << std::endl;
// }


/*int main(){
	timer call_timer;
	printf("%d\n", call_timer.counter);
	call_timer.write(0xff04, 100);
	printf("%d\n", call_timer.counter);

	//std::cout << std::string("Hello World").c_str() << std::endl;
}*/