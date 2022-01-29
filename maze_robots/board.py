import pygame
from constants import *
import random
import numpy as np

from robot import Robot



class Character:
    def __init__(self, x, y):
        self.pos_x = x
        self.pos_y = y


class Board:
    def __init__(self):
        self.characters = []
        self.robots_red = []
        self.robots_blue = []
        self.characters_queue = []
        self.board = np.zeros((COLS, ROWS), dtype=int)
        self.is_fin = False

    def generate_maze(self, freq):
        for row in range(ROWS):
            for col in range(COLS):
                if 1 == random.randrange(0, freq):
                    if (col != SPAWN_X and row != SPAWN_Y) or (col != ROWS and col != COLS):
                        self.board[col][row] = 1

    def place_character(self, x, y):
        c = Character(x, y)
        self.characters.append(c)
        self.characters_queue.append(c)

    def remove_character(self, x, y):
        for i in range(len(self.characters)):
            if self.characters[i].pos_x == x and self.characters[i].pos_y == y:
                self.characters.remove(self.characters[i])
                break

    def generate_robots(self, cnt):
        for i in range(cnt):
            self.robots_red.append(Robot(SPAWN_X, SPAWN_Y))
            self.robots_blue.append(Robot(SPAWN_X, SPAWN_Y))

    def generate_characters(self, cnt):
        for i in range(cnt):
            while True:
                pos_x = random.randrange(0, COLS)
                pos_y = random.randrange(0, ROWS)
                if not self.pos_has_character(pos_x, pos_y) and self.board[pos_x][pos_y] != 1:
                    break
            self.place_character(pos_x, pos_y)

    def move_robot(self, robot):
        if robot.is_finished:
            return
        if len(robot.path) == 0:
            if robot.is_coming_back:
                if len(self.characters_queue) == 0:
                    robot.is_finished = True
                    return
                robot.goal_character = self.characters_queue.pop()
            robot.get_path(self.board)
            return
        else:
            robot.step()
            #if robot.goal_character == None:
            #    return
            if robot.pos_x == robot.goal_character.pos_x and robot.pos_y == robot.goal_character.pos_y:
                self.remove_character(robot.pos_x, robot.pos_y)

    def move_robots(self):
        if not self.is_fin:
            fin_red = 0
            fin_blue = 0
            for robot in self.robots_red:
                self.move_robot(robot)
                if robot.is_finished:
                    fin_red += 1
            #print("FIN", fin_red)
            for robot in self.robots_blue:
                self.move_robot(robot)
                if robot.is_finished:
                    fin_blue += 1
            if fin_red == ROBOT_NUM:
                print("RED IS FINISHED")
            if fin_blue == ROBOT_NUM:
                print("BLUE IS FINISHED")
            if fin_red + fin_blue == CHARACTER_NUM:
                print("FINISHED")
                self.is_fin = True

    def pos_has_character(self, x, y):
        for character in self.characters:
            if character.pos_x == x and character.pos_y == y:
                return True
        return False

    def draw_board(self, win):
        win.fill(BLACK)
        for row in range(ROWS):
            for col in range(COLS):
                if self.board[col][row] == 1:
                    pygame.draw.rect(win, BLACK, (row * SQUARE_SIZE, col * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))
                else:
                    pygame.draw.rect(win, WHITE, (row * SQUARE_SIZE, col * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))
        pygame.draw.rect(win, (180, 100, 100), (SPAWN_Y * SQUARE_SIZE, SPAWN_X * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))

    def draw_characters(self, win):
        for character in self.characters:
            pygame.draw.circle(win, YELLOW, (character.pos_y * SQUARE_SIZE + SQUARE_SIZE/2,
                                           character.pos_x * SQUARE_SIZE + SQUARE_SIZE/2), SQUARE_SIZE/2)

    def draw_robot(self, win, robot, COLOR):
        pygame.draw.rect(win, COLOR, (robot.pos_y * SQUARE_SIZE, robot.pos_x * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))
        if robot.is_coming_back:
            pygame.draw.circle(win, YELLOW, (robot.pos_y * SQUARE_SIZE + SQUARE_SIZE / 2,
                                             robot.pos_x * SQUARE_SIZE + SQUARE_SIZE / 2), SQUARE_SIZE / 2)
    def draw_robots(self, win):
        for robot in self.robots_red:
            self.draw_robot(win, robot, RED)
        for robot in self.robots_blue:
            self.draw_robot(win, robot, BLUE)

