#include <iostream>
#include <vector>

using namespace std;

class MyClass
{
public:
  MyClass() : MyClass(0) {}

  MyClass(size_t size) : size(size), data(new int[size] {}), index(0)
  {
//    this->size = size;
//    this->data = new int[this->size] {};
    cout << "Вызвался конструктор: " << this << endl;
  }

  MyClass(const MyClass& other)
  {
    this->size = other.size;
    this->data = new int[this->size];
    for (size_t i = 0; i < this->size; i++)
      this->data[i] = other.data[i];
    cout << "Вызвался конструктор копирования: " << this << endl;
  }

  MyClass(MyClass&& other)
  {
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
    cout << "Вызвался конструктор перемещения: " << this << endl;
  }

  ~MyClass()
  {
    delete[] this->data;
    cout << "Вызвался деструктор: " << this << endl;
  }

  MyClass& operator=(const MyClass& other)
  {
    delete[] this->data;
    this->size = other.size;
    this->data = new int[this->size];
    for (size_t i = 0; i < this->size; i++)
      this->data[i] = other.data[i];
    cout << "Вызвался конструктор присваивания копирования: " << this << endl;
    return *this;
  }

  MyClass& operator=(MyClass&& other)
  {
    delete[] this->data;
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
    cout << "Вызвался конструктор присваивания перемещения: " << this << endl;
    return *this;
  }

  void Print() const // <- Can be called in const MyClass
  {
    cout << "[ ";
    for (size_t i = 0; i < this->size; i++)
      cout << this->data[i] << " ";
    cout << "]" << endl;
  }

private:
  size_t size;
  int* data;
  mutable size_t index; // <- Can be changed in const MyClass
};

MyClass CreateObject(bool flag)
{
  MyClass first_object(3);
  MyClass second_object(4);
  if (flag)
    return first_object;
  else
    return second_object;
}

class Number
{
public:
  Number(int value) : value(value) {}

  Number operator+(const Number& other) const
  {
    return Number(this->value + other.value);
  }

  Number& operator+() 
  {
    return *this;
  }

  Number& operator++() 
  {
    ++this->value;
    return *this;
  }

  Number operator++(int) 
  {
    Number copy(*this);
    this->value++;
    return copy;
  }

  friend Number operator+(const Number& first, const Number& second);

private:
    int value;
};

Number operator+(const Number& first, const Number& second)
{
  return Number(first.value + second.value);
}

int main()
{
  setlocale(LC_ALL, "Russian");

  // MyClass object_1(MyClass(5));

  // MyClass object_1 = CreateObject(true);

  MyClass&& ref = CreateObject(true);
  MyClass object_1(ref);

  object_1.Print();

  const MyClass object_2(5);
  object_2.Print();

  MyClass object_3(5);
  MyClass object_4(5);
  MyClass object_5 = object_3;
  // object_3 = object_4; <- Error
  object_3 = object_4;
  object_4 = MyClass(5);

  Number number_1(4);
  Number number_2(3);
  number_1 = number_2;
  auto number_3 = number_1 + number_2;
  +number_3;
  number_3 = 2 + number_1;
}