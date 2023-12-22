import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.linear_model import LinearRegression

from numpy import nan
from sklearn.preprocessing import PolynomialFeatures

X = np.array([
    [nan, 0, 3],
    [3, 7, 9],
    [3, 5, 2],
    [4, nan, 6],
    [8, 8, 1],
])
y = np.array([14, 16, -1, 8, -5])

imputer = SimpleImputer(strategy='mean')
X = imputer.fit_transform(X)

polynomial = PolynomialFeatures(degree=2)
X = polynomial.fit_transform(X)

model = LinearRegression()
model.fit(X, y)

print(y)
print(model.predict(X))
