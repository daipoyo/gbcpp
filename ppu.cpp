#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include "ppu_h.cpp"
//using namespace std;


void read_rom::fetch(std::string filename){
	std::ifstream ifs(filename);

	if (!ifs){
        std::cout << "Error! File can not be opened" << std::endl;
    }else{
    	std::string buf;
    	std::string buf_line;
		// std::vector<int> a;
    	int n = 0;
		while (std::getline(ifs, buf)){
			// std::cout << std::hex << std::showbase << buf << std::endl;
			std::stringstream ss{buf};
    		while (std::getline(ss, buf_line, ' ')){
	    		int buf_num = std::stoi(buf_line, nullptr, 16);
	    		int up = buf_num >> 8 & 0xff;
				a.push_back(up);
	    		int down = buf_num & 0xff;
				a.push_back(down);
    		}
		}
		// std::cout << std::hex << std::showbase << a[16] << std::endl;
    }
}

std::tuple<int, int> render::fetch_tile(int tile_no, int offset_y, bool tile_data_sel){
	int tile_data_addr;

	if (tile_data_sel) {
		// Use tile set #1 (0x0000-0x07ff) and #2 (0x0800-0x0fff)
		tile_data_addr = tile_no << 4;
	}else{
		// Use tile set #2 (0x0800-0x0fff) and #3 (0x1000-0x17ff)
		tile_data_addr = 0x1000 + tile_no << 4;
		// printf("%x",tile_data_addr);
	}
	int row_addr = tile_data_addr + (offset_y << 1);
	int tile0 = a[row_addr];
	int tile1 = a[row_addr + 1];

	return std::forward_as_tuple(tile0,tile1);
}


std::tuple<int, int> render::fetch_bg_window_tile(int tile_x, int tile_y, int offset_y, int tile_map_base){
	int tile_map_addr = tile_map_base | ((tile_x & 0x1f) + (tile_y << 5));
	int tile_no = a[tile_map_addr];
	int lcdc = a[0xff40];

	//render render;
	//render.fetch_tile(tile_no, offset_y, lcdc & 0x10 > 0);
	fetch_tile(tile_no, offset_y, lcdc & 0x10 > 0);
}
std::tuple<int, int> render::fetch_bg_tile(int tile_x, int tile_y, int offset_y){
	int lcdc = a[0xff40];
	int tile_map_base;

	if(lcdc & 0x40 > 0){
		tile_map_base = 0x1c00;
	}else{
		tile_map_base = 0x1800;
	}

	//render render;
	fetch_bg_window_tile(tile_x, tile_y, offset_y, tile_map_base);
}

std::tuple<int, int> render::fetch_window_tile(int tile_x, int tile_y, int offset_y){
	int lcdc = a[0xff40];
	int tile_map_base;

	if(lcdc & 0x8 > 0){
		tile_map_base = 0x1c00;
	}else{
		tile_map_base = 0x1800;
	}

	//render render;
	fetch_bg_window_tile(tile_x, tile_y, offset_y, tile_map_base);
}

int render::map_color(int color_no, int palette){
	switch((palette >> (color_no << 1)) & 0x3){
		case 0:
			return 0xff;
			break;
		case 1:
			return 0xaa;
			break;
		case 2:
			return 0x55;
			break;
		default:
			return 0x00;
			break;
	}
}

int render::get_color_no(int tile0, int tile1, int bitpos){
	int lo_bit = tile0 >> bitpos & 1;
	int hi_bit = tile1 >> bitpos & 1;

	return hi_bit << 1 | lo_bit;
}


