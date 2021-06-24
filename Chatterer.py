#! -*- coding: utf-8 -*-

"""
Make a text generator based on statistical data. 
Let's calculate how often the letter X is followed 
by the letter Y based on some text. 
After that, we will start with an arbitrary letter 
and each next one will be selected depending on the 
frequency of its appearance in the statistics.
"""

from pprint import pprint
from random import randint
from zipfile import ZipFile


class Chatterer:
    def __init__(self, zip_archive_name,
                 amount_symbols_to_analyze,
                 amount_symbols_to_generate):
        self.zip_archive_name = zip_archive_name
        self.amount_symbols_to_analyze = amount_symbols_to_analyze
        self.amount_symbols_to_generate = amount_symbols_to_generate
        self.filenames_to_analyze = list()
        self.statistics = dict()  # TODO: что за статистика, рефактор, объясни
        self.totals = dict()
        self.statistics_for_generating = dict()

    # TODO: метод unzip, возможно требуется рефакторниг имён в зависимости от сущности
    def unzip(self):
        filenames_in_zip_archive = ZipFile(self.zip_archive_name, 'r')
        for filename in filenames_in_zip_archive.namelist():
            filenames_in_zip_archive.extract(filename)
            self.filenames_to_analyze.append(filename)

    def collect_statistics_from_file(self):
        sequence = ' ' * self.amount_symbols_to_analyze
        for filename in self.filenames_to_analyze:
            with open(filename, 'r', encoding='cp1251') as file_to_collect_statistics:
                for line in file_to_collect_statistics:
                    line = line[:-1]
                    for current_character in line:
                        if sequence in self.statistics:
                            if current_character in self.statistics[sequence]:
                                self.statistics[sequence][current_character] += 1
                            else:
                                self.statistics[sequence][current_character] = 1
                        else:
                            self.statistics[sequence] = {current_character: 1}
                        sequence = sequence[1:] + current_character

    # TODO: приготовить как?
    # TODO: ниже - дичь, а не метод, исправить
    def prepare_statistics_to_generate(self):
        for sequence, char_stat in self.statistics.items():
            self.totals[sequence] = 0
            self.statistics_for_generating[sequence] = []
            for char, count in char_stat.items():
                self.totals[sequence] += count
                self.statistics_for_generating[sequence].append([count, char])
            self.statistics_for_generating[sequence].sort(reverse=True)

    def generate_sequence_symbols_according_to_statistics(self):
        printed = 0
        sequence = ' ' * self.amount_symbols_to_analyze
        amount_printed_spaces_for_good_formatting = 0
        while printed < self.amount_symbols_to_generate:
            char_stat = self.statistics_for_generating[sequence]
            total = self.totals[sequence]
            dice = randint(1, total + 1)
            pos = 0
            for count, char in char_stat:
                pos += count
                if dice <= pos:
                    break
            print(char, end='')
            if char == ' ':
                amount_printed_spaces_for_good_formatting += 1
                if amount_printed_spaces_for_good_formatting > 9:
                    print()
                    amount_printed_spaces_for_good_formatting = 0
            printed += 1
            sequence = sequence[1:] + char


if __name__ == '__main__':
    # TODO: сделай, чтобы брал и zip, и txt, да вообще многие форматы, побалуйся
    analyzer = Chatterer(zip_archive_name='C:/Users/MACHINE/PycharmProjects/'
                                          'Course/Lesson 9/lesson_009/python_snippets/'
                                          'voyna-i-mir.txt.zip',
                         amount_symbols_to_analyze=5,
                         amount_symbols_to_generate=10000)
    analyzer.unzip()
    analyzer.collect_statistics_from_file()
    analyzer.prepare_statistics_to_generate()
    analyzer.generate_sequence_symbols_according_to_statistics()
