import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Class to represent a Nonogram board puzzle, which includes the row
 * and column hints and the grid board
 *
 * @author awallien (Alex Wall)
 * @version 1.0
 *
 */
public class NonoBoard {

	private final char MARK = '@';
	private final char BLANK = ' ';

    private boolean[][] board;

    private ArrayList<Integer>[] rowHints;
    private ArrayList<Integer>[] colHints;

    public final int NUMBER_OF_ROWS;
    public final int NUMBER_OF_COLS;

    /**
     * Constructor
     * @param configfile the configuration file to build the board
     */
    public NonoBoard(String configfile){
        int rowNum = 0, colNum = 0;
        try {

            Scanner file = new Scanner(new File(configfile));

            rowNum = file.nextInt();
            colNum = file.nextInt();

            board = new boolean[rowNum][colNum];

            file.nextLine();
            file.nextLine();

            rowHints = initializeHints(file, rowNum);
            file.nextLine();
            colHints = initializeHints(file, colNum);

        } catch( FileNotFoundException fnfe ){

            System.err.println("[Error] File Not Found: " + configfile);
            System.exit(1);

        }

        this.NUMBER_OF_ROWS = rowNum;
        this.NUMBER_OF_COLS = colNum;
    }

    /**
     * Constructor for the purpose of deep copying Nonoboard objects
     * @param other - the Nonoboard to copy
     */
    public NonoBoard(NonoBoard other) {

        this.NUMBER_OF_COLS = other.NUMBER_OF_COLS;
        this.NUMBER_OF_ROWS = other.NUMBER_OF_ROWS;

        this.rowHints = other.rowHints;
        this.colHints = other.colHints;

        this.board = copyBoard(other.board);

    }

    /**
     * Helper function to copy a board object to a new object
     * @param board - the board to copy
     * @return - new board object
     */
    private boolean[][] copyBoard(boolean[][] board){
        boolean[][] newBoard = new boolean[this.NUMBER_OF_ROWS][this.NUMBER_OF_COLS];
        for(int r=0; r<NUMBER_OF_ROWS; r++){
            if (NUMBER_OF_COLS >= 0) System.arraycopy(board[r], 0, newBoard[r], 0, NUMBER_OF_COLS);
        }
        return newBoard;
    }

    /**
     * Helper method to initialize a collection of hints seen
     * on a Nonogram puzzle
     * @param file scanner object that reads a config file
     * @param number the number of collections of hints
     */
    private ArrayList<Integer>[] initializeHints(Scanner file, int number){
        ArrayList<Integer>[] arr = new ArrayList[number];

        for(int r=0; r<number; r++){
            arr[r] = new ArrayList<Integer>();
            for(String i : file.nextLine().split(" "))
                arr[r].add(Integer.parseInt(i));
        }

        return arr;
    }

    /**
     * Get list of hints for a specified row
     * @param row the row number
     * @return the list of hints
     */
    public ArrayList<Integer> getRowHints(int row) {
        return rowHints[row];
    }

    /**
     * Get list of hints for a specified column
     * @param column the column number
     * @return the list of hints
     */
    public ArrayList<Integer> getColHints(int column) {
        return colHints[column];
    }

    /**
     * Mark a row of cells on the board
     * @param row the row to mark the cell(s)
     * @param col the column to mark the cell(s)
     * @param len the number of cells in the group
     * @param markdown true if marking; otherwise, false if erasing
     * @return true if successful mark; otherwise, false
     */
    public boolean mark(int row, int col, int len, boolean markdown){
        if(col+len > NUMBER_OF_COLS || row < 0 || col < 0 || len < 0)
            return false;

        for(int i=col; i<col+len; i++)
            board[row][i] = markdown;

        return true;
    }


    /**
     * helper function to make the border lines of the board
     * @return a line of + and -
     */
	private String border( ){
		StringBuilder res = new StringBuilder("+");

		for( int i=0; i<board[0].length; i++ )
			res.append("-+");

		return res.toString();
	}

    /**
     * Get a string representation of the board
     * @return only the board, no hints
     */
	public String getBoard() {
        StringBuilder s = new StringBuilder(String.format("\n%s\n", border()));

        // print the cells
        for( int i=0; i<board.length; i++ ){
            s.append( "|" );
            for( int j=0; j<board[0].length; j++ ){
                s.append( board[i][j] ? MARK : BLANK ).append("|");
            }
            s.append("\n").append(border()).append("\n");
        }

        return s.toString();
    }

    /**
     * String representation of Nonogram puzzle board
     * @return string format of board
     */
    @Override
    public String toString(){
        StringBuilder s = new StringBuilder(getBoard());

        s.append( "\n" );

        // print the row hints
        s.append( "Row\n---\n");
        for( int r=0; r<rowHints.length; r++ ){
            s.append( r ).append( ": " );
            for( int rh=0; rh<rowHints[r].size(); rh++ ){
                s.append( rowHints[r].get(rh) ).append( " " );
            }
            s.append( "\n" );
        }

        // print the column hints
        s.append( "\nColumn\n------\n" );
        for( int c=0; c<colHints.length; c++ ){
            s.append( c ).append( ": " );
            for( int ch=0; ch<colHints[c].size(); ch++ ){
                s.append( colHints[c].get(ch) ).append( " " );
            }
            s.append( "\n" );
        }

        return s.toString();
    }


    /**
     * Main method for testing
     * @param args 0: config file
     */
    public static void main(String[] args) {
        if(args.length != 1){
            System.err.println("Usage: java NonoBoard config-file");
            System.exit(1);
        }

        NonoBoard board = new NonoBoard(args[0]);

        // mark diagonals, all true except one false
        for(int r=0; r<=board.NUMBER_OF_ROWS; r++){
            System.out.println(board.mark(r,r,1,true));
        }

        // mark a row
        System.out.println(board.mark(0,0,board.NUMBER_OF_COLS,true));

        // no marking for this
        System.out.println(board.mark(1,-5,4,true));

        System.out.println(board);

    }

}
