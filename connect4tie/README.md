# Connect4Tie

Connect Four is a two-player game in which players competitively take turn dropping their 
color pieces in a column of a suspended 6x7 grid. A player is deemed winner if one of the players
is able to be the first to make a horizontal, vertical, or diagonal line of four of their pieces.

According to the Online Encyclopedia of Integer Sequences, computer scientist Stefan Edelkamp and
Peter Kissmann found that there are over 4 trillion possible games board positions for all game
boards populated with 0 to 42 pieces. This has some relevance to this program (not really), but
it seems like a cool fact to state here.

Anyway, the objective of this program to calculate the number of full connect 4 boards where there are no
declared winners.

# Implementation

This program counts for all possible solutions. After a board is completely filled, the number of Xs
equals the number of Os, and there are no declared winners, the program would increment the counter and
backtrack to find the next possible solutions.

When a board is validated on a certain cell, it will check adjacency cells that are already filled previously
for any lines of four similar pieces. The program will validate a piece on a cell if there are no winning lines;
else, it invalidates and either puts the next piece to check or backtracks. 

# Computational Complexity

As mentioned, the board is validated based on the current cell and also the cells that were previously
filled. Consider the diagram below that represents a Connect Four board.

| A | A | A | B | B | B | B |
|---|---|---|---|---|---|---|
| A | A | A | B | B | B | B |
| A | A | A | B | B | B | B |
| C | C | C | D | E | E | E |
| C | C | C | D | E | E | E |
| C | C | C | D | E | E | E |

Each letter on the grid symbolizes a piece on the board. For example, if the backtracker is currently
on an **A** cell, then it would automatically validate the board since a line of four pieces cannot 
be made in any directions. The backtracker only checks the west direction if on a **B** cell; north and
north-east directions if on a **C** cell; north, north-west, north-east, and west directions if on a **D**
cell; and north, north-west, and west directions if on an **E** cell.

When the backtracker is beyond the last cell of the grid, it would check if
the number of Os and Xs are the same. If so, a valid and no-win board is made.


