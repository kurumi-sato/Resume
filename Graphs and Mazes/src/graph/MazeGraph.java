package graph;

import graph.WeightedGraph;
import maze.Juncture;
import maze.Maze;

/**
 * <P>
 * The MazeGraph is an extension of WeightedGraph. The constructor converts a
 * Maze into a graph.
 * </P>
 */
public class MazeGraph extends WeightedGraph<Juncture> {

	/**
	 * The Maze is a rectangular grid of "junctures", each defined by its X and Y
	 * coordinates, using the usual convention of (0, 0) being the upper left
	 * corner. Each juncture in the maze should be added as a vertex to this graph.
	 * For every pair of adjacent junctures (A and B) which are not blocked by a
	 * wall, two edges should be added: One from A to B, and another from B to A.
	 * 
	 * @param maze to be used as the source of information for adding vertices and
	 *             edges to this MazeGraph.
	 */
	public MazeGraph(Maze maze) {

		super();
		
		for (int x = 0; x < maze.getMazeWidth(); x++) {

			for (int y = 0; y < maze.getMazeHeight(); y++) {

				this.addVertex(new Juncture(x, y));
			}
		}

		for (int x = 0; x < maze.getMazeWidth(); x++) {
			for (int y = 0; y < maze.getMazeHeight(); y++) {
				Juncture curr = new Juncture(x, y);
				Juncture up = new Juncture(x, y - 1);
				Juncture down = new Juncture(x, y + 1);
				Juncture left = new Juncture(x - 1, y);
				Juncture right = new Juncture(x + 1, y);

				if (!maze.isWallAbove(curr) && this.containsVertex(up)) {
					this.addEdge(curr, up, maze.getWeightAbove(curr));
				}

				if (!maze.isWallBelow(curr) && this.containsVertex(down)) {
					this.addEdge(curr, down, maze.getWeightBelow(curr));
				}

				if (!maze.isWallToLeft(curr) && this.containsVertex(left)) {
					this.addEdge(curr, left, maze.getWeightToLeft(curr));
				}

				if (!maze.isWallToRight(curr) && this.containsVertex(right)) {
					this.addEdge(curr, right, maze.getWeightToRight(curr));
				}
			}
		}
	}
}
