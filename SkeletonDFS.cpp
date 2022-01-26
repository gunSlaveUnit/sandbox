#include <iostream>
#include <fstream>
#include <windows.h>

#include "stack.cpp"

const char* FILENAME_WRITE_MATRIXES = "matrix.txt";

template <class T>
class Matrix {
public:
    void fill() {
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j) {
                std::cout << "[" << i << "]" << "[" << j << "]=";
                std::cin >> m[i][j];
                m[j][i] = m[i][j];
            }
    }
    void fill(const char* filename) {
        std::ifstream f(filename, std::ios_base::in);
        if (!f.is_open()) {
            std::cout<<"Can't open file to read a matrix"<<std::endl;
            return;
        }
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                f >> m[i][j];
        f.close();
    }
    explicit Matrix(int n = 0) : n(n) {
        m = new T *[n];
        for (int i = 0; i < n; ++i) m[i] = new T[n];
        for(int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) m[i][j] = 0;
    }
    void showLikeMatrix() {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                std::cout << m[i][j] << " | ";
            std::cout << std::endl;
        }
    }
    void showLikeEdges() {
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (m[i][j]) std::cout<<i<<" -> "<<j<<std::endl;

    }
    virtual ~Matrix() {
        for (int i = 0; i < n; ++i) delete[] m[i];
        delete[] m;
    }
    void set(int i, int j, T v) { m[i][j] = v; }
    T get(int i, int j) { return m[i][j]; }
protected:
    T **m;
    int n;
};

class MatrixGenerator {
public:
    static void generateAdjacencyMatrix(int n) {
        std::ofstream f(FILENAME_WRITE_MATRIXES, std::ios_base::out);
        if (!f.is_open()) {
            std::cout<<"Can't open file to write matrix"<<std::endl;
            return;
        }
        auto* m = new Matrix<int>(n);
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j) {
               int v = rand() % 2;
               m->set(i, j, v);
               m->set(j, i, v);
            }

        for (int i = 0; i < n; ++i) {
            bool isolatedVertexIsFound = true;
            for (int j = i + 1; j < n; ++j) if (m->get(i, j)) isolatedVertexIsFound = false;
            if (isolatedVertexIsFound) {
                int vertind = rand() % n;
                while (vertind == i) vertind = rand() % n;
                m->set(i, vertind, 1);
                m->set(vertind, i, 1);
            }
        }

        for (int i = 0; i < n; ++i){
            for (int j = 0; j < n; ++j)
                f << m->get(i, j) << " ";
            f << std::endl;
        }

        f.flush();
        f.close();
        delete m;
    }
};

template<class T>
class Graph : public Matrix<T>{
public:
    void dfs() {
        auto sp = new Matrix<T>(this->n); // остов
        auto s = new Stack<T>;
        bool *v = new bool[this->n]; // окраски вершин
        memset(v, false, this->n);
        s->push(0);
        v[0] = true;

        std::optional<T> e = s->top();
        while(e) {
            bool found = false;
            for (int i = 0; i < this->n; ++i) {
                if (this->m[*e][i] && !v[i]) {
                    s->push(i);
                    v[i] = true;
                    sp->set(*e, i, 1);
                    sp->set(i, *e, 1);
                    e = i;
                    found = true;
                    break;
                }
            }
            if (!found) {
                s->pop();
                e = s->top();
            }
        }

        for (int i = 0; i < this->n; ++i) if (!v[i]) std::cout<<"Не все вершины были посещены"<<std::endl;

        std::cout<<"Остов:"<<std::endl;
        sp->showLikeEdges();

        delete s;
        delete[] v;
    }
    explicit Graph(int n) : Matrix<T>(n) {}
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    int min = 5;
    int max = min + rand() % 10;
    for (int i = min; i < max; ++i) {
        MatrixGenerator::generateAdjacencyMatrix(i);
        auto* g = new Graph<int>(i);
        g->fill(FILENAME_WRITE_MATRIXES);
        std::cout<<"Исходный граф "<<i<<"x"<<i<<std::endl;
        g->showLikeEdges();
        g->dfs();
        std::cout<<"--------------------------------"<<std::endl;
        delete g;
    }
}
