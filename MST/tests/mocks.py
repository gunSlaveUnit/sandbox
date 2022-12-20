def empty_enter_mock() -> tuple:
    return 0, []


def one_vertices_enter_mock() -> tuple:
    return 1, []


def two_vertices_enter_mock() -> tuple:
    return 2, [(0, 1, 5.0)]


def three_vertices_not_full_enter_mock() -> tuple:
    return 3, [(0, 1, 2.0), (0, 2, 7.0), (1, 2, 2147483648)]
