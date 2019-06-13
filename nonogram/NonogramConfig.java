import java.util.Collection;

/**
 * Class that handles marking the board and producing
 * multiple successors of the board
 *
 * @author awallien (Alex Wall)
 * @version 1.0
 *
 */
public class NonogramConfig implements Configuration {

    private NonoBoard board;
    private int status;

    public NonogramConfig(NonoBoard board){
        this.board = board;
        this.status = 0;
    }

    /**
     * Built by marking down each rows according to the hints and building possible
     * combinations of hints within that row
     * @return
     */
    @Override
    public Collection<Configuration> getSuccessors() {
        return null;
    }

    @Override
    public boolean isValid() {
        return false;
    }

    /**
     * Goal is reached if the "cursor" runs through all the rows and
     * everything is validated
     * @return true if reached; otherwise, false
     */
    @Override
    public boolean isGoal() {
        return status == board.NUMBER_OF_ROWS;
    }
}
