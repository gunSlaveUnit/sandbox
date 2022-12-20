import os
import subprocess

ROOT_PATH = os.path.abspath("../../")
SCRIPT_PATH = os.path.join(ROOT_PATH, 'app.py')


def execute_process(commands):
    with subprocess.Popen(
            ["python", SCRIPT_PATH],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            bufsize=1,
            universal_newlines=True
    ) as process:
        for command in commands:
            input_command = ''.join([command, '\n'])
            process.stdin.write(input_command)

        output, errs = process.communicate()
        return output


class TestBlackBoxGraph:
    def test_exit(self):
        commands_sequence = ["3"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))

    def test_show_empty_graph(self):
        commands_sequence = ["2"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - ' \
                          'Exit\n< MST:  []\n1 - Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))

    def test_input_two_vertices(self):
        commands_sequence = ["1", "2", "2"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - Exit\n< Graph vertices amount: ' \
                          'Edge weight 0 -> 1 = 1 - Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))

    def test_input_two_vertices_and_show_graph(self):
        commands_sequence = ["1", "2", "2", "2"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - Exit\n< Graph vertices amount: ' \
                          'Edge weight 0 -> 1 = 1 - Enter graph\n2 - Show MST\n3 - Exit\n< MST:  ' \
                          '[(0, 1, 2.0)]\n1 - Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))

    def test_input_three_vertices_and_show_graph(self):
        commands_sequence = ["1", "3", "2", "5", "4", "2"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - Exit\n< Graph vertices amount: ' \
                          'Edge weight 0 -> 1 = Edge weight 0 -> 2 = Edge weight 1 -> 2 = 1 - ' \
                          'Enter graph\n2 - Show MST\n3 - Exit\n< MST:  [(0, 1, 2.0), (1, 2, 4.0)]\n1 ' \
                          '- Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))

    def test_input_four_vertices_and_show_graph(self):
        commands_sequence = ["1", "4", "7", "4", "15", "13", "9", "5", "2"]
        expected_output = '1 - Enter graph\n2 - Show MST\n3 - Exit\n< Graph vertices amount: ' \
                          'Edge weight 0 -> 1 = Edge weight 0 -> 2 = Edge weight 0 -> 3 = ' \
                          'Edge weight 1 -> 2 = Edge weight 1 -> 3 = Edge weight 2 -> 3 = 1 - ' \
                          'Enter graph\n2 - Show MST\n3 - Exit\n< MST:  [(0, 2, 4.0), (0, 1, 7.0), ' \
                          '(2, 3, 5.0)]\n1 - Enter graph\n2 - Show MST\n3 - Exit\n< '

        assert (expected_output == execute_process(commands_sequence))
