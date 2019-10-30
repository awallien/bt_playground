__file__ = "skyscrapers.py"
__author__ = "awallien"
__revision__ = "1.0"
__doc__ = "skyscraper solver using the backtracking algorithm"

import json
import sys
import tkinter


class Skyscrapers:
    def __init__(self, config_fp):
        self.dim = None
        self.rows = []
        self.columns = []

        self.is_marked = []
        self.hints = {"north": [], "south": [], "east": [], "west": []}

        self.__read_config_file(config_fp)
        self.__assert_values()

    def __assert_values(self):
        assert self.dim is not None, "dimension of skyscraper is not defined"
        assert self.rows, "rows are not properly instantiated"
        assert self.columns, "columns are not properly instantiated"
        assert self.is_marked, "marked board is not properly instantiated"
        assert len(self.hints.keys()) == 4, "not all hints have been read"
        for title, hint in self.hints.items():
            assert (len(hint) == self.dim), f"{title} hints do not have the right amount of hints"

    def __read_config_file(self, config_fp):
        config_json = json.loads(config_fp)
        self.dim = config_json['size']
        self.hints = config_json['hints']

        self.rows = [([0] * self.dim) for _ in range(self.dim)]
        self.columns = [([0] * self.dim) for _ in range(self.dim)]
        self.is_marked = [([False] * self.dim) for _ in range(self.dim)]
        for mark in config_json['marks']:
            mark_number = mark['number']
            mark_loc = mark['location']
            self.rows[mark_loc[0]][mark_loc[1]] = mark_number
            self.columns[mark_loc[1]][mark_loc[0]] = mark_number
            self.is_marked[mark_loc[0]][mark_loc[1]] = True

    def mark(self, row, col, number):
        self.rows[row][col] = number
        self.columns[col][row] = number

    def get_row_number(self, on):
        return on // self.dim

    def get_col_number(self, on):
        return on % self.dim

    def get_row_contents(self, row):
        return self.rows[row]

    def get_column_contents(self, column):
        return self.columns[column]

    def get_hints(self, row, col):
        return (
            self.hints['north'][col],
            self.hints['south'][col],
            self.hints['east'][row],
            self.hints['west'][row]
        )

    def __str__(self):
        res = ""
        for i in range(self.dim):
            for j in range(self.dim):
                res += f"{self.rows[i][j]} "
            res += "\n"

        res += f"north: {self.hints['north']}\n"
        res += f"south: {self.hints['south']}\n"
        res += f"east:  {self.hints['east']}\n"
        res += f"west:  {self.hints['west']}\n"

        return res


class SkyscrapersSolver:
    def __init__(self, skyscrapers):
        self.skyscrapers = skyscrapers
        self.solved = False
        self.__on = 0

    def solve(self):
        """run the solver"""
        self.solved = self.__bt()

    def __is_goal(self):
        """has the backtracker travelled beyond the last cell?"""
        return self.__on == self.skyscrapers.dim ** 2

    def __is_valid(self, row, col):
        """at a given configuration, a board is considered valid if:
            - there are no duplicate rows or columns on the board
            - hints follow the sight of skyscrapers from their respective row and column
            - all rows and columns are unique
            :param row: current row the backtracker is on
            :param col: current column the backtracker is on
            :return: the backtracker is valid at row,col -> True/False
        """

        def check_hints_to_skyscrapers(hint1, hint2, contents, v):
            """checking hints to given contents from the skyscraper board
             :param hint1: hint corresponding to the start of the contents
             :param hint2: hint corresponding to the end of the contents
             :param contents: contents from the skyscraper board
             :param v: the last index of given contents
             :return: True if hints coincides with contents; otherwise, false
            """
            h1_max_ss = contents[0]
            h2_max_ss = contents[v]
            for i in range(1, self.skyscrapers.dim):
                if contents[i] > h1_max_ss:
                    hint1 -= 1
                    h1_max_ss = contents[i]
                if contents[self.skyscrapers.dim - i - 1] > h2_max_ss:
                    hint2 -= 1
                    h2_max_ss = contents[self.skyscrapers.dim - i - 1]
            return hint1 - 1 == 0 and hint2 - 1 == 0

        row_contents = self.skyscrapers.get_row_contents(row)
        col_contents = self.skyscrapers.get_column_contents(col)
        n, s, e, w = self.skyscrapers.get_hints(row, col)
        marked_num = row_contents[col]

        if row_contents.count(marked_num) == 2 or col_contents.count(marked_num) == 2:
            return False

        if col == self.skyscrapers.dim - 1 and not check_hints_to_skyscrapers(w, e, row_contents, col):
            return False

        if row == self.skyscrapers.dim - 1 and not check_hints_to_skyscrapers(n, s, col_contents, row):
            return False

        return True

    def __continue(self):
        """forward the backtracker to the next location on the board"""
        self.__on += 1
        if self.__bt():
            return True
        self.__on -= 1
        return False

    def __bt(self):
        """main backtracking algorithm"""
        if self.__is_goal():
            return True

        col = self.skyscrapers.get_col_number(self.__on)
        row = self.skyscrapers.get_row_number(self.__on)
        if self.skyscrapers.is_marked[row][col]:
            if self.__is_valid(row, col):
                return self.__continue()
        else:
            for i in range(1, 1 + self.skyscrapers.dim):
                self.skyscrapers.mark(row, col, i)
                if self.__is_valid(row, col) and self.__continue():
                    return True
            self.skyscrapers.mark(row, col, 0)

            if self.__on == -1:
                return False

        # implicit backtracking here
        return False


class SkyscrapersGUI:
    def __init__(self, solver):
        self.window = self.__mk_window()
        self.solver = solver

    @staticmethod
    def __mk_window():
        w = tkinter.Tk()
        w.title("Skyscrapers")
        return w

    def run(self):
        self.window.mainloop()


def main():
    skyscrapers = Skyscrapers(open("data/valid/data02.json").read())
    solver = SkyscrapersSolver(skyscrapers)
    solver.solve()
    if not solver.solved:
        print("No solution")
    else:
        print(solver.skyscrapers)


if __name__ == '__main__':
    main()
