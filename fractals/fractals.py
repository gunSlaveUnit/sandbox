import turtle

t = turtle.Turtle()
t.speed(100)
t.ht()


def draw_koch_curve(segment_length):
    if segment_length > 6:
        draw_koch_curve(segment_length // 3)
        t.left(60)
        draw_koch_curve(segment_length // 3)
        t.right(120)
        draw_koch_curve(segment_length // 3)
        t.left(60)
        draw_koch_curve(segment_length//3)
    else:
        t.forward(segment_length)
        t.left(60)
        t.forward(segment_length)
        t.right(120)
        t.forward(segment_length)
        t.left(60)
        t.forward(segment_length)


def draw_koch_snowflake(segment_length):
    draw_koch_curve(segment_length)
    t.right(120)
    draw_koch_curve(segment_length)
    t.right(120)
    draw_koch_curve(segment_length)


def draw_harter_heighway_dragon(segment_length, amount_iterations):
    t.forward(segment_length)
    t.left(90)


if __name__ == '__main__':
    # draw_koch_curve(150)
    draw_koch_snowflake(150)
    turtle.done()
