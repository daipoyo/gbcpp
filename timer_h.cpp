class timer : public io_device{
public:
	//Timer counter
	mutable unsigned short tima;
	//Timer modulo
	mutable unsigned short tma;
	//Timer control
	mutable unsigned short tac;
	//Internal 16-bit counter
	mutable unsigned int counter;
	//Interrupt request
	bool irq;

private:
	void write(unsigned int addr, unsigned short val) override;

};