#include "io_device.cpp"
#include "cartridge_h.cpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct cartridge::Kartridge cartridge::Nnew(std::string fname){

	std::ifstream ifs(fname);

	if (!ifs){
        std::cout << "Error! File can not be opened" << std::endl;
    }else{
    	std::string buf;
    	std::string buf_line;
		// std::vector<int> a;
		while (std::getline(ifs, buf)){
			// std::cout << std::hex << std::showbase << buf << std::endl;
			std::stringstream ss{buf};
    		while (std::getline(ss, buf_line, ' ')){
	    		int buf_num = std::stoi(buf_line, nullptr, 16);
	    		unsigned short up = buf_num >> 8 & 0xff;
				rom.push_back(up);
	    		unsigned short down = buf_num & 0xff;
				rom.push_back(down);
    		}
		}
		std::cout << std::hex << std::showbase << rom[16] << std::endl;
    }

    unsigned short rom_index = rom[0x0148];
    unsigned int rom_size = 0;
    switch(rom_index){
    	case 0:
    		rom_size = 32 * 1024;
    		break;
    	case 1 ... 5:
    		rom_size = 32 * 1024 << rom_index;
    		break;
    }

    K_ridge.num_rom_banks = 2 << rom[0x0148];

    unsigned short ram_index = rom[0x0149];
    unsigned int ram_size = 0;
    switch(ram_index){
    	case 0:
    		ram_size = 0;
    		break;
    	case 1:
    		ram_size = 2 * 1024;
    		break;
    	case 2:
    		ram_size = 8 * 1024;
    		break;
    	case 3:
    		ram_size = 32 * 1024;
    		break;
    	case 4:
    		ram_size = 128 * 1024;
    		break;
    	case 5:
    		ram_size = 64 * 1024;
    		break;
    	case 6:
    		printf("RAM size invalid");
    		break;
    }

    K_ridge.mbc_type = rom[0x0147];

    std::string mbc_name;

    switch(K_ridge.mbc_type){
    	case 0x00:
    		mbc_name = "ROM ONLY";
    		break;
    	case 0x01:
    		mbc_name = "MBC1";
    		break;
    	case 0x02:
    		mbc_name = "MBC1+RAM";
    		break;
    	case 0x03:
    		mbc_name = "MBC1+RAM+BATTERY";
    		break;
        case 0x05:
            mbc_name = "MBC2";
            break;
        case 0x06:
            mbc_name = "MBC2+BATTERY";
            break;
        case 0x08:
            mbc_name = "ROM+RAM";
            break;
        case 0x09:
            mbc_name = "ROM+RAM+BATTERY";
            break;
        case 0x0b:
            mbc_name = "MMM01";
            break;
        case 0x0c:
            mbc_name = "MMM01+RAM";
            break;
        case 0x0d:
            mbc_name = "MMM01+RAM+BATTERY";
            break;
        case 0x0f:
            mbc_name = "MBC3+TIMER+BATTERY";
            break;
        case 0x10:
            mbc_name = "MBC3+TIMER+RAM+BATTERY";
            break;
        case 0x11:
            mbc_name = "MBC3";
            break;
        case 0x12:
            mbc_name = "MBC3+RAM";
            break;
        case 0x13:
            mbc_name = "MBC3+RAM+BATTERY";
            break;
        case 0x19:
            mbc_name = "MBC5";
            break;
        case 0x1a:
            mbc_name = "MBC5+RAM";
            break;
        case 0x1b:
            mbc_name = "MBC5+RAM+BATTERY";
            break;
        case 0x1c:
            mbc_name = "MBC5+RUMBLE";
            break;
        case 0x1d:
            mbc_name = "MBC5+RUMBLE+RAM";
            break;
        case 0x1e:
            mbc_name = "MBC5+RUNMBLE+RAM+BATTERY";
            break;
        case 0x20:
            mbc_name = "MBC6";
            break;
        case 0x22:
            mbc_name = "MBC7+SENSOR+RUBMBLE+RAM+BATTERY";
            break;
        case 0xfc:
            mbc_name = "POCKET CAMERA";
            break;
        case 0xfd:
            mbc_name = "BANDAI TAMA5";
            break;
        case 0xfe:
            mbc_name = "HuC3";
            break;
        case 0xff:
            mbc_name = "HuC1+RAM+BATTERY";
            break;
        default:
            printf("Unknown cartridge type");
            break;
    }

    unsigned short chksum = 0;

    for(int i = 0x0134; i < 0x014E; i = i + 1){
        if((chksum - rom[i]) < 0){
            chksum = chksum - rom[i] + 0xFF;
            if((chksum - 1) < 0){
                chksum = chksum - 1 + 0xFF;
            }else{
                chksum = chksum - 1;
            }
        }else{
            chksum = chksum - rom[i];
            if((chksum - 1) < 0){
                chksum = chksum - 1 + 0xFF;
            }else{
                chksum = chksum - 1;
            }
        }
    }

    if(rom_size != rom.size()){
        printf("ROM file invalid!");
    }

    if(chksum != rom[0x014D]){
        printf("ROM header checksum is incorrect");
    }

    printf("ROM size %dKB", rom_size/1024);
    printf("RAM size %dKB", ram_size/1024);
    printf("MBC type %s", mbc_name.c_str());

    std::vector<unsigned int> vec(ram_size, 0);
    //input Cartridge info 
    K_ridge.rom = rom;
    K_ridge.ram = vec;
    K_ridge.ram_enable = false;
    K_ridge.bank_no_upper = 0;
    K_ridge.bank_no_lower = 0;
    K_ridge.mode = false;

    return K_ridge;
}


