package graph;

import java.util.Map;
import java.util.HashMap;
import java.util.HashSet;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;
import java.util.LinkedList;
import java.util.List;

/**
 * <P>
 * This class represents a general "directed graph", which could be used for any
 * purpose. The graph is viewed as a collection of vertices, which are sometimes
 * connected by weighted, directed edges. This graph will never store duplicate
 * vertices. The WeightedGraph will be capable of performing three algorithms:
 * Depth-First-Search, Breadth-First-Search, and Djikatra's. The Weighted Graph
 * will maintain a collection of "GraphAlgorithmObservers", which will be
 * notified during the performance of the graph algorithms to update the
 * observers on how the algorithms are progressing.
 * </P>
 */
public class WeightedGraph<V> {

	private Map<V, Map<V, Integer>> weightedGraph;

	/*
	 * Collection of observers. The graph algorithms (DFS, BFS, and Dijkstra) will
	 * notify these observers to let them know how the algorithms are progressing.
	 */
	private Collection<GraphAlgorithmObserver<V>> observerList;

	/**
	 * Initialize the data structures to "empty", including the collection of
	 * GraphAlgorithmObservers (observerList).
	 */
	public WeightedGraph() {
		weightedGraph = new HashMap<V, Map<V, Integer>>();
		observerList = new HashSet<GraphAlgorithmObserver<V>>();
	}

	/**
	 * Add a GraphAlgorithmObserver to the collection maintained by this graph
	 * (observerList).
	 * 
	 * @param observer
	 */
	public void addObserver(GraphAlgorithmObserver<V> observer) {
		observerList.add(observer);
	}

	/**
	 * Add a vertex to the graph. If the vertex is already in the graph, throw an
	 * IllegalArgumentException.
	 * 
	 * @param vertex vertex to be added to the graph
	 * @throws IllegalArgumentException if the vertex is already in the graph
	 */
	public void addVertex(V vertex) {

		if (weightedGraph.containsKey(vertex)) {
			throw new IllegalArgumentException();

		} else {
			weightedGraph.put(vertex, new HashMap<V, Integer>());

		}
	}

	/**
	 * Searches for a given vertex.
	 * 
	 * @param vertex the vertex we are looking for
	 * @return true if the vertex is in the graph, false otherwise.
	 */
	public boolean containsVertex(V vertex) {
		if (weightedGraph.containsKey(vertex)) {
			return true;

		} else {
			return false;
		}
	}

	/**
	 * Add an edge from one vertex of the graph to another, with the weight
	 * specified. This method throws an IllegalArgumentExeption in three cases: 1.
	 * The "from" vertex is not already in the graph. 2. The "to" vertex is not
	 * already in the graph. 3. The weight is less than 0.
	 * 
	 * @param from   the vertex the edge leads from
	 * @param to     the vertex the edge leads to
	 * @param weight the (non-negative) weight of this edge
	 * @throws IllegalArgumentException when either vertex is not in the graph, or
	 *                                  the weight is negative.
	 */
	public void addEdge(V from, V to, Integer weight) {

		// if vertex does not exist, throw an exception
		if (weightedGraph.containsKey(from) != true || weightedGraph.containsKey(to) != true) {
			throw new IllegalArgumentException();

			// if the weight is negative, throw and exception
		} else if (weight < 0) {
			throw new IllegalArgumentException();

		} else {
			Map<V, Integer> value = weightedGraph.get(from);
			value.put(to, weight);
			weightedGraph.put(from, value);

		}
	}

	/**
	 * Returns weight of the edge connecting one vertex to another. Returns null if
	 * the edge does not exist.
	 * 
	 * Throws an IllegalArgumentException if either of the vertices specified are
	 * not in the graph.
	 * 
	 * @param from vertex where edge begins
	 * @param to   vertex where edge terminates
	 * @return weight of the edge, or null if there is no edge connecting these
	 *         vertices
	 * @throws IllegalArgumentException if either of the vertices specified are not
	 *                                  in the graph.
	 */
	public Integer getWeight(V from, V to) {
		if (weightedGraph.containsKey(from) != true || weightedGraph.containsKey(to) != true) {

			throw new IllegalArgumentException();

		} else {

			if (!(weightedGraph.get(from).containsKey(to))) {
				return null;
			}
			return weightedGraph.get(from).get(to);
		}
	}

