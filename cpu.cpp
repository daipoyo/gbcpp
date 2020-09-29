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
	f = (f & ~(1 << 7)) | z << 7; //if z==true -> z = 1 else if z==false ->z = 0;
}

//Return Z flag
bool cpu::f_z(){
	if(((f >> 7) & 1 ) == 1){
		return true;
	}else{
		return false;
	}
}

//Set N flag
void cpu::set_f_n(bool n){
	f = (f & ~(1 << 6)) | n << 6;
}

//Return N flag
bool cpu::f_n(){
	if(((f >> 6) & 1 ) == 1){
		return true;
	}else{
		return false;
	}	
}

//Set H flag
void cpu::set_f_h(bool h){
	f = (f & ~(1 << 5)) | h << 5;
}

//Return H flag
bool cpu::f_h(){
	if(((f >> 5) & 1 ) == 1){
		return true;
	}else{
		return false;
	}	
}

//Set C flag
void cpu::set_f_c(bool c){
	f = (f & ~(1 << 4)) | c << 4;
}

//Return C flag
bool cpu::f_c(){
	if(((f >> 4) & 1 ) == 1){
		return true;
	}else{
		return false;
	}	
}

//Convert 8-bit register index to name
std::string cpu::reg_to_string(int idx){

	std::string reg8;

	switch(idx){
		case 0:
			reg8 = "B";
			break;
		case 1:
			reg8 = "C";
			break;
		case 2:
			reg8 = "D";
			break;
		case 3:
			reg8 = "E";
			break;
		case 4:
			reg8 = "H";
			break;
		case 5:
			reg8 = "L";
			break;
		case 6:
			reg8 = "HL";
			break;
		case 7:
			reg8 = "A";
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
	return reg8;
}

//Convert 16-bit register index to name
std::string cpu::reg16_to_string(int idx){

	std::string reg16;

	switch(idx){
		case 0:
			reg16 = "BC";
			break;
		case 1:
			reg16 = "DE";
			break;
		case 2:
			reg16 = "HL";
			break;
		case 3:
			reg16 = "SP";
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
	return reg16;
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

	int operand;

	switch(idx){
		case 0:
			operand = b;
			break;
		case 1:
			return c;
			break;
		case 2:
			operand = d;
			break;
		case 3:
			operand = e;
			break;
		case 4:
			operand = h;
			break;
		case 5:
			operand = l;
			break;
		case 6:{
			int hl_val = hl();
			operand = read_mem8(hl_val);
			break;
		}
		case 7:
			operand = a;
			break;
		default:
			printf("Invalid Operand index %d", idx);
			break;
	}
	return operand;
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
	int operand;

	switch(idx){
		case 0:
			operand =  bc();
			break;
		case 1:
			operand = de();
			break;
		case 2:
			operand = hl();
			break;
		case 3:
			operand = sp;
			break;
		default:
			printf("Invalid Operand Index %d", idx);
	}
	return operand;
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
	bool result;

	switch(idx){
		case 0:
			result = !f_z();
		case 1:
			result = f_z();
		case 2:
			result = !f_c();
		case 3:
			result = f_c();
		default:
			printf("Invalid Operand index %d", idx);
	}
	return result;
}

//Convert branch condition to name
std::string cpu::cc_to_string(int idx){
	std::string result;

	switch(idx){
		case 0:
			//return "NZ";
			result = "NZ";
			break;
		case 1:
			//return "Z";
			result = "Z";
			break;
		case 2:
			//return "NC";
			result = "NC";
			break;
		case 3:
			//return "C";
			result = "C";
			break;
		default:
			printf("Invalid branch condition index %d", idx);
			break;
	}
	return result;
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
	printf("AND %s", reg_to_string(reg).c_str()); //ayashii...

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
	printf("OR %s", reg_to_string(reg).c_str()); //ayashii...

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
	printf("XOR %s", reg_to_string(reg).c_str());

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
	printf("CP %s", reg_to_string(reg).c_str());

	int a = this->a;
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
	printf("ADD %s", reg_to_string(reg).c_str());

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
	printf("ADC %s", reg_to_string(reg).c_str());

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

    printf("SUB %s", reg_to_string(reg).c_str());

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

    printf("SBC %s", reg_to_string(reg).c_str());

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

    printf("ADD 0x%02x", val);

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
    printf("BIT %d, %s", pos, reg_to_string(reg).c_str());
    bool z = (read_r8(reg) >> pos & 1) == 0;

    set_f_z(z);
    set_f_n(false);
    set_f_h(true);
}

//Set bit
void cpu::set(unsigned short pos, unsigned short reg){
    printf("SET %d, %s", pos, reg_to_string(reg).c_str());

    unsigned short val = read_r8(reg);
    write_r8(reg, val | (1 << pos));
}

//Reset bit
void cpu::res(unsigned short pos, unsigned short reg){
    printf("RES %d, %s", pos, reg_to_string(reg).c_str());

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
    printf("RL %s", reg_to_string(reg).c_str());

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
    printf("RLC %s", reg_to_string(reg).c_str());

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
    printf("RR %s", reg_to_string(reg).c_str());

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
    printf("RRC %s", reg_to_string(reg).c_str());

    _rrc(reg);
}

//Shift left into carry
void cpu::sla(unsigned short reg){
    printf("SLA %2s", reg_to_string(reg).c_str());
    
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
    printf("SRA %2s", reg_to_string(reg).c_str());

    unsigned short orig = read_r8(reg);
    unsigned short res = (orig >> 1) | (orig & 0x80);
    write_r8(reg, res);

    set_f_z(res == 0);
    set_f_n(false);
    set_f_h(false);
    set_f_c(orig & 1 > 0);
}

//Swap low/hi-nibble *nibble = 4bit
void cpu::swap(unsigned short reg){
    printf("SWAP %2s", reg_to_string(reg).c_str());

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
    printf("SRL %2s", reg_to_string(reg).c_str());

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

    printf("JP %s, %04x", cc_to_string(cci).c_str(), addr);

    if(cc(cci)){
        _jp(addr);
    }
}

//Unconditional jump to d16
void cpu::jp_d16(){
    unsigned int address = read_d16();

    printf("JP %04x", address);

    _jp(address);
}

//Unconditional jump to HL
void cpu::jp_hl(){
	printf("JP (HL)");

	pc = hl();
}

//Jump to pc+d8 if CC
void cpu::jr_cc_d8(unsigned short cci){
    int offset = read_d8();

    printf("JR %s %d", cc_to_string(cci).c_str(), offset);

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

void cpu::ld_a_io_d8(){
	unsigned int offset = read_d8();
	unsigned int addr = 0xff00 | offset;

	printf("LD A, 0xff00+0x%02x", offset);

	a = read_mem8(addr);
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
    printf("LD %s, 0x[%02x]", reg_to_string(reg).c_str(), imm);

    write_r8(reg, imm);
}

//INC r8
void cpu::inc_r8(unsigned short reg){
    printf("INC %s", reg_to_string(reg).c_str());

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
    printf("DEC %s", reg_to_string(reg).c_str());

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
    printf("LD %s, %s", reg_to_string(reg1).c_str(), reg_to_string(reg2).c_str());

    unsigned short val = read_r8(reg2);
    write_r8(reg1, val);
}

void cpu::_call(unsigned int addr){
    if(sp - 1 < 0){
        sp = 0xff;
        sp = sp - 1;
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

//CALL d16
void cpu::call_d16(){
    unsigned int addr = read_d16();

    printf("CALL [%x]",  addr);

    _call(addr);
}

//CALL CC, d16
void cpu::call_cc_d16(unsigned short cci){
    unsigned int addr = read_d16();

    printf("CALL %s, 0x[%04x]", cc_to_string(cci).c_str(), addr);

    if(cc(cci)){
        _call(addr);
    }
}

void cpu::rst(unsigned short addr){
    printf("RST 0x[%02x]", addr);

    _call(addr);
}

void cpu::_ret(){
    pc = read_mem16(sp);
    
    if(sp + 1 > 0xff){
        sp = 0;
        sp = sp + 1;
    }else{
        sp = sp + 1;
        if(sp + 1 > 0xff){
            sp = 0;
        }else{
            sp = sp + 1;
        }
    }

    tick = tick + 4;
}

//RET
void cpu::ret(){
    printf("RET");

    _ret();
}

//RET CC
void cpu::ret_cc(unsigned short cci){
    printf("RET %s", cc_to_string(cci).c_str());

    tick = tick + 4;

    if(cc(cci)){
        _ret();
    }
}

//PUSH BC
void cpu::push_bc(){
	printf("PUSH BC");

	if(sp - 1 < 0){
      sp = 0xff;
      sp = sp - 1;
  }else{
      sp = sp - 1;
      if(sp - 1 < 0){
          sp = 0xff;
      }else{
          sp = sp - 1;
      }
  }

  unsigned int val = bc();

  tick = tick + 4;

  write_mem16(sp, val);
}

//PUSH DE
void cpu::push_de(){
	  printf("PUSH DE");

	  if(sp - 1 < 0){
        sp = 0xff;
        sp = sp - 1;
    }else{
        sp = sp - 1;
        if(sp - 1 < 0){
            sp = 0xff;
        }else{
            sp = sp - 1;
        }
    }

    unsigned int val = de();

    tick = tick + 4;

    write_mem16(sp, val);
}

//PUSH HL
void cpu::push_hl(){
	  printf("PUSH HL");

	  if(sp - 1 < 0){
        sp = 0xff;
        sp = sp - 1;
    }else{
        sp = sp - 1;
        if(sp - 1 < 0){
            sp = 0xff;
        }else{
            sp = sp - 1;
        }
    }

    unsigned int val = hl();

    tick = tick + 4;

    write_mem16(sp, val);
}

//PUSH AF
void cpu::push_af(){
	  printf("PUSH AF");

	  if(sp - 1 < 0){
        sp = 0xff;
        sp = sp - 1;
    }else{
        sp = sp - 1;
        if(sp - 1 < 0){
            sp = 0xff;
        }else{
            sp = sp - 1;
        }
    }

    unsigned int val = af();

    tick = tick + 4;

    write_mem16(sp, val);
}

//POP BC
void cpu::pop_bc(){
	printf("POP BC");

	unsigned int val = read_mem16(sp);
	set_bc(val);

  if(sp + 1 > 0xff){
      sp = 0;
      sp = sp + 1;
  }else{
      sp = sp + 1;
      if(sp + 1 > 0xff){
          sp = 0;
      }else{
          sp = sp + 1;
      }
  }
}


//POP DE
void cpu::pop_de(){
	printf("POP DE");

	unsigned int val = read_mem16(sp);
	set_de(val);

  if(sp + 1 > 0xff){
      sp = 0;
      sp = sp + 1;
  }else{
      sp = sp + 1;
      if(sp + 1 > 0xff){
          sp = 0;
      }else{
          sp = sp + 1;
      }
  }
}

//POP HL
void cpu::pop_hl(){
	printf("POP HL");

	unsigned int val = read_mem16(sp);
	set_hl(val);

  if(sp + 1 > 0xff){
      sp = 0;
      sp = sp + 1;
  }else{
      sp = sp + 1;
      if(sp + 1 > 0xff){
          sp = 0;
      }else{
          sp = sp + 1;
      }
  }
}

//POP AF
void cpu::pop_af(){
	printf("POP AF");

	// lower nibble of F is always zero
	unsigned int val = read_mem16(sp) & 0xfff0;
	set_af(val);

  if(sp + 1 > 0xff){
      sp = 0;
      sp = sp + 1;
  }else{
      sp = sp + 1;
      if(sp + 1 > 0xff){
          sp = 0;
      }else{
          sp = sp + 1;
      }
  }
}

void cpu::rlca(){
	printf("RLCA");

	_rlc(7);
	set_f_z(false);
}

void cpu::rla(){
	printf("RLA");

	_rl(7);
	set_f_z(false);
}

void cpu::rrca(){
	printf("RRCA");

	_rrc(7);
	set_f_z(false);
}

void cpu::rra(){
	printf("RRA");

	_rr(7);
	set_f_z(false);
}

void cpu::inc_r16(unsigned short reg){
	printf("INC %s", reg16_to_string(reg));

	unsigned int val = read_r16(reg);

	if(val + 1 > 0xff){
		val = 0;
	}else{
		val = val + 1;
	}

	write_r16(reg, val);

	tick = tick + 4;
}

void cpu::dec_r16(unsigned short reg){
	printf("DEC %s", reg16_to_string(reg));

	unsigned int val = read_r16(reg);

	if(val - 1 < 0){
		val = 0xff;
	}else{
		val = val - 1;
	}

	write_r16(reg, val);

	tick = tick + 4;
}

void cpu::ld_ind_d16_a(){
	unsigned int addr = read_d16();
	printf("LD 0x%04x, A", addr);

	write_mem8(addr, a);
}

void cpu::ld_a_ind_d16(){
	unsigned int addr = read_d16();
	printf("LD A, 0x%04x", addr);

	a = read_mem8(addr);
}

//Disable interrupt
void cpu::di(){
	printf("DI");

	ime = false;
}

//Enable interrupt
void cpu::ei(){
	printf("EI");

	ime = true;
}

//Enable interrupt and return
void cpu::reti(){
	printf("RETI");

	ime = true;

	_ret();
}

//Prefixed instructions
void cpu::prefix(){
	unsigned short opcode = read_d8();
	unsigned int pos = opcode >> 3 & 0x7;
	unsigned int reg = opcode & 0x7;

	if(0x00 <= opcode <= 0x07){
		rlc(reg);
	}else if(0x08 <= opcode <= 0x0f){
		rrc(reg);
	}else if(0x10 <= opcode <= 0x17){
		rl(reg);
	}else if(0x18 <= opcode <= 0x1f){
		rr(reg);
	}else if(0x20 <= opcode <= 0x27){
		sla(reg);
	}else if(0x28 <= opcode <= 0x2f){
		sra(reg);
	}else if(0x30 <= opcode <= 0x37){
		swap(reg);
	}else if(0x38 <= opcode <= 0x3f){
		srl(reg);
	}else if(0x40 <= opcode <= 0x7f){
		bit(pos, reg);
	}else if(0x80 <= opcode <= 0xbf){
		res(pos, reg);
	}else if(0xc0 <= opcode <= 0xff){
		set(pos, reg);
	}else{
		printf("Unimplemented opcode 0xcb 0x%x", opcode);
	}
}

//HALT
void cpu::halt(){
	printf("HALT");

	if(ime){
		halted = true;
	}
}


//[public] Execute a single instruction and handle IRQs.
unsigned short cpu::step(){
	unsigned short total_tick = 0;
	tick = 0;

	if(halted){
		tick = tick + 4;
	}else{
		fetch_and_exec();
	}

	total_tick = total_tick + tick;

	mmu.update(tick);

	if(ime){
		tick = 0;
		check_irqs();
		mmu.update(tick);

		total_tick = total_tick + tick;
	}

	return total_tick;
}

//Check IRQs and execute ISRs if rewuested.
void cpu::check_irqs(){
	//Bit 0 has the highest priority
	for(int i = 0; i < 6; i = i + 1){
		bool irq = mmu.int_flag & (1 << i) > 0;
		bool ie = mmu.int_enable & (1 << i) > 0;

		//If interrupt is requested and enabled
		if(irq && ie){
			call_isr(i);
			break;
		}
	}
}

//Calls requested interrupt service routine.
void cpu::call_isr(short id){
	//Reset corresponding bit in IF
	mmu.int_flag &= !(1 << id);
	//Clear IME(disable any further interrupts)
	ime = false;
	halted = false;

	unsigned int isr;

	switch(id){
		case 1:
			isr = 0x40;
			break;
		case 2:
			isr = 0x48;
			break;
		case 3:
			isr = 0x50;
			break;
		case 4:
			isr = 0x80;
			break;
		default:
			printf("Invalid IRQ id %d", id);
			break;
	}

	tick = tick + 8;

	printf("Calling ISR 0x%02x", isr);

	_call(isr);

} 


//Fetches and executes a single instructions.
void cpu::fetch_and_exec(){
	unsigned short opcode = read_d8();
	unsigned short reg = opcode & 7;
	unsigned short reg2 = opcode >> 3 & 7;

	switch(opcode){
		//NOP
		case 0x00:
			nop();
			break;
		//LD r16, d16
		case 0x01:
		case 0x11:
		case 0x21:
		case 0x31:
			ld_r16_d16(opcode >> 4);
			break;
		//LD (d16), SP
		case 0x08:
			ld_ind_d16_sp();
			break;
		//LD SP, HL
		case 0xf9:
			ld_sp_hl();
		//LD A, r16
		case 0x02:
			ld_ind_bc_a();
			break;
		case 0x12:
			ld_ind_de_a();
			break;
		case 0x0a:
			ld_ind_a_bc();
			break;
		case 0x1a:
			ld_ind_a_de();
			break;
		//PUSH r16
		case 0xc5:
			push_bc();
			break;
		case 0xd5:
			push_de();
			break;
		case 0xe5:
			push_hl();
			break;
		case 0xf5:
			push_af();
			break;
		//POP r16
		case 0xc1:
			pop_bc();
			break;
		case 0xd1:
			pop_de();
			break;
		case 0xe1:
			pop_hl();
			break;
		case 0xf1:
			pop_af();
			break;
		//Conditional absolute jump
		case 0xc2:
		case 0xd2:
		case 0xca:
		case 0xda:
			jp_cc_d8(reg2);
			break;
		//Unconditional absolute jump
		case 0xc3:
			jp_d16();
			break;
		case 0xe9:
			jp_hl();
			break;
		//Conditional relative jump
		case 0x20:
		case 0x30:
		case 0x28:
		case 0x38:
			jr_cc_d8(reg2 - 4);
			break;
		//Unconditional relative jump
		case 0x18:
			jr_d8();
			break;
		//Bit rotate on A
		case 0x07:
			rlca();
			break;
		case 0x17:
			rla();
			break;
		case 0x0f:
			rrca();
			break;
		case 0x1f:
			rra();
			break;
		//Arithmethic/logical operation on 16-bit register
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
			add_hl_r16(opcode >> 4);
			break;
		case 0xe8:
			add_sp_d8();
			break;
		case 0xf8:
			ld_hl_sp_d8();
			break;
		//Arithmethic/logical operation on 8-bit register
		case 0x80 ... 0x87:
			add_r8(reg);
			break;
		case 0x88 ... 0x8f:
			adc_r8(reg);
			break;
		case 0x90 ... 0x97:
			sub_r8(reg);
			break;
		case 0x98 ... 0x9f:
			sbc_r8(reg);
			break;
		case 0xa0 ... 0xa7:
			and_r8(reg);
			break;
		case 0xb0 ... 0xb7:
			or_r8(reg);
			break;
		case 0xa8 ... 0xaf:
			xor_r8(reg);
			break;
		case 0xb8 ... 0xbf:
			cp_r8(reg);
			break;
		//DAA
		case 0x27:
			daa();
			break;
		//CPL
		case 0x2f:
			cpl();
			break;
		//SCF
		case 0x37:
			scf();
			break;
		//CCF
		case 0x3f:
			ccf();
			break;
		//Arithmethic/logical operation on A
		case 0xc6:
			add_d8();
			break;
		case 0xd6:
			sub_d8();
			break;
		case 0xe6:
			and_d8();
			break;
		case 0xf6:
			or_d8();
			break;
		case 0xce:
			adc_d8();
			break;
		case 0xde:
			sbc_d8();
			break;
		case 0xee:
			xor_d8();
			break;
		case 0xfe:
			cp_d8();
			break;
		//LDI
		case 0x22:
			ldi_hl_a();
			break;
		case 0x2a:
			ldi_a_hl();
			break;
		//LDD
		case 0x32:
			ldd_hl_a();
			break;
		case 0x3a:
			ldd_a_hl();
			break;
		//LD IO port
		case 0xe0:
			ld_io_d8_a();
			break;
		case 0xf0:
			ld_a_io_d8();
			break;
		case 0xe2:
			ld_io_c_a();
			break;
		case 0xf2:
			ld_a_io_c();
			break;
		//LD r8, d8
		case 0x06:
		case 0x0e:
		case 0x16:
		case 0x1e:
		case 0x26:
		case 0x2e:
		case 0x36:
		case 0x3e:
			ld_r8_d8(reg2);
			break;
		//INC r8
		case 0x04:
		case 0x0c:
		case 0x14:
		case 0x1c:
		case 0x24:
		case 0x2c:
		case 0x34:
		case 0x3c:
			inc_r8(reg2);
			break;
		//DEC r8
		case 0x05:
		case 0x0d:
		case 0x15:
		case 0x1d:
		case 0x25:
		case 0x2d:
		case 0x35:
		case 0x3d:
			dec_r8(reg2);
			break;
		//LD r8, r8
		case 0x40 ... 0x75:
		case 0x77 ... 0x7f:
			ld_r8_r8(reg2, reg);
			break;
		//LD (d16), A
		case 0xea:
			ld_ind_d16_a();
			break;
		//LD A, (d16)
		case 0xfa:
			ld_a_ind_d16();
			break;
		//INC r16
		case 0x03:
		case 0x13:
		case 0x23:
		case 0x33:
			inc_r16(opcode >> 4);
			break;
		//DEC r16
		case 0x0b:
		case 0x1b:
		case 0x2b:
		case 0x3b:
			dec_r16(opcode >> 4);
			break;
		//Unconditional call
		case 0xcd:
			call_d16();
			break;
		//Conditioncal call
		case 0xc4:
		case 0xd4:
		case 0xcc:
		case 0xdc:
			call_cc_d16(reg2);
			break;
		//Unconditional ret
		case 0xc9:
			ret();
			break;
		//Conditional ret
		case 0xc0:
		case 0xd0:
		case 0xc8:
		case 0xd8:
			ret_cc(reg2);
			break;
		//RETI
		case 0xd9:
			reti();
			break;
		//RST
		case 0xc7:
		case 0xcf:
		case 0xd7:
		case 0xdf:
		case 0xe7:
		case 0xef:
		case 0xf7:
		case 0xff:
			rst(opcode - 0xc7);
			break;
		//DI
		case 0xf3:
			di();
			break;
		//EI
		case 0xfb:
			ei();
			break;
		//CB prefixed
		case 0xcb:
			prefix();
			break;
		//HALT
		case 0x76:
			halt();
			break;
		default:
			printf("Unimplemented opcode 0x%x", opcode);
			break;
	}
} 

//Dumps current CPU state
void cpu::dump(){
	printf("CPU State:");
	printf("PC: 0x%04x SP: 0x%04x", pc, sp);
	printf("AF: 0x%04x BC: 0x%04x", af(), bc());
	printf("DE: 0x%04x HL: 0x%04x", de(), hl());
	printf("T: %d", tick);
}

int main(){
	cpu cpu;
	cpu.af();
}