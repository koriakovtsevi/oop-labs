#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================
// Базовый класс: жизненный цикл, виртуальные методы и проверка типа.
// ============================================================
class Base {
protected:
    int value;

public:
    Base() : value(0) {
        cout << "Base(): конструктор без параметров\n";
    }

    explicit Base(int v) : value(v) {
        cout << "Base(int): конструктор с параметром\n";
    }

    // Конструктор с параметром-указателем.
    Base(Base* other) : value(other ? other->value : -1) {
        cout << "Base(Base*): конструктор с указателем\n";
    }

    // Конструктор с параметром-ссылкой.
    Base(Base& other) : value(other.value) {
        cout << "Base(Base&): конструктор с ссылкой\n";
    }

    // Конструктор копирования нужен для примеров возврата по значению.
    Base(const Base& other) : value(other.value) {
        cout << "Base(const Base&): конструктор копирования\n";
    }

    void nonVirtualMethod2() const {
        cout << "Base::nonVirtualMethod2()\n";
    }

    virtual void virtualMethod2() const {
        cout << "Base::virtualMethod2()\n";
    }

    void method1() const {
        cout << "Base::method1() вызывает невиртуальный метод:\n";
        nonVirtualMethod2();
    }

    void methodWithVirtualCall() const {
        cout << "Base::methodWithVirtualCall() вызывает виртуальный метод:\n";
        virtualMethod2();
    }

    virtual string classname() const {
        return "Base";
    }

    virtual bool isA(const string& className) const {
        return className == "Base";
    }

    virtual void show() const {
        cout << "Объект Base, value=" << value << '\n';
    }

    virtual ~Base() {
        cout << "~Base(): деструктор, value=" << value << '\n';
    }
};