	/**
	 * <P>
	 * This method will perform a Breadth-First-Search on the graph. The search will
	 * begin at the "start" vertex and conclude once the "end" vertex has been
	 * reached. Just after a particular vertex is visited, this method will go
	 * through the collection of observers calling notifyVisit on each one (passing
	 * in the vertex being visited as the argument.) After the "end" vertex has been
	 * visited, this method will go through the collection of observers calling
	 * notifySearchIsOver on each one, after which the method should terminate
	 * immediately, without processing further vertices.
	 * 
	 * @param start vertex where search begins
	 * @param end   the algorithm terminates just after this vertex is visited
	 */
	public void DoBFS(V start, V end) {

		List<V> visitedSet = new ArrayList<V>();
		Queue<V> discoveredQueue = new LinkedList<V>();

		discoveredQueue.add(start); // add one into discovered

		/*
		 * Before the search begins, this method will go through the collection of
		 * Observers, calling notifyBFSHasBegun on each one.
		 */
		for (GraphAlgorithmObserver<V> e : observerList) {
			e.notifyBFSHasBegun();
		}

		// while the discovered queue is not empty
		while (!discoveredQueue.isEmpty()) {
			// take from discoverQueue and remove it
			V key = discoveredQueue.poll();
			//// if the key
			if (!visitedSet.contains(key)) {
				
				if (key.equals(end)) {
					for (GraphAlgorithmObserver<V> e : observerList) {
						e.notifySearchIsOver();
					}
					return;
				}

				for (GraphAlgorithmObserver<V> e : observerList) {
					e.notifyVisit(key);
				}
				 // add key to visited set
				visitedSet.add(key);
				
				Map<V, Integer> neighbors = weightedGraph.get(key); // get the neighbors

				for (V vertex : neighbors.keySet()) {
					if (!visitedSet.contains(vertex)) {
						discoveredQueue.add(vertex);
					}
				}
			}
		}
	}

	/**
	 * This method will perform a Depth-First-Search on the graph. The search will
	 * begin at the "start" vertex and conclude once the "end" vertex has been
	 * reached. Before the search begins, this method will go through the collection
	 * of Observers, calling notifyDFSHasBegun on each one. Just after a particular
	 * vertex is visited, this method will go through the collection of observers
	 * calling notifyVisit on each one (passing in the vertex being visited as the
	 * argument). After the "end" vertex has been visited, this method will go
	 * through the collection of observers calling notifySearchIsOver on each one,
	 * after which the method should terminate immediately, without visiting further
	 * vertices.
	 * 
	 * @param start vertex where search begins
	 * @param end   the algorithm terminates just after this vertex is visited
	 */
	public void DoDFS(V start, V end) {

		for (GraphAlgorithmObserver<V> e : observerList) {
			e.notifyDFSHasBegun();
		}
		ArrayList<V> visitedSet = new ArrayList<>();
		Stack<V> stack = new Stack<V>();
		Set<V> adjacents;

		stack.push(start);
		while (!stack.isEmpty()) {
			V key = stack.pop();
			// if the key hasn't been visited yet, go visit it
			if (!visitedSet.contains(key)) {
				
				// If key is equal to the end, notify that the search is over
				if (key.equals(end)) {
					
					for (GraphAlgorithmObserver<V> e : observerList) {
						e.notifySearchIsOver();
					} // leave
					return;
				}
				// notify that you are visiting the key
				for (GraphAlgorithmObserver<V> e : observerList) {
					e.notifyVisit(key);
				}
				// add the key to the visited set
				visitedSet.add(key);

				adjacents = weightedGraph.get(key).keySet();
				for (V vertex : adjacents) {
					if (!visitedSet.contains(vertex)) {
						stack.push(vertex);
					}
				}
			}
		}
	}

