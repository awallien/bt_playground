__file__ = "skyscrapers.py"
__author__ = "awallien"
__revision__ = "1.0"
__doc__ = "skyscraper solver using the backtracking algorithm"

import sys
import tkinter


class Skyscrapers:
    def __init__(self, config_file):
        at_config = 0
        with open(config_file, "r") as config:
            for line in config:
                line = line.split()
                if not line or "#" in line:
                    continue
                if at_config == 0:
                    self.length, self.width = map(int, line)
                    self.board = [[0 for _ in range(self.width)] for _ in range(self.length)]
                elif at_config == 1:
                    self.north = list(map(int, line))
                elif at_config == 2:
                    self.south = list(map(int, line))
                elif at_config == 3:
                    self.east = list(map(int, line))
                elif at_config == 4:
                    self.west = list(map(int, line))
                at_config += 1

    def mark(self, row, col, number):
        self.board[row][col] = number


class SkyscrapersSolver:
    def __init__(self):
        pass

    def is_goal(self):
        pass

    def is_valid(self):
        pass

    def bt(self):
        pass


class SkyscrapersUI:
    def __init__(self, board):
        self.window = self._mk_window()
        self.sky_board = board

    def _mk_window(self):
        w = tkinter.Tk()
        w.title("Skyscrapers")
        return w

    def run(self):
        self.window.mainloop()


if __name__ == '__main__':
    pass