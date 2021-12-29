#include <iostream>
#include <optional>
#include <windows.h>

template<class T>
struct Node {
    explicit Node(T data) : data(data) {}
    T data;
    Node<T>* prev;
    Node<T>* next;
};

template<class T>
class Deque {
public:
    std::optional<T> popFront() {
        if (head) {
            T data = head->data;
            if (head == tail) {
                delete tail;
                head = tail = nullptr;
                return data;
            }
            head = head->next;
            if (head) {
                delete head->prev;
                head->prev = nullptr;
            }
            --size;
            return data;
        } else {
            if (!tail) return {};
            auto* temp = tail;
            while(temp->prev) temp = temp->prev;
            head = temp;
            T data = head->data;
            head = temp->next;
            if (temp == tail) tail = nullptr;
            if (head) {
                delete head->prev;
                head->prev = nullptr;
            }
            --size;
            return data;
        }
    }
    std::optional<T> popBack() {
        if (tail) {
            T data = tail->data;
            if (tail == head) {
                delete head;
                tail = head = nullptr;
                return data;
            }
            tail = tail->prev;
            if (tail) {
                delete tail->next;
                tail->next = nullptr;
            }
            --size;
            return data;
        } else {
            if (!head) return {};
            auto* temp = head;
            while(temp->next) temp = temp->next;
            tail = temp;
            T data = tail->data;
            tail = temp->prev;
            if (temp == head) head = nullptr;
            if (tail) {
                delete tail->next;
                tail->next = nullptr;
            }
            --size;
            return data;
        }
    }
    void pushFront(T data) {
        if (head) {
            auto* n = new Node<T>(data);
            n->prev = nullptr;
            n->next = head;
            head->prev = n;
            head = n;
        } else {
            head = new Node<T>(data);
            head->prev = nullptr;
            if (tail) {
                auto* temp = tail;
                while(temp->prev) temp = temp->prev;
                head->next = temp;
                temp->prev = head;
            } else head->next = tail;
        }
        ++size;
    }
    void pushBack(T data) {
        if (tail) {
            auto* n = new Node<T>(data);
            n->next = nullptr;
            n->prev = tail;
            tail->next = n;
            tail = n;
        } else {
            tail = new Node<T>(data);
            tail->next = nullptr;
            if (head) {
                auto* temp = head;
                while(temp->next) temp = temp->next;
                tail->prev = temp;
                temp->next = tail;
            } else tail->prev = head;
        }
        ++size;
    }
    virtual ~Deque() {
        Node<T>* temp;
        if (head) {
            while (head->next) {
                head = head->next;
                temp = head->prev;
                delete temp;
            }
            delete head;
        }
    }
    bool isEmpty() { return head == nullptr && tail == nullptr; }
    [[nodiscard]] int length() const { return size; }
    Deque() : head(nullptr), tail(nullptr), size(0) {}
protected:
    Node<T>* head;
    Node<T>* tail;
    int size;
};

enum TASK_TYPES {START, STOP};

struct Task {
    Task() : startTime(0), processTime(0), type(START) {}
    Task(int start, int process, TASK_TYPES type)
            : startTime(start), processTime(process), type(type) {}
    bool operator <(Task& t) const { return this->startTime < t.startTime; }
    int startTime;
    int processTime;
    TASK_TYPES type;
};

template<class T>
class Calendar : public Deque<T>    {
public:
    void insertByAsc(T& t) {
        if (!this->tail || (this->tail->data < t)) this->pushBack(t);
        else if (!this->tail->prev) this->pushFront(t);
        else {
            auto* nt = new Node<Task>(t);
            auto* temp = this->tail;
            while(temp->prev) {
                if (temp->data < t) break;
                temp = temp->prev;
            }
            nt->prev = temp;
            nt->next = temp->next;
            temp->next->prev = nt;
            temp->next = nt;
        }
    }
};

