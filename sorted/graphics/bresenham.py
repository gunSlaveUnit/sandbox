"""Rasterization using the Bresenham algorithm."""

from collections import namedtuple

Point = namedtuple('Point', ['x', 'y'])


def draw_point(p: Point) -> None:
    print(f'x={p.x} y={p.y}')


p0 = Point(1, 2)
p1 = Point(5, 22)

dx = p1.x - p0.x
dy = p1.y - p0.y

error_rate = 2 * dy - dx
x, y = p0
for _ in range(0, dx):
    draw_point(Point(x, y))

    x += 1

    error_rate += dy

    if error_rate > dx:
        error_rate -= dx
        y -= 1
