class CManiac {
public:    // クラスの外で使うなら、public または protected とする。
    enum E_HowManiac { E_ALittle, E_Very, E_Ultra, E_Super, }; // クラスの中で列挙型を定義する。
public:
    void Func() {
        E_HowManiac howmaniac = E_Super;      // クラスの中では、そのまま使用できる。
    }
};

int main() {
    CManiac::E_HowManiac howmaniac = CManiac::E_Ultra; // クラスの外では、CManiac:: で修飾する必要がある。
    return 0;
}