# ! usr/bin/env python3
# -*- coding: utf8 -*-

import pygame

WINDOW_WIDTH = 1200
WINDOW_HEIGHT = 800
WINDOW_CAPTION = "PingPong"

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

FPS_LOCKING = 60


class Platform:
    def __init__(self, x, y, length, speed):
        self._x = x
        self._y = y
        self._length = length
        self._speed = speed

    def move(self):
        """
        A hook for motion
        :return: None
        """

    @property
    def position(self):
        return self._x, self._y

    @property
    def length(self):
        return self._length


class PlayerPlatform(Platform):
    def __init__(self, x, y, length, speed):
        super().__init__(x, y, length, speed)

    def move(self):
        pressed_keys = pygame.key.get_pressed()
        if pressed_keys[pygame.K_UP] or pressed_keys[pygame.K_w]:
            self._y -= self._speed
        if pressed_keys[pygame.K_DOWN] or pressed_keys[pygame.K_s]:
            self._y += self._speed


class EnemyPlatform(Platform):
    def __init__(self, x, y, length, speed):
        super().__init__(x, y, length, speed)

    def follow_ball(self, ball_position):
        if ball_position[1] > self._y:
            self._y += self._speed
        else:
            self._y -= self._speed


class Ball:
    def __init__(self, x, y, radius, speed):
        self._x = x
        self._y = y
        self._radius = radius
        self._x_speed = speed
        self._y_speed = speed

    def move(self):
        if self._radius // 2 < self._x < WINDOW_WIDTH - self._radius // 2:
            self._x += self._x_speed
        else:
            self._x_speed = -self._x_speed
            self._x += self._x_speed

        if self._radius // 2 < self._y < WINDOW_HEIGHT - self._radius // 2:
            self._y += self._y_speed
        else:
            self._y_speed = -self._y_speed
            self._y += self._y_speed

    def check_collision_with_player_platform(self, platform):
        if (platform.position[1] - platform.length // 2 + self._radius // 2 < self._y <
                platform.position[1] + platform.length // 2 - self._radius // 2) and \
                (self._x <= platform.position[0] + self._radius // 2):
            self._x_speed *= -1

    def check_collision_with_enemy_platform(self, platform):
        if (platform.position[1] - platform.length // 2 + self._radius // 2 < self._y <
                platform.position[1] + platform.length // 2 - self._radius // 2) and \
                (self._x >= platform.position[0] - self._radius // 2):
            self._x_speed *= -1

    @property
    def position(self):
        return self._x, self._y

    @property
    def radius(self):
        return self._radius

    @property
    def x_speed(self):
        return self._x_speed

    @x_speed.setter
    def x_speed(self, new_value):
        self._x_speed = new_value

    @property
    def y_speed(self):
        return self._y_speed

    @y_speed.setter
    def y_speed(self, new_value):
        self._y_speed = new_value


if __name__ == '__main__':
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption(WINDOW_CAPTION)

    player_platform = PlayerPlatform(100, WINDOW_HEIGHT // 2, 50, 4)
    enemy_platform = EnemyPlatform(WINDOW_WIDTH - 100, WINDOW_HEIGHT // 2, 50, 4)
    ball = Ball(WINDOW_WIDTH // 2, WINDOW_HEIGHT // 2, 10, 2)

    clock = pygame.time.Clock()

    up_ball_speed_interval = 10000
    last_up_ball_speed_time = pygame.time.get_ticks()

    game_is_running = True
    while game_is_running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_is_running = False
                exit(0)

        window.fill(BLACK)

        pygame.draw.line(window, WHITE, (player_platform.position[0],
                                         player_platform.position[1] - player_platform.length // 2),
                         (player_platform.position[0],
                          player_platform.position[1] + player_platform.length // 2))
        pygame.draw.line(window, WHITE, (enemy_platform.position[0],
                                         enemy_platform.position[1] - enemy_platform.length // 2),
                         (enemy_platform.position[0],
                          enemy_platform.position[1] + player_platform.length // 2))

        pygame.draw.line(window, WHITE, (WINDOW_WIDTH // 2, 0), (WINDOW_WIDTH // 2, WINDOW_HEIGHT))
        pygame.draw.circle(window, WHITE, ball.position, ball.radius)

        player_platform.move()
        enemy_platform.follow_ball(ball.position)
        ball.move()
        ball.check_collision_with_player_platform(player_platform)
        ball.check_collision_with_enemy_platform(enemy_platform)

        now = pygame.time.get_ticks()
        if now - last_up_ball_speed_time > up_ball_speed_interval:
            last_up_ball_speed_time = now
            if ball.x_speed < 0:
                ball.x_speed -= 1
            else:
                ball.x_speed += 1
            if ball.y_speed < 0:
                ball.y_speed -= 1
            else:
                ball.y_speed += 1

        pygame.display.flip()
        clock.tick(FPS_LOCKING)
