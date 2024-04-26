"""
Unsupervised learning: Iris data clustering
"""

import seaborn as sns
from matplotlib import pyplot as plt
from sklearn.decomposition import PCA
from sklearn.mixture import GaussianMixture

# Data preparing
iris = sns.load_dataset('iris')
X_iris = iris.drop('species', axis=1)

# Learning model for dimensionality reduction
model = PCA(n_components=2)
model.fit(X_iris)

# Dimensionality reduction
X_2D = model.transform(X_iris)

# Learning model for clustering
model = GaussianMixture(n_components=3, covariance_type='full')
model.fit(X_iris)

# Prediction
y_gmm = model.predict(X_iris)

# Drawing
iris['PCA1'] = X_2D[:, 0]
iris['PCA2'] = X_2D[:, 1]
iris['cluster'] = y_gmm
sns.lmplot(data=iris, x="PCA1", y="PCA2", hue='species', col='cluster', fit_reg=False)
plt.show()
