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

    public NonogramConfig(NonoBoard board){
        this.board = board;
    }

    @Override
    public Collection<Configuration> getSuccessors() {
        return null;
    }

    @Override
    public boolean isValid() {
        return false;
    }

    @Override
    public boolean isGoal() {
        return false;
    }
}
