#include <iostream>
#include <string>

using namespace std;

class Pet
{
public:
    int variable;
    Pet() 
    {
        cout << "Pet constructor" << endl;
    }
    Pet(string name) : name(name)
    {
        cout << "Pet 1 constructor" << endl;
    }
    Pet(int age) : age(age)
    {
        cout << "Pet 2 constructor" << endl;
    }
    ~Pet()
    {
        cout << "Pet destructor" << endl;
    }

protected:
    string name;
    int age;
    double weight;
private:

};

class Cat : public Pet
{
public:
    void Print()
    {
        cout << this->name << endl;
    }
    Cat()
    {
        cout << "Cat constructor" << endl;
    }
    ~Cat()
    {
        cout << "Cat destructor" << endl;
    }
private:
    using Pet::variable;
    using Pet::Pet;
    int lives = 9;
};

// class StrayCat : public Cat
// {
// public:
//     StrayCat()
//     {
//         cout << "StrayCat constructor" << endl;
//     }
//     ~StrayCat()
//     {
//         cout << "StrayCat destructor" << endl;
//     }
// };

// class Dog : public Pet
// {
// public:
//     using Pet::Pet;
//     bool human_friend = true;
// };

int main()
{
    // Pet pet;
    // Cat cat;

    // StrayCat stray_cat;

    // Dog dog("Rex");

    // StrayCat stray_cat;

    Cat cat;
    cat.Print();
}