// ============================================================
// Производный класс Desc.
// ============================================================
class Desc : public Base {
public:
    Desc() : Base() {
        value = 100;
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

    Desc(const Desc& other) : Base(static_cast<const Base&>(other)) {
        cout << "Desc(const Desc&): конструктор копирования\n";
    }

    void nonVirtualMethod2() const {
        cout << "Desc::nonVirtualMethod2()\n";
    }

    void virtualMethod2() const override {
        cout << "Desc::virtualMethod2()\n";
    }

    string classname() const override {
        return "Desc";
    }

    bool isA(const string& className) const override {
        return className == "Desc" || Base::isA(className);
    }

    void show() const override {
        cout << "Объект Desc, value=" << value << '\n';
    }

    ~Desc() override {
        cout << "~Desc(): деструктор\n";
    }
};

// ============================================================
// Три отдельные функции передачи объектов.
// ============================================================
void func1(Base obj) {
    cout << "func1(Base obj): объект передан по значению: ";
    obj.show();
}

void func2(Base* obj) {
    cout << "func2(Base* obj): объект передан по указателю: ";
    if (obj) {
        obj->show();
    }
}

void func3(Base& obj) {
    cout << "func3(Base& obj): объект передан по ссылке: ";
    obj.show();
}

// ============================================================
// Возврат статических (локальных static) объектов: функции 1–3.
// ============================================================
Base func4() {
    static Base obj(41);
    cout << "func4: возвращается копия static Base по значению\n";
    return obj;
}

Base* func5() {
    static Base obj(42);
    cout << "func5: возвращается указатель на static Base\n";
    return &obj;
}

Base& func6() {
    static Base obj(43);
    cout << "func6: возвращается ссылка на static Base\n";
    return obj;
}

// ============================================================
// Возврат объектов, созданных динамически через new: функции 4–6.
// ============================================================
Base func7() {
    Base* obj = new Base(51);
    cout << "func7: объект создан через new, копируется в результат\n";
    Base result(*obj);
    delete obj;
    return result;
}

Base* func8() {
    cout << "func8: возвращается указатель на объект, созданный через new\n";
    return new Base(52);
}

Base& func9() {
    cout << "func9: возвращается ссылка на объект, созданный через new\n";
    return *(new Base(53));
}

void demonstrateSmartPointers() {
    cout << "\n=== unique_ptr ===\n";
    {
        unique_ptr<Base> first = make_unique<Base>(61);
        first->show();

        // unique_ptr нельзя копировать, но можно передать владение через move.
        unique_ptr<Base> second = move(first);
        if (!first) {
            cout << "first больше не владеет объектом\n";
        }
        second->show();
        cout << "При выходе из блока unique_ptr автоматически удалит объект\n";
    }

    cout << "\n=== shared_ptr ===\n";
    {
        shared_ptr<Base> first = make_shared<Base>(62);
        cout << "Счётчик владельцев: " << first.use_count() << '\n';
        {
            shared_ptr<Base> second = first;
            cout << "После копирования shared_ptr: "
                 << first.use_count() << " владельца(ев)\n";
            second->show();
        }
        cout << "После выхода второго владельца: "
             << first.use_count() << " владелец\n";
        cout << "Объект будет удалён, когда исчезнет последний shared_ptr\n";
    }
}

int main() {
    cout << "=== 1. Конструкторы и наследование ===\n";
    Base baseObject;
    Base baseWithValue(10);
    Base baseFromPointer(&baseWithValue);
    Base baseFromReference(baseWithValue);

    Desc descObject;
    Desc descWithValue(20);
    Desc descFromPointer(&descWithValue);
    Desc descFromReference(descWithValue);

    cout << "\n=== 2. Невиртуальный и виртуальный вызовы ===\n";
    Base* basePointer = &descObject;
    cout << "Вызов невиртуального метода через Base*:\n";
    basePointer->nonVirtualMethod2();
    cout << "Вызов виртуального метода через Base*:\n";
    basePointer->virtualMethod2();

    cout << "Вызов method1 у Desc: внутри вызывается Base-версия "
            "невиртуального метода:\n";
    descObject.method1();
    cout << "Вызов метода с виртуальным вызовом у Desc:\n";
    descObject.methodWithVirtualCall();

    cout << "\n=== 3. classname, isA и приведение типов ===\n";
    cout << "Фактический тип: " << basePointer->classname() << '\n';
    cout << "isA(Desc): " << (basePointer->isA("Desc") ? "true" : "false") << '\n';
    cout << "isA(Base): " << (basePointer->isA("Base") ? "true" : "false") << '\n';

    // Опасный пример показан только в безопасном виде:
    // сначала проверяем isA, затем выполняем static_cast.
    if (basePointer->isA("Desc")) {
        Desc* checkedManual = static_cast<Desc*>(basePointer);
        cout << "Ручное приведение после проверки isA: ";
        checkedManual->show();
    }

    Desc* checkedDynamic = dynamic_cast<Desc*>(basePointer);
    if (checkedDynamic) {
        cout << "dynamic_cast успешен: ";
        checkedDynamic->show();
    }

    Base ordinaryBase(70);
    Desc* failedCast = dynamic_cast<Desc*>(&ordinaryBase);
    cout << "dynamic_cast для обычного Base: "
         << (failedCast ? "успешен" : "nullptr, приведение невозможно") << '\n';

    cout << "\n=== 4. Передача объектов в функции ===\n";
    func1(baseObject);
    func1(descObject); // Передача Base по значению вызывает срезку.
    func2(&baseObject);
    func2(&descObject);
    func3(baseObject);
    func3(descObject);

    cout << "\n=== 5. Возврат static-объектов ===\n";
    Base returnedValue = func4();
    returnedValue.show();

    Base* returnedPointer = func5();
    returnedPointer->show(); // static-объект не удаляем вручную.

    Base& returnedReference = func6();
    returnedReference.show(); // static-объект не удаляем вручную.

    cout << "\n=== 6. Возврат динамических объектов ===\n";
    Base returnedDynamicValue = func7();
    returnedDynamicValue.show();

    Base* returnedDynamicPointer = func8();
    returnedDynamicPointer->show();
    delete returnedDynamicPointer;

    Base& returnedDynamicReference = func9();
    returnedDynamicReference.show();
    delete &returnedDynamicReference;

    demonstrateSmartPointers();

    cout << "\n=== Конец main; локальные объекты уничтожаются в обратном порядке ===\n";
    return 0;
}
