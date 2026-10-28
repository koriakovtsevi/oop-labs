#include <iostream>
#include <string>

using namespace std;

// ============================================================
// Point: поля, методы и разные конструкторы.
// ============================================================
class Point {
private:
    int x;
    int y;

public:
    Point() : x(0), y(0) {
        cout << "Point: конструктор без параметров\n";
    }

    Point(int xValue, int yValue) : x(xValue), y(yValue) {
        cout << "Point: конструктор с параметрами\n";
    }

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
// Shape и Circle: наследование, доступность, виртуальные методы.
// ============================================================
class Shape {
private:
    int id;

protected:
    string name;

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

    void identify() const {
        cout << "Обычный метод класса Shape\n";
    }

    virtual void describe() const {
        cout << "Виртуальный метод класса Shape\n";
    }

    virtual string classname() const {
        return "Shape";
    }

    virtual bool isA(const string& className) const {
        return className == "Shape";
    }

    virtual ~Shape() {
        cout << "Shape: деструктор (" << name << ")\n";
    }
};

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
// Композиция объектов.
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
    }
};

class PointHolderPointer {
private:
    Point* point;

public:
    PointHolderPointer() : point(new Point(5, 6)) {
        cout << "PointHolderPointer: конструктор\n";
    }

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

// ============================================================
// Base и Desc: конструкторы без параметров, с указателем и ссылкой.
// ============================================================
class Base {
protected:
    int value;

public:
    Base() : value(1) {
        cout << "Base(): конструктор без параметров\n";
    }

    explicit Base(int v) : value(v) {
        cout << "Base(int): конструктор с параметром\n";
    }

    Base(Base* other) : value(other ? other->value : 0) {
        cout << "Base(Base*): конструктор с указателем\n";
    }

    Base(Base& other) : value(other.value) {
        cout << "Base(Base&): конструктор с ссылкой\n";
    }

    virtual ~Base() {
        cout << "~Base(): деструктор, value=" << value << '\n';
    }

    virtual void show() const {
        cout << "Base::show(), value=" << value << '\n';
    }
};

class Desc : public Base {
public:
    Desc() : Base() {
        value = 2;
        cout << "Desc(): конструктор без параметров\n";
    }

    explicit Desc(int v) : Base(v) {
        cout << "Desc(int): конструктор с параметром\n";
    }

    Desc(Desc* other) : Base(static_cast<Base*>(other)) {
        cout << "Desc(Desc*): конструктор с указателем\n";
    }

    Desc(Desc& other) : Base(static_cast<Base&>(other)) {
        cout << "Desc(Desc&): конструктор с ссылкой\n";
    }

    ~Desc() override {
        cout << "~Desc(): деструктор\n";
    }

    void show() const override {
        cout << "Desc::show(), value=" << value << '\n';
    }
};

// Отдельные функции передачи объектов: значение, указатель, ссылка.
void passByValue(Base obj) {
    cout << "Внутри passByValue: ";
    obj.show();
}

void passByPointer(Base* obj) {
    cout << "Внутри passByPointer: ";
    if (obj != nullptr) {
        obj->show();
    }
}

void passByReference(Base& obj) {
    cout << "Внутри passByReference: ";
    obj.show();
}

// Функции 1–3 возвращают локальный статический объект по значению,
// указателю и ссылке соответственно.
Base returnStaticByValue() {
    static Base obj(11);
    cout << "returnStaticByValue: возвращаем копию статического объекта\n";
    return obj;
}

Base* returnStaticByPointer() {
    static Base obj(12);
    cout << "returnStaticByPointer: возвращаем адрес статического объекта\n";
    return &obj;
}

Base& returnStaticByReference() {
    static Base obj(13);
    cout << "returnStaticByReference: возвращаем ссылку на статический объект\n";
    return obj;
}

// Функции 4–6 используют динамическое создание через new.
Base returnDynamicByValue() {
    Base* obj = new Base(21);
    cout << "returnDynamicByValue: копируем объект в возвращаемое значение\n";
    Base result(*obj);
    delete obj; // Удаляем временный динамический объект, утечки нет.
    return result;
}

Base* returnDynamicByPointer() {
    cout << "returnDynamicByPointer: возвращаем динамический указатель\n";
    return new Base(22); // Получатель обязан выполнить delete.
}

Base& returnDynamicByReference() {
    cout << "returnDynamicByReference: возвращаем ссылку на динамический объект\n";
    return *(new Base(23)); // Получатель обязан удалить адрес &result.
}

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

    cout << "\n=== 3. Конструктор копирования Circle ===\n";
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
    cout << "Это Circle? " << (shapePointer->isA("Circle") ? "Да" : "Нет") << '\n';
    cout << "Это Shape? " << (shapePointer->isA("Shape") ? "Да" : "Нет") << '\n';
    Circle* checkedCircle = dynamic_cast<Circle*>(shapePointer);
    if (checkedCircle != nullptr) {
        cout << "dynamic_cast успешен\n";
        checkedCircle->showCircle();
    }

    cout << "\n=== 6. Виртуальный деструктор ===\n";
    Shape* dynamicShape = new Circle(7.0);
    dynamicShape->showInfo();
    dynamicShape->describe();
    delete dynamicShape;

    cout << "\n=== 7. Композиция: объект ===\n";
    {
        PointHolder holder;
        holder.show();
    }

    cout << "\n=== 8. Композиция: указатель ===\n";
    {
        PointHolderPointer holderPointer;
        holderPointer.show();
    }

    cout << "\n=== 9. Передача Base и Desc в функции ===\n";
    Base baseObject;
    Desc descObject(30);

    cout << "-- Base по значению: объект копируется --\n";
    passByValue(baseObject);
    passByValue(descObject); // Срезка: параметр Base содержит только базовую часть.

    cout << "-- По указателю: копирования нет --\n";
    passByPointer(&baseObject);
    passByPointer(&descObject); // Виртуальный show() вызовет Desc::show().

    cout << "-- По ссылке: копирования нет --\n";
    passByReference(baseObject);
    passByReference(descObject); // Виртуальный show() вызовет Desc::show().

    cout << "\n=== 10. Возврат статических объектов ===\n";
    Base value1 = returnStaticByValue();
    value1.show();

    Base* pointer2 = returnStaticByPointer();
    pointer2->show(); // Не удаляем: объект статический.

    Base& reference3 = returnStaticByReference();
    reference3.show(); // Не удаляем: объект статический.

    cout << "\n=== 11. Возврат динамических объектов ===\n";
    Base value4 = returnDynamicByValue();
    value4.show();

    Base* pointer5 = returnDynamicByPointer();
    pointer5->show();
    delete pointer5;

    Base& reference6 = returnDynamicByReference();
    reference6.show();
    delete &reference6; // Объект создан через new, поэтому удаляем его.

    cout << "\n=== Конец main ===\n";
    return 0;
}
