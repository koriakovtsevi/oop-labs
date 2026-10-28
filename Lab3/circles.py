"""
Лабораторная работа № 3, часть 1 — «Круги на форме».
Язык: Python, tkinter.

Возможности:
- создание кругов нажатием левой кнопки мыши по пустому месту;
- выделение круга нажатием мыши;
- множественное выделение с Ctrl;
- удаление выделенных кругов клавишей Delete;
- собственный контейнер CircleStorage;
- каждый круг сам умеет рисоваться и проверять попадание мыши.
"""

import tkinter as tk


class CCircle:
    """Графический круг с координатами, радиусом и состоянием выделения."""

    def __init__(self, x, y, radius=30):
        self.x = x
        self.y = y
        self.radius = radius
        self.selected = False

    def contains(self, x, y):
        """Проверяет, попала ли точка мыши внутрь круга."""
        return (x - self.x) ** 2 + (y - self.y) ** 2 <= self.radius ** 2

    def draw(self, canvas):
        """Рисует круг самостоятельно."""
        r = self.radius
        outline = "red" if self.selected else "navy"
        width = 3 if self.selected else 2
        canvas.create_oval(
            self.x - r, self.y - r, self.x + r, self.y + r,
            outline=outline, width=width, fill="lightblue"
        )
        canvas.create_text(self.x, self.y, text="Круг")


class CircleStorage:
    """Собственный контейнер для хранения объектов CCircle."""

    def __init__(self):
        self.__circles = []

    def add(self, circle):
        self.__circles.append(circle)

    def remove_selected(self):
        self.__circles = [circle for circle in self.__circles
                          if not circle.selected]

    def clear_selection(self):
        for circle in self.__circles:
            circle.selected = False

    def circles_at(self, x, y):
        """Возвращает круги, в которые попала мышь."""
        # Каждый круг сам проверяет, находится ли точка внутри него.
        return [circle for circle in self.__circles if circle.contains(x, y)]

    def draw_all(self, canvas):
        for circle in self.__circles:
            circle.draw(canvas)

    def count(self):
        return len(self.__circles)

    def selected_count(self):
        return sum(1 for circle in self.__circles if circle.selected)


class CirclesApp:
    """Окно приложения и обработчики событий пользователя."""

    def __init__(self, root):
        self.root = root
        self.root.title("Лабораторная работа № 3 — Круги на форме")
        self.root.geometry("800x550")
        self.root.minsize(450, 300)

        self.storage = CircleStorage()

        toolbar = tk.Frame(root, padx=8, pady=6)
        toolbar.pack(fill="x")

        tk.Label(
            toolbar,
            text="ЛКМ: создать/выбрать круг | Ctrl + ЛКМ: несколько | Delete: удалить"
        ).pack(anchor="w")

        self.status = tk.StringVar(value="Кругов: 0 | Выделено: 0")
        tk.Label(root, textvariable=self.status, anchor="w",
                 relief="sunken", padx=6).pack(side="bottom", fill="x")

        self.canvas = tk.Canvas(root, bg="white", highlightthickness=0)
        self.canvas.pack(fill="both", expand=True, padx=8, pady=8)

        self.canvas.bind("<Button-1>", self.on_click)
        self.root.bind("<Delete>", self.delete_selected)
        self.canvas.bind("<Configure>", self.on_resize)

        # Начальная перерисовка после создания окна.
        self.root.after(100, self.redraw)

    def on_click(self, event):
        """Обрабатывает нажатие мыши на рабочей области."""
        hit_circles = self.storage.circles_at(event.x, event.y)
        ctrl_pressed = bool(event.state & 0x0004)

        if hit_circles:
            if not ctrl_pressed:
                self.storage.clear_selection()

            # Если кругов несколько, выбираем верхний (последний созданный).
            hit_circles[-1].selected = True
        else:
            # Нажатие на пустое место создаёт новый круг.
            if not ctrl_pressed:
                self.storage.clear_selection()
            new_circle = CCircle(event.x, event.y)
            self.storage.add(new_circle)
            new_circle.selected = True

        self.redraw()

    def delete_selected(self, _event=None):
        """Удаляет все выделенные круги клавишей Delete."""
        self.storage.remove_selected()
        self.redraw()

    def on_resize(self, _event=None):
        """Перерисовывает объекты после изменения размера окна."""
        self.redraw()

    def redraw(self):
        """Очищает Canvas и рисует все круги из контейнера."""
        if not hasattr(self, "canvas"):
            return
        self.canvas.delete("all")
        self.storage.draw_all(self.canvas)
        self.status.set(
            f"Кругов: {self.storage.count()} | "
            f"Выделено: {self.storage.selected_count()}"
        )


if __name__ == "__main__":
    root = tk.Tk()
    app = CirclesApp(root)
    root.mainloop()
