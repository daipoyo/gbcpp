#pragma once
#include "joypad_h.cpp"
#include <string>


void joypad::keydown(enum Key direction){
	switch(direction){
		case Down:
			key_state = key_state & ~0x80;
			break;
		case Up:
			key_state = key_state & ~0x40;
			break;
		case Left:
			key_state = key_state & ~0x20;
			break;
		case Right:
			key_state = key_state & ~0x10;
			break;
		case Start:
			key_state = key_state & ~0x08;
			break;
		case Select:
			key_state = key_state & ~0x04;
			break;
		case B:
			key_state = key_state & ~0x02;
			break;
		case A:
			key_state = key_state & ~0x01;
			break;
	}
	irq = true;
}

void joypad::keyup(enum Key direction){
	switch(direction){
		case Down:
			key_state = key_state | 0x80;
			break;
		case Up:
			key_state = key_state | 0x40;
			break;
		case Left:
			key_state = key_state | 0x20;
			break;
		case Right:
			key_state = key_state | 0x10;
			break;
		case Start:
			key_state = key_state | 0x08;
			break;
		case Select:
			key_state = key_state | 0x04;
			break;
		case B:
			key_state = key_state | 0x02;
			break;
		case A:
			key_state = key_state | 0x01;
			break;
	}
}

void joypad::write(unsigned int addr, unsigned short val){
	switch(addr){
		case 0xFF00:
			joyp = (joyp & 0xCF) | (val & 0x30);
			break;
		default:
			printf("Unexpected address: 0x%04x", addr);
			break;
	}
}

unsigned int joypad::read(unsigned int addr, unsigned short val){
	unsigned int temp = 0;
	switch(addr){
		case 0xFF00:
			//Direction keys selected
			if((joyp & 0x10) == 0){
				temp = (joyp & 0xF0) | ((key_state >> 4) & 0x0F); 
			//Button keys selected
			}else if((joyp & 0x20) == 0){
				temp = (joyp & 0xF0) | (key_state & 0x0F);
			}else{
				temp = joyp;
			}
			break;
		default:
			printf("Unexpexted address: 0x%04x", addr);
			break;

	}
	return temp;
}

void joypad::update(unsigned int tick){}
