/*
One long evening I decided that I needed a calculator.
Below is a program that is able to solve simple examples of the form (a + b) * (c / d)
*/

#include <map>
#include <string>
#include <iostream>
#include <algorithm>

std::map<char, int> OPERATOR_WITH_PRIORITY {
        {'+', 2}, {'-', 2},
        {'*', 3}, {'/', 3},
        {'^', 4},
};

std::map<char, double> VALUES {};

template<class T>
class Tree {
public:
    Tree<T>* New(T data)
    {Tree<T>* V;
        V=(Tree*)malloc (sizeof(Tree<T>));
        V->value=data;
        V->left=V->right=nullptr;
        return V;
    };
    Tree<T>* Add(Tree<T>* v, T data)
    {
        if(v==nullptr) return New(data);
        if(v->left == nullptr ) v->left=Add(v->left, data);
        else if(v->right == nullptr) v->right = Add(v->right, data);
        return v;
    };
    void setLeft(Tree<T>* v) { left = v; }
    void setRight(Tree<T>* v) { right = v; }
    void Print(Tree<T>* v)
    { static int n=0;
        if ( !v ) return;
        n++;
        Print(v->right);
        std::cout<<"Level "<<n<<" "<<v->value<<std::endl;
        Print(v->left);
        n--;
    }
    double Count(Tree<T>* v) {
        if(v) {
            if (!v->left && !v->right) {
                if (!VALUES.count(v->value)) {
                    double numericValue = 0.0;
                    std::cout<<v->value<<"=";
                    std::cin>>numericValue;
                    VALUES.insert(std::pair<char, double>(v->value, numericValue));
                    return numericValue;
                } else return VALUES.at(v->value);
            }
            double leftval = Count(v->left);
            double rightval = Count(v->right);
            switch (v->value) {
                case '+':
                    return leftval + rightval;
                case '-':
                    return leftval - rightval;
                case '*':
                    return leftval * rightval;
                case '/':
                    return leftval / rightval;
                case '^':
                    return pow(leftval, rightval);
            }
        }
    }
private:
    T value;
    Tree<T>* left;
    Tree<T>* right;
};

int countBrackets(std::string exp) {
    int amount = 0;
    for(auto c : exp) if (c == '(' || c == ')') ++amount;
    return amount;
}

Tree<char>* tree = nullptr;

Tree<char>* fillTree(std::string expression) {
    int minimumPriorityOperation = 10;
    int indexMinimumOperation = -1;
    int counterCheckBrackets = 0;
    for (int i = 0; i < expression.length(); ++i) {
        if (expression[i] == '(') ++counterCheckBrackets;
        if (expression[i] == ')') --counterCheckBrackets;
        if (!counterCheckBrackets && expression[i] != ')' && OPERATOR_WITH_PRIORITY[expression[i]] !=0 &&
            (OPERATOR_WITH_PRIORITY[expression[i]] < minimumPriorityOperation))
        {
            minimumPriorityOperation = OPERATOR_WITH_PRIORITY[expression[i]];
            indexMinimumOperation = i;
        }
    }
    Tree<char>* newSubTree = nullptr;
    newSubTree = newSubTree->Add(newSubTree, expression[indexMinimumOperation]);

    std::string l = expression.substr(0, indexMinimumOperation);
    std::string r = expression.substr(1 + indexMinimumOperation, expression.length());

    if (l[0] == '(' && l[l.length()-1] == ')' && countBrackets(l) == 2) {
        l.erase(0, 1);
        l.erase(l.size() - 1);
    }
    if (r[0] == '(' && r[r.length()-1] == ')' && countBrackets(r) == 2) {
        r.erase(0, 1);
        r.erase(r.size() - 1);
    }

    if (l.length() == 1) newSubTree = newSubTree->Add(newSubTree, l[0]);
    else newSubTree->setLeft(fillTree(l));
    if (r.length() == 1) newSubTree = newSubTree->Add(newSubTree, r[0]);
    else newSubTree->setRight(fillTree(r));
    return newSubTree;
}

int main() {
    std::string expression;
    std::cout<<"Enter expression: "<<std::endl;
    std::getline(std::cin, expression);
    expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());
    tree = fillTree(expression);
    double result = tree->Count(tree);
    std::cout<<expression<<"="<<result<<std::endl;
}