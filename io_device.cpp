template<class T, class U>
class io_device{
public:
	void write(T addr, U val);
	unsigned int read(T addr, U val);
	void update(T tick);
};