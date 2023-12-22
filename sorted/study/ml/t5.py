"""
Supervised learning: handwritten digit analysis
"""

import matplotlib
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from sklearn.manifold import Isomap
from sklearn.metrics import confusion_matrix
from sklearn.metrics import accuracy_score
from sklearn.naive_bayes import GaussianNB

digits = load_digits()

fig, axes = plt.subplots(
    10,
    10,
    figsize=(8, 8),
    subplot_kw={'xticks': [], 'yticks': []},
    gridspec_kw=dict(hspace=0.1, wspace=0.1)
)

for i, ax in enumerate(axes.flat):
    ax.imshow(digits.images[i], cmap='binary', interpolation='nearest')
    ax.text(0.05, 0.05, str(digits.target[i]), transform=ax.transAxes, color='green')
plt.show()

X = digits.data
y = digits.target

iso = Isomap(n_components=2, n_neighbors=10)
iso.fit(digits.data)
data_projected = iso.transform(digits.data)

plt.scatter(data_projected[:, 0], data_projected[:, 1],
            c=digits.target, edgecolor='none', alpha=0.5,
            cmap=matplotlib.colormaps['Accent'])
plt.colorbar(label='digit label', ticks=range(10))
plt.clim(-0.5, 9.5)
plt.show()

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=0)

model = GaussianNB()
model.fit(X_train, y_train)
y_model = model.predict(X_test)

print(accuracy_score(y_test, y_model))

mat = confusion_matrix(y_test, y_model)
sns.heatmap(mat, square=True, annot=True, cbar=True)
plt.xlabel('predicted value')
plt.ylabel('true value')
plt.show()

fig, axes = plt.subplots(10, 10, figsize=(8, 8),
                         subplot_kw={'xticks': [], 'yticks': []},
                         gridspec_kw=dict(hspace=0.1, wspace=0.1))

test_images = X_test.reshape(-1, 8, 8)

for i, ax in enumerate(axes.flat):
    ax.imshow(test_images[i], cmap='binary', interpolation='nearest')
    ax.text(0.05, 0.05, str(y_model[i]),
            transform=ax.transAxes,
            color='green' if (y_test[i] == y_model[i]) else 'red')

plt.show()
