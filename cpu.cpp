#include <string>
#include "cpu_h.cpp"
#include "mmu.cpp"

mmu mmu;

//Read AF register
int cpu::af(){
	return a << 8 | f;
}

//Wrtie AF register
void cpu::set_af(int val){
	a = (val >> 8) & 0xff;
	f = val & 0xff;
}

//Read BC register
int cpu::bc(){
	return b << 8 | c;
}

//Write BC register
void cpu::set_bc(int val){
	b = (val >> 8 ) & 0xff;
	c = val & 0xff;
}

//Read DE register
int cpu::de(){
	return d << 8 | e;
}

//Write DE register
void cpu::set_de(int val){
	d = (val >> 8) & 0xff;
	e = val & 0xff;
}

//Read HL register
int cpu::hl(){
	return h << 8 | l;
}

//Write HL register
void cpu::set_hl(int val){
	h = (val >> 8) & 0xff;
	l = val & 0xff;
}

//Set Z flag
void cpu::set_f_z(bool z){
	f = (f & !(1 << 7)) | z << 7; //if z==true -> z = 1 else if z==false ->z = 0;
}

//Return Z flag
bool cpu::f_z(){
	if((f >> 7) & 1 == 1){
		return true;
	}else{
		return false;
	}
}

//Set N flag
void cpu::set_f_n(bool n){
	f = (f & !(1 << 6)) | n << 6;
}

