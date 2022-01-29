from collections import deque

from constants import SPAWN_X, SPAWN_Y, COLS


class Robot:
    def __init__(self, x, y):
        self.pos_x = x
        self.pos_y = y
        self.stack = []
        self.parent = dict()
        self.visited = set()
        self.path = []
        self.is_coming_back = False
        self.goal_character = None
        self.is_finished = False
        self.spawn = None

    def get_neighbours(self, map, node):
        ngb = []
        if node['x'] - 1 >= 0:
            if map[node['x'] - 1][node['y']] == 0:
                ngb.append({'x': node['x'] - 1, 'y': node['y']})
        try:
            if map[node['x'] + 1][node['y']] == 0:
                ngb.append({'x': node['x'] + 1, 'y': node['y']})
        except:
            pass
        try:
            if map[node['x']][node['y'] - 1] == 0:
                ngb.append({'x': node['x'], 'y': node['y'] - 1})
        except:
            pass
        try:
            if map[node['x']][node['y'] + 1] == 0:
                ngb.append({'x': node['x'], 'y':  node['y'] + 1})
        except:
            pass

        return ngb

    def get_path(self, board):
        if self.is_coming_back:
            self.path = self.dfs(board, {'x': self.pos_x, 'y': self.pos_y}, {'x': self.goal_character.pos_x, 'y': self.goal_character.pos_y})
            self.is_coming_back = False
        else:
            self.path = self.dfs(board, {'x': self.pos_x, 'y': self.pos_y}, {'x': SPAWN_X, 'y': SPAWN_Y})
            self.is_coming_back = True

    def step(self):
        pos = self.to_vec(self.path.pop(0))
        self.pos_x = pos['x']
        self.pos_y = pos['y']

    def reconstruct_path(self, end_node, parent):
        path = []
        current_path = end_node
        cnt = 0
        while current_path in parent:
            path.append(current_path)
            current_path = parent[current_path]
            cnt = cnt + 1
        path.reverse()
        #print(path)
        return path

    def to_coord(self, vec):
        return vec['y'] * COLS + vec['x']

    def to_vec(self, coord):
        return {'x': coord % COLS, 'y': coord // COLS}

    def dfs(self, board, start_node, end_node):
        qu = deque()
        parent = dict()
        visited = set()

        qu.append(self.to_coord(start_node))
        visited.add(self.to_coord(start_node))

        while len(qu) > 0:
            current = qu.popleft()

            if current == end_node:
                print("Path found")
                break

            ngb = self.get_neighbours(board, self.to_vec(current))
            for nextVec in ngb:
                neighbour = self.to_coord(nextVec)

                if neighbour not in visited:
                    visited.add(neighbour)
                    parent[neighbour] = current
                    qu.append(neighbour)

        return self.reconstruct_path(self.to_coord(end_node), parent)