#include <string>
#include "h.cpp"
#include "mmu.cpp"



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
void cpu::set_bc(val){
	b = (val >> 8 ) & 0xff;
	c = val & 0xff;
}

//Read DE register
int cpu::de(){
	return d << 8 | e;
}

//Write DE register
void cpu::set_de(){
	d = (val >> 8) & 0xff;
	e = val & 0xff;
}

//Read HL register
int cpu::hl(){
	return h << 8 | l;
}

//Write HL register
void cpu::set_hl(){
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
	f = (f & !(1 << 5)) | n << 5;
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
		case 6:
			hl = hl();
			write_mem8(hl, val);
			break;
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
		case 6:
			hl = hl();
			read_mem8(hl);
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
	write(addr, val);

	tick = tick + 4;
}

//read 8-bit value from memory
int cpu::read_mem8(int addr){
	int ret = read(addr);

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

