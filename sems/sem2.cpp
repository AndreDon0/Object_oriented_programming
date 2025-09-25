#include<iostream>
#include<string>

using namespace std;

class Point {
public:
    Point() { SetX(0); SetY(0); }
    Point (const int xy) { SetX(xy); SetY(xy); }
    Point (const int x, const int y) { SetX(x); SetY(y); }

    void SetX(const int x) { this->x = x; }
    void SetY(const int y) { this->y = y; }
    int GetX() const {return this->x; }
    int GetY() const {return this->y; }

    void Print() const { cout << "x: " << this->x << ", y: " <<  this->y << endl; }

private:
    int x;
    int y;
};

class MyClass
{
public:
    MyClass() 
    { 
        this->array = new int[5] {};
        cout << "Constractor called: " << this << endl; 
    }
    ~MyClass() 
    { 
        delete[] this->array;
        cout << "Destractor called: " << this << endl; 
    }
private:
    int* array;
};

int Sum(int x, int y) { return x + y; }
double Sum(double x, double y) { return x + y; }

int main()
{ 
    Point point(0, 1); // Point point{0, 1};
    // point.SetX(0);
    // point.SetY(1);
    point.Print();

    // int a = 4;
    // double b = 4;
    // Sum(a, b);

    MyClass myclass_1;
    // myclass_1.~MyClass;
    MyClass myclass_2;

}