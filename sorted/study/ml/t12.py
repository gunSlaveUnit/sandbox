from abc import abstractmethod, ABC

import numpy as np
from sklearn import svm
from matplotlib import pyplot as plt
from sklearn.datasets import make_blobs
from sklearn.datasets import make_circles
from sklearn.inspection import DecisionBoundaryDisplay


def plt_svc(model, title):
    ax = plt.gca()

    plt.title(title)

    plt.scatter(
        X[:, 0],
        X[:, 1],
        c=y,
        s=40,
        cmap='tab20b'
    )

    plt.scatter(
        model.support_vectors_[:, 0],
        model.support_vectors_[:, 1],
        s=120,
        facecolors='none',
        edgecolors='k'
    )

    DecisionBoundaryDisplay.from_estimator(
        model,
        X,
        plot_method="contour",
        colors="k",
        levels=[-1, 0, 1],
        alpha=0.5,
        linestyles=["--", "-", "--"],
        ax=plt.gca(),
    )

    plt.show()


# 1. Сгенерировать линейно разделимый набор данных blobs с помощью генератора из библиотеки sklearn.datasets:
X, y = make_blobs(n_samples=50, centers=2, cluster_std=0.60)

# 2. Построить модель классификации линейным методом опорных векторов.

model = svm.SVC(kernel='linear')

# 3. Обучить классификатор на основе линейного метода опорных векторов.

model.fit(X, y)

# 4. Распечатать:
# - координаты опорных векторов;
print("Standard SVC with linear kernel")
print("Support vectors")
print(model.support_vectors_)

# - значения решающей функции в опорных точках (расстояние до разделяющей гиперплоскости);
print("Distances")
print(model.decision_function(model.support_vectors_))

# - значения категориальной функции в опорных точках (соответствующие метки)
print("Marks")
print(model.predict(model.support_vectors_))

# 5. Показать на рисунке разделяющую линию, границы отступов (штриховыми линиями) и опорные векторы (окружностями).
plt_svc(model, "Standard SVC with linear kernel")

# 6. Сгенерировать линейно неразделимый набор данных circles с помощью генератора из библиотеки sklearn.datasets:

X, y = make_circles(100, factor=.1, noise=.1)

# 7. Построить модель классификации методом опорных векторов с использованием ядра rbf

model = svm.SVC(kernel='rbf', C=1e6)

# 8. Обучить классификатор на основе метода опорных векторов с rbf ядром.

model.fit(X, y)

# 9. Распечатать:
# - координаты опорных векторов
print("Standard SVC with RBF kernel")
print("Support vectors")
print(model.support_vectors_)

# - значения решающей функции в опорных точках
print("Distances")
print(model.decision_function(model.support_vectors_))

# - значения категориальной функции в опорных точках
print("Marks")
print(model.predict(model.support_vectors_))

# 10. Показать на рисунке:
# - исходные данные (разноцветными закрашенными окружностями);
# - разделяющую линию;
# - границы отступов (штриховыми линиями);
# - опорные векторы (окружностями).

plt_svc(model, "Standard SVC with RBF kernel")


# 11. Разработать пользовательский класс классификатора методом опорных векторов для
# двумерных данных, выполнив переход к трехмерному спрямляющему пространству за счет
# использования


class SVC3(svm.SVC, ABC):
    def __init__(self, C=1.0):
        def _gram_matrix(x1, x2):
            gram_matrix = np.zeros((x1.shape[0], x2.shape[0]))
            for i, x1_i in enumerate(x1):
                for j, x2_i in enumerate(x2):
                    gram_matrix[i, j] = self.kernel_(x1_i, x2_i)
            return gram_matrix

        super().__init__(kernel=_gram_matrix, C=C)

    def fit(self, X, y, **kwargs):
        super().fit(X, y, **kwargs)
        self.support_vectors_ = X[self.support_]

    @abstractmethod
    def kernel_(self, x1, x2):
        raise NotImplemented


# a) радиальной базисной функции


class RBFSVC3(SVC3):
    def __init(self, C=1.0):
        super().__init__(C)

    def kernel_(self, x1, x2):
        """Радиальная базисная функция Гаусса"""

        r = np.sqrt((x1 - x2) ** 2)
        E = 1.0
        return np.exp(-E * np.dot(r, r))


# b) квадратичного ядра


class QuadraticSVC3(SVC3):
    def __init(self, C=1.0):
        super().__init__(C)

    def kernel_(self, x1, x2):
        """ Полиномиальное ядро степени 2"""

        return (np.dot(x1, x2)) ** 2


# 12. Обучить обе модели классификатора на линейно неразделимом наборе данных

model_rbf = RBFSVC3(C=10)
model_rbf.fit(X, y)

model_quad = QuadraticSVC3(C=10)
model_quad.fit(X, y)

# 13 Показать на рисунке исходные данные, разделяющую линию, границы отступов и опорные векторы

plt_svc(model_rbf, "SVC with custom RBF kernel")

plt_svc(model_quad, "SVC with custom quadratic kernel")
