from typing import NamedTuple, Tuple, List

INF = 1 << 32 - 1


class Edge(NamedTuple):
    start: int
    finish: int
    weight: float


def enter() -> Tuple[int, List[Edge]]:
    """
    Graph input: number of vertices and weights of edges
    """

    vertices_amount = input_vertices_amount()

    return vertices_amount, input_edges(vertices_amount)


def input_vertices_amount() -> int:
    """
    Asks for input the number of graph vertices
    """
    while True:
        vertices_amount = input("Количество вершин графа: ")
        try:
            if vertices_amount.isdigit():
                vertices_amount = int(vertices_amount)
                if 0 <= vertices_amount <= 10:
                    break
            print("Граф может иметь от 0 до 10 вершин")
            continue
        except ValueError:
            print("Что-то пошло не так")
    return int(vertices_amount)


def input_edges(vertices_amount) -> List[Edge]:
    """
    Asks for the weight of the edges
    above the main diagonal of the matrix
    """
    edges = []
    for i in range(vertices_amount):
        for j in range(i + 1, vertices_amount):
            while True:
                weight = input(f"Вес ребра {i} -> {j} = ")
                try:
                    if weight == "-1" \
                            or weight.isdigit() \
                            or weight.replace('.', '', 1).isdigit() and weight.count('.') < 2:
                        weight = float(weight)
                        if weight < 0:
                            weight = INF
                        edges.append(Edge(i, j, weight))
                        break
                    print("Вес ребра может быть числом с плавающей точкой или -1")
                    continue
                except ValueError:
                    print("Что-то пошло не так")
    return edges


def find(parent, i):
    """
    Рекурсивно ищет корень заданной i-ой компоненты
    """
    return i if parent[i] == i else find(parent, parent[i])


def find_spanning_tree(get_data):
    """
    Using the Boruvka's algorithm
    Link: https://en.wikipedia.org/wiki/Bor%C5%AFvka%27s_algorithm

    :return: list of edges of minimum weight
    """

    vertices_amount, edges = get_data()

    parents = [i for i in range(vertices_amount)]
    component_sizes = [1 for _ in range(vertices_amount)]
    cheapest_edges = [-1 for _ in range(vertices_amount)]
    mst = []
    while len(mst) != vertices_amount - 1 and parents:
        # Ищем для каждой компоненты минимальное ребро, которое соединяет ее с другой компонентой
        for i in range(len(edges)):
            u, v, w = edges[i]
            set1 = find(parents, u)
            set2 = find(parents, v)

            if set1 != set2:
                if cheapest_edges[set1] == -1:
                    cheapest_edges[set1] = [u, v, w]

                if cheapest_edges[set1][2] > w:
                    cheapest_edges[set1] = [u, v, w]

                if cheapest_edges[set2] == -1:
                    cheapest_edges[set2] = [u, v, w]

                if cheapest_edges[set2][2] > w:
                    cheapest_edges[set2] = [u, v, w]

        # Объединяем компоненты
        for node in range(vertices_amount):
            if cheapest_edges[node] != -1:
                u, v, w = cheapest_edges[node]
                set1 = find(parents, u)
                set2 = find(parents, v)

                if set1 != set2:
                    x_root = find(parents, set1)
                    y_root = find(parents, set2)

                    if component_sizes[x_root] < component_sizes[y_root]:
                        parents[x_root] = y_root
                    elif component_sizes[x_root] > component_sizes[y_root]:
                        parents[y_root] = x_root
                    else:
                        parents[y_root] = x_root
                        component_sizes[x_root] += 1

                    mst.append((u, v, w))

        cheapest_edges = [-1] * vertices_amount

    return mst
