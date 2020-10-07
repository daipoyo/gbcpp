#pragma once
#include <string>
#include <vector>
#include "io_device.cpp"

class mmu : public io_device{
public:
    int ram[0x2000];
    int hram[0x7f];
    int int_flag;
    int int_enable;
    void do_dma(int val);
    void write(unsigned int addr, unsigned short val) override;
    unsigned int read(unsigned int addr, unsigned short val) override;
    void update(unsigned int tick) override;
    //void write(int addr, int val);
    //unsigned int read(int addr);
    //void update(int tick);

};
