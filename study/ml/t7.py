from typing import List


def dict_fir_transform(x: List[dict]):
    feature_names = set([(k, v) if isinstance(v, str) else k for row in x for k, v in row.items()])

    feature_names = sorted(feature_names, key=lambda k: k if isinstance(k, tuple) else (k, ''))

    features = []
    for row in x:
        feature = []
        for feature_name in feature_names:
            if isinstance(feature_name, tuple):
                key, value = feature_name
                feature.append(int(row[key] == value))
            else:
                feature.append(row[feature_name])

        features.append(feature)

    return features


data = [
    {'price': 850000, 'rooms': 4, 'neighborhood': 'Queen Anne'},
    {'price': 700000, 'rooms': 3, 'neighborhood': 'Fremont'},
    {'price': 650000, 'rooms': 3, 'neighborhood': 'Wallingford'},
    {'price': 600000, 'rooms': 2, 'neighborhood': 'Fremont'}
]

print(dict_fir_transform(data))
