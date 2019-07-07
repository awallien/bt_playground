import java.util.Optional;

/**
 * Main class for Nonogram solver
 *
 * @author awallien (Alex Wall)
 * @version 1.0
 */
public class Nonogram {
    /**
     * Main method to simulate the Nonogram solver, including the Backtracker and initial board configuration
     *
     * @param args [0]: configuration file, [1]: enable debugger (optional)
     */
    public static void main(String[] args) {
        boolean debug = false;

        // invalid number of arguments
        if (args.length < 1 || args.length > 3) {
            System.err.println("Usage: java Nonogram config-file [debug: true|false]");
            System.exit(1);
        }

        // debug is enabled
        if (args.length == 2 && args[1].equals("true"))
            debug = true;

        // initialize the objects
        NonoBoard board = new NonoBoard(args[0]);
        Backtracker bt = new Backtracker(debug);
        Optional<Configuration> sol = bt.solve(new NonogramConfig(board));

        // print the solution
        System.out.println(sol.isPresent() ? sol.get() : "No Solution!");
    }

}
