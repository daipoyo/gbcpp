#pragma once
class io_device{
//private:
public:
	virtual void write(unsigned int addr, unsigned short val) = 0;
	virtual unsigned int read(unsigned int addr, unsigned short val) = 0;
	virtual void update(unsigned int tick) = 0;
};