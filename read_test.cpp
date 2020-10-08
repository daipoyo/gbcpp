#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int main(){
  std::ifstream ifs("test", std::ios::binary);
  std::vector<char> result(
    std::istreambuf_iterator<char>(ifs),
    std::istreambuf_iterator<char>()
  );
  for(int i = 0; i < 100; i++){
    std::cout << result[i] << "\n" << std::endl;
  }
}