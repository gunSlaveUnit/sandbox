"""
Supervised learning: Iris data classification
"""

import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score

# Data preparing
iris = sns.load_dataset('iris')
X_iris = iris.drop('species', axis=1)
y_iris = iris['species']

# Divide data for training and test sets
X_train, X_test, y_train, y_test = train_test_split(
    X_iris,
    y_iris,
    random_state=1
)

# Learning model
model = GaussianNB()
model.fit(X_train, y_train)

# Prediction
y_model = model.predict(X_test)

print(accuracy_score(y_test, y_model))
