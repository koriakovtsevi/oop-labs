#include <iostream>

using namespace std;

// Класс точки на плоскости.
class Point {
private:
    int x;
    int y;

public:
    Point() : x(0), y(0) {
        cout << "Конструктор без параметров\n";
    }

    Point(int xValue, int yValue) : x(xValue), y(yValue) {
        cout << "Конструктор с параметрами\n";
    }

    Point(const Point& other) : x(other.x), y(other.y) {
        cout << "Конструктор копирования\n";
    }

    ~Point() {
        cout << "Деструктор точки (" << x << ", " << y << ")\n";
    }

    void show() const {
        cout << "Координаты: (" << x << ", " << y << ")\n";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
        cout << "Точка перемещена\n";
    }
};

// Базовый класс геометрической фигуры.
class Shape {
private:
    int id;  // private: доступно только внутри Shape.

protected:
    string name;  // protected: доступно также классам-наследникам.

public:
	virtual string classname() const {
	    return "Shape";
	}

	virtual bool isA(const string& className) const {
 	   return className == "Shape";
	}

    // Конструктор без параметров.
    Shape() : id(0), name("Фигура") {
        cout << "Shape: конструктор без параметров\n";
    }

    // Конструктор с параметрами.
    Shape(int idValue, const string& nameValue)
        : id(idValue), name(nameValue) {
        cout << "Shape: конструктор с параметрами\n";
    }

    // Конструктор копирования.
    Shape(const Shape& other) : id(other.id), name(other.name) {
        cout << "Shape: конструктор копирования\n";
    }

    // Обычный, пока не виртуальный, метод.
    void showInfo() const {
        cout << "Фигура: " << name << ", номер: " << id << '\n';
    }

	// Обычный метод — выбор зависит от типа указателя.
	void identify() const {
   	 cout << "Обычный метод класса Shape\n";
	}

	// Виртуальный метод — позволяет вызвать версию наследника.
	virtual void describe() const {
  	  cout << "Виртуальный метод класса Shape\n";
	}

    virtual ~Shape() {
        cout << "Shape: деструктор (" << name << ")\n";
    }
};

// Класс Circle наследуется от Shape.
class Circle : public Shape {
private:
    double radius;

public:
	string classname() const override {
  	  return "Circle";
	}

	bool isA(const string& className) const override {
   	 return className == "Circle" || Shape::isA(className);
	}

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
        // name доступно, потому что оно объявлено как protected.
        cout << "Это " << name << ", радиус: " << radius << '\n';
    }
	// Метод с таким же именем, но без virtual в базовом классе.
	void identify() const {
  	  cout << "Обычный метод класса Circle\n";
	}

	// Переопределяем виртуальный метод базового класса.
	void describe() const override {
 	   cout << "Виртуальный метод класса Circle\n";
	}
    ~Circle() {
        cout << "Circle: деструктор\n";
    }
};

// Класс содержит объект Point напрямую.
class PointHolder {
private:
    Point point;

public:
    PointHolder() : point(1, 2) {
        cout << "PointHolder: конструктор\n";
    }

    void show() const {
        cout << "Точка внутри PointHolder:\n";
        point.show();
    }

    ~PointHolder() {
        cout << "PointHolder: деструктор\n";
    }
};

// Класс содержит указатель на отдельный объект Point.
class DynamicPointHolder {
private:
    Point* point;

public:
    DynamicPointHolder() : point(new Point(5, 6)) {
        cout << "DynamicPointHolder: конструктор\n";
    }

    void show() const {
        cout << "Точка внутри DynamicPointHolder:\n";
        point->show();
    }

    ~DynamicPointHolder() {
        cout << "DynamicPointHolder: деструктор\n";
        delete point;
        point = nullptr;
    }
};

int main() {
    cout << "=== 1. Создание объектов Point ===\n";
    Point first;
    first.show();

    Point second(3, 5);
    second.show();

    Point third(second);
    third.show();

    cout << "\n=== 2. Наследование: создание круга ===\n";
    Circle circle(4.5);
    circle.showInfo();
    circle.showCircle();

    cout << "\n=== 3. Конструктор копирования круга ===\n";
    Circle copiedCircle(circle);
    copiedCircle.showInfo();
    copiedCircle.showCircle();

    cout << "\n=== 4. Указатель на базовый класс ===\n";
    Circle anotherCircle(7.0);
    Shape* shapePointer = &anotherCircle;

	cout << "\n--- Вызовы через Shape* ---\n";
	shapePointer->identify();
	shapePointer->describe();

	cout << "\n--- Вызовы через Circle* ---\n";
	Circle* circlePointer = &anotherCircle;
	circlePointer->identify();
	circlePointer->describe();
	cout << "\n=== Проверка типа объекта ===\n";

	cout << "Класс объекта: "
    	 << shapePointer->classname() << '\n';

	cout << "Это Circle? "
     	<< (shapePointer->isA("Circle") ? "Да" : "Нет") << '\n';

	cout << "Это Shape? "
   	  << (shapePointer->isA("Shape") ? "Да" : "Нет") << '\n';

	Circle* checkedCircle = dynamic_cast<Circle*>(shapePointer);

	if (checkedCircle != nullptr) {
   	 cout << "dynamic_cast успешен\n";
   	 checkedCircle->showCircle();
	} else {
 	   cout << "Объект не является Circle\n";
	}

	cout << "\n=== Проверка виртуального деструктора ===\n";

	Shape* dynamicShape = new Circle(7.0);
	delete dynamicShape;

	cout << "\n=== Композиция: обычный объект ===\n";
	PointHolder holder;
	holder.show();

	cout << "\n=== Композиция: указатель ===\n";
	DynamicPointHolder dynamicHolder;
	dynamicHolder.show();

    cout << "\n=== Конец main ===\n";
    return 0;
}
