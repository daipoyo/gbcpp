#include <string>
#include <vector>

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