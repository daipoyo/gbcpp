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
public:
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


class cpu{
public:
    int pc = 0x100;
    int sp = 0;
    int a = 0;
    int f = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int h = 0;
    int l = 0;
    bool ime = false;
    int tick = 0;
    bool halted = 0;

    int af();
    void set_af(int val);
    int bc();
    void set_bc(int val);
    int de();
    void set_de(int val);
    int hl();
    void set_hl(int val);
    void set_f_z(bool z);
    bool f_z();
    void set_f_n(bool n);
    bool f_n();
    void set_f_h(bool h);
    bool f_h();
    void set_f_c(bool c);
    bool f_c();
    std::string reg_to_string(int idx);
    std::string reg16_to_string(int idx);
    void write_r8(int idx, int val);
    int read_r8(int idx);
    void write_r16(int idx, int val);
    int read_r16(int idx);
    char read_d8();
    int read_d16();
    bool cc(int idx);
    std::string cc_to_string(int idx);
    void write_mem8(int addr, int val);
    int read_mem8(int addr);
    void write_mem16(int addr, int val);
    int read_mem16(int addr);
    void ld_r16_d16(int reg);
    void ld_ind_d16_sp();
    void ld_sp_hl();
    void add_hl_r16(int reg);
    int add_sp(char offset);
    void add_sp_d8();
    void ld_hl_sp_d8();
    void and_r8(char reg);
    void or_r8(char reg);
    void xor_r8(char reg);
    void cp_r8(char reg);
    void daa();
    void cpl();
    void ccf();
    void scf();
    void add(char val);

};

class mmu{
public:
    int ram[0x2000];
    int hram[0x7f];
    int int_flag;
    int int_enable;

    void do_dma(int val);
    void write(int addr, int val);
    int read(int addr);
    void update(int tick);

};