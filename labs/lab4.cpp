#include<iostream>

using std::cout;

namespace lab4 {
    int run();
}

class Five;
class Four;
class Three;
class Two;

class One : public Two
{
public:
    One() : five_ptr(this), three_ref(four)
    {
        cout << "[WORD] ";
    }
    ~One()
    {
        cout << "[WORD] ";
    }
private:
    Four four;
    Five* five_ptr;
    Three& three_ref;
};


int lab4::run() {
    One one;
    return 0;
}

int main() {
    return lab4::run();
}
