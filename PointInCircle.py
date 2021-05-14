# !usr/bin/env python3
# -*- coding: utf-8 -*-

class Point:
    def __init__(self, x=0.0, y=0.0):
        self.x = x
        self.y = y

    def set_coordinates(self):
        self.x = float(input("X = "))
        self.y = float(input("Y = "))


class Circle:
    def __init__(self, radius=0.0):
        self.radius = radius

    def set_radius(self):
        self.radius = float(input("R = "))


if __name__ == '__main__':
    def check_point_in_circle(point_object, circle_object):
        return point.x**2 + point.y**2 <= circle.radius**2


    point = Point()
    point.set_coordinates()

    circle = Circle()
    circle.set_radius()

    if check_point_in_circle(point, circle):
        print('Yes')
    else:
        print('No')
