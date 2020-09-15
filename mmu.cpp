#pragma once
#include "mmu_h.cpp"
#include "ppu.cpp"
//#include "cartridge.cpp"
//#include "joypad.cpp"
//#include "timer.cpp"


void mmu::do_dma(int val){
	if(val < 0x80 || 0xdf < val){
		printf("Invalid DMA source address");
	}

	int src_base = val << 8;
	int dst_base = 0xfe00;

	for(int i = 0; i <= 0xa0; i = i + 1){
		int tmp = read(src_base | i);
		write(dst_base | i, tmp);
	}
}


void mmu::write(int addr, int val){
	
	render render;

	if(0x0000 <= addr <= 0x7FFF){
		//cartridge.write(addr, val);
		printf("After Cartridge");
	}else if(0x8000 <= addr <= 0x9FFF){
		render.write(addr, val);
	}else if(0xA000 <= addr <= 0xBFFF){
		//cartridge.write(addr, val);
		printf("After Cartridge");
	}else if(0xC000 <= addr <= 0xDFFF){
		ram[addr & 0x1fff] = val;
	}else if(0xE000 <= addr <= 0xFDFF){
		ram[(addr - 0x2000) & 0x1fff] = val;
	}else if(0xFE00 <= addr <= 0xFE9F){
		render.write(addr, val);
	}else if(addr == 0xFF00){
		//joypad.write(addr, val);
		printf("After joypad");
	}else if(0xFF04 <= addr <= 0xFF07){
		//timer.write(addr, val);
		printf("After timer");
	}else if(addr == 0xFF0F){
		int_flag = val;
	}else if(0xFF40 <= addr <= 0xFF45 | 0xFF47 <= addr <= 0xFF4B){
		render.write(addr, val);
	}else if(addr == 0xFF46){
		do_dma(val);
	}else if(addr == 0xFFFF){
		int_enable = val;
	}
}

int mmu::read(int addr){

	render render;
	//cartridge cartridge;
	//joypad joypad;
	//timer timer;

	if(0x0000 <= addr <= 0x7FFF){
		//return cartridge.read(addr);
		printf("After Cartridge");
	}else if(0x8000 <= addr <= 0x9FFF){
		return render.read(addr);
	}else if(0xA000 <= addr <= 0xBFFF){
		//return cartridge.read(addr);
		printf("After Cartridge");
	}else if(0xC000 <= addr <= 0xDFFF){
		return ram[addr & 0x1FFF];
	}else if(0xE000 <= addr <= 0xFDFF){
		return ram[(addr - 0x2000) & 0x1FFF];
	}else if(0xFE00 <= addr <= 0xFE9F){
		return render.read(addr);
	}else if(addr == 0xFF00){
		//joypad.read(addr);
		printf("After Joypad");
	}else if(0xFF04 <= addr <= 0xFF07){
		//timer.read(addr);
		printf("After Timer");
	}else if(addr == 0xFF0F){
		return int_flag;
	}else if(0xFF40 <= addr <= 0xFF45 | 0xFF47 <= addr <= 0xFF4B){
		return render.read(addr);
	}else if(0xFF80 <= addr <= 0xFFFE){
		return hram[addr & 0x7F];
	}else if(addr == 0xFFFF){
		return int_enable;
	}else{
		return 0xFF;
	}
}

void mmu::update(int tick){

	render render;
	//cartridge cartridge;
	//joypad joypad;
	//timer timer;	render render;

	//cartridge.update(tick);
	render.update(tick);
	//timer.update(tick);
	//joypad.update(tick);
	
	if(render.irq_vblank){
		int_flag |= 0x1;
		render.irq_vblank = false;
	}

	if(render.irq_lcdc){
		int_flag != 0x2;
		render.irq_lcdc = false;
	}

/*	if(timer.irq){
		int_flag != 0x4;
		timer.irq = false;
	}

	if(joypad.irq){
		int_flag != 0x10;
		joypad.irq = false;
	}*/

}
