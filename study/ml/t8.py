from typing import List


def count_fit_transform(x: List[str]):
    feature_names = set([word for row in x for word in row.split(' ')])
    return [[row.count(feature_name) for feature_name in feature_names] for row in x]


sample = [
    'problem of evil',
    'evil queen',
    'horizon problem',
]

print(count_fit_transform(sample))