void render::render_bg(){
	//render render;
	//read_rom rom;

	int tile_x = scx >> 3;
	int tile_y = scy + ly >> 3;

	int offset_x = scx & 0x7;
	int offset_y = scy + ly & 0x7;

	int tile0,tile1;
	//std::tie(tile0, tile1) = render.fetch_bg_tile(tile_x, tile_y, offset_y);
	std::tie(tile0, tile1) = fetch_bg_tile(tile_x, tile_y, offset_y);

	bool window = false;

	for(int i = 0; i < screen_width; i++){
		//Check window ON/OFF
		if(lcdc & 0x20 == 1){
			if(wy <= ly && wx == i + 7){
				tile_x = 0;
				tile_y = (ly - wy) >> 3;
				offset_x = 0;
				offset_y = (ly - wy) & 0x7;
				//std::tie(tile0, tile1)  = render.fetch_window_tile(tile_x, tile_y, offset_y);
				std::tie(tile0, tile1)  = fetch_window_tile(tile_x, tile_y, offset_y);
				window = true;
			}
		}

		//int color_no = render.get_color_no(tile0, tile1, 7 - offset_x);
		int color_no = get_color_no(tile0, tile1, 7 - offset_x);
		//int color = render.map_color(color_no, bg_palette);
		int color = map_color(color_no, bg_palette);

		if(color_no == 0){
			bg_prio[i] = "Color0";        //colorless??
		}else{
			bg_prio[i] = "Color123";
		}

		scanline[i] = color;

		offset_x ++;

		//Next tile
		if(offset_x >= 8){
			offset_x = 0;
			tile_x ++;

			if(window){
				//std::tie(tile0, tile1)  = render.fetch_window_tile(tile_x, tile_y, offset_y);
				std::tie(tile0, tile1)  = fetch_window_tile(tile_x, tile_y, offset_y);
			}else{
				//std::tie(tile0, tile1)  = render.fetch_bg_tile(tile_x, tile_y, offset_y);
				std::tie(tile0, tile1)  = fetch_bg_tile(tile_x, tile_y, offset_y);
			}
		}
	}
}


void render::render_splite(){
	int n_splites = 0;
	int height, palette, tile_no, offset_y, offset_x, bitpos;
	int tile0,tile1;
	render render;

	if(lcdc & 0x4 > 0){
		height = 16;
	}else{
		height = 8;
	}

	for(int i = 0; i < 40; i++){
		int entry_addr = i << 2;
		int splite_y = OAM[entry_addr];
		int splite_x = OAM[entry_addr + 1];
		int flags = OAM[entry_addr + 3];

		bool obj_prio = flags & 0x80 > 0;
		bool flip_y = flags & 0x40 > 0;
		bool flip_x = flags & 0x20 > 0;
		if(flags & 0x10 > 0){
			palette = ob_palette1;
		}else{
			palette = ob_palette0;
		}

		//splite visible on this scanline
		if(splite_y <= ly + 16 - height || splite_y > ly + 16){
			continue;
		}

		//max splite count = 10
		n_splites ++;
		if(n_splites > 10){
			break;
		}

		if(splite_x == 0 || splite_x > screen_width + 8){
			continue;
		}

		if(lcdc & 0x4 > 0){
			//8x16
			if((ly + 8 < splite_y) ^ flip_y){
				tile_no = OAM[entry_addr + 2] & 0xfe;
			}else{
				tile_no = OAM[entry_addr + 2] | 0x01;
			}
		//8x8
		}else{
			tile_no = OAM[entry_addr + 2];
		}

		//Y-offset within the tile
		if(flip_y){
			offset_y = 7 - ((ly + 16 - splite_y) & 0x7);
		}else{
			offset_y = (ly + 16 - splite_y) & 0x7;
		}

		std::tie(tile0, tile1) = render.fetch_tile(tile_no, offset_y, true);

		for(int offset_x = 0; offset_x < 8; offset_x++){
			if(offset_x + splite_x < 8){
				continue;
			}

			int x = offset_x + splite_x - 8;

			if(x >= screen_width){
				break;
			}

			if(flip_x){bitpos = offset_x;}else{bitpos = 7 - offset_x;}
			int color_no = render.get_color_no(tile0, tile1, bitpos);
			if(color_no == 0){
				continue;
			}
			if(bg_prio[x] == "Color123" && obj_prio){
				continue;
			}
			int color = render.map_color(color_no, palette);

			scanline[x] = color;
		}

		
	}
}

void render::render_scanline(){
	if(lcdc & 0x1 > 0){
		render_bg();
	}
	if(lcdc & 0x2 > 0){
		render_splite();
	}

	for(int x = 0; x < screen_width; x++){
		int ix = x + ly * screen_width;
		frame_buffer[ix] = scanline[x];
	}
}

std::array<int, screen_width * screen_height> render::exe_frame_buffer(){
	return frame_buffer;
}

void render::check_lyc_interrupt(){
	if(ly == lyc){
		stat |= 0x4;
		if(stat & 0x40 > 0){
			irq_lcdc = true;
		}
	}else{
		stat &= !0x4;
	}
}

