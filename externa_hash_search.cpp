/*
 * Вариант 3, внешний хэш-поиск: метод сложения со сдвигом
 * */

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <iostream>
#include <sstream>

template<class T>
uint_fast32_t hash(const T &key, const uint_fast32_t &address_space_size) {
    /*
     * Хешированние аддитивным методом (сложение со сдвигом).
     * Возвращает позицию в адресном пространстве, где должен быть размещен ключ.
     * */

    auto power = sizeof(address_space_size) - 1;
    auto power_shift = key >> power;
    auto sum = key ^ power_shift;

    return sum % address_space_size;
}

template<class T>
void open_file_helpers(std::vector<T> &helpers) {
    /*
     * Открывает вектор указанных файловых объектов.
     * */

    auto length = helpers.size();
    for (int i = 0; i < length; ++i) {
        auto helper = (std::stringstream() << i << ".txt").str();
        helpers[i] = T(helper);
        if (!helpers[i].is_open())
            throw std::runtime_error((std::stringstream() << "Can't open file " << helper).str());
    }
}

void hash_file(const std::string &source_filename,
               const std::string &hashed_filename,
               const uint_fast32_t &address_space_size) {
    /*
     * Создает хешированный файл.
     * Разрешение коллизий - списковая стратегия.
     * */


    /*
     * Открыли источник на чтение данных.
     * */
    std::ifstream source_file(source_filename);
    if (!source_file.is_open())
        throw std::runtime_error("Can't open file to write data");

    /*
     * Определили вектор смещений списков
     * и вспомогательных файлов.
     * */
    std::vector<uint_fast32_t> segments_lengths(address_space_size);
    std::vector<std::ofstream> helpers_write(address_space_size);
    open_file_helpers(helpers_write);

    /*
     * Читаем значения из исходного файла
     * Вычисляем его хеш
     * Выводим в соответствующий файл
     * Увеличиваем длину списка со значением этого хеша
     * */
    uint_fast32_t value;
    while (source_file >> value) {
        auto h = hash(value, address_space_size);
        helpers_write[h] << value << " ";

        ++segments_lengths[h];
    }

    for (int i = 0; i < address_space_size; ++i) {
        helpers_write[i].flush();
        helpers_write[i].close();
    }

    /*
     * Открываем вспомогательные файлы на чтение данных.
     * */
    std::vector<std::ifstream> helpers_read(address_space_size);
    open_file_helpers(helpers_read);

    /*
     * Отрываем хешированный файл для записи данных
     * в нужном виде.
     * */
    std::ofstream hashed_file(hashed_filename);
    if (!hashed_file.is_open())
        throw std::runtime_error("Can't open file to write data");

    /*
     * Вспомогательный файл
     * для оценки распределения хеш-функции
     * (как она справляется с коллизиями).
     * */
    std::ofstream distribution("distribution.csv");

    /*
     * Пишем заголовок хешированного файла.
     * */
    uint_fast32_t start_segment = 0;
    for (unsigned int segments_length: segments_lengths) {
        hashed_file << start_segment + address_space_size + 1 << " ";
        start_segment += segments_length;
        distribution << segments_length << std::endl;
    }

    /*
    * Выводим длину последнего сегмента.
    * */
    hashed_file << start_segment + address_space_size + 1 << " ";

    distribution.flush();
    distribution.close();

    /*
     * Собираем данные со всех
     * вспомогательных файлов в
     * хешированный файл.
     * */
    for (int i = 0; i < address_space_size; ++i)
        while (helpers_read[i] >> value)
            hashed_file << value << " ";

    for (int i = 0; i < address_space_size; ++i)
        helpers_read[i].close();

    hashed_file.flush();
    hashed_file.close();

    source_file.close();

    /*
     * Удаляем вспомогательные файлы.
     * */
    for (int i = 0; i < address_space_size; ++i)
        std::remove((std::stringstream() << i << ".txt").str().c_str());
}

int amount_operations = 0;

int search_hash_file(const std::string &filename,
                     const uint_fast32_t &search_item,
                     const uint_fast32_t &address_space_size) {
    /*
     * Ищет значение в хешированном файле.
     * Возвращает позицию найденных данных в файле (смещение относительно начала).
     * */

    /*
     * Открываем хешированный файл.
     * */
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Can't open file to read data");

    /*
     * Вычисляем хеш поискового значения.
     * Определяем начало нужного сипска.
     * */
    auto h = hash(search_item, address_space_size);
    int count = 0;
    auto start = 0;
    while (count <= h) {
        file >> start;
        ++count;
    }

    /*
     * Определяем длину списка.
     * */
    auto end = 0;
    file >> end;

    int length = end - start;

    /*
     * Идем до нужной позиции - начала списка.
     * */
    file.seekg(0);
    count = 0;
    auto chunk_value = 0;
    while (count < start) {
        file >> chunk_value;
        ++count;
    }

    auto pos = count;

    /*
     * Просматриваем список.
     * */
    count = 0;
    while (count < length) {
        file >> chunk_value;
        ++count;

        ++amount_operations;

        /*
         * Проверяем, совпадает ли значение из списка
         * с поисковым значением.
         * */
        if (chunk_value == search_item) {
            file.close();
            return count + pos - 1;
        }
    }

    file.close();

    return -1;
}

int main() {
    std::random_device os_seed;
    const uint_fast32_t seed = os_seed();
    std::mt19937 generator(seed);
    const uint_fast32_t DOWN = 0, UP = 100000;
    std::uniform_int_distribution<int_fast32_t> distribute(DOWN, UP);

    std::ofstream results("results.csv");

    for (auto s = 500; s < 100000; s += 500) {
        /*
         * Открываем источник для записи в него данных.
         * */
        const std::string SOURCE_FILENAME = "source.txt";
        std::ofstream source(SOURCE_FILENAME, std::ios::binary);
        if (!source.is_open())
            throw std::runtime_error("Can't open file to write source data");

        /*
         * Генерируем случайные данные и записываем их.
         * */
        for (auto i = 0; i < s; ++i)
            source << distribute(generator) << " ";

        source.flush();
        source.close();

        /*
         * Создаем хешированный файл на основе файла-источника.
         * */
        const uint_fast32_t ADDRESS_SPACE_SIZE = 500;
        const std::string HASHED_FILENAME = "hashed.txt";
        hash_file(SOURCE_FILENAME, HASHED_FILENAME, ADDRESS_SPACE_SIZE);

        /*
         * Ищем в хешированном файле случайное значение.
         * */
        auto search_item = distribute(generator);
        auto count = search_hash_file(HASHED_FILENAME, search_item, ADDRESS_SPACE_SIZE);

        results << s << "," << amount_operations << std::endl;
    }
}
