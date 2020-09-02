#include <string>
#include <vector>

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
    short read_d8();
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
    int add_sp(short offset);
    void add_sp_d8();
    void ld_hl_sp_d8();
    void and_r8(short reg);
    void and_d8();
    void or_r8(short reg);
    void or_d8();
    void xor_r8(short reg);
    void xor_d8();
    void cp_r8(short reg);
    void cp_d8();
    void daa();
    void cpl();
    void ccf();
    void scf();
    void add(short val);
    void add_r8(unsigned short reg);
    void sub_r8(unsigned short reg);
    void adc(unsigned short val);
    void adc_r8(unsigned short reg);
    void adc_d8();
    void sub(unsigned short val);
    void sbc(unsigned short val);
    void sbc_r8(unsigned short reg);

};