void render::check_lcdmode_interrupt(){
	switch(stat & 0x3){
		case 0: //H-Blank interrupt
			if(stat & 0x8 > 0){
				irq_lcdc = true;
			}
			break;
		case 1: //V-Blank interrupt
			if(stat & 0x10 > 0){
				irq_lcdc = true;
			}
			break;
		case 2: //OAM Search interrupt
			if(stat & 0x20 > 0){
				irq_lcdc = true;
			}
			break;
		default:
			break;
	}
}

void render::write(int addr, int val){
	if(0x8000 <= addr <= 0x9FFF){
		if(stat & 0x3 != 3){
			VRAM[(addr & 0x1fff)] = val;
		}
	}else if(0xfe00 <= addr <= 0xfe9f){
		if(stat & 0x3 == 0 || stat & 0x3 == 1){
			OAM[(addr & 0x00ff)] = val;
		}
	}else if(addr = 0xff40){
		if(lcdc & 0x80 != val & 0x80){
			ly = 0;
			counter = 0;

			int mode;
			if(val & 0x80 > 0){
				mode = 2;	
			}else{
				mode = 0;
			}
			stat = (stat & 0xf8) | mode;
			check_lcdmode_interrupt();
		}
		lcdc = val;
	}
}

int render::read(int addr){
	int temp;

	if(0x8000 <= addr <= 0x9FFF){
		if(stat & 0x3 != 3){
			VRAM[addr & 0x1fff];
		}else{
			temp = 0xff;
		}
	}else if(0xfe00 <= addr <= 0xfe9f){
		if(stat & 0x3 == 0 || stat & 0x3 == 1){
			OAM[(addr & 0x00ff)];
		}else{
			temp =  0xff;
		}
	}else if(0xff40 <= addr <= 0xff4b){
		switch(addr & 0x1){
			case 0:
				temp =  lcdc;
				break;
			case 1:
				temp =  stat;
				break;
			case 2:
				temp =  scy;
				break;
			case 3:
				temp =  scx;
				break;
			case 4:
				temp =  ly;
				break;
			case 5:
				temp =  lyc;
				break;
			case 6:
				temp =  dma;
				break;
			case 7:
				temp =  bg_palette;
				break;
			case 8:
				temp =  ob_palette0;
				break;
			case 9:
				temp =  ob_palette1;
				break;
			case 0xa:
				temp =  wy;
				break;
			case 0xb:
				temp =  wx;
				break;
		}
	}
	return temp;
}

void render::update(int tick){
	if(lcdc & 0x80 == 0){
		return;
	}

	counter += tick;

	//OAM Search (80 clocks)
	if(stat & 0x3 == 2){
		if(counter >= 80){
			counter -= 80;
			stat = (stat & 0xf8) | 3;
			render_scanline();
		}
	//Pixel Transfer (172 clocks)
	}else if(stat & 0x3 == 3){
		if(counter >= 172){
			counter -= 172;
			stat = (stat & 0xf8);
			check_lcdmode_interrupt();
		}
	// H-Blank (204 clocks)
	}else if(stat & 0x3 == 0){
		if(counter >= 204){
			counter -= 204;
			ly += 1;
			if(ly >= screen_height){
				stat = (stat & 0xf8) | 1;
				irq_vblank = true;
			}else{
				stat = (stat & 0xf8) | 2;
			}
			check_lyc_interrupt();
			check_lcdmode_interrupt();
		}
	// V-Blank (4560 clocks or 10 lines)
	}else{ // = 1 | _
		if(counter >= 456){
			counter -= 456;
			ly += 1;
			if(ly >= 154){
				stat = (stat & 0xf8) | 2;
				ly = 0;
				check_lcdmode_interrupt();
			}
			check_lyc_interrupt();
		}
	}
}


//int main(){
//	read_rom rom;
//	render render;
//	std::string filename = "test";
//	rom.fetch(filename);
//	int a, b;
//	//std::tie(a,b) = render.fetch_tile(80, 0, false);
//	//std::tie(a,b) = render.fetch_bg_tile(10, 10, 5);
//	//std::cout << a << ", " << b << std::endl;
//	int c = 2;
//	if(3 <= c){
//		printf("AAA");
//	}
//}
