//Read AF register
int af(){
	return a << 8 | f;
}

//Wrtie AF register
void set_af(int val){
	a = (val >> 8) & 0xff;
	f = val & 0xff;
}

//Read BC register
int bc(){
	return b << 8 | c;
}

//Write BC register
void set_bc(val){
	b = (val >> 8 ) & 0xff;
	c = val & 0xff;
}

//Read DE register
int de(){
	return d << 8 | e;
}

//Write DE register
void set_de(){
	d = (val >> 8) & 0xff;
	e = val & 0xff;
}

//Read HL register
int hl(){
	return h << 8 | l;
}

//Write HL register
void set_hl(){
	h = (val >> 8) & 0xff;
	l = val & 0xff;
}

//Set Z flag
void set_f_z(bool z){
	f = (f & !(1 << 7)) | z << 7; //if z==true -> z = 1 else if z==false ->z = 0;
}

//Return Z flag
bool f_z(){
	if((f >> 7) & 1 == 1){
		return true;
	}else{
		return false;
	}
}

//Set N flag
void set_f_n(bool n){
	f = (f & !(1 << 6)) | n << 6;
}

//Return N flag
bool f_n(){
	if((f >> 6) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Set H flag
void set_f_h(bool h){
	f = (f & !(1 << 5)) | n << 5;
}

//Return H flag
bool f_h(){
	if((f >> 5) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Set C flag
void set_f_c(bool c){
	f = (f & !(1 << 4)) | c << 4;
}

//Return C flag
bool f_c(){
	if((f >> 4) & 1 == 1){
		return true;
	}else{
		return false;
	}	
}

//Convert 8-bit register index to name
std:string reg_to_string(idx){
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

	}
}
