package tests;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.junit.Test;

import graph.WeightedGraph;

public class PublicTests {

	@Test
	public void testAddVertexAndContainsVertex() {
		WeightedGraph<String> graph = new WeightedGraph<String>();
		graph.addVertex("A");
		graph.addVertex("B");
		graph.addVertex("C");
		graph.addVertex("D");
		assertTrue(graph.containsVertex("A"));
		assertTrue(graph.containsVertex("B"));
		assertTrue(graph.containsVertex("C"));
		assertTrue(graph.containsVertex("D"));
		assertFalse(graph.containsVertex("E"));
	}
	
	@Test
	public void testAddEdgeAndGetWeight() {
		WeightedGraph<String> graph = new WeightedGraph<String>();
		graph.addVertex("A");
		graph.addVertex("B");
		graph.addVertex("C");
		graph.addVertex("D");
		graph.addEdge("A", "B", 1);
		graph.addEdge("A", "C", 2);
		graph.addEdge("A", "D", 3);
		graph.addEdge("B", "C", 4);
		graph.addEdge("D", "C", 5);
		assertTrue(graph.getWeight("A", "B") == 1);
		assertTrue(graph.getWeight("B", "A") == null);
		assertTrue(graph.getWeight("A", "C") == 2);
		assertTrue(graph.getWeight("A", "D") == 3);
		assertTrue(graph.getWeight("B", "C") == 4);
		assertTrue(graph.getWeight("D", "C") == 5);
		boolean caught = false;
		try {
			graph.getWeight("X",  "A");
		} catch (IllegalArgumentException e) {
			caught = true;
		}
		assertTrue(caught);
		caught = false;
		try {
			graph.getWeight("A", "X");
		} catch (IllegalArgumentException e) {
			caught = true;
		}
		assertTrue(caught);
		assertTrue(graph.getWeight("B", "D") == null);
	}
	
	public void testEasyDFS() {
		WeightedGraph<String> graph = new WeightedGraph<String>();
		
		
	}
	
}