#include <iostream>
#include <string>

using namespace std;

// ============================================================
// Класс Point: пример полей, методов и разных конструкторов.
// ============================================================
class Point {
private:
    int x;
    int y;

public:
    // Конструктор без параметров.
    Point() : x(0), y(0) {
        cout << "Point: конструктор без параметров\n";
    }

    // Конструктор с параметрами.
    Point(int xValue, int yValue) : x(xValue), y(yValue) {
        cout << "Point: конструктор с параметрами\n";
    }

    // Конструктор копирования.
    Point(const Point& other) : x(other.x), y(other.y) {
        cout << "Point: конструктор копирования\n";
    }

    ~Point() {
        cout << "Point: деструктор (" << x << ", " << y << ")\n";
    }

    void show() const {
        cout << "Координаты точки: (" << x << ", " << y << ")\n";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
        cout << "Point: точка перемещена\n";
    }
};

// ============================================================
// Базовый класс Shape: демонстрация private/protected/public,
// обычных и виртуальных методов, наследования и dynamic_cast.
// ============================================================
class Shape {
private:
    int id;  // Доступно только внутри Shape.

protected:
    string name;  // Доступно в Shape и его наследниках.

public:
    Shape() : id(0), name("Фигура") {
        cout << "Shape: конструктор без параметров\n";
    }

    Shape(int idValue, const string& nameValue)
        : id(idValue), name(nameValue) {
        cout << "Shape: конструктор с параметрами\n";
    }

    Shape(const Shape& other) : id(other.id), name(other.name) {
        cout << "Shape: конструктор копирования\n";
    }

    void showInfo() const {
        cout << "Фигура: " << name << ", номер: " << id << '\n';
    }

    // Обычный метод: при вызове через Shape* выбирается Shape::identify.
    void identify() const {
        cout << "Обычный метод класса Shape\n";
    }

    // Виртуальный метод: при вызове через Shape* выбирается метод объекта.
    virtual void describe() const {
        cout << "Виртуальный метод класса Shape\n";
    }

    virtual string classname() const {
        return "Shape";
    }

    virtual bool isA(const string& className) const {
        return className == "Shape";
    }

    // Виртуальный деструктор необходим для корректного удаления наследника
    // через указатель на базовый класс.
    virtual ~Shape() {
        cout << "Shape: деструктор (" << name << ")\n";
    }
};

// ============================================================
// Circle — производный класс. Наследует публичные методы Shape.
// ============================================================
class Circle : public Shape {
private:
    double radius;

public:
    Circle() : Shape(), radius(1.0) {
        cout << "Circle: конструктор без параметров\n";
    }

    Circle(double radiusValue)
        : Shape(2, "Круг"), radius(radiusValue) {
        cout << "Circle: конструктор с параметрами\n";
    }

    Circle(const Circle& other)
        : Shape(other), radius(other.radius) {
        cout << "Circle: конструктор копирования\n";
    }

    void showCircle() const {
        // name — protected, поэтому наследник может его использовать.
        cout << "Это " << name << ", радиус: " << radius << '\n';
    }

    void identify() const {
        cout << "Обычный метод класса Circle\n";
    }

    void describe() const override {
        cout << "Виртуальный метод класса Circle\n";
    }

    string classname() const override {
        return "Circle";
    }

    bool isA(const string& className) const override {
        return className == "Circle" || Shape::isA(className);
    }

    ~Circle() override {
        cout << "Circle: деструктор\n";
    }
};

// ============================================================
// Композиция № 1: Point хранится внутри PointHolder как объект.
// ============================================================
class PointHolder {
private:
    Point point;

public:
    PointHolder() : point(1, 2) {
        cout << "PointHolder: конструктор\n";
    }

    void show() const {
        cout << "Point внутри PointHolder: ";
        point.show();
    }

    ~PointHolder() {
        cout << "PointHolder: деструктор\n";
        // point будет уничтожен автоматически после тела деструктора.
    }
};

// ============================================================
// Композиция № 2: PointHolderPointer владеет динамическим Point.
// ============================================================
class PointHolderPointer {
private:
    Point* point;

public:
    PointHolderPointer() : point(new Point(5, 6)) {
        cout << "PointHolderPointer: конструктор\n";
    }

    // Запрещаем копирование, чтобы два владельца случайно
    // не удалили один и тот же указатель.
    PointHolderPointer(const PointHolderPointer&) = delete;
    PointHolderPointer& operator=(const PointHolderPointer&) = delete;

    void show() const {
        cout << "Point через указатель: ";
        point->show();
    }

    ~PointHolderPointer() {
        cout << "PointHolderPointer: деструктор\n";
        delete point;
        point = nullptr;
    }
};

int main() {
    cout << "=== 1. Объекты Point ===\n";
    Point first;
    first.show();

    Point second(3, 5);
    second.show();

    Point third(second);
    third.show();

    second.move(2, 4);
    second.show();

    cout << "\n=== 2. Наследование и конструкторы ===\n";
    Circle circle(4.5);
    circle.showInfo();
    circle.showCircle();

    cout << "\n=== 3. Конструктор копирования наследника ===\n";
    Circle anotherCircle(circle);
    anotherCircle.showInfo();
    anotherCircle.showCircle();

    cout << "\n=== 4. Обычные и виртуальные методы ===\n";
    Shape* shapePointer = &anotherCircle;

    cout << "Через Shape*:\n";
    shapePointer->identify();
    shapePointer->describe();

    cout << "Через Circle*:\n";
    Circle* circlePointer = &anotherCircle;
    circlePointer->identify();
    circlePointer->describe();

    cout << "\n=== 5. Проверка типа и dynamic_cast ===\n";
    cout << "Фактический класс: " << shapePointer->classname() << '\n';
    cout << "Это Circle? "
         << (shapePointer->isA("Circle") ? "Да" : "Нет") << '\n';
    cout << "Это Shape? "
         << (shapePointer->isA("Shape") ? "Да" : "Нет") << '\n';

    Circle* checkedCircle = dynamic_cast<Circle*>(shapePointer);
    if (checkedCircle != nullptr) {
        cout << "dynamic_cast успешен\n";
        checkedCircle->showCircle();
    } else {
        cout << "dynamic_cast не выполнен\n";
    }

    cout << "\n=== 6. Динамический объект через базовый указатель ===\n";
    Shape* dynamicShape = new Circle(7.0);
    dynamicShape->showInfo();
    dynamicShape->describe();
    delete dynamicShape;  // Сначала Circle, затем Shape.

    cout << "\n=== 7. Композиция: объект внутри объекта ===\n";
    {
        PointHolder holder;
        holder.show();
        cout << "Сейчас блок закончится, и holder будет уничтожен.\n";
    }

    cout << "\n=== 8. Композиция: указатель на объект ===\n";
    {
        PointHolderPointer holderPointer;
        holderPointer.show();
        cout << "Сейчас блок закончится, и динамический Point будет удалён.\n";
    }

    cout << "\n=== Конец main ===\n";
    return 0;
}
