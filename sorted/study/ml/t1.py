"""
Supervised learning: linear regression
"""

import random

import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

rng = np.random.RandomState(random.randrange(50))

# Data preparing
x = 10 * rng.rand(50)
y = 2 * x - 1 + rng.randn(50)

# Learning model
model = LinearRegression(fit_intercept=True)
X = x[:, np.newaxis]
model.fit(X, y)

# Prediction
x_fit = np.linspace(-1, 11)
x_fit = x_fit[:, np.newaxis]
y_fit = model.predict(x_fit)

# Drawing
plt.scatter(X, y)
plt.plot(x_fit, y_fit)
plt.show()
