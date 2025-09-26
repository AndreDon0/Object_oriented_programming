#include <cassert>
#include "lab2.h"

void run_tests() {
    // Тест 1: пустой вектор
    Vector v1;
    assert(v1.Size() == 0);
    assert(v1.OwnsMemory());

    // Тест 2: вектор с размером
    Vector v2(5);
    assert(v2.Size() == 5);
    for (int i = 0; i < 5; i++) {
        assert(v2.GetElement(i) == 0);
    }

    // Тест 3: вектор с размером и значением
    Vector v3(4, 7);
    for (int i = 0; i < 4; i++) {
        assert(v3.GetElement(i) == 7);
    }

    // Тест 4: внешний буфер
    double data[3] = {1, 2, 3};
    Vector v4(data, 3);
    assert(!v4.OwnsMemory());
    assert(v4.GetElement(1) == 2);

    // Тест 5: скалярное произведение
    Vector v5(3, 2); // [2,2,2]
    Vector v6(3, 3); // [3,3,3]
    assert(v5.Dot(v6) == 18); // 2*3+2*3+2*3

    // Тест 6: Link и Unlink
    double buf[2] = {10, 20};
    v5.Link(buf, 2);
    assert(!v5.OwnsMemory());
    assert(v5.GetElement(0) == 10);

    v5.Unlink();
    assert(v5.OwnsMemory());
    assert(v5.Size() == 2);
    assert(v5.GetElement(1) == 20);
}
