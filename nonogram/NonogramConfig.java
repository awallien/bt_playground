import java.util.Collection;
import java.util.LinkedList;
import java.util.ListIterator;

/**
 * Class that handles producing multiple successors
 * of the board, validating the configurations, and checking
 * if the goal is reached.
 *
 * @author awallien (Alex Wall)
 * @version 1.0
 */
public class NonogramConfig implements Configuration {

    /**
     * The Nonogram board for this configuration
     */
    private NonoBoard board;

    /**
     * Current row status when checking for goal
     */
    private int status;

    /**
     * Constructor
     *
     * @param board - the initial board
     */
    public NonogramConfig(NonoBoard board) {
        this.board = board;
        this.status = -1;
    }

    /**
     * Constructor for the purpose of building copies of Nonogram configurations
     *
     * @param other - the other Nonogram configuration to copy to this object
     */
    private NonogramConfig(NonogramConfig other) {
        this.board = new NonoBoard(other.board);
        this.status = other.status;
    }

    /**
     * Built by marking down each rows according to the hints for every possible combinations
     *
     * @return a collection of successors at a current "status"
     */
    @Override
    public Collection<Configuration> getSuccessors() {
        LinkedList<Configuration> successors = new LinkedList<>();
        ++this.status;
        buildRow(successors, board.getRowHints(this.status).listIterator(), 0, new NonogramConfig(this));
        return successors;
    }

    /**
     * Helper function to create successors for the backtracker by populating
     * the board at the current row with a combination of marks
     *
     * @param successors - the list to store successors
     * @param hints      - the hints for a row
     * @param config     - the configuration of this successor
     * @param atColumn   - next available spot when marking a row on the board
     */
    private void buildRow(LinkedList<Configuration> successors, ListIterator<Integer> hints,
                          int atColumn, NonogramConfig config) {
        if (hints.hasNext()) {
            int hint = hints.next();
            while (config.board.mark(config.status, atColumn, hint, true)) {
                buildRow(successors, hints, hint + atColumn + 1, new NonogramConfig(config));
                config.board.mark(config.status, atColumn, hint, false);
                atColumn++;
            }
            hints.previous();
        } else
            successors.add(config);
    }

    /**
     * Validates by comparing the column hints to the respective columns on the board
     *
     * @return true if valid configuration; otherwise, false
     */
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


    /**
     * Goal is reached if all rows are reached and
     * everything is validated
     *
     * @return true if reached; otherwise, false
     */
    @Override
    public boolean isGoal() {
        return status + 1 == board.NUMBER_OF_ROWS;
    }

    /**
     * String representation of Configuration object
     */
    @Override
    public String toString() {
        return board.toString();
    }
}
