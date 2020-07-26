#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//using namespace std;

class read_rom{
public:
	int rom_data;
	void fetch(std::string filename);
};

void read_rom::fetch(std::string filename){
	std::ifstream ifs(filename);

	if (!ifs){
        std::cout << "Error! File can not be opened" << std::endl;
    }else{
    	std::string buf;
    	std::string buf_line;
		std::vector<int> a;
    	int n = 0;
    	/*while (std::getline(ifs, buf, ' ')){

			std::cout << std::hex << std::showbase << buf << std::endl;
			// std::cout << std::hex << std::showbase << buf << std::endl;
    		int buf_num = std::stoi(buf, nullptr, 16);
    		int up = buf_num >> 8 & 0xff;
			a.push_back(up);
    		int down = buf_num & 0xff;
			a.push_back(down);
		}*/
		while (std::getline(ifs, buf)){

			std::cout << std::hex << std::showbase << buf << std::endl;
			std::stringstream ss{buf};
    		while (std::getline(ss, buf_line, ' ')){
	    		int buf_num = std::stoi(buf_line, nullptr, 16);
	    		int up = buf_num >> 8 & 0xff;
				a.push_back(up);
	    		int down = buf_num & 0xff;
				a.push_back(down);
    		}
		}
		std::cout << std::hex << std::showbase << a[16] << std::endl;
		// printf("num : %d", a[100]);

    	/*while (ifs >> std::hex >> n)
		a.push_back(n);
		std::cout << std::hex << std::showbase << a[100] << std::endl;*/
		// for debug
		/*for (int i = 0; i < a.size(); ++i)
			std::cout << "a[" << std::dec << i << "] == "
				<< std::hex << std::showbase << a[i] << std::endl;*/
    }


}


/*int main()
{
    std::ifstream ifs("test");
    if (!ifs) {
        std::cerr << "error" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    char c;
    while (ifs.get(c)) {
        std::cout.put(c);
    }
}*/

int main(){
	read_rom rom;
	std::string filename = "test";
	rom.fetch(filename);
}
