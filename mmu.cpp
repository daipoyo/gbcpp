#include "h.cpp"
#include "ppu.cpp"

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