class Graph:
    INF = float('inf')

    def __init__(self, edges: list[list[int, int, float]]) -> None:
        """
        Creates a graph object.
        Based on these edges, calculates the number of points in the graph
        and compiles a symmetric adjacency matrix of the graph.

        :param edges: list of edges of the source graph in the form: start vertex, end vertex, edge weight.
        """

        self.vertices_amount = max(max(edge[:2]) for edge in edges) + 1
        self.adjacency_matrix = [[0] * self.vertices_amount for _ in range(self.vertices_amount)]

        self._push_edges(edges)

    def mst(self) -> list[list[int, int, float]]:
        """
        Uses the Prim algorithm to find the minimum spanning tree.

        :return: minimum spanning tree like a list of edges: start, end, weight.
        """

        mst_edges = []
        mst_edges_amount = 0

        marked = [False for _ in range(self.vertices_amount)]
        marked[0] = True

        while mst_edges_amount < self.vertices_amount - 1:
            start = 0
            end = 0

            min_vertex_edge = Graph.INF

            for i in range(self.vertices_amount):
                if marked[i]:
                    for j in range(self.vertices_amount):
                        edge_weight = self.adjacency_matrix[i][j]

                        if edge_weight and not marked[j]:
                            if min_vertex_edge > edge_weight:
                                start = i
                                end = j

                                min_vertex_edge = edge_weight
            
            marked[end] = True

            mst_edges_amount += 1

            mst_edges.append([start, end, min_vertex_edge])

        return mst_edges

    def _push_edges(self, edges) -> None:
        """
        Fills the adjacency matrix with given edges symmetrically.

        :param edges: list of edges of the source graph in the form: start vertex, end vertex, edge weight.
        """

        for edge in edges:
            u, v, weight = edge
            self.adjacency_matrix[u][v] = weight
            self.adjacency_matrix[v][u] = weight


a = 0
b = 1
c = 2
e = 3
d = 4

edges = [
    [a, b, 3],
    [a, c, 4],
    [a, e, 1],
    [b, c, 5],
    [c, e, 6],
    [c, d, 2],
    [e, d, 7],
]

graph = Graph(edges)

mst = graph.mst()

for edge in mst:
    v1, v2, w = edge
    print(f'{v1}->{v2}:{w}')
