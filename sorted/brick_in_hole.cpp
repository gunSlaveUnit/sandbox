/*
* Learning task:
* Does a brick of a certain size fit into a given hole?
*/

#include <iostream>

struct Hole {
    double width, height;
};

struct Brick {
    bool fits_into(const Hole &hole) const {
        auto x = width;
        auto y = height;
        auto z = length;

        auto a = hole.width;
        auto b = hole.height;

        return (x <= a && (y <= b || z <= b) ||
                y <= a && (x <= b || z <= b) ||
                z <= a && (x <= b || y <= b));
    }

    double width, height, length;
};

int main() {
    auto brick = Brick(5, 5, 5);
    auto hole = Hole(5, 5);

    std::cout << brick.fits_into(hole) << std::endl;
}