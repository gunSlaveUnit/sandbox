#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <windows.h>

/* Настройки генетического алгоритма */
const int GENERATIONS_AMOUNT = 30;
int POPULATION_SIZE = 0; // Определим, когда получим количество вершин
const float CROSSING_OVER_PROBABILITY = 0.9;
const float MUTATION_PROBABILITY = 0.1;

struct Entity {
    /* Представляет собой особь в популяции
     * chromosome - хромосома, в данном случае - индексы вершин
     * fit - приспособленность данной особи, в данном случае - длина пути
     * Чем fit меньше, тем лучше
     * sumFit - применяется для отбора методом рулетки,
     * sumFit = fit данной особи + fit всех предыдущих особей */
    std::vector<int> chromosome;
    int fit{};
    int sumFit{};
};

const int INF = 100; // Величина отсутствующих ребер
int generalFit = 0;  // Общая приспособленность всех особей в конкретном поколении
int** m;             // Матрица смежности графа

int enterVertex(int n) {
    /* Получение индекса вершины от пользователя
     * с проверкой на выход за границы */
    int v;
    std::cin>>v;
    while (v > n || v < 0) {
        std::cout<<"Неверно. Повторите: ";
        std::cin>>v;
    }
    return v;
}

void enterWays(int n) {
    /* Получаем веса ребер - длину пути от вершины к вершине
     * Вводим лишь веса выше главной диагонали,
     * поскольку матрица смежности симметрична */
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            std::cout << "[" << i << "]" << "[" << j << "]=";
            std::cin >> m[i][j];
            if (m[i][j] < 0) m[i][j] = INF;
            m[j][i] = m[i][j];
        }
}

int countFitness(Entity& e, int to) {
    /* Расчет приспособленность особи,
     * т.е. сумма весов ребер в пути.
     * Считается до конечной вершины. */
    int f = 0;
    for(int i = 0; i < e.chromosome.size() - 1; ++i)
        if (e.chromosome[i] != to) f += m[e.chromosome[i]][e.chromosome[i+1]];
        else break;
    return f;
}

Entity crossover(Entity& e1, Entity& e2) {
    /* Скрещиваем хромосомы двух родителей,
     * используя для этого случайно сгенерированную битовую маску,
     * размер которой n - 1 */
    Entity descendant;
    std::vector<int> newChromosome(e1.chromosome.size());
    for(auto& i : newChromosome) i = -1;

    std::vector<int> cover(e1.chromosome.size() - 1);
    for(int & i : cover) i = rand() % 2;

    std::vector<int> zeroGeneIndices;
    for(int i = 0; i < cover.size(); ++i)
        if (cover[i]) newChromosome[i+1] = e1.chromosome[i+1];
        else
            for(int j = 1; j < e2.chromosome.size(); ++j)
                if (e2.chromosome[j] == e1.chromosome[i+1])
                    zeroGeneIndices.push_back(j);

    sort(zeroGeneIndices.begin(), zeroGeneIndices.end(), [](int a, int b) -> bool { return a < b; });

    for(auto& n : zeroGeneIndices)
        for(int j = 1; j < newChromosome.size(); ++j)
            if (newChromosome[j] < 0) {
                newChromosome[j] = e2.chromosome[n];
                break;
            }

    newChromosome[0] = e1.chromosome[0];
    descendant.chromosome = newChromosome;
    for(int i = 0; i < newChromosome.size()-1; ++i)
        for(int j = i+1; j < newChromosome.size(); ++j)
            if(newChromosome[i] == newChromosome[j] ) std::cout<<"PANIC";
    return descendant;
}

void mutate(Entity& e) {
    /* Мутация в данном случае достаточно мощная -
     * перемешивание всей хромосомы
     * кроме первого стартового элемента */
    std::mt19937 g(rand());
    std::shuffle(e.chromosome.begin() + 1, e.chromosome.end(), g);
}

std::vector<Entity> fitnessProportionateSelection(std::vector<Entity>& p) {
    /* Выбор двух родителей из популяции согласно методу рулетки */
    std::vector<Entity> parents;
    for(int i = 0; i < 2; ++i) {
        int parent;
        int topFit = rand() % (p[p.size()-1].sumFit-1);
        for(int j = 0; j < p.size(); ++j) {
            parent = j;
            if (p[j].sumFit > topFit) break;
        }
        parents.push_back(p[parent]);
    }
    return parents;
}

