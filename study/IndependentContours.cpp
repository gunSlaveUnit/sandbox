﻿#include <iostream>
#include <list>
#include <fstream>

struct Edge {
	int start;
	int end;
};

int n;
int** m;
int** sp;
std::list<int> s;
std::list<Edge> branches;
std::list<Edge> chords;
std::list<int> contour;
int operationsAmount;

enum coloration { WHITE, BLACK, RED };

void Skeleton(int** m, int** sp, std::list<Edge>& branches, std::list<Edge>& chords) {
	auto* colors = new int[n];
	for (int i = 0; i < n; ++i) colors[i] = WHITE; // все вершины в начале белые
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) sp[i][j] = m[i][j];
	int v = 0; // стартовая вершина
	colors[v] = RED; // отмечаем как вершину для рассмотрения
	s.push_back(v);
	while (!s.empty()) {
		v = s.back();
		s.pop_back();
		colors[v] = BLACK; // отмечаем вершину как пройденную
		std::list<int> r; // список сначала хорд, потом ветвей
		for (int u = 0; u < n; ++u) {
			if (colors[u] == RED && m[v][u] != 0) r.push_back(u); // найдена хорда
			++operationsAmount;
		}
		// определяем направление хорды в зависимости от направления тока
		for (auto& u : r) {
			if (m[v][u] > 0) {
				chords.push_back(Edge{ u, v });
			}
			else {
				chords.push_back(Edge{ v, u });
			}
			++operationsAmount;
		}
		for (auto& u : r) {
			sp[v][u] = sp[u][v] = 0; // удаляем найденные хорды из остова
			++operationsAmount;
		}
		r.clear();
		for (int u = 0; u < n; ++u) {
			if (colors[u] == WHITE && m[v][u] != 0) r.push_back(u); // найдена ветвь
			++operationsAmount;
		}
		for (auto& u : r) {
			s.push_back(u); // белую вершину сохраняем в общем списке для рассмотрения
			++operationsAmount;
		} 
		for (auto& u : r) {
			branches.push_back(Edge{ v, u }); // добавляем ветвь
			++operationsAmount;
		} 
		for (auto& u : r) {
			colors[u] = RED; // после идентификации ветви красим ее белую вершину в красный цвет
			++operationsAmount;
		} 
	}
	delete[] colors;
}

void Contour(int** sp, int start, int end, std::list<int>& contour) {
	auto* colors = new int[n];
	for (int i = 0; i < n; ++i) colors[i] = WHITE;
	std::list<int> p;
	int v = start;
	p.push_back(v);
	colors[v] = RED; // помечаем стартовую вершину как текущую рассматриваемую
	contour.push_back(end);
	while (true) {
		// находим висячие вершины, т.е. такие, степень смежности которых = 1
		int deg = 0;
		int last;
		for (int i = 0; i < n; ++i) {
			if (sp[end][i]) {
				++deg;
				last = i;
			}
			++operationsAmount;
		}
			
		if (deg > 1) break; // вершина не висячая
		contour.push_front(last);
		end = last; // перемещаем указатель на смежную вершину
		if (end == start) { // если совпали, контур построен, это простейший случай
			delete[] colors;
			return;
		}
	}
	while (true) {
		// основной цикл алгоритма
		while (!p.empty() && colors[v = p.back()] != RED) p.pop_back();
		colors[v] = BLACK;
		for (int i = 0; i < n; i++) {
			if (sp[v][i] != 0 && colors[i] == WHITE) {
				if (i == end) {
					p.remove_if([&colors](int v) { return colors[v] == RED; });
					contour.splice(contour.begin(), p);
					p.clear();
					delete[] colors;
					return;
				}
				p.push_back(i);
				colors[i] = RED;
			}
			++operationsAmount;
		}
	}
}

void generate(int n) {
	for (int i = 0; i < n; ++i)
		for (int j = i; j < n; ++j)
			if (i == j) m[i][j] = 0;
			else {
				bool sign = rand() % 2;
				m[i][j] = (sign) ? -1 : 1;
				m[j][i] = -m[i][j];
			}
}

int main() {
	srand(time(0));
	setlocale(LC_ALL, "rus");

	std::ofstream skeletonOperations("skeleton.csv");
	std::ofstream contourOperations("contour.csv");

	for (int j = 4; j < 101; ++j) {
		n = j;
		operationsAmount = 0;
		m = new int* [n];
		for (int i = 0; i < n; ++i) m[i] = new int[n];

		generate(n);

		sp = new int* [n];
		for (int i = 0; i < n; ++i) sp[i] = new int[n];
		std::cout << "Size: " << j << std::endl;
		skeletonOperations << n << ",";
		contourOperations << n << ",";
		Skeleton(m, sp, branches, chords);
		std::cout << "Skeleton: " << operationsAmount << std::endl;
		skeletonOperations << operationsAmount << "\n";
		operationsAmount = 0;
		for (auto i = chords.begin(); i != chords.end(); ++i) {
			Contour(sp, i->start, i->end, contour);
			contour.clear();
		}
		std::cout << "Contour: " << operationsAmount << std::endl << std::endl;
		contourOperations << operationsAmount << "\n";

		for (int i = 0; i < n; ++i) delete[] sp[i];
		delete[] sp;

		for (int i = 0; i < n; ++i) delete[] m[i];
		delete[] m;
	}
	std::cout << "Результаты успешно записаны в csv файл" << std::endl;
}
