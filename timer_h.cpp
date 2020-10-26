#pragma once
#include "io_device.cpp"

class timer : public io_device{
public:
	//Timer counter
	unsigned short tima = 0;
	//Timer modulo
	unsigned short tma = 0;
	//Timer control
	unsigned short tac = 0;
	//Internal 16-bit counter
	unsigned int counter = 0;
	//Interrupt request
	bool irq;
	void write(unsigned int addr, unsigned short val) override;
	unsigned int read(unsigned int addr, unsigned short val) override;
	void update(unsigned int tick) override;

private:

};