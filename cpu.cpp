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
