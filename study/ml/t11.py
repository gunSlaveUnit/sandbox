import math
import pprint

import mglearn
import numpy as np
from matplotlib import pyplot as plt
from sklearn.preprocessing import normalize
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import train_test_split
from sklearn.base import ClassifierMixin, BaseEstimator

# 1. Скачать набор данных forge из библиотеки sklearn.datasets.

X, y = mglearn.datasets.make_forge()

# 2. Построить модель классификации методом k  ближайших соседей.

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=0)

clf = KNeighborsClassifier(n_neighbors=3)

clf.fit(X_train, y_train)

print(f"Прогнозы на тестовом наборе: {clf.predict(X_test)}")
print(f"Правильность на тестовом наборе: {clf.score(X_test, y_test):.2f}")

# 3. Построить границы разбивающие плоскость на две области: о
# бласть, где алгоритм присваивает объекту класс 0,
# и область, где алгоритм присваивает объекту класс 1

fig, axes = plt.subplots(1, 3, figsize=(10, 3))
for width, ax in zip([1, 3, 9], axes):
    clf = KNeighborsClassifier(n_neighbors=width).fit(X, y)

    mglearn.plots.plot_2d_separator(clf, X, fill=True, eps=0.5, ax=ax, alpha=.4)
    mglearn.discrete_scatter(X[:, 0], X[:, 1], y, ax=ax)

    ax.set_title(f"Количество соседей: {width}")
    ax.set_xlabel("Признак 0")
    ax.set_ylabel("Признак 1")

    axes[0].legend(loc=3)

plt.show()

# 4. Исследовать зависимость качества  модели классификации от количества используемых ближайших соседей:
# a. построить кривую проверки;

training_accuracy = []
test_accuracy = []
neighbors_settings = range(1, 20)

for n_neighbors in neighbors_settings:
    clf = KNeighborsClassifier(n_neighbors=n_neighbors)
    clf.fit(X_train, y_train)
    training_accuracy.append(clf.score(X_train, y_train))
    test_accuracy.append(clf.score(X_test, y_test))

plt.plot(neighbors_settings, training_accuracy, label="Оценка на обучающем наборе")
plt.plot(neighbors_settings, test_accuracy, label="Оценка на тестовом наборе")
plt.ylabel("Оценка")
plt.xlabel("Количество соседей")
plt.legend(loc='best')
plt.show()

# b. определить оптимальное количество ближайших соседей.

N_opt = neighbors_settings[np.argmax(test_accuracy)]
print(f"Лучшее количество соседей: {N_opt}")

# 5. Построить границу принятия решений (см. пункт 3) для оптимального числа ближайших соседей.

fig, ax = plt.subplots()
clf = KNeighborsClassifier(n_neighbors=N_opt).fit(X, y)

mglearn.plots.plot_2d_separator(clf, X, fill=True, eps=0.5, ax=ax, alpha=.4)
mglearn.discrete_scatter(X[:, 0], X[:, 1], y, ax=ax)

ax.set_title(f"Количество соседей: {N_opt}")
ax.set_xlabel("Признак 0")
ax.set_ylabel("Признак 1")

ax.legend(loc=3)

plt.show()


# 6. Разработать пользовательский класс классификатора методом окна Парзена в стиле библиотеки Scikit-Learn.
# Программный код пользовательского класса дополнить исчерпывающими комментариями.


class ParzenWindowClassifier(BaseEstimator, ClassifierMixin):
    """Метод классификации методом окна Парзена"""

    MU = 0.0
    SIGMA = 1.0

    def __init__(self, h: float | int = 1.0):
        """
        Классификатор методом окна Парзена
        :param h: параметр сглаживания, ширина окна
        """
        if not isinstance(h, int | float) or h < 0.0:
            raise TypeError("ОШИБКА: h должна быть положительным "
                            "целым числом или числом с плавающей точкой.")

        self.h = h

    @staticmethod
    def dist(x1, x2):
        """
        Евклидово расстояние между двумя объектами
        :param x1: первый объект
        :param x2: второй объект
        :return: matrix or vector norm
        """

        return np.linalg.norm(x1 - x2)

    @staticmethod
    def k(
            x: int | float,
            mu: int | float = 0.0,
            sigma: int | float = 1.0
    ) -> float:
        """
        Нормальная функция распределения плотности,
        взяли из-за "удобных математических свойств".

        Вид: f(x) = 1 / (sigma * sqrt(2 * PI)) * exp(-1/2 * pow((x - mu) / sigma, 2)

        :param x: аргумент.
        :param mu: среднее или математическое ожидание распределения (а также его медиана и мода).
        :param sigma: стандартное отклонение
        :return: f(x)
        """

        if not isinstance(x, int | float):
            raise TypeError("ОШИБКА: x должен быть целым числом или числом с плавающей точкой.")

        if not isinstance(mu, int | float):
            raise TypeError("ОШИБКА: mu должен быть целым числом или числом с плавающей точкой.")

        if not isinstance(sigma, int | float):
            raise TypeError("ОШИБКА: sigma должен быть целым числом или числом с плавающей точкой.")

        double_pi = 2 * math.pi
        return 1 / (sigma * math.sqrt(double_pi)) * math.exp(-0.5 * pow((x - mu) / sigma, 2))

    def density_estimation(self, x, x_class):
        return np.sum([
            ParzenWindowClassifier.k(
                ParzenWindowClassifier.dist(x, x_i) / self.h
            ) for x_i, y in self.samples_with_features if x_class == y
        ])

    def fit(self, X, y):
        self.classes_ = list(range(len(np.unique(y))))
        self.samples_with_features = list(zip(X, y))

        return self

    def predict(self, X):
        return np.array([
            np.argmax([self.density_estimation(x, y) for y in self.classes_]) for x in X
        ])

    def predict_proba(self, X):
        return normalize([[self.density_estimation(x, y) for y in self.classes_] for x in X])


# 7. Построить модель классификации методом окна Парзена.

clf = ParzenWindowClassifier()

clf.fit(X_train, y_train)
print(f"Прогнозы на тестовом наборе: {clf.predict(X_test)}")
print(f"Правильность на тестовом наборе: {clf.score(X_test, y_test):.2f}")

# 8. Исследовать зависимость качества  модели классификации от размера парзеновского окна:
# a. построить кривую проверки;

training_accuracy = []
test_accuracy = []
widths = np.linspace(0.01, 5.0, 100)

for width in widths:
    clf = ParzenWindowClassifier(width)
    clf.fit(X_train, y_train)
    training_accuracy.append(clf.score(X_train, y_train))
    test_accuracy.append(clf.score(X_test, y_test))

plt.plot(widths, training_accuracy, label="Оценка на обучающем наборе")
plt.plot(widths, test_accuracy, label="Оценка на тестовом наборе")
plt.ylabel("Оценка")
plt.xlabel("Ширина окна")
plt.legend(loc='best')
plt.show()

# b. определить оптимальный размер окна Парзена.

width_opt = widths[np.argmax(test_accuracy)]
print(width_opt)

# 9. Построить границу принятия решений (см. пункт 3) для оптимального размера парзеновского окна.

fig, ax = plt.subplots()
clf = ParzenWindowClassifier(h=width_opt).fit(X, y)

mglearn.plots.plot_2d_separator(clf, X, fill=True, eps=0.5, ax=ax, alpha=.4)
mglearn.discrete_scatter(X[:, 0], X[:, 1], y, ax=ax)

ax.set_title(f"Ширина окна: {width_opt:0.3f}")
ax.set_xlabel("Признак 0")
ax.set_ylabel("Признак 1")

ax.legend(loc=3)

plt.show()
