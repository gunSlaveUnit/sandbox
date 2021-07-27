import random

import pygame

"""Window Settings"""
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 800
WINDOW_CAPTION = "SNAKE"

"""Colors"""
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)


class Area:
    """Describes the playing field where the snake moves"""

    MIN_INTERVAL_GENERATE_FOOD = 100
    MAX_INTERVAL_GENERATE_FOOD = 1000

    def __init__(self, amount_vert_horiz_cells=10):
        if amount_vert_horiz_cells < 0 or not isinstance(amount_vert_horiz_cells, int):
            raise ValueError("The length of the snake is a non-negative number")
        if not isinstance(amount_vert_horiz_cells, int):
            raise TypeError("The length of the snake is an integer")

        self.amount_cells = amount_vert_horiz_cells
        self.distance_between_lines = WINDOW_WIDTH // self.amount_cells
        self.food = []
        self.interval_generate_new_food = random.randint(Area.MIN_INTERVAL_GENERATE_FOOD, Area.MAX_INTERVAL_GENERATE_FOOD)
        self.last_time_generate_food = pygame.time.get_ticks()

    def generate_random_food(self):
        now = pygame.time.get_ticks()
        if now - self.last_time_generate_food > self.interval_generate_new_food:
            self.last_time_generate_food = now
            self.interval_generate_new_food = random.randint(Area.MIN_INTERVAL_GENERATE_FOOD, Area.MAX_INTERVAL_GENERATE_FOOD)
            self.food.append([random.randint(1, self.amount_cells + 1), random.randint(1, self.amount_cells + 1)])

    def draw(self, screen):
        '''
        for x in range(area.distance_between_lines, WINDOW_WIDTH, area.distance_between_lines):
            pygame.draw.line(screen, WHITE, (x, 0), (x, WINDOW_HEIGHT))

        for y in range(area.distance_between_lines, WINDOW_HEIGHT, area.distance_between_lines):
            pygame.draw.line(screen, WHITE, (0, y), (WINDOW_WIDTH, y))
        '''
        for food in self.food:
            pygame.draw.rect(screen, RED, (food[0]*self.distance_between_lines, food[1]*self.distance_between_lines, 40, 40), 1)

    def get_food(self):
        return self.food


class Snake:
    """"""
    def __init__(self, x_cell=0, y_cell=0, length=1, width=WINDOW_WIDTH // 10):
        if length < 0 or not isinstance(length, int):
            raise ValueError("The length of the snake is a non-negative number")
        if not isinstance(length, int):
            raise TypeError("The length of the snake is an integer")

        self.length = length
        self.width = width
        self.head = [x_cell, y_cell]
        self.body = []
        self.move_interval = 100
        self.last_move = pygame.time.get_ticks()
        self.direction = 1

    def move(self):
        def update_bod_relative_head():
            if len(self.body) >= 1:
                for i in range(len(self.body)-1, 0, -1):
                    self.body[i][0] = self.body[i - 1][0]
                    self.body[i][1] = self.body[i - 1][1]
                self.body[0][0] = self.head[0]
                self.body[0][1] = self.head[1]

        pressed_keys = pygame.key.get_pressed()
        if pressed_keys[pygame.K_LEFT] and self.direction != 1:
            self.direction = 0
        if pressed_keys[pygame.K_RIGHT] and self.direction != 0:
            self.direction = 1
        if pressed_keys[pygame.K_UP] and self.direction != 3:
            self.direction = 2
        if pressed_keys[pygame.K_DOWN] and self.direction != 2:
            self.direction = 3

        now = pygame.time.get_ticks()
        if now - self.last_move >= self.move_interval:
            self.last_move = now

            if self.direction == 0:
                update_bod_relative_head()
                self.head[0] -= 1
            if self.direction == 1:
                update_bod_relative_head()
                self.head[0] += 1
            if self.direction == 2:
                update_bod_relative_head()
                self.head[1] -= 1
            if self.direction == 3:
                update_bod_relative_head()
                self.head[1] += 1

    def grow(self):
        if not self.body:
            if self.direction == 0:
                self.body.append([self.head[0]+1, self.head[1]])
            if self.direction == 1:
                self.body.append([self.head[0] - 1, self.head[1]])
            if self.direction == 2:
                self.body.append([self.head[0], self.head[1]+1])
            if self.direction == 3:
                self.body.append([self.head[0], self.head[1]-1])
        else:
            if self.direction == 0:
                self.body.append([self.body[-1][0]+1, self.body[-1][1]])
            if self.direction == 1:
                self.body.append([self.body[-1][0] - 1, self.body[-1][1]])
            if self.direction == 2:
                self.body.append([self.body[-1][0], self.body[-1][1]+1])
            if self.direction == 3:
                self.body.append([self.body[-1][0], self.body[-1][1]-1])

    def draw(self, screen):
        x0, y0 = self.head[0] * self.width, self.head[1] * self.width
        pygame.draw.rect(screen, GREEN, (x0, y0, self.width, self.width), 1)

        for body_cell in self.body:
            x0, y0 = body_cell[0] * self.width, body_cell[1] * self.width
            pygame.draw.rect(screen, GREEN, (x0, y0, self.width, self.width), 1)


if __name__ == '__main__':
    pygame.init()

    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption(WINDOW_CAPTION)

    area = Area(20)
    snake = Snake(width=40)

    clock = pygame.time.Clock()
    game_is_running = True
    while game_is_running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit(0)
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    exit(0)

        window.fill(BLACK)

        area.generate_random_food()
        area.draw(window)
        snake.draw(window)

        for food in area.food:
            if snake.head[0] == food[0] and snake.head[1] == food[1]:
                snake.grow()
                area.food.remove(food)
        snake.move()

        pygame.display.flip()
        clock.tick(60)
