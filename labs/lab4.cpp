#include<iostream>

using std::cout;


class Three
{
public:
    Three()
    {
        cout << "Three ";
    }
    ~Three()
    {
        cout << "Eight ";
    }
};

class Four : public Three
{
public:
    Four()
    {
        cout << "Four ";
    }
    ~Four()
    {
        cout << "Seven ";
    }
};

class Five
{
public:
    Five()
    {
        cout << "One ";
    }
    ~Five()
    {
        cout << "Ten ";
    }
};

class Two : public Five
{
public:
    Two()
    {
        cout << "Two ";
    }
    ~Two()
    {
        cout << "Nine ";
    }
};

class One : public Two
{
public:
    One() : five_ptr(this), three_ref(four)
    {
        cout << "Five ";
    }
    ~One()
    {
        cout << "Six ";
    }
private:
    Four four;
    Five* five_ptr;
    Three& three_ref;
};


int main() {
    One one;
}
