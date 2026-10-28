
#include <iostream>

using namespace std;

// Класс точки на плоскости.
class Point {
private:
    // Приватные поля доступны только внутри класса.
    int x;
    int y;

public:
    // Конструктор без параметров.
    Point() : x(0), y(0) {
        cout << "Конструктор без параметров\n";
    }

    // Конструктор с параметрами.
    Point(int xValue, int yValue) : x(xValue), y(yValue) {
        cout << "Конструктор с параметрами\n";
    }

    // Конструктор копирования.
    Point(const Point& other) : x(other.x), y(other.y) {
        cout << "Конструктор копирования\n";
    }

    // Деструктор вызывается при уничтожении объекта.
    ~Point() {
        cout << "Деструктор точки (" << x << ", " << y << ")\n";
    }

    // Метод выводит координаты точки.
    void show() const {
        cout << "Координаты: (" << x << ", " << y << ")\n";
    }

    // Метод изменяет координаты.
    void move(int dx, int dy) {
        x += dx;
        y += dy;
        cout << "Точка перемещена\n";
    }
};

int main() {
    cout << "=== Создание обычных объектов ===\n";

    // Объект создаётся с конструктором без параметров.
    Point first;
    first.show();

    // Объект создаётся с конструктором с параметрами.
    Point second(3, 5);
    second.show();

    // Создаётся копия другого объекта.
    Point third(second);
    third.show();

    cout << "\n=== Вызов метода ===\n";
    second.move(2, 4);
    second.show();

    cout << "\n=== Динамический объект ===\n";

    // new создаёт объект в динамической памяти.
    Point* dynamicPoint = new Point(10, 20);
    dynamicPoint->show();

    // delete уничтожает динамический объект.
    delete dynamicPoint;
    dynamicPoint = nullptr;

    cout << "\n=== Завершение программы ===\n";

    return 0;
}
