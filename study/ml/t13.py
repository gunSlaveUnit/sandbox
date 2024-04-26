"""Метод главных компонент"""

import matplotlib
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.datasets import load_digits

sns.set()


def draw_vector(v0, v1, ax=None):
    ax = ax or plt.gca()

    props = {
        'arrowstyle': '->',
        'linewidth': 2,
        'shrinkA': 0,
        'shrinkB': 0,
        'color': 'orange',
    }

    ax.annotate('', v1, v0, arrowprops=props)


def plot_vectors(start, directions, lengths):
    for direction, length in zip(directions, lengths):
        vertex = direction * 3 * np.sqrt(length)
        draw_vector(start, start + vertex)
    plt.axis('equal')
    plt.show()


def plot_digits(data):
    fig, axes = plt.subplots(
        4,
        10,
        figsize=(10, 4),
        subplot_kw={'xticks': [], 'yticks': []},
        gridspec_kw=dict(hspace=0.1, wspace=0.1)
    )

    for i, ax in enumerate(axes.flat):
        ax.imshow(
            data[i].reshape(8, 8),
            cmap='binary',
            interpolation='nearest',
            clim=(0, 16)
        )

    plt.show()


np.random.seed(1)
X = np.dot(np.random.random(size=(2, 2)), np.random.normal(size=(2, 200))).T
plt.scatter(X[:, 0], X[:, 1])
plt.axis('equal')
plt.show()

pca = PCA(n_components=2)
pca.fit(X)

print('Components:\n', pca.components_)
print('Explained variance:\n', pca.explained_variance_)

plt.scatter(X[:, 0], X[:, 1], alpha=0.5)
plot_vectors(pca.mean_, pca.components_, pca.explained_variance_)

X_pca = pca.transform(X) / np.sqrt(pca.explained_variance_)
components = pca.transform(pca.components_ + pca.mean_)

plt.scatter(X_pca[:, 0], X_pca[:, 1], alpha=0.5)
plot_vectors(pca.mean_, components, [1, 1])

pca = PCA(n_components=1)
pca.fit(X)
X_pca = pca.transform(X)
print("Original shape:", X.shape)
print("Transformed shape:", X_pca.shape)

X_new = pca.inverse_transform(X_pca)
plt.scatter(X[:, 0], X[:, 1], alpha=0.2)
plt.scatter(X_new[:, 0], X_new[:, 1], alpha=0.8)
plt.axis('equal')
plt.show()

digits = load_digits()

pca = PCA(2)
projected = pca.fit_transform(digits.data)

plt.scatter(
    projected[:, 0],
    projected[:, 1],
    c=digits.target,
    edgecolor='none',
    alpha=0.5,
    cmap=matplotlib.colormaps['plasma']
)
plt.xlabel('Component 1')
plt.ylabel('Component 2')
plt.colorbar()
plt.show()

pca = PCA()
pca.fit(digits.data)
plt.plot(np.cumsum(pca.explained_variance_ratio_))
plt.xlabel('Components number')
plt.ylabel('Cumulative explained variance')
plt.show()

plot_digits(digits.data)

noisy = np.random.normal(digits.data, 4)
plot_digits(noisy)

pca = PCA(0.5)
pca.fit(noisy)
print('Noisy components number:', pca.n_components_)

components = pca.transform(noisy)
filtered = pca.inverse_transform(components)
plot_digits(filtered)


class CustomPCA(PCA):
    def __init__(self, n_components=2):
        super().__init__(n_components)
        self.mean_ = None
        self.components_ = None
        self.n_components_ = None
        self.explained_variance_ = None
        self.n_components = n_components

    def fit(self, X, **kwargs):
        # Центрирование данных относительно среднего значения
        self.mean_ = np.mean(X, axis=0)
        x_centered = X - self.mean_

        # Разложение по сингулярным значениям
        u, s, v_transposed = np.linalg.svd(x_centered, full_matrices=False)
        u, v_transposed = self._flip(u, v_transposed)

        n_samples, n_features = x_centered.shape
        # Объясненная дисперсия через сингулярные значения
        explained_variance_ = (s ** 2) / (n_samples - 1)

        # Если дробное от 0 до 1, то показывает, сколько сохранить дисперсии
        # Ищем минимальное количество компонент, которые обеспечивают указанную долю дисперсии
        if 0 <= self.n_components < 1.0:
            total_variance = explained_variance_.sum()
            for n in range(len(explained_variance_)):
                # Сравниваем сумму дисперсий с долей дисперсии каждой компоненты
                if np.sum(explained_variance_[:n]) / total_variance > self.n_components:
                    # Нашли минимально необходимое количество компонент
                    self.n_components_ = n
                    break
        else:
            self.n_components_ = self.n_components

        # Сохраняем значения для дальнейшего использования
        self.components_ = v_transposed[:self.n_components_]
        self.n_components_ = self.n_components_
        self.explained_variance_ = explained_variance_[:self.n_components_]

        return self

    @staticmethod
    def _flip(u, vt):
        # Смена знаков собственных векторов
        max_abs_cols = np.argmax(np.abs(u), axis=0)
        signs = np.sign(u[max_abs_cols, range(u.shape[1])])
        u *= signs
        vt *= signs[:, np.newaxis]
        return u, vt

    def transform(self, X):
        return np.dot(X - self.mean_, self.components_.T)

    def inverse_transform(self, X_transformed):
        return np.dot(X_transformed, self.components_) + self.mean_


pca = CustomPCA(n_components=2)
pca.fit(X)

print('Custom components:', pca.components_)
print('Custom explained variance:', pca.explained_variance_)

plt.scatter(X[:, 0], X[:, 1], alpha=0.5)
plot_vectors(pca.mean_, pca.components_, pca.explained_variance_)

X_pca = pca.transform(X) / np.sqrt(pca.explained_variance_)
components = pca.transform(pca.components_ + pca.mean_)

plt.scatter(X_pca[:, 0], X_pca[:, 1], alpha=0.5)
plot_vectors(pca.mean_, components, [1, 1])

pca = PCA(0.5)
pca.fit(noisy)

components = pca.transform(noisy)
filtered = pca.inverse_transform(components)
plot_digits(filtered)
