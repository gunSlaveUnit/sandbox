# -*- coding: utf-8 -*-

# Имеется файл events.txt вида:
#
# [2018-05-17 01:55:52.665804] NOK
# [2018-05-17 01:56:23.665804] OK
# [2018-05-17 01:56:55.665804] OK
# [2018-05-17 01:57:16.665804] NOK
# [2018-05-17 01:57:58.665804] OK
# ...
#
# Напишите программу, которая считывает файл
# и выводит число событий NOK за каждую минуту в другой файл в формате
#
# [2018-05-17 01:57] 1234
# [2018-05-17 01:58] 4321
# ...
#
# Входные параметры: файл для анализа, файл результата
# Требования к коду: он должен быть готовым к расширению функциональности. Делать сразу на классах.

# TODO здесь ваш код
class LogParser:
    def __init__(self, filename=None):
        self.filename = filename
        self.amount_nok_per_minute = dict()

    def count_nok_events_per_minute(self):
        with open(self.filename, 'r', encoding='utf8') as input_file:
            for line in input_file:
                line = line[:-1]
                if line.endswith('NOK'):
                    minute = line[1:17]
                    if minute in self.amount_nok_per_minute:
                        self.amount_nok_per_minute[minute] += 1
                    else:
                        self.amount_nok_per_minute[minute] = 1

    def ouput(self):
        with open('output.txt', 'w') as output_file:
            for minute, amount_nok_per_minute in self.amount_nok_per_minute.items():
                output_file.write(f'{minute} {amount_nok_per_minute}\n')


if __name__ == '__main__':
    eventer = LogParser('D:/events.txt')
    eventer.count_nok_events_per_minute()
    eventer.ouput()
