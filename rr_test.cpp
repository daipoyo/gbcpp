#include <iostream>
#include <fstream>     // ifstream
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>

int main()
{
    std::string name = "test";
    size_t size = std::filesystem::file_size(name);

    std::ifstream ifs(name, std::ios::binary);
    if (!ifs) return 1;

    std::vector<char> a(size);
    if (!ifs.read(&a[0], size)) return 2;

    std::cout << std::hex << std::setfill('0');
    size_t i = 0;
    while (i < size) {
        std::cout << ' ' << std::setw(2) << (a[i] & 0xff);
        if ((++i & 15) == 0) std::cout << std::endl;
    }
    if (i & 15) std::cout << std::endl;
}