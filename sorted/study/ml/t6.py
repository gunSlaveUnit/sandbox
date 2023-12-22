"""
Curves
"""

import numpy as np
import matplotlib.pyplot as plt
import seaborn
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.pipeline import make_pipeline
from sklearn.model_selection import validation_curve, learning_curve, GridSearchCV


def polynomial_regression(polynomial_degree=2, **kwargs):
    return make_pipeline(PolynomialFeatures(polynomial_degree),
                         LinearRegression(**kwargs))


def make_data(d0, err=1.0, seed=1):
    rng = np.random.RandomState(seed)
    x = rng.rand(d0, 1) ** 2
    y_l = 10 - 1. / (x.ravel() + 0.1)
    if err > 0:
        y_l += err * rng.randn(d0)
    return x, y_l


seaborn.set()

X, y = make_data(40)

X_test = np.linspace(-0.1, 1.1, 500)[:, None]
plt.scatter(X.ravel(), y, color='black')
axis = plt.axis()
for degree in [1, 3, 5]:
    model = polynomial_regression(degree)
    model.fit(X, y)
    y_test = model.predict(X_test)
    plt.plot(X_test.ravel(), y_test, label='degree={0}'.format(degree))
plt.xlim(-0.1, 1.0)
plt.ylim(-2, 12)
plt.legend(loc='best')
plt.show()

degree = np.arange(0, 21)
train_score, val_score = validation_curve(
    polynomial_regression(),
    X=X,
    y=y,
    param_name='polynomialfeatures__degree',
    param_range=degree,
    cv=7
)
plt.plot(degree, np.median(train_score, 1), color='blue', label='training score')
plt.plot(degree, np.median(val_score, 1), color='red', label='validation score')
plt.legend(loc='best')
plt.ylim(0, 1)
plt.xlabel('degree')
plt.ylabel('score')

plt.show()

plt.scatter(X.ravel(), y)
lim = plt.axis()
model = polynomial_regression(3)
model.fit(X, y)
y_test = model.predict(X_test)
plt.plot(X_test.ravel(), y_test)
plt.axis(lim)

plt.show()

X2, y2 = make_data(200)
plt.scatter(X2.ravel(), y2)

plt.show()

degree = np.arange(21)
train_score2, val_score2 = validation_curve(
    polynomial_regression(),
    X=X2,
    y=y2,
    param_name='polynomialfeatures__degree',
    param_range=degree,
    cv=7
)
plt.plot(degree, np.median(train_score2, 1), color='blue', label='training score')
plt.plot(degree, np.median(val_score2, 1), color='red', label='validation score')
plt.plot(degree, np.median(train_score, 1), color='blue', alpha=0.3, linestyle='dashed')
plt.plot(degree, np.median(val_score, 1), color='red', alpha=0.3, linestyle='dashed')
plt.legend(loc='lower center')
plt.ylim(0, 1)
plt.xlabel('degree')
plt.ylabel('score')

plt.show()

fig, ax = plt.subplots(1, 2, figsize=(16, 6))
fig.subplots_adjust(left=0.0625, right=0.95, wspace=0.1)
for i, degree in enumerate([2, 9]):
    N, train_lc, val_lc = learning_curve(
        polynomial_regression(degree),
        X,
        y,
        cv=7,
        train_sizes=np.linspace(0.3, 1, 25),
    )
    ax[i].plot(N, np.mean(train_lc, 1), color='blue', label='training score')
    ax[i].plot(N, np.mean(val_lc, 1), color='red', label='validation score')
    ax[i].hlines(
        np.mean(
            [train_lc[-1],
             val_lc[-1]]
        ),
        N[0],
        N[-1],
        color='gray',
        linestyle='dashed',
    )
    ax[i].set_ylim(0, 1)
    ax[i].set_xlim(N[0], N[-1])
    ax[i].set_xlabel('training size')
    ax[i].set_ylabel('score')
    ax[i].set_title('degree = {0}'.format(degree), size=14)
    ax[i].legend(loc='best')

plt.show()

param_grid = {
    'polynomialfeatures__degree': np.arange(21),
    'linearregression__fit_intercept': [True, False],
    'linearregression__copy_X': [True, False],
}
grid = GridSearchCV(polynomial_regression(), param_grid, cv=7)
grid.fit(X, y)

model = grid.best_estimator_
plt.scatter(X.ravel(), y)
lim = plt.axis()
y_test = model.fit(X).predict(X_test)
plt.plot(X_test.ravel(), y_test)
plt.axis(lim)

plt.show()
