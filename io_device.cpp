//template<class T, class U>
class io_device{
//private:
public:
	virtual void write(unsigned int addr, unsigned short val) const = 0;
	virtual unsigned int read(unsigned int addr, unsigned short val) const = 0;
	virtual void update(unsigned int tick) const = 0;
};