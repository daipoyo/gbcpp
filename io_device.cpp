template<typename T>
class io_device{
public:
	void write(int addr, int val);
	int read(int addr, int val);
	void update(int tick);
};