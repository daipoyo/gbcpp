#pragma once
#include "io_device.cpp"

class joypad : public io_device{
public:
	//Interrupt request
	bool irq = false;

	enum Key {
		Down,
		Up,
		Left,
		Right,
		Start,
		Select,
		B,
		A,
	};
	
	void keydown(enum Key direction);
	void keyup(enum Key direction);
    void write(unsigned int addr, unsigned short val) override;
	unsigned int read(unsigned int addr);
    void update(unsigned int tick) override;

private:
	//Joypad
	unsigned short joyp = 0xff;
	//Keypress state
	unsigned short key_state = 0xff;

};