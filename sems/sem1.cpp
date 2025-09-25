#include<iostream>
#include<string>

#define PI 3.1415;
constexpr double pi = 3.1415;

using namespace std;

class Human
{
public:
    void Print()
    {
        cout << "Point: " << this << endl;
    }
    void Info()
    {
        PrintAge();
        PrintName();
    }
    int GetAge()
    {
        return this->age;
    }
    string GetName()
    {
        return this->name;
    }
    void SetAge(int age)
    {
        this->age = age;
    }
    void SetName(string name)
    {
        this->name = name;
    }
protected:

private:
    int age = 0;
    string name = u8"Noname";
    void PrintAge()
    {
        cout << u"Age: " << this->age << endl;
    }

    void PrintName()
    {
        cout << U"Hame: " << this->name << endl;
    }
};

class Circle
{
public:
    int GetRadius()
    {
        return radius;
    }
    int GetArea()
    {
        return area;
    }
    void SetRadius(double value_radius)
    {
        radius = value_radius;
        area = pi * radius * radius;
    }
    void Print()
    {
        cout << U"Radius: " << radius << U"\tArea: " << area << endl;
    }
private:
    double radius = 0;
    double area = 0;
};

int main()
{
    Human first_human;
    first_human.SetAge(10);

    first_human.Info();

    int first_human_age = first_human.GetAge();

    first_human.Print();

    Human* second_human = new Human();
    second_human->Info();

    Circle circle;
    circle.SetRadius(2);
    circle.Print();

    return 0;
}