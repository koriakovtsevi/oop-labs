"""
Лабораторная работа № 3, часть 2 — MVC.
Python + tkinter.

Модель хранит A, B, C, обеспечивает 0 <= A <= B <= C <= 100
и сохраняет значения в mvc_values.json рядом с программой.
Представление содержит Entry, Spinbox (аналог NumericUpDown) и Scale
(ползунок) для каждого числа.
"""

import json
from pathlib import Path
import tkinter as tk
from tkinter import ttk, messagebox


SETTINGS_FILE = Path(__file__).with_name("mvc_values.json")


class NumbersModel:
    """Модель: данные, правила, уведомление и сохранение."""

    def __init__(self, file_path=SETTINGS_FILE):
        self.file_path = Path(file_path)
        self.a = 20
        self.b = 50
        self.c = 80
        self._observers = []
        self.load(notify=False)

    def subscribe(self, callback):
        """Подписывает представление на изменения модели."""
        if callback not in self._observers:
            self._observers.append(callback)

    def _notify(self):
        """Одно уведомление после завершённого изменения модели."""
        for callback in list(self._observers):
            callback()

    @staticmethod
    def _limit(value):
        return max(0, min(100, int(value)))

    def set_a(self, value):
        """Разрешающее изменение A: при необходимости сдвигает B и C."""
        value = self._limit(value)
        new_a = value
        new_b = max(self.b, new_a)
        new_c = max(self.c, new_b)
        self._set_all(new_a, new_b, new_c)

    def set_b(self, value):
        """Ограничивающее изменение B: удерживает значение между A и C."""
        value = self._limit(value)
        value = max(self.a, min(self.c, value))
        self._set_all(self.a, value, self.c)

    def set_c(self, value):
        """Разрешающее изменение C: при необходимости сдвигает A и B."""
        value = self._limit(value)
        new_c = value
        new_b = min(self.b, new_c)
        new_a = min(self.a, new_b)
        self._set_all(new_a, new_b, new_c)

    def _set_all(self, a, b, c):
        """Атомарно заменяет все значения и уведомляет не более одного раза."""
        if (a, b, c) == (self.a, self.b, self.c):
            return
        self.a, self.b, self.c = a, b, c
        self.save()
        self._notify()

    def save(self):
        """Сохраняет состояние модели в читаемый JSON-файл."""
        data = {"A": self.a, "B": self.b, "C": self.c}
        self.file_path.write_text(
            json.dumps(data, ensure_ascii=False, indent=2),
            encoding="utf-8"
        )

    def load(self, notify=True):
        """Восстанавливает значения; при старте уведомление не требуется."""
        try:
            data = json.loads(self.file_path.read_text(encoding="utf-8"))
            a = self._limit(data.get("A", self.a))
            b = self._limit(data.get("B", self.b))
            c = self._limit(data.get("C", self.c))
            # Восстанавливаем корректный порядок даже при испорченном файле.
            b = max(a, b)
            c = max(b, c)
            if c > 100:
                c = 100
                b = min(b, c)
                a = min(a, b)
            changed = (a, b, c) != (self.a, self.b, self.c)
            self.a, self.b, self.c = a, b, c
            if notify and changed:
                self._notify()
        except (OSError, ValueError, TypeError, json.JSONDecodeError):
            # Если файла ещё нет или он повреждён, остаются значения по умолчанию.
            return


