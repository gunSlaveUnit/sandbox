from graph import find_spanning_tree
from tests import mocks


class TestIntegrationGraph:
    def test_null_vertices(self):
        assert find_spanning_tree(mocks.empty_enter_mock) == []

    def test_one_vertices(self):
        assert find_spanning_tree(mocks.one_vertices_enter_mock) == []

    def test_two_vertices(self):
        assert find_spanning_tree(mocks.two_vertices_enter_mock) == [
            (0, 1, 5.0)
        ]

    def test_three_vertices_not_full(self):
        assert find_spanning_tree(mocks.three_vertices_not_full_enter_mock) == [
            (0, 1, 2.0),
            (0, 2, 7.0)
        ]
