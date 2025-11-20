#include <iostream>
#include <string>

using namespace std;

// int sum(int a, int b)
// {
//     return a + b;
// }

// double sum(double a, double b)
// {
//     return a + b;
// }

// template <typename T>
// T sum(T a, T b)
// {
//     return a + b;
// }

// template<typename T1, typename T2>
// auto sum(T1 a, T2 b)
// {
//     return a + b;
// }

// auto sum(auto a, auto b)
// {
//     return a + b;
// }

// template<>
// string sum<string>(string a, string b)
// {
//     return a + " " + b;
// }

// template<typename T>
// class A
// {
// public:
//     A() : value(T()) {}
//     A(T value) : value(value) {}
//     void Print() { cout << this->value << endl; }

// private:
//     T value;
// };

// template<typename T, typename U>
// class B : public A<U>
// {
// public:

// private:
//     T value;
// };

// class Animal
// {
// public:
//     Animal(string name) : name(name) {}
//     virtual void Voice() = 0;
// protected:
//     string name;
// };

// void Animal::Voice()
// {
//     cout << "Sound" << endl;
// }

// class Cat : public Animal
// {
// public:
//     Cat() : Animal("Cat") {}
//     void Voice() override
//     {
//         Animal::Voice();
//         cout << "Meow" << endl;
//     }
// };

// class Dog : public Animal
// {
// public:
//     Dog() : Animal("Dog") {}
//     void Voice() override
//     {
//         Animal::Voice();
//         cout << "Bark" << endl;
//     }
// };

class Animal
{
public:
    Animal() { cout << "Call animal constructor" << endl; }
    // virtual ~Animal() { cout << "Call animal destructor" << endl; }
    virtual ~Animal() = 0;
};

Animal::~Animal()
{
    cout << "Call animal destructor" << endl;
}

class Cat : public Animal
{
public:
    Cat() { cout << "Call cat constructor" << endl; }
    ~Cat() { cout << "Call cat destructor" << endl; }
};

int main()
{
    // cout << sum(1, 2) << endl;
    // cout << sum(1.5, 2.5) << endl;

    // cout << sum(1, 2.5) << endl;

    // cout << sum(string("Hello"), string("World!")) << endl;

    // A<int> a;
    // A<double> b;

    // a.Print();
    // b.Print();

    // B<int, double> c;

    // Animal* animals[2];
    // animals[0] = new Cat();
    // animals[1] = new Dog();

    // for (size_t i = 0; i < 2; i++)
    // {
    //     animals[i]->Voice();
    // }

    Animal* animal = new Cat();
    delete animal;
    
    return 0;
}