unsigned short cartridge::rom_bank_no(){
    unsigned short bank_no = 0;

    if(K_ridge.mode){
        bank_no = K_ridge.bank_no_lower;
    }else{
        bank_no = (K_ridge.bank_no_upper << 5) | K_ridge.bank_no_lower;
    }

    switch(bank_no){
        case 0:
        case 0x20:
        case 0x40:
        case 0x60:
            bank_no = bank_no + 1;
            break;
        default:
            break;
    }

    return bank_no & (K_ridge.num_rom_banks -1);
}

unsigned short cartridge::ram_bank_no(){
    unsigned short temp = 0;

    if(K_ridge.mode){
        temp = K_ridge.bank_no_upper;
    }else{
        temp = 0;
    }
    return temp;
}

void cartridge::read_save_file(std::string fname){
    printf("Reading save file from: %s\n", fname.c_str());
    std::ifstream ifs(fname, std::hex);

    if(ifs){
        K_ridge.ram.clear();
        printf("%d\n", K_ridge.ram.size());
        std::string buf;
        std::string buf_line;
        // std::vector<int> a;
        while (std::getline(ifs, buf)){
            //std::cout << std::hex << std::showbase << buf << std::endl;
            //std::cout << buf << std::endl;
            std::stringstream ss{buf};
            while (std::getline(ss, buf_line, ' ')){
                //std::cout << std::hex << buf_line << std::endl;
                try{
                    int buf_num = std::stoi(buf_line, nullptr, 16);
                    unsigned short up = buf_num >> 8 & 0xff;
                    K_ridge.ram.push_back(up);
                    unsigned short down = buf_num & 0xff;
                    K_ridge.ram.push_back(down);
                    printf("OK");
                }
                catch(const std::invalid_argument& e){
                    std::cout << "invalid argument" << std::endl;
                }
                catch(const std::out_of_range& e){
                    std::cout << "Out of range" <<std::endl;
                }
            }
        }

/*        unsigned int d;
        while(ifs.eof()){
            ifs.read((char *) &d, sizeof(int));
            std::cout << d << std::endl;
        }*/
    }

    //Debug
    printf("%d\n", K_ridge.ram.size());
    for(unsigned int i = 0; i < K_ridge.ram.size(); i = i + 1){
        printf("%d", K_ridge.ram[i]);
    }
    //END Debug
}

void cartridge::write_save_file(std::string fname){
    printf("Writing save file to: %s\n", fname.c_str());
    std::ofstream ofs(fname);
    std::ofstream outputFile(fname);

    if(ofs){
        for(unsigned int i = 0; i < K_ridge.ram.size(); i = i + 1){
            outputFile << K_ridge.ram[i];
        }
    }
    outputFile.close();
}


int main(){
    cartridge cartridge;

    cartridge.read_save_file("test");
    cartridge.write_save_file("test2");
}