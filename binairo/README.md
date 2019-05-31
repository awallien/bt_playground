# Binairo

Binairo is a puzzle game involving two symbols, 0s and 1s or black and white, on a square grid.
The goal of the puzzle is to fill the board with the symbols such that there is an equal number
of symbols in each row and column, no more than two of either symbols are adjacent to each other,
and that each row and column on the puzzle board are unique.

The figure below illustrates an example of the initial grid of a binairo puzzle board and the solution to the puzzle.
![init grid](https://github.com/awallien/bt_playground/blob/master/binairo/doc/init_grid.PNG)
![sol grid](https://github.com/awallien/bt_playground/blob/master/binairo/doc/sol_grid.png)

(Pictures credit to: https://www.puzzle-binairo.com )

## Implementation

The backtracker will find one solution to a binairo puzzle. It walks through each cell on the grid from top left
to bottom right corner and reaches a goal after validating the last cell on the board or "traversing off the board"
if unable to validate both symbols on the first cell. 

For validation, the backtracker would have to check three criteria as explained in the introduction: there is an equal number
of symbols in each row and column, no more than two of either symbols are adjacent to each other, and
each row and column on the board is unqiue.

The puzzle board is displayed as a grid and utilizes 0 and 1 as the symbols. 

## Computational Complexity

The overall time complexity for this puzzle depends on the two symbols, the validation methods, and
the number of empty cells on the puzzle board. In a brute force algorithm, a solution in worst case scenario may take
up to 

![time_cpxy](https://github.com/awallien/bt_playground/blob/master/binairo/doc/eqn_tc.png)

where <em>k</em> represents the number of empty, markable cells on the board, and <em>H(n)</em> is the time complexity
for validating the symbols in each markable cell in big O notation. 
