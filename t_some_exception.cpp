#include <string>
#include <iostream>

// 例外を表すクラス
class some_exception
{
private:
    const char* msg;   // 例外を説明するメッセージ
public:
    some_exception(const char* msg) : msg(msg) { }  // コンストラクタ
    const char* what() { return msg; }  // メッセージを返す
};

int main(){
	std::string s1 = "testtest";

	try{
		if(s1.empty() == 0){
			throw some_exception("Erorr@romname arguement");
		} 
	}
	catch (some_exception e) {
		std::cerr << "some_exception: " << e.what() << std::endl;
	}
	catch (...)  // その他の例外をキャッチ
    {
        std::cerr << "unknown exeption" << std::endl;
    }

    return 0;
}