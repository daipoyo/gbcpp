#include <vector>
#include <string>

class cartridge{
public:
	struct Kartridge{
		std::vector<unsigned short> rom;
		std::vector<unsigned int> ram;
		unsigned short mbc_type;
		bool ram_enable;
		unsigned short bank_no_upper;
		unsigned short bank_no_lower;
		unsigned short num_rom_banks;
		bool mode;
	};

	struct Kartridge K_ridge;

	std::vector<unsigned short> rom;
	struct Kartridge Nnew(std::string fname);
	void read_save_file(std::string fname);
	void write_save_file(std::string fname);



private:
	unsigned short rom_bank_no();
	unsigned short ram_bank_no();

};

