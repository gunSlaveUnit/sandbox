"""
Unsupervised learning: Iris data dimensionality reduction
"""

import seaborn as sns
from matplotlib import pyplot as plt
from sklearn.decomposition import PCA

# Data preparing
iris = sns.load_dataset('iris')
X_iris = iris.drop('species', axis=1)

# Learning model
model = PCA(n_components=2)
model.fit(X_iris)
X_2D = model.transform(X_iris)

# Drawing
iris['PCA1'] = X_2D[:, 0]
iris['PCA2'] = X_2D[:, 1]
sns.lmplot(data=iris, x="PCA1", y="PCA2", hue='species', fit_reg=False)
plt.show()
