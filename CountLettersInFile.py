#! usr/bin/env python3
# -*- coding:utf-8 -*-

import zipfile
import os


class LetterCounter:
    def __init__(self, file_name=None, file_encoding='utf-8'):
        self._file_name = file_name
        self._file_encoding = file_encoding

        self._file_names_with_letters_statistic = dict()

    def _unzip(self):
        files_zip_object = zipfile.ZipFile(self._file_name, 'r')
        for file_name in files_zip_object.namelist():
            files_zip_object.extract(file_name)
            self._file_names_with_letters_statistic[file_name] = list()

    def count(self):
        if self._file_name.endswith('zip'):
            self._unzip()
        else:
            self._file_names_with_letters_statistic[self._file_name] = list()
        for key in self._file_names_with_letters_statistic.keys():
            self._count_amount_each_letter_in_text(key)
            self._count_total_number_letters(key)

    def _count_amount_each_letter_in_text(self, file_name_to_counting):
        try:
            with open(file_name_to_counting, 'r', encoding=self._file_encoding) as file_to_count:
                self._file_names_with_letters_statistic[file_name_to_counting].append(dict())
                for line in file_to_count:
                    for character in line:
                        if character.isalpha():
                            if character in self._file_names_with_letters_statistic[file_name_to_counting][0]:
                                self._file_names_with_letters_statistic[file_name_to_counting][0][character] += 1
                            else:
                                self._file_names_with_letters_statistic[file_name_to_counting][0][character] = 1
        # TODO: проработать исключения
        except Exception as error:
            print('Something was wrong.')

    def _count_total_number_letters(self, file_name_to_counting):
        total_number_letters_in_file = 0
        for value in self._file_names_with_letters_statistic[file_name_to_counting][0].values():
            total_number_letters_in_file += value
        self._file_names_with_letters_statistic[file_name_to_counting].append(total_number_letters_in_file)

    def output(self, file_name_to_output=None):
        for file_name, list_of_statistic in self._file_names_with_letters_statistic.items():
            phrase_to_output_total_amount = 'Total'
            number_digits_in_largest_number = len(str(list_of_statistic[1]))
            if number_digits_in_largest_number < len(phrase_to_output_total_amount):
                number_digits_in_largest_number = len(phrase_to_output_total_amount)

            dash_in_right_amount_to_decorate_frame = ''
            for i in range(number_digits_in_largest_number):
                dash_in_right_amount_to_decorate_frame += '-'

            if file_name_to_output:
                try:
                    with open(file_name_to_output, 'w+', encoding='utf8') as output_file:
                        def print_start_or_end_part_frame_to_file():
                            output_file.write(f'+{dash_in_right_amount_to_decorate_frame}+'
                                              f'{dash_in_right_amount_to_decorate_frame}+\n')

                        output_file.write(f'FILE: {file_name}\n')
                        print_start_or_end_part_frame_to_file()
                        for letter, amount_letter_in_text in list_of_statistic[0].items():
                            output_file.write(f'|{letter: ^{number_digits_in_largest_number}}|'
                                              f'{amount_letter_in_text: ^{number_digits_in_largest_number}}|\n')
                        print_start_or_end_part_frame_to_file()
                        output_file.write(f'|{phrase_to_output_total_amount: ^{number_digits_in_largest_number}}|'
                                          f'{list_of_statistic[1]: ^{number_digits_in_largest_number}}|\n')
                        print_start_or_end_part_frame_to_file()
                        output_file.write('\n')
                except:
                    pass
            else:
                def print_start_or_end_part_frame_to_console():
                    print(f'+{dash_in_right_amount_to_decorate_frame}+{dash_in_right_amount_to_decorate_frame}+')
                print(f'FILE: {file_name}')
                print_start_or_end_part_frame_to_console()
                for letter, amount_letter_in_text in list_of_statistic[0].items():
                    print(f'|{letter: ^{number_digits_in_largest_number}}|'
                          f'{amount_letter_in_text: ^{number_digits_in_largest_number}}|')
                print_start_or_end_part_frame_to_console()
                print(f'|{phrase_to_output_total_amount: ^{number_digits_in_largest_number}}|'
                      f'{list_of_statistic[1]: ^{number_digits_in_largest_number}}|')
                print()


if __name__ == '__main__':
    file_name_to_test = 'D:\\voyna-i-mir.txt'
    file_name_to_test = os.path.normpath(file_name_to_test)

    counter = LetterCounter(file_name=file_name_to_test, file_encoding='cp1251')
    counter.count()
    counter.output('D:\\stat.txt')