class SMO {
public:
    void start() {
        Task init(0, 1 + rand() % 10, START);
        c.pushBack(init);

        while(currentTime<=monitoringTime) {
            auto t = c.popFront();
            if (t) {
                currentTime = t->startTime;
                std::cout<<"Текущее время: "<<currentTime<<std::endl;
                std::cout<<"Задача ["<<t->startTime<<" "<<t->processTime<<" ";
                if (!t->type) {
                    std::cout<<"START]"<<std::endl;
                    if (handlerFree && ts.isEmpty()) {
                        std::cout<<"Начало обслуживания задачи. \nЕе завершение назначено на: "
                                 <<t->startTime+t->processTime<<std::endl<<std::endl;
                        handlerFree = false;
                        Task taskStop(t->startTime+t->processTime, t->processTime, STOP);
                        c.insertByAsc(taskStop);
                    } else {
                        ts.pushBack(*t);
                        std::cout<<"Обработчик занят или очередь задач не пуста."
                                   "\nЗадача помещена в очередь"
                                   <<"\nСейчас в очереди: "<<ts.length()<<std::endl<<std::endl;
                    }
                } else {
                    ++completedTasksAmount;
                    averageProcessTime += t ->processTime;
                    std::cout<<"STOP]"<<std::endl;
                    std::cout<<"Окончание обслуживания"<<std::endl<<std::endl;
                    handlerFree = true;
                    generateNewTasks();
                    pickUpTasksFromQueue();
                }
            }
        }
        printStats();
    }
    void generateNewTasks() {
        int newTasksAmount = 1 + rand() % 2;
        for (int i = 0; i < newTasksAmount; ++i) {
            int delta = 5 + rand() % 10;
            Task newTask(currentTime + delta, 5 + rand() % 20, START);
            c.insertByAsc(newTask);
        }
    }
    void pickUpTasksFromQueue() {
        if (!ts.isEmpty()) {
            handlerFree = false;
            auto e = ts.popFront();
            std::cout<<"Очередь не пуста"<<std::endl;
            std::cout<<"Взята задача ["<<e->startTime<<" "<<e->processTime<<" "<<"START]"<<std::endl;
            std::cout<<"Ее завершение назначено на: "<<currentTime+e->processTime<<std::endl<<std::endl;
            if (e) {
                Task taskStop(currentTime+e->processTime, e->processTime, STOP);
                c.insertByAsc(taskStop);
            }
        }
    }
    void printStats() const {
        std::cout<<"Превышено время наблюдения"<<std::endl;
        std::cout<<"Завершено задач: "<<completedTasksAmount<<std::endl;
        std::cout<<"Среднее время обработки: "<<averageProcessTime/float(completedTasksAmount)<<std::endl;
        std::cout<<"Задач в очереди: "<<ts.length()<<std::endl;
    }
private:
    int currentTime = 0;
    int monitoringTime = 100;
    bool handlerFree = true;
    int completedTasksAmount = 0;
    float averageProcessTime = 0;
    Deque<Task> ts;
    Calendar<Task> c;
};

void tests() {
    Deque<int> d;

    /* Тест 1. Простая работа с дэком без граничных случаев */
    std::cout<<"-Test 1-"<<std::endl;

    d.pushFront(3);
    d.pushFront(2);
    d.pushFront(1);

    auto v = d.popFront();
    if (v) std::cout<<*v<<std::endl;
    v = d.popFront();
    if (v) std::cout<<*v<<std::endl;
    v = d.popFront();
    if (v) std::cout<<*v<<std::endl;

    std::cout<<"-Test 1 End-"<<std::endl<<std::endl;
    /* Конец теста 1. */

    /* Тест 2. Проверка на доступность элемента */
    std::cout<<"-Test 2-"<<std::endl;

    d.pushBack(4);
    d.pushBack(5);
    d.pushBack(6);

    auto b = d.popBack();
    if (b) std::cout<<*b<<std::endl;
    b = d.popBack();
    if (b) std::cout<<*b<<std::endl;
    b = d.popBack();
    if (b) std::cout<<*b<<std::endl;
    b = d.popBack(); // Программа не выдает ошибку, даже если в очереди уже нет элементов
    /* Пример, как проверить, пуста ли очередь */
    if (b) std::cout<<*b<<std::endl;
    else std::cout<<"Deque is empty now"<<std::endl;

    std::cout<<"-Test 2 End-"<<std::endl<<std::endl;
    /* Конец теста 2. */

    /* Тест 3. Проверка на изъятие элементов с одной стороны в большем количестве , чем было положено */
    std::cout<<"-Test 3-"<<std::endl;

    d.pushFront(9);
    d.pushFront(8);
    d.pushFront(7);
    d.pushBack(10);

    auto f = d.popFront();
    if (f) std::cout<<*f<<std::endl;
    f = d.popFront();
    if (f) std::cout<<*f<<std::endl;
    f = d.popFront();
    if (f) std::cout<<*f<<std::endl;
    f = d.popFront(); // Достает элемент из tail, поскольку в head ничего нет
    if (f) std::cout<<*f<<std::endl;
    f = d.popFront(); // Пустая очередь
    if (f) std::cout<<*f<<std::endl;
    else std::cout<<"Deque is empty now"<<std::endl;

    std::cout<<"-Test 3 End-"<<std::endl<<std::endl;
    /* Конец теста 3. */

    /* Тест 4. Запись в цикле, вывод */
    std::cout<<"-Test 4-"<<std::endl;

    for (int i = 0; i < 10; ++i)
        if (i % 2 == 0) d.pushFront(i);
        else d.pushBack(i);

    // Вид очереди: 8 6 4 2 0 1 3 5 7 9

    while (true) {
        auto a = d.popFront();
        auto c = d.popBack();

        if (a) std::cout<<*a<<std::endl;
        else break;
        if (c) std::cout<<*c<<std::endl;
        else break;
    }

    std::cout<<"-Test 4 End-"<<std::endl<<std::endl;
    /* Конец теста 4. */
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    //tests();
    SMO smo;
    smo.start();
}
