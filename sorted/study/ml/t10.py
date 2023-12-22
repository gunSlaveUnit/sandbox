from datetime import datetime

import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import Lasso
from sklearn.linear_model import Ridge
from sklearn.pipeline import make_pipeline
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import validation_curve, GridSearchCV
from sklearn.base import BaseEstimator, TransformerMixin


# 1. (448) Разработать пользовательский класс преобразователя данных в Гауссовы базисные функции
class GaussianFeatures(BaseEstimator, TransformerMixin):
    def __init__(self, N: int, width_factor: float = 2.0):
        self.N = N
        self.width_factor = width_factor

    @staticmethod
    def _gauss_basis(x, y, width, axis=None):
        """Вычисление значения базисной функции Гаусса"""

        # (x - y) - сдвиг пика функции (колокола) от нуля
        # / width - влияет на ширину (размах) колокола
        arg = (x - y) / width

        # расчет по формуле
        return np.exp(-0.5 * np.sum(arg ** 2, axis))

    def fit(self, X, y=None):
        """Обучение"""

        # Расчет равномерно распределенных центров
        self.centers_ = np.linspace(X.min(), X.max(), self.N)

        # Ширина базисных функций как фактор * расстояние между двумя центрами
        self.width_ = self.width_factor * (self.centers_[1] - self.centers_[0])

        return self

    def transform(self, X):
        """Строим базисные функции Гаусса для входных данных"""

        return self._gauss_basis(
            X[:, :, np.newaxis],
            self.centers_,
            self.width_, axis=1
        )


# 2. (448) Генерировать исходный набор данных в виде зашумленной равномерным шумом синусоиды.
seed = int(datetime.now().timestamp())
rng = np.random.RandomState(seed)
x = 10 * rng.rand(50)
y = np.sin(x) + 0.1 * rng.randn(50)

true_x = 0.1 * np.arange(100)
true_y = np.sin(true_x)

plt.scatter(x, y)
plt.plot(true_x, true_y)
plt.show()


# 3. (448) Построить модель линейной регрессии с использованием базисных функций Гаусса.

def GaussModel(N: int):
    return make_pipeline(
        GaussianFeatures(N),
        LinearRegression()
    )


N = 20
gauss_model = GaussModel(N)

gauss_model.fit(x[:, np.newaxis], y)

x_fit = np.linspace(0, 10, 1000)
y_fit = gauss_model.predict(x_fit[:, np.newaxis])

plt.plot(true_x, true_y, label='sin')
plt.scatter(x, y, label='samples')
plt.plot(x_fit, y_fit, label=f'Gauss')
plt.xlim(0, 10)
plt.legend(loc='best')
plt.show()

# 4. (417) Исследовать качество  модели регрессии от количества используемых базисных функций Гаусса:
# a. построить кривую проверки;
Ns = np.arange(2, 30)
train_score, val_score = validation_curve(gauss_model, x[:, np.newaxis], y, param_name='gaussianfeatures__N',
                                          param_range=Ns, cv=7)
plt.plot(Ns, np.median(train_score, 1), color='blue', label='training score')
plt.plot(Ns, np.median(val_score, 1), color='red', label='validation score')
plt.ylim(0, 1)
plt.xlabel('N')
plt.ylabel('score')
plt.legend(loc='best')
plt.show()

# b. определить оптимальное количество базисных функций.
N_opt = Ns[np.median(val_score, 1).argmax()]
print(N_opt)

# 5. (449) Построить графики аппроксимации Гауссовыми базисными функциями при оптимальном количестве базисных функций.
gauss_model = GaussModel(N_opt)
gauss_model.fit(x[:, np.newaxis], y)

x_fit = np.linspace(0, 10, 1000)
y_fit = gauss_model.predict(x_fit[:, np.newaxis])

plt.plot(true_x, true_y, label='sin')
plt.scatter(x, y, label='samples')
plt.plot(x_fit, y_fit, label=f'Gauss')
plt.xlim(0, 10)
plt.legend(loc='best')
plt.show()


# 6. (450, 5.48) Построить графики аппроксимации Гауссовыми базисными функциями и коэффициентов при Гауссовых базисных функциях в чрезмерно сложной модели (при количестве базисных функций равном 30).
def basis_plot(model, title=None):
    fig, ax = plt.subplots(2, sharex=True)
    model.fit(x[:, np.newaxis])
    ax[0].plot(true_x, true_y, label='real')
    ax[0].scatter(x, y, label='samples')
    ax[0].plot(x_fit, model.predict(x_fit[:, np.newaxis]), label=f'fit')
    ax[0].legend(loc='best')
    ax[0].set(xlabel='x', ylabel='y', ylim=(-1.5, 1.5))
    if title:
        ax[0].set_title(title)

    ax[1].plot(model.steps[0][1].centers_, model.steps[1][1].coef_)
    ax[1].set(
        xlabel='basis location',
        ylabel='coefficient',
        xlim=(0, 10)
    )
    plt.show()


basis_plot(GaussModel(30))

# 7. (425, 451, 452) Добавить в регрессионную модель п.6 регуляризацию:
# a. гребневую регуляризацию (L2-регуляризация);
ridge_model = make_pipeline(GaussianFeatures(30), Ridge(alpha=0.1))


# b. лассо-регуляризацию (L1-регуляризация);
lasso_model = make_pipeline(GaussianFeatures(30), Lasso(alpha=0.001))


# c. определить оптимальное значение свободного параметра  α (гиперпараметра)
# посредством автоматического поиска по сетке;
def grid_a_opt(model, param_grid):
    grid = GridSearchCV(
        model,
        param_grid=param_grid,
        n_jobs=-1,
        cv=7
    )

    grid.fit(x[:, np.newaxis], y)

    return grid

lasso_model.steps[1][1].max_iter = 2 << 31 - 1
ridge_grid = grid_a_opt(ridge_model, param_grid={"ridge__alpha": np.linspace(0.0001, 1.0, 1000)})
lasso_grid = grid_a_opt(lasso_model, param_grid={"lasso__alpha": np.linspace(0.0001, 1.0, 1000)})
print(f'{ridge_grid.best_params_["ridge__alpha"]:.20f}')
print(f'{lasso_grid.best_params_["lasso__alpha"]:.20f}')

# d. построить графики аппроксимации Гауссовыми базисными функциями с регуляризацией и коэффициентов при Гауссовых базисных функциях.
basis_plot(ridge_model, title='Ridge Regression')
basis_plot(lasso_model, title='Lasso Regression')
basis_plot(ridge_grid.best_estimator_, title='Ridge Regression with optimization')
basis_plot(lasso_grid.best_estimator_, title='Lasso Regression with optimization')
