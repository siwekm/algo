import pygame
from constants import *
from board import Board

def game_loop():
    WIN = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Bludistaci")
    run = True
    clock = pygame.time.Clock()
    board = Board()

    board.generate_maze(7)
    board.generate_characters(CHARACTER_NUM)
    board.generate_robots(ROBOT_NUM)

    while run:
        clock.tick(FPS)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            if event.type == pygame.MOUSEBUTTONDOWN:
                pass
        board.move_robots()
        board.draw_board(WIN)
        board.draw_characters(WIN)
        board.draw_robots(WIN)
        pygame.display.update()

    pygame.quit()

if __name__ == '__main__':
    game_loop()