	/**
	 * Perform Dijkstra's algorithm, beginning at the "start" vertex. The algorithm
	 * DOES NOT terminate when the "end" vertex is reached. It will continue until
	 * EVERY vertex in the graph has been added to the finished set. Before the
	 * algorithm begins, this method goes through the collection of Observers,
	 * calling notifyDijkstraHasBegun on each Observer. Each time a vertex is added
	 * to the "finished set", this method goes through the collection of Observers,
	 * calling notifyDijkstraVertexFinished on each one (passing the vertex that was
	 * just added to the finished set as the first argument, and the optimal "cost"
	 * of the path leading to that vertex as the second argument.) After all of the
	 * vertices have been added to the finished set, the algorithm will calculate
	 * the "least cost" path of vertices leading from the starting vertex to the
	 * ending vertex. Next, it will go through the collection of observers, calling
	 * notifyDijkstraIsOver on each one, passing in as the argument the "lowest
	 * cost" sequence of vertices that leads from start to end (I.e. the first
	 * vertex in the list will be the "start" vertex, and the last vertex in the
	 * list will be the "end" vertex.)
	 * 
	 * @param start vertex where algorithm will start
	 * @param end   special vertex used as the end of the path reported to observers
	 *              via the notifyDijkstraIsOver method.
	 */
	public void DoDijsktra(V start, V end) {

		for (GraphAlgorithmObserver<V> e : observerList) {
			e.notifyDijkstraHasBegun();
		}

		Map<V, Integer> cost = new HashMap<V, Integer>();
		Map<V, V> predecessor = new HashMap<V, V>();
		ArrayList<V> visitedSet = new ArrayList<V>();
		// to move the the start it costs zero
		cost.put(start, 0);

		// go through the vertex and set all of the weights to a large number
		for (V curr : weightedGraph.keySet()) {
			// if the current value is not start
			if (!curr.equals(start)) {
				cost.put(curr, 1000000);
			} // set predecessors of all the vertexes to null
			predecessor.put(curr, null);
		}

		// while the visitedSet is not equal to the total number of vertex
		while (visitedSet.size() != weightedGraph.size()) {
			int smallest = 1000000;
			V next = null;
			// go through all of the costs, find the smallest
			for (V curr : cost.keySet()) {
				// if the current vertex has not been visited, visit it
				if (!visitedSet.contains(curr)) {
					// comparing cost of current and smallest
					if (cost.get(curr) < smallest) {
						// if smaller, replace the smallest with current vertex
						smallest = cost.get(curr);
						// next vertex is being visited
						next = curr;
					}
				}
			}
			// notifying that the next vertex has been visited and adding it to the
			// visitedSet
			visitedSet.add(next);
			for (GraphAlgorithmObserver<V> e : observerList) {
				e.notifyDijkstraVertexFinished(next, cost.get(next));
			}

			// Now doing the neighbors of that smallest
			for (V curr : weightedGraph.get(next).keySet()) {
				// if the visited set doesn't contain the current vertex
				if (!visitedSet.contains(curr)) {
					// Checking if the weight of the neighbor is smaller than smallest
					if (cost.get(next) + this.getWeight(next, curr) < cost.get(curr)) {
						cost.put(curr, cost.get(next) + this.getWeight(next, curr));

						// if yes replace with neighbor
						predecessor.put(curr, next);
					}
				}
			}
		}
		// the path is made backwards, starting at the end
		V curr = end;
		// path is in a format that can be passed through the notify
		ArrayList<V> path = new ArrayList<V>();
		// while the path is not null
		while (curr != null) {
			// add the current vertex to the path
			path.add(0, curr);
			// move forward (backwards) in the path
			curr = predecessor.get(path.get(0));
		}
		// method is over and is notified
		for (GraphAlgorithmObserver<V> e : observerList) {
			e.notifyDijkstraIsOver(path);
		}
	}
}
