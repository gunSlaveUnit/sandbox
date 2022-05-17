#include <cstdint>
#include <random>
#include <iostream>

template<class T>
struct Node {
    explicit Node(T k) :
            k(k),
            l(nullptr),
            r(nullptr), h(1) {}

    T k;
    uint_fast32_t h;
    Node *l, *r;
};

template<class T>
uint_fast32_t height(Node<T> *root) {
    return root ? root->h : 0;
}

template<class T>
int_fast32_t balance_factor(Node<T> *root) {
    return height(root->r) - height(root->l);
}

template<class T>
void repair_height(Node<T> *root) {
    uint_fast32_t hl = height(root->l);
    uint_fast32_t hr = height(root->r);
    root->h = (hl > hr ? hl : hr) + 1;
}

template<class T>
Node<T> *rl(Node<T> *around) {
    Node<T> *t = around->r;
    around->r = t->l;
    t->l = around;

    repair_height(around);
    repair_height(t);

    return t;
}

template<class T>
Node<T> *rr(Node<T> *around) {
    Node<T> *t = around->l;
    around->l = t->r;
    t->r = around;

    repair_height(around);
    repair_height(t);

    return t;
}

template<class T>
Node<T> *balance(Node<T> *what) {
    repair_height(what);

    if (balance_factor(what) == 2) {
        if (balance_factor(what->r) < 0)
            what->r = rr(what->r);
        return rl(what);
    }

    if (balance_factor(what) == -2) {
        if (balance_factor(what->l) > 0)
            what->l = rl(what->l);
        return rr(what);
    }

    return what;
}

template<class T>
Node<T> *insert(Node<T> *root, T key) {
    if (!root)
        return new Node<T>(key);

    if (key < root->k)
        root->l = insert(root->l, key);
    else
        root->r = insert(root->r, key);

    return balance(root);
}

template<class T>
Node<T> *find_min(Node<T> *root) {
    return root->l ? find_min(root->l) : root;
}

template<class T>
Node<T> *remove_min(Node<T> *root) {
    if (root->l == nullptr)
        return root->r;

    root->l = remove_min(root->l);

    return balance(root);
}

template<class T>
Node<T> *remove(Node<T> *root, T key) {
    if (root == nullptr)
        return nullptr;

    if (key < root->k)
        root->l = remove(root->l, key);
    else if (key > root->k)
        root->r = remove(root->r, key);
    else {
        Node<T> *q = root->l;
        Node<T> *r = root->r;
        delete root;

        if (r == nullptr)
            return q;

        Node<T> *min = find_min(r);
        min->r = remove_min(r);
        min->l = q;

        return balance(min);
    }

    return balance(root);
}

template<class T>
Node<T> *search(Node<T> *root, T key) {
    if (root == nullptr)
        return root;

    while (root->key != key) {
        if (key < root->key)
            root = root->l;
        else
            root = root->r;

        if (root == nullptr)
            break;
    }
    return root;
}

template<class T>
void clear(Node<T> *root) {
    if (root != nullptr) {
        clear(root->l);
        clear(root->r);
        delete root;
    }
}

template<class T>
void print(Node<T> *root, int space = 0) {
    if (root == nullptr)
        return;

    enum {
        COUNT = 2
    };
    space += COUNT;

    print(root->r, space);

    for (int i = COUNT; i < space; ++i)
        std::cout << "  ";

    std::cout << root->k << std::endl;

    print(root->l, space);
}

int main() {
    uint_fast32_t amount;
    std::cout << "Enter an amount of keys in tree: ";
    std::cin >> amount;

    int_fast32_t low, up;
    std::cout << "Enter a lower bound of tree keys: ";
    std::cin >> low;
    std::cout << "Enter a upper bound of tree keys: ";
    std::cin >> up;

    std::random_device os_seed;
    const uint_fast32_t seed = os_seed();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int_fast32_t> distribute(low, up);

    Node<int_fast32_t> *tree = nullptr;
    for (int_fast32_t i = 0; i < amount; ++i)
        tree = insert(tree, distribute(generator));

    std::cout << std::endl;
    print(tree);
    std::cout << std::endl;

    std::cout << "Enter a key to delete it: ";
    int_fast32_t itemToDelete;
    std::cin >> itemToDelete;
    tree = remove(tree, itemToDelete);
    std::cout << std::endl;

    print(tree);

    clear(tree);
}