class MVCApp:
    """Представление: показывает данные и передаёт действия модели."""

    def __init__(self, root):
        self.root = root
        self.root.title("Лабораторная работа № 3 — MVC")
        self.root.geometry("760x390")
        self.root.minsize(680, 350)

        self.model = NumbersModel()
        self._updating_view = False
        self.entry_vars = {}
        self.spin_vars = {}

        self._build_view()
        self.model.subscribe(self.update_view)
        # После создания всех элементов обновляем форму один раз.
        self.update_view()
        self.root.protocol("WM_DELETE_WINDOW", self.close)

    def _build_view(self):
        outer = ttk.Frame(self.root, padding=12)
        outer.pack(fill="both", expand=True)

        ttk.Label(
            outer,
            text="Три числа: всегда выполняется 0 ≤ A ≤ B ≤ C ≤ 100",
            font=("Arial", 12, "bold")
        ).pack(anchor="w", pady=(0, 10))

        header = ttk.Frame(outer)
        header.pack(fill="x")
        ttk.Label(header, text="Число", width=12).grid(row=0, column=0, sticky="w")
        ttk.Label(header, text="Поле ввода", width=18).grid(row=0, column=1, sticky="w")
        ttk.Label(header, text="Spinbox", width=14).grid(row=0, column=2, sticky="w")
        ttk.Label(header, text="Ползунок").grid(row=0, column=3, sticky="w")

        for row, name in enumerate(("A", "B", "C"), start=1):
            ttk.Label(header, text=name, width=12).grid(
                row=row, column=0, sticky="w", pady=5
            )

            entry_var = tk.StringVar()
            self.entry_vars[name] = entry_var
            entry = ttk.Entry(header, textvariable=entry_var, width=14)
            entry.grid(row=row, column=1, sticky="w", padx=(0, 8), pady=5)
            entry.bind("<Return>", lambda event, n=name: self.commit_entry(n))
            entry.bind("<FocusOut>", lambda event, n=name: self.commit_entry(n))

            spin_var = tk.StringVar()
            self.spin_vars[name] = spin_var
            spin = ttk.Spinbox(
                header, from_=0, to=100, increment=1,
                textvariable=spin_var, width=10,
                command=lambda n=name: self.commit_spin(n)
            )
            spin.grid(row=row, column=2, sticky="w", padx=(0, 8), pady=5)
            spin.bind("<Return>", lambda event, n=name: self.commit_spin(n))
            spin.bind("<FocusOut>", lambda event, n=name: self.commit_spin(n))

            scale = ttk.Scale(
                header, from_=0, to=100, orient="horizontal",
                command=lambda value, n=name: self.change_from_scale(n, value)
            )
            scale.grid(row=row, column=3, sticky="ew", padx=5, pady=5)
            setattr(self, f"scale_{name}", scale)

        header.columnconfigure(3, weight=1)

        ttk.Label(
            outer,
            text="A и C могут менять соседние значения, чтобы сохранить порядок. "
                 "B ограничивается диапазоном от A до C.",
            wraplength=700
        ).pack(anchor="w", pady=(14, 8))

        self.status_var = tk.StringVar()
        ttk.Label(outer, textvariable=self.status_var, relief="sunken",
                  padding=6).pack(fill="x", side="bottom")

    def _get_model_value(self, name):
        return {"A": self.model.a, "B": self.model.b, "C": self.model.c}[name]

    def _set_model_value(self, name, value):
        {"A": self.model.set_a, "B": self.model.set_b, "C": self.model.set_c}[name](value)

    def commit_entry(self, name):
        if self._updating_view:
            return
        try:
            raw = self.entry_vars[name].get().strip()
            if not raw:
                raise ValueError
            value = int(raw)
        except ValueError:
            self.update_view()
            return
        self._set_model_value(name, value)

    def commit_spin(self, name):
        if self._updating_view:
            return
        try:
            raw = self.spin_vars[name].get().strip()
            if not raw:
                raise ValueError
            value = int(raw)
        except ValueError:
            self.update_view()
            return
        self._set_model_value(name, value)

    def change_from_scale(self, name, value):
        if self._updating_view:
            return
        self._set_model_value(name, round(float(value)))

    def update_view(self):
        """Синхронизирует все поля с уже полностью изменённой моделью."""
        self._updating_view = True
        try:
            values = {"A": self.model.a, "B": self.model.b, "C": self.model.c}
            for name, value in values.items():
                self.entry_vars[name].set(str(value))
                self.spin_vars[name].set(str(value))
                getattr(self, f"scale_{name}").set(value)
            self.status_var.set(f"Текущие значения: A={self.model.a}, "
                                f"B={self.model.b}, C={self.model.c}")
        finally:
            self._updating_view = False

    def close(self):
        self.model.save()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = MVCApp(root)
    root.mainloop()
