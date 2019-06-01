# Binairo

Binairo is a puzzle game involving two symbols, 0s and 1s or black and white, on a square grid.
The goal of the puzzle is to fill the board with the symbols such that there is an equal number
of symbols in each row and column, no more than two of either symbols are adjacent to each other,
and that each row and column on the puzzle board are unique.

The figure below illustrates an example of the initial grid of a binairo puzzle board and the solution to the puzzle.
![init grid](https://github.com/awallien/bt_playground/blob/master/binairo/doc/init_grid.PNG)
![sol grid](https://github.com/awallien/bt_playground/blob/master/binairo/doc/sol_grid.png)

(Pictures credit to: https://www.puzzle-binairo.com )

# Implementation

The backtracker will find one solution to a binairo puzzle. It walks through each cell on the grid from top left
to bottom right corner and reaches a goal after validating the last cell on the board or "traversing off the board"
if unable to validate both symbols on the first cell. 

For validation, the backtracker would have to check three criteria as explained in the introduction: there is an equal number
of symbols in each row and column, no more than two of either symbols are adjacent to each other, and
each row and column on the board is unqiue.

The puzzle board is displayed as a grid and utilizes 0 and 1 as the symbols. 

# Computational Complexity

The overall time complexity for this puzzle depends on the two symbols, the validation methods, and
the number of empty cells on the puzzle board. In a brute force algorithm, a solution in worst case scenario may take
up to 

![time_cpxy](https://github.com/awallien/bt_playground/blob/master/binairo/doc/eqn_tc.png)

where <em>k</em> represents the number of empty, markable cells on the board, and <em>H(n)</em> is the time complexity
for validating the symbols in each markable cell in big O notation. The <em>n</em> in this matter is the whole grid of
the board.  

## H(n) Analysis

The function from the backtracking source file, binairo_bt.c::is_valid, includes the validation checks in a listed order. 

### Number of 0s == Number of 1s

The first procedure checks whether there is an equal number of 0s and 1s for each row and
column. For each cell check, it would check if the number of a certain symbol in a row
or column is at most half the dimension of the board as shown here:
```C
if( numberof_row_BinairoBoard( brd, status/dim, ZERO ) > dim/2 || 
    numberof_row_BinairoBoard( brd, status/dim, ONE ) > dim/2 ){
    DEBUG_FALSE;
    return false;
}
	
if( numberof_column_BinairoBoard( brd, status%dim, ZERO ) > dim/2 ||
    numberof_column_BinairoBoard( brd, status%dim, ONE ) > dim/2 ){
    DEBUG_FALSE;
    return false;
}
```
One thing to note is that for each cell check, when a symbol is put on the board, it would
increase the number of that symbols on the binairo board. Thus, the time complexity for this
procedure is <em>O</em>(1).

### Adjacency Check

The next procedure checks a cell's adjacent neighbors. Consider the current cell to put a symbol to be
the middle cell. The algorithm would check all four cardinal directions and count the number of neighbors
along with counting the adjacent neighbor left-right and top-bottom. The code shown below is an example on
how the backtracker validates the adjacent neighbors. All neighbor counting functions follow the same design.
```C
static bool chk_down_adj( int status, Digit digit ){
    char idx = 2;
    char count = 0;
    while( (status+=dim) < dim*dim && idx-- )
        count += get_BinairoBoard( brd, status ) == digit ? 1 : 0;
    return count != 2;  
}
```
In total, this procedure examines at most 12 cells on every validation check, and the time complexity for getting a symbol in a cell on board is constant time. Therefore, the whole time complexity for this procedure is technically 12*<em>O</em>(2<sup>k</sup>), or simply <em>O</em>(2<sup>k</sup>).

### Unique Rows and Columns

The last procedure in the validation methods check if there are unique vectors on the puzzle board.
After a whole row or column is filled, the backtracker would compare the current direction vector with the previous direction vectors to check for any duplicates. To do the comparison, since each vector is filled with 0s and 1s, the hash would be that vector's binary value. The code below illustrates the hashing and comparison process to check for duplicate vectors.
```C
static void store_hash( Vector dir, int status ){
	char* str = calloc( dim+1, sizeof( char ) );
	int idx;
	
	for( idx=0; idx<dim; idx++ ){
		switch( dir ){
			case ROW:
				switch( get_BinairoBoard( brd, status-dim+1+idx ) ){
					case ZERO:
						str[idx] = '0';
						break;
					case ONE:
						str[idx] = '1';
						break;
					default:
						assert( get_BinairoBoard( brd, status-dim+1+idx ) );
				}
				break;
			case COL:
				switch( get_BinairoBoard( brd, status-idx*dim ) ){
					case ZERO:
						str[dim-idx-1] = '0'; 
						break;
					case ONE:
						str[dim-idx-1] = '1';
						break;
					default:
						assert( get_BinairoBoard( brd, status-idx*dim ) );
				}	
		}
	}
	str[idx] = '\0';

	put_HashInfo( hashinfo, str, dir, dir == ROW ? status/dim : status%dim );
	free( str );
} 
static bool chk_unique_rows( int status ){
	if( status%dim != dim-1 )
		return true;

	store_hash( ROW, status );

	size_t cur_hash = get_HashInfo( hashinfo, ROW, status/dim );	

	while( (status-=dim) > 0 )
		if( cur_hash == get_HashInfo( hashinfo, ROW, status/dim ) )
			return false;	

	return true;
}
```
As a side note, HashInfo is a data structure that holds a list of hashes for each row and column.
Since the board would not change in size, and HashInfo object is allocated beforehand, the get and
put functions associated with HashInfo is in constant time.

In order to calculate the full time complexity, we will consider running this procedure on the
last cell of the board. The complexity to make the hash value for each vector and storing the result
into the data collection is 2*<em>O</em>(n). Additionally, the complexity to compare each hash value
to the last row and column is another 2*<em>O</em>(n). Thus, the whole time complexity for this procedure
is simply <em>O</em>(n).

<br/>

Overall, the total time complexity for validating a symbol in a cell, <em>H</em>(n), is 
![total_h_n](https://github.com/awallien/bt_playground/blob/master/binairo/doc/eqn_H_n.png)

## Applying Heuristics

< To be written >

## Installation and Usage
<em>gcc</em> or equivalent must be installed on machine to compile C source files. Then, execute 
<em>make</em> to run the Makefile and compile the source and header files into an executable file. 