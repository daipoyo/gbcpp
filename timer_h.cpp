class timer{
public:
	//Timer counter
	unsigned short tima;
	//Timer modulo
	unsigned short tma;
	//Timer control
	unsigned short tac;
	//Internal 16-bit counter
	unsigned int counter;
	//Interrupt request
	bool irq;

private:
	void write(unsigned int addr, unsigned short val);


};