#pragma once
#include "mmu_h.cpp"
#include "ppu.cpp"
//#include "cartridge.cpp"
//#include "joypad.cpp"
#include "timer.cpp"
render render;
timer timer;


void mmu::do_dma(int val){
	if(val < 0x80 || 0xdf < val){
		printf("Invalid DMA source address");
	}

	int src_base = val << 8;
	int dst_base = 0xfe00;

	for(int i = 0; i <= 0xa0; i = i + 1){
		int tmp = read(src_base | i, 0);
		write(dst_base | i, tmp);
	}
}

void mmu::write(unsigned int addr, unsigned short val){
	

	switch(addr){
		case 0x0000 ... 0x7FFF:
			//cartridge.write(addr, val);
			printf("After Cartridge");
			break;
		case 0x8000 ... 0x9FFF:
			render.write(addr, val);
			break;
		case 0xA000 ... 0xBFFF:
			//cartridge.write(addr, val);
			printf("After Cartridge");
			break;
		case 0xC000 ... 0xDFFF:
			ram[addr & 0x1fff] = val;
			break;
		case 0xE000 ... 0xFDFF:
			ram[(addr - 0x2000) & 0x1fff] = val;
			break;
		case 0xFE00 ... 0xFE9F:
			render.write(addr, val);
			break;
		case 0xFF00:
			//joypad.write(addr, val);
			printf("After joypad");
			break;
		case 0xFF04 ... 0xFF07:
			timer.write(addr, val);
			break;
		case 0xFF0F:
			int_flag = val;
			break;
		case 0xFF40 ... 0xFF45:
		case 0xFF47 ... 0xFF4B:
			render.write(addr, val);
			break;
		case 0xFF46:
			do_dma(val);
			break;
		case 0xFFFF:
			int_enable = val;
			break;
	}
}

unsigned int mmu::read(unsigned int addr, unsigned short val){

	unsigned int temp = 0;

	switch(addr){
		case 0x0000 ... 0x7FFF:
			//return cartridge.read(addr);
			printf("After Cartridge");
			break;
		case 0x8000 ... 0x9FFF:
			temp = render.read(addr, 0);
			break;
		case 0xA000 ... 0xBFFF:
			//return cartridge.read(addr);
			printf("After Cartridge");
			break;
		case 0xC000 ... 0xDFFF:
			temp =  ram[addr & 0x1FFF];
			break;
		case 0xE000 ... 0xFDFF:
			temp =  ram[(addr - 0x2000) & 0x1FFF];
			break;
		case 0xFE00 ... 0xFE9F:
			temp =  render.read(addr, 0);
			break;
		case 0xFF00:
			//joypad.read(addr);
			printf("After Joypad");
			break;
		case 0xFF04 ... 0xFF07:
			timer.read(addr, 0);
			break;
		case 0xFF0F:
			temp =  int_flag;
			break;
		case 0xFF40 ... 0xFF45:
		case 0xFF47 ... 0xFF4B:
			temp =  render.read(addr, 0);
			break;
		case 0xFF80 ... 0xFFFE:
			temp =  hram[addr & 0x7F];
			break;
		case 0xFFFF:
			temp =  int_enable;
			break;
		default:
			temp =  0xFF;
			break;
		}

	return temp;
}


void mmu::update(unsigned int tick){

	//cartridge.update(tick);
	render.update(tick);
	timer.update(tick);
	//joypad.update(tick);
	
	if(render.irq_vblank){
		int_flag |= 0x1;
		render.irq_vblank = false;
	}

	if(render.irq_lcdc){
		int_flag |= 0x2;
		render.irq_lcdc = false;
	}

    if(timer.irq){
		int_flag |= 0x4;
		timer.irq = false;
	}

	/*if(joypad.irq){
		int_flag |= 0x10;
		joypad.irq = false;
	}*/

}

/*int main(){
	
}*/