from tests import mocks


class TestMocksGraphDataCollecting:
    def test_empty_mock(self):
        assert (0, []) == mocks.empty_enter_mock()

    def test_one_vertex_mock(self):
        assert (1, []) == mocks.one_vertices_enter_mock()

    def test_two_vertices_mock(self):
        assert (2, [(0, 1, 5.0)]) == mocks.two_vertices_enter_mock()

    def test_three_vertices_mock(self):
        assert (
                   3, [
                       (0, 1, 2.0), (0, 2, 7.0), (1, 2, 2147483648)
                   ]
               ) == mocks.three_vertices_not_full_enter_mock()