void replaceWorst(std::vector<Entity>& p, Entity& c) {
    /* Заменяет худшую особь в популяции на потомка */
    int index = 0, fit = 0;
    for(int i = 0; i < p.size(); ++i) {
        if (p[i].fit > fit) {
            fit = p[i].fit;
            index = i;
        }
    }
    p[index] = c;
}

int countGeneralFitness(std::vector<Entity>& p) {
    /* Считает общую приспособленность для всех особей в поколении */
    int s = 0;
    for (auto & i : p) s += i.fit;
    return s;
}

std::vector<int> searchShortestPath(int from, int to, int n) {
    std::mt19937 g(rand());

    std::vector<int> indicesToGenerateWays;
    for(int i = 0; i < n; ++i) if (i != from) indicesToGenerateWays.push_back(i);

    /* Создаем популяцию:
     * В индексы выше добавляем стартовую вершину
     * Получается новая хромосома, на основе которой создается особь
     * Затем вершины перемешиваются
     * 0 - старотовая
     * 4 - конечная
     * 0 2 1 3 4 5 6 7*/

    std::vector<Entity> population;
    for(int i = 0; i < POPULATION_SIZE; ++i) {
        auto w = indicesToGenerateWays;
        w.insert(w.begin(), from);
        Entity newE;
        newE.chromosome = w;
        newE.fit = countFitness(newE, to);
        newE.sumFit = 0;
        population.push_back(newE);
        std::shuffle(indicesToGenerateWays.begin(), indicesToGenerateWays.end(), g);
    }


    for (int i = 0; i < GENERATIONS_AMOUNT; ++i) {
        /* Считаем суммарную приспособленность для выбора рулеткой */
        generalFit = countGeneralFitness(population);

        /* Сортируем по fit, от меньшего к большему, в начале окажутся особи с самым коротким маршрутом */
        sort(population.begin(), population.end(), [](const Entity& a, const Entity& b) -> bool { return a.fit < b.fit; });

        /* Для каждой особи считаем ее sumFit = ее fit + все предыдущие fit */
        population[0].sumFit = population[0].fit;
        for(int j = 1; j < population.size(); ++j) {
            population[j].sumFit = population[j].fit + population[j-1].sumFit;
        }

        std::cout<<"Поколение #"<<i+1
        <<"\tЛучшая: "<<population[0].fit
        <<"\tСредняя приспособленность: "
        <<double(generalFit)/population.size()<<std::endl;

        /* Скрещивание.
         * Выбор двух родителей методом рулетки,
         * получение потомка,
         * замена худшей особи в популяции потомком */
        int crossesNumber = int(population.size() * CROSSING_OVER_PROBABILITY);
        for(int j = 0; j < crossesNumber; ++j) {
            auto parents = fitnessProportionateSelection(population);
            auto child = crossover(parents[0], parents[1]);
            child.fit = countFitness(child, to);
            replaceWorst(population, child);
        }

        /* Мутация, для каждой особи может произойти или не произойти с определенной вероятностью */
        std::uniform_real_distribution<> dis(0, 1);
        for(auto & e : population) if(dis(g) < MUTATION_PROBABILITY) {
                mutate(e);
                e.fit = countFitness(e, to);
            }
    }
    return population[0].chromosome;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));

    /* Получаем количество вершин */
    int n;
    std::cout<<"Количество вершин: ";
    std::cin>>n;

    /* Память под матрицу */
    m = new int*[n];
    for (int i = 0; i < n; ++i) {
        m[i] = new int[n];
        for (int j = 0; j < n; ++j) m[i][j] = 0;
    }

    /* Размер популяции */
    POPULATION_SIZE = 200;

    /* Получение весов ребер */
    std::cout<<"Нумеровка начинается с 0"<<std::endl;
    std::cout<<"Если пути нет - введите -1"<<std::endl;
    enterWays(n);

    /* Получаем, откуда и куда надо найти путь */
    std::cout<<"Кратчайший путь"<<std::endl;
    std::cout<<"Из: ";
    int from = enterVertex(n);
    std::cout<<"В: ";
    int to = enterVertex(n);

    /* Ищем кратчайший путь по алгоритму */
    auto p = searchShortestPath(from, to, n);

    /* Выводим найденный путь, но лишь до конечной, нужной вершины */
    std::cout<<"Кратчайший путь из "<<from<<" в "<<to<<": ";
    for(auto& v : p) {
        std::cout<<v;
        if (v == to) break;
        std::cout<<"->";
    }

    /* Освобождаем матрицу */
    for(int i = 0; i < n; ++i) delete[] m[i];
    delete[] m;
}