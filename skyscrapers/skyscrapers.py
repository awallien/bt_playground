__file__ = "skyscrapers.py"
__author__ = "awallien"
__revision__ = "1.0"
__doc__ = "skyscraper solver using the backtracking algorithm"

import sys
import tkinter


class Skyscrapers:
    def __init__(self, config_file):
        self.dim = None
        self.board = []

        self.is_marked = []

        self.hints = {}

        self.__read_config_file(config_file)

        assert (all(vars(self).values())), "all values for skyscrapers puzzle are not instantiated"

    def __read_config_file(self, config_file):
        config = open(config_file)
        line_no = 0
        while True:
            line = config.readline()
            line_no += 1
            if not line:
                break
            if line == '\n':
                continue
            if not self.dim:
                self.dim = int(config.readline().strip())
            elif not self.board and not self.is_marked and '.' in line:
                for i in range(self.dim):
                    row = line.split()
                    if len(row) < self.dim:
                        raise ValueError(f"Configuration file contains invalid width size on line {line_no}")
                    self.board.append(list(map(lambda x: 0 if x == '.' else int(x), row)))
                    self.is_marked.append(list(map(lambda x: False if x == '.' else True, row)))
                    line = config.readline()
                if sum(self.board) < self.dim or sum(self.board) > self.dim:
                    raise ValueError("Configuration file contains incorrect number of row markings")
            elif not self.board and not self.is_marked:
                self.board = [[0 for _ in range(self.dim)] for _ in range(self.dim)]
                self.is_marked = [[False for _ in range(self.dim) for _ in range(self.dim)]]
            elif line.startswith("north: "):
                self.hints["north"] = list(map(int, line.split()[1:]))
            elif line.startswith("east: "):
                self.hints["east"] = list(map(int, line.split()[1:]))
            elif line.startswith("west: "):
                self.hints["west"] = list(map(int, line.split()[1:]))
            elif line.startswith("south: "):
                self.hints["south"] = list(map(int, line.split()[1:]))
        config.close()

    def mark(self, row, col, number):
        self.board[row][col] = number

    def __str__(self):
        pass


class SkyscrapersSolver:
    def __init__(self, board):
        self.board = board

    def is_goal(self):
        pass

    def is_valid(self):
        return True

    def bt(self):
        pass


class SkyscrapersUI:
    def __init__(self, solver):
        self.window = self.__mk_window()
        self.solver = solver

    def __mk_window(self):
        w = tkinter.Tk()
        w.title("Skyscrapers")
        return w

    def run(self):
        self.window.mainloop()


if __name__ == '__main__':
    Skyscrapers("asdf")