//Return N flag
bool cpu::f_n(){
	if((f >> 6) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Set H flag
void cpu::set_f_h(bool h){
	f = (f & !(1 << 5)) | h << 5;
}

//Return H flag
bool cpu::f_h(){
	if((f >> 5) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Set C flag
void cpu::set_f_c(bool c){
	f = (f & !(1 << 4)) | c << 4;
}

//Return C flag
bool cpu::f_c(){
	if((f >> 4) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Convert 8-bit register index to name
std::string cpu::reg_to_string(int idx){
	switch(idx){
		case 0:
			return "B";
			break;
		case 1:
			return "C";
			break;
		case 2:
			return "D";
			break;
		case 3:
			return "E";
			break;
		case 4:
			return "H";
			break;
		case 5:
			return "L";
			break;
		case 6:
			return "HL";
			break;
		case 7:
			return "A";
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
}

//Convert 16-bit register index to name
std::string cpu::reg16_to_string(int idx){
	switch(idx){
		case 0:
			return "BC";
			break;
		case 1:
			return "DE";
			break;
		case 2:
			return "HL";
			break;
		case 3:
			return "SP";
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
}



//Write 8-bit operand
void cpu::write_r8(int idx, int val){
	switch(idx){
		case 0:
			b = val;
			break;
		case 1:
			c = val;
			break;
		case 2:
			d = val;
			break;
		case 3:
			e = val;
			break;
		case 4:
			h = val;
			break;
		case 5:
			l = val;
			break;
		case 6:{
			int hl_val = hl();
			write_mem8(hl_val, val);
			break;
		}
		case 7:
			a = val;
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
}

//Read 8-bit operand
int cpu::read_r8(int idx){
	switch(idx){
		case 0:
			return b;
			break;
		case 1:
			return c;
			break;
		case 2:
			return d;
			break;
		case 3:
			return e;
			break;
		case 4:
			return h;
			break;
		case 5:
			return l;
			break;
		case 6:{
			int hl_val = hl();
			read_mem8(hl_val);
			break;
		}
		case 7:
			return a;
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
}

//Write 16-bit operand
void cpu::write_r16(int idx, int val){
	switch(idx){
		case 0:
			set_bc(val);
			break;
		case 1:
			set_de(val);
			break;
		case 2:
			set_hl(val);
			break;
		case 3:
			sp = val;
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
}

//Read 16-bit operand
int cpu::read_r16(int idx){
	switch(idx){
		case 0:
			return bc();
			break;
		case 1:
			return de();
			break;
		case 2:
			return hl();
			break;
		case 3:
			return sp;
			break;
		default:
			printf("Invalid Operand Index %d", idx);
	}
}
//Read 8-bit immediate from memory
short cpu::read_d8(){
	//unsigned int pc = pc;
	int imm = read_mem8(pc);
	pc = pc + 1;

	return imm;
}

//Read 16-bit immediate from memory
int cpu::read_d16(){
	int imm = read_mem16(pc);
	pc = pc + 2;

	return imm;
}

//Checks branch condition
bool cpu::cc(int idx){
	switch(idx){
		case 0:
			return !f_z();
		case 1:
			return f_z();
		case 2:
			return !f_c();
		case 3:
			return f_c();
		default:
			printf("Invalid Operand index %d", idx);
	}
}

//Convert branch condition to name
std::string cpu::cc_to_string(int idx){
	switch(idx){
		case 0:
			return "NZ";
			break;
		case 1:
			return "Z";
			break;
		case 2:
			return "NC";
			break;
		case 3:
			return "C";
			break;
		default:
			printf("Invalid branch condition index %d", idx);
	}
}


//Write 8-bit value to memory
void cpu::write_mem8(int addr, int val){
	mmu.write(addr, val);

	tick = tick + 4;
}

//read 8-bit value from memory
int cpu::read_mem8(int addr){
	int ret = mmu.read(addr);

	tick = tick + 4;
	return ret;
}

//Write 16-bit value to memory
void cpu::write_mem16(int addr, int val){
	write_mem8(addr, val & 0xff);
	write_mem8(addr + 1, val >> 8);
}

//Read 16-bit value from memory
int cpu::read_mem16(int addr){
	int lo = read_mem8(addr);
	int hi = read_mem8(addr + 1);

	return hi << 8 | lo;
}

//NOP
void nop(){
	printf("NOP");
}

//LD r16, d16
void cpu::ld_r16_d16(int reg){
	int val = read_d16();

	printf("LD %d %d", reg, val);
	
	write_r16(reg, val);
}

//LD (d16), SP
void cpu::ld_ind_d16_sp(){
	int addr = read_d16();
	int sp = sp;

	printf("LD %d SP", addr);

	write_mem16(addr, sp);
}

//LD SP, HL
void cpu::ld_sp_hl(){
	printf("LD SP, HL");

	tick = tick + 4;

	sp = hl();
}

//ADD HL, r16
void cpu::add_hl_r16(int reg){
	printf("ADD HL %d" ,reg);

	int hl_val = hl();
	int val = read_r16(reg);

	bool half_carry = (hl_val & 0xFFF) + (val & 0xFFF) > 0xFFF;
	int res = hl_val + val;
	bool carry = res;  //tuple!!
	set_hl(res);

	tick = tick + 4;

	set_f_n(false);
	set_f_h(half_carry);
	set_f_c(carry);
}

int cpu::add_sp(short offset){
	int val = offset;

	bool half_carry = (sp & 0x0F) + (val & 0x0F) > 0x0F;
	bool carry = (sp & 0xFF) + (val & 0xFF) > 0xFF;

	set_f_z(false);
	set_f_n(false);
	set_f_h(half_carry);
	set_f_c(carry);

	return sp + val;
}

// ADD SP, d8
void cpu::add_sp_d8(){
	int val = read_d8();

	printf("ADD SP, %d", val);

	sp = add_sp(val);

	tick = tick + 8;
}

// LD HL, SP+d8
void cpu::ld_hl_sp_d8(){
	short offset = read_d8();

	printf("LD HL, SP + %d", offset);

	tick = tick + 4;

	int res = add_sp(offset);
	set_hl(res);
}

//AND r8
void cpu::and_r8(short reg){
	printf("AND %s", reg_to_string(reg)); //ayashii...

	int res = a & read_r8(reg);
	a = res;

	set_f_z(res == 0);
	set_f_n(false);
	set_f_h(true);
	set_f_c(false);
}

//AND d8
void cpu::and_d8(){
    unsigned short val = read_d8();
    printf("AND 0x%02x", val);

    unsigned short res = a & val;
    a = res;

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(true);
    set_f_c(false);
}

//OR r8
void cpu::or_r8(short reg){
	printf("OR %s", reg_to_string(reg)); //ayashii...

	int res = a | read_r8(reg);
	a = res;

	set_f_z(res == 0);
	set_f_n(false);
	set_f_h(false);
	set_f_c(false);
}

//OR d8
void cpu::or_d8(){
    unsigned short val = read_d8();
    printf("OR 0x%02x", val);

    unsigned short res = a | val;
    a = res;

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(false);
}

//XOR r8
void cpu::xor_r8(short reg){
	printf("XOR %s", reg_to_string(reg));

	int res = a ^ read_r8(reg);
	a = res;

	set_f_z(res == 0);
	set_f_n(false);
	set_f_h(false);
	set_f_c(false);
}

//XOR d8
void cpu::xor_d8(){
    unsigned short val = read_d8();
    printf("XOR 0x%02x", val);

    unsigned short res = a ^ val;
    a = res;

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(false);
}

//CP r8
void cpu::cp_r8(short reg){
	printf("CP %s", reg_to_string(reg));

	int a = a;
	int val = read_r8(reg);

	set_f_z(a == val);
	set_f_n(true);
	set_f_h(a & 0x0F < val & 0x0F);
	set_f_c(a < val);	
}

//CP d8
void cpu::cp_d8(){
    unsigned short imm = read_d8();
    printf("CP 0x%02x", imm);

    set_f_z(a == imm);
    set_f_n(true);
    set_f_h(a & 0x0f < imm & 0x0f);
    set_f_c(a < imm);
}

//Decimal adjust re;gister A
void cpu::daa(){
	printf("DAA");

	int aa = a;

	if(!f_n()){
		if(f_c() || aa > 0x99){
			aa = aa + 0x60;
			set_f_c(true);
		}
		if(f_h() || aa & 0x0F > 0x09){
			aa = aa + 0x06;
		}
	}else{
		if(f_c()){
			aa = aa + 0x60;
		}
		if(f_h()){
			aa = aa + 0x06;
		}
	}

	a = aa;

	set_f_z(a == 0);
	set_f_h(false);
}

//Complement A
void cpu::cpl(){
	printf("CPL");

	a = !a;
	set_f_n(true);
	set_f_h(true);
}

//Complement carry flag
void cpu::ccf(){
	printf("CCF");

	set_f_n(false);
	set_f_h(false);

	int c = f_c();
	set_f_c(!c);
}

//Set carry flag
void cpu::scf(){
	printf("SCF");

	set_f_n(false);
	set_f_h(false);
	set_f_c(true);
}

void cpu::add(short val){
	bool half_carry = (a & 0xF) + (val & 0xF) > 0xF;
	//int res = a + val;
	//bool carry = res; //tuple!!
	bool carry = (a & 0xFF) + (val & 0xFF) > 0xFF;
	int res;
	if(carry){
		res = a + val - 0xFF;
	}else{
		res = a + val;
	}

	a = res;

	set_f_z(res == 0);
	set_f_n(false);
	set_f_h(half_carry);
	set_f_c(carry);

}

//ADD r8
void cpu::add_r8(unsigned short reg){
	unsigned short val = read_r8(reg);
	printf("ADD %d", reg_to_string(reg));

	add(val);
}

void cpu::adc(unsigned short val){
	short c;
	if(f_c()){
		c = 1;
	}else{
		c = 0;
	}

	short res;
	if(a + val > 0xF){
		res = a + val - 0xF;
		if(res + c > 0xF){
			res = res + c - 0xF;
		}else{
			res = res + c;
		}
	}else{
		res = a + val;
		if(res + c > 0xF){
			res = res + c - 0xF;
		}else{
			res = res + c;
		}
	}

	bool half_carry = (a & 0xF) + (val & 0xF) + c > 0xF;
	bool carry = a + val + c > 0xFF;

	a = res;

	set_f_z(res == 0);
	set_f_n(false);
	set_f_h(half_carry);
	set_f_c(carry);
}

//ADC r8
void cpu::adc_r8(unsigned short reg){
	unsigned short val = read_r8(reg);
	printf("ADC %d", reg_to_string(reg));

	adc(val);

}

//ADC d8
void cpu::adc_d8(){
    unsigned short val = read_d8();
    printf("ADC 0x%02x", val);

    adc(val);
}


void cpu::sub(unsigned short val){
    bool half_carry = (a & 0xf) < (val & 0xf);
    bool carry = (a & 0xf) - (val & 0xf) < 0x0;
    
    unsigned short res;
    if(carry){
        res = 0;
    }else{
        res = (a & 0xf) - (val & 0xf);
    }
    
    a = res;

    set_f_z(res == 0);
    set_f_n(true);
    set_f_h(half_carry);
    set_f_c(carry);
}


//SUB r8
void cpu::sub_r8(unsigned short reg){
    unsigned short val = read_r8(reg);

    printf("SUB 0x%02x", reg_to_string(reg));

    sub(val);
}

//SUB d8
void cpu::sub_d8(){
    unsigned int val = read_d8();

    printf("SUB 0x%02X", val);

    sub(val);
}    

void cpu::sbc(unsigned short val){
    unsigned short c;
    if(f_c()){
        c = 1;
    }else{
        c = 0;
    }

    bool half_carry = (a & 0xf) < (val & 0xf) + c;
    bool carry = (a & 0xff) < (val & 0xff) + c;

    unsigned short res;
    if((a & 0xff) - (val & 0xff) - c < 0){
        res = 0;
    }else{
        res = (a & 0xff) - (val & 0xff) - c;
    }

    a = res;

    set_f_z(res == 0);
    set_f_n(true);
    set_f_h(half_carry);
    set_f_c(carry);

}

//SBC r8
void cpu::sbc_r8(unsigned short reg){
    unsigned short val = read_r8(reg);

    printf("SBC %d", reg_to_string(reg));

    sbc(val);
}

//SBC d8
void cpu::sbc_d8(){
    unsigned int val = read_d8();
    printf("SBC 0x%02x", val);

    sbc(val);
}

//ADD d8
void cpu::add_d8(){
    unsigned short val = read_d8();

    printf("ADD 0x%d", val);

    add(val); 
}

//LDI HL, A
void cpu::ldi_hl_a(){
    printf("LD (HL+), A");

    unsigned int addr = hl();
    write_mem8(addr, a);
    unsigned int hll = hl();
    if(hll + 1 > 0xffff){
        set_hl(0);
    }else{
        set_hl(hll + 1);    
    }
} 

//LDD HL, A
void cpu::ldd_hl_a(){
    printf("LD (HL-), A");

    unsigned int addr = hl();
    write_mem8(addr, a);
    unsigned int hll = hl();
    if(hll - 1 < 0){          //ayashii.....
        set_hl(0xffff);
    }else{
        set_hl(hll - 1);
    }
}

//LDI A, HL
void cpu::ldi_a_hl(){
    printf("LD A, (HL+)");

    unsigned int addr = hl();
    a = read_mem8(addr);
    unsigned int hll = hl();
    if(hll + 1 > 0xffff){
        set_hl(0);
    }else{
        set_hl(hll + 1);
    }
}

//LDD A, HL
void cpu::ldd_a_hl(){
    printf("LD A, (HL-)");

    unsigned int addr = hl();
    a = read_mem8(addr);
    unsigned int hll = hl();
    if(hll - 1 < 0){
        set_hl(0xffff);
    }else{
        set_hl(hll - 1);
    }
}

//LD ind BC, A
void cpu::ld_ind_bc_a(){
    printf("LD (BC), A");

    unsigned int addr = bc();
    write_mem8(addr, a);
}

//LD ind DE, A
void cpu::ld_ind_de_a(){
    printf("LD (DE), A");

    unsigned int addr = de();
    write_mem8(addr, a);
}

//LD ind A, BC
void cpu::ld_ind_a_bc(){
    printf("LD A, (BC)");

    unsigned int bcc = bc();
    a = read_mem8(bcc);
}

//LD ind A, DE
void cpu::ld_ind_a_de(){
    printf("LD A, (DE)");

    unsigned int dee = de();
    a = read_mem8(dee);
}

//Test bit
void cpu::bit(unsigned short pos, unsigned short reg){
    printf("BIT %d, %d", pos, reg_to_string(reg));
    bool z = (read_r8(reg) >> pos & 1) == 0;

    set_f_z(z);
    set_f_n(false);
    set_f_h(true);
}

//Set bit
void cpu::set(unsigned short pos, unsigned short reg){
    printf("SET %d, %d", pos, reg_to_string(reg));

    unsigned short val = read_r8(reg);
    write_r8(reg, val | (1 << pos));
}

//Reset bit
void cpu::res(unsigned short pos, unsigned short reg){
    printf("RES %d, %d", pos, reg_to_string(reg));

    unsigned short val = read_r8(reg);
    write_r8(reg, val & !(1 << pos));
}


void cpu::_rl(unsigned short reg){
    unsigned short orig = read_r8(reg);
    unsigned short res;

    if((orig << 1) || (f_c())){
        res = 1;
    }else{
        res = 0;
    }

    write_r8(reg,res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig >> 7 & 1 == 1);
}

//Rotate left through carry
void cpu::rl(unsigned short reg){
    printf("RL %s", reg_to_string(reg));

    _rl(reg);
}

void cpu::_rlc(unsigned short reg){
    unsigned short orig = read_r8(reg);
    unsigned short res = orig << 1;
    write_r8(reg, res);

    set_f_z(res);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig >> 7 & 1 == 1);

}

//Rotate left
void cpu::rlc(unsigned short reg){
    printf("RLC %s", reg_to_string(reg));

    rlc(reg);
}

void cpu::_rr(unsigned short reg){
    unsigned short orig = read_r8(reg);
    unsigned short res;

    if(f_c()){
        res = (orig >> 1) | 1 << 7;
    }else{
        //res = (orig >> 1) | 0 << 7;
        res = (orig >> 1) | 0;
    }

    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 1 == 1);
}

//Rotate right through carry
void cpu::rr(unsigned short reg){
    printf("RR %s", reg_to_string(reg));

    _rr(reg);
}

void cpu::_rrc(unsigned short reg){
    unsigned short orig = read_r8(reg);
    unsigned short res = orig >> 1;
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 1 == 1);
}

//Rotate right
void cpu::rrc(unsigned short reg){
    printf("RRC %s", reg_to_string(reg));

    _rrc(reg);
}

//Shift left into carry
void cpu::sla(unsigned short reg){
    printf("SLA [%2s]", reg_to_string(reg));
    
    unsigned short orig = read_r8(reg);
    unsigned int res = orig << 1;
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 0x80 > 0);
}

//Shift right into carry
void cpu::sra(unsigned short reg){
    printf("SRA [%2s]", reg_to_string(reg));

    unsigned short orig = read_r8(reg);
    unsigned short res = (orig >> 1) | (orig & 0x80);
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 1 > 0);
}

//Swap low/hi-nibble
void cpu::swap(unsigned short reg){
    printf("SWAP [%2s]", reg_to_string(reg));

    unsigned short orig = read_r8(reg);
    unsigned int res = ((orig & 0x0f) << 4) | ((orig & 0xf0) >> 4);
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(false);
}

//Shift right through carry
void cpu::srl(unsigned short reg){
    printf("SRL [%2s]", reg_to_string(reg));

    unsigned short orig = read_r8(reg);
    unsigned int res = orig >> 1;
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 1 == 1);
}

void cpu::_jp(unsigned int addr){
    pc = addr;

    tick = tick + 4;
}

void cpu::jp_cc_d8(unsigned short cci){
    unsigned int addr = read_d16();

    printf("JP [%s], [%04x]", cc_to_string(cci), addr);

    if(cc(cci)){
        _jp(addr);
    }
}

//Unconditional jump to d16
void cpu::jp_d16(){
    unsigned int address = read_d16();

    printf("JP [%04x]", address);

    _jp(address);
}

//Jump to pc+d8 if CC
void cpu::jr_cc_d8(unsigned short cci){
    int offset = read_d8();

    printf("JR [%s] [%s]", cc_to_string(cci), offset);

    if(cc(cci)){
        _jr(offset);
    }
}

void cpu::_jr(short offset){
    if(pc + offset > 0xffff){
        pc = pc + offset - 0xffff;
    }else{
        pc = pc + offset;
    }

    tick = tick + 4;
}

//Jump to pc+d8
void cpu::jr_d8(){
    short offset = read_d8();
    printf("JR [%d]", offset);

    _jr(offset);
}

void cpu::ld_io_d8_a(){
    unsigned int offset = read_d8();
    unsigned int addr  = 0xff00 | offset;

    printf("LD (0xff00+0x[%02x]), A", offset);

    write_mem8(addr, a);
}

void cpu::ld_io_c_a(){
    unsigned int addr = 0xff00 | c;
    printf("LD (0xff00 + C), A");
    
    write_mem8(addr, a);
}

void cpu::ld_a_io_c(){
    unsigned int addr = 0xff00 | c;
    printf("LD A, (0xff00 + c)");

    a = read_mem8(addr);
}

void cpu::ld_r8_d8(unsigned short reg){
    unsigned short imm = read_d8();
    printf("LD [%s], 0x[%02x]", reg_to_string(reg), imm);

    write_r8(reg, imm);
}

//INC r8
void cpu::inc_r8(unsigned short reg){
    printf("INC [%s]", reg_to_string(reg));

    unsigned short orig = read_r8(reg);
    unsigned short res;

    if(orig + 1 > 0xff){
        res = 0;
    }else{
        res = orig + 1;
    }

    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_h(orig & 0x0f == 0x0f);
    set_f_n(false);
}

//DEC r8
void cpu::dec_r8(unsigned short reg){
    printf("DEC [%s]", reg_to_string(reg));

    unsigned short orig = read_r8(reg);
    unsigned short res;

    if(orig - 1 < 0){
        res = 0xff;
    }else{
        res = orig - 1;
    }

    set_f_z(res == 0);
    set_f_h(orig & 0x0f == 0x00);
    set_f_n(true);
}

//LD r8, r8
void cpu::ld_r8_r8(unsigned short reg1, unsigned short reg2){
    printf("LD [%s], [%s]", reg_to_string(reg1), reg_to_string(reg2));

    unsigned short val = read_r8(reg2);
    write_r8(reg1, val);
}

void cpu::_call(unsigned int addr){
    if(sp - 1 < 0){
        sp = 0xff;
    }else{
        sp = sp - 1;
        if(sp - 1 < 0){
            sp = 0xff;
        }else{
            sp = sp - 1;
        }
    }
    
    tick = tick + 4;

    write_mem16(sp, pc);
    pc = addr;
}





int main(){
    cpu cpu;
    printf("RRC [%2s]", cpu.reg_to_string(3));

    return EXIT_SUCCESS;
}

















