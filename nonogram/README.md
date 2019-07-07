# Nonogram

Nonogram is a popular logic puzzle game in which cells must be filled
or left blank in order to create a picture based on the given row and
column hints. 

# Implementation

Source files for Configuration.java and Backtracker.java are both provided by
RIT CS from CSCI-142 course lecture code website.

The backtracker will find only one solution to this puzzle game. For each row
on the board, it will generate successors with the hints in that row by building 
all possible combinations. For example, if a row of length 7 has hints [2,3]
then the possible combinations built for that row are:

@ @ . @ @ @ .

@ @ . . @ @ @

. @ @ . @ @ @

where '@' is a filled cell.

For each successor, the validator will check each column on the board to
determine if the column hints comply. Then, the backtracker will generate
the next successor based off that valid successor for the next row. A solution
is found if the backtracker has gone through all the rows.

# Computational Complexity

The time complexity for this solver is depended on the three methods
in the Configuration interface. Therefore, the complexity is:

![eqn total complexity](https://github.com/awallien/bt_playground/blob/master/nonogram/doc/eqn_total_cpx.png)

where S represents the number of combinations generated in the kth row, G(n) is the time
complexity for checking if a goal is reached, and V(n) is the time complexity
for the validation method. The data, *n*, in this case is the board 

## G(n)

The goal method for this program basically checks if the backtracker
has gone through all the rows. 

```java
@Override 
public boolean isGoal(){ return status + 1 == board.NUMBER_OF_ROWS; }
```

Overall, the time complexity is *O*(1).

## V(n)

The validation method traverses through each column and checks if the
hints comply with the marked cells in that column. 

```java
@Override
public boolean isValid() {
        for (int column = 0; column < board.NUMBER_OF_COLS; column++) {
            ListIterator<Integer> colHints = board.getColHints(column).listIterator();
            int count = 0, row = 0;
            boolean checking = false;

            // go to first marked cell in the column
            while (row < board.NUMBER_OF_ROWS && !board.isMarked(row, column))
                row++;

            for (; row < board.NUMBER_OF_ROWS; row++) {
                if (!colHints.hasNext())
                    break;
                else if (board.isMarked(row, column)) {
                    count++;
                    checking = true;
                } else if (checking && colHints.next() < count)
                    return false;
                else {
                    count = 0;
                    checking = false;
                }
            }

            // checking for status row
            if (isGoal() && colHints.hasNext() && colHints.next() != count)
                return false;

            // went through all hints, check if any for other marked cells in column
            while (row < board.NUMBER_OF_ROWS)
                if (board.isMarked(row++, column))
                    return false;
        }

        // everything is valid
        return true;
    }
```

Since the method traverses through every cell on the board, the
time complexity in this case is *O*(n).

<br/>

The overall time complexity for validating the board is

![total_cpx_2](https://github.com/awallien/bt_playground/blob/master/nonogram/doc/total_cpx_2.png)


 
# Applying Heuristics
< to be written later >

# Installation and Usage

Java 6 and above is required.

Compile all the java files into their class files, and execute
the program with the Nonogram class.

The usage to run this program is:
```java Nonogram config-file [debug]```

where ```config-file``` is a configuration file that holds
information about the Nonogram board. The first line of the file
contains the respective row and column numbers. Then, the following
lines are the hints for both the rows and columns of the board.
See ```data/``` directory for example files. 

```debug``` enables debugging where the program prints the
successors, their validity, and any goal configurations to a
```debug``` file.