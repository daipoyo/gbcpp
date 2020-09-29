#pragma once
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
    unsigned short step();
    void dump();

    int af();

private:
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
    void sub_d8();
    void adc(unsigned short val);
    void adc_r8(unsigned short reg);
    void adc_d8();
    void sub(unsigned short val);
    void sbc(unsigned short val);
    void sbc_r8(unsigned short reg);
    void sbc_d8();
    void add_d8();
    void ldi_hl_a();
    void ldd_hl_a();
    void ldi_a_hl();
    void ldd_a_hl();
    void ld_ind_bc_a();
    void ld_ind_de_a();
    void ld_ind_a_bc();
    void ld_ind_a_de();
    void bit(unsigned short pos, unsigned short reg);
    void set(unsigned short pos, unsigned short reg);
    void res(unsigned short res, unsigned short reg);
    void _rl(unsigned short reg);
    void rl(unsigned short reg);
    void _rlc(unsigned short reg);
    void rlc(unsigned short reg);
    void _rr(unsigned short reg);
    void rr(unsigned short reg);
    void _rrc(unsigned short reg);
    void rrc(unsigned short reg);
    void sla(unsigned short reg);
    void sra(unsigned short reg);
    void swap(unsigned short reg);
    void srl(unsigned short reg);
    void _jp(unsigned int addr);
    void jp_cc_d8(unsigned short cci);
    void jp_d16();
    void jp_hl();
    void jr_cc_d8(unsigned short cci);
    void _jr(short offset);
    void jr_d8();
    void ld_io_d8_a();
    void ld_a_io_d8();
    void ld_io_c_a();
    void ld_a_io_c();
    void ld_r8_d8(unsigned short reg);
    void inc_r8(unsigned short reg);
    void dec_r8(unsigned short reg);
    void ld_r8_r8(unsigned short reg1, unsigned short reg2);
    void _call(unsigned int addr);
    void call_d16();
    void call_cc_d16(unsigned short cci);
    void rst(unsigned short addr);
    void _ret();
    void ret();
    void ret_cc(unsigned short cci);
    void push_bc();
    void push_de();
    void push_hl();
    void push_af();
    void pop_bc();
    void pop_de();
    void pop_hl();
    void pop_af();
    void rlca();
    void rla();
    void rrca();
    void rra();
    void inc_r16(unsigned short reg);
    void dec_r16(unsigned short reg);
    void ld_ind_d16_a();
    void ld_a_ind_d16();
    void di();
    void ei();
    void reti();
    void prefix();
    void halt();
    void check_irqs();
    void call_isr(short id);
    void fetch_and_exec();
};
