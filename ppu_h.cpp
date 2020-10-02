#pragma once
#include <string>
#include <vector>


std::vector<int> a;
const int screen_width = 160;
const int screen_height = 144;

/*enum BGPriority{
    Color0, Color123,
};*/


class read_rom{
public:
	void fetch(std::string filename);
};


class render{
private:
	int map_color(int color_no, int palette);
	int get_color_no(int tile0, int tile1, int bitpos);
    int VRAM[0x2000];
    int OAM[0xa0];
    int lcdc;
    int stat;
    int scx;
    int scy;
    int ly;
    int lyc;
    int dma;
    int bg_palette;
    int ob_palette0;
    int ob_palette1;
    int wy;
    int wx;
    bool irq_vblank;
    bool irq_lcdc;
    int counter;
    std::array<int,screen_width * screen_height> frame_buffer;
    std::array<int,screen_width> scanline;
    // BGPriority bg_prio = screen_width;
    std::string bg_prio[screen_width];

	//int lcdc = a[0xff40];
	std::tuple<int, int> fetch_tile(int tile_no, int offset, bool tile_data_sel);
	std::tuple<int, int> fetch_bg_window_tile(int tile_x, int tile_y, int offset_y, int tile_map_base);
	std::tuple<int, int> fetch_bg_tile(int tile_x, int tile_y, int offset_y);
	std::tuple<int, int> fetch_window_tile(int tile_x, int tile_y, int offset_y);
	void render_bg();
	void render_splite();
    void render_scanline();
    std::array<int, screen_width * screen_height> exe_frame_buffer();
    void check_lyc_interrupt();
    void check_lcdmode_interrupt();
    void write(int addr, int val);
    int read(int addr);
    void update(int tick);

};
