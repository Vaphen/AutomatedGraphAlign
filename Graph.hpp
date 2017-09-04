#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include "Node.hpp"
#include "Edge.hpp"

template<typename T, bool isDirected = false>
class Graph
{
	// Vector containing pointers to all nodes of this graph
	std::vector<Node<T>*> nodes;
	std::vector<Edge<T>*> edges;
public:
	Graph<T, isDirected>() {};

	Graph<T, isDirected>(Graph<T, isDirected> &copyGraph) {
		nodes = copyGraph.getNodes();
		edges = copyGraph.getEdges();
	}

	/**
	 * @brief Destructor deleting all node allocations
   */
	~Graph() {
		for(auto node : nodes) {
			delete node;
		}

		for(auto edge : edges) {
			delete edge;
		}
	}

	/**
	 * @brief Add a new node with a given value to the graph.
	 * @param value is the value of the node
	 * @return a pointer to the new node
   */
	Node<T> *addNode(T value) {
		Node<T> *newNode = new Node<T>(value);
		this->nodes.push_back(newNode);
		return newNode;
	}

	/**
	 * @brief Add a new node with a given value and adjacent nodes to the graph.
	 * @param value is the value of the node
	 * @param adjacentNodes an initializer list containing pointers to adjacent nodes
	 * @return a pointer to the new node
   */
	Node<T> *addNode(T value, std::initializer_list<Node<T>*> adjacentNodes) {
		Node<T> *newNode = new Node<T>(value, adjacentNodes);
		this->nodes.push_back(newNode);
		if(!isDirected) {
			for(auto adjacentNode : adjacentNodes) {
				adjacentNode->addAdjacentNode(newNode);
			}
		}
		for(auto adjacentNode : adjacentNodes) {
			edges.push_back(new Edge<T>(newNode, adjacentNode));
		}
		return newNode;
	}

	/**
	 * @brief Add an existing node to the graph
	 * @param node a pointer to the node that should be added
	 * @return false if the node is already a part of the graph, else true.
     */
	bool addNode(Node<T> *node) {
		auto isExistantIt = std::find_if(nodes.begin(), nodes.end(), [node](Node<T>* p) { return (*node == *p); });
		if(isExistantIt == nodes.end()) {
			this->nodes.push_back(node);
			return true;
		}
		return false;
	}

	/**
	 * @brief Add a new edge between two given nodes if not existant
	 * @param n1 is the target node
	 * @param n2 is the destination node
 	 * @param directed must be true if the edge should be directed, else false
	 * @return a pointer to the edge
	 */
	Edge<T> *addEdge(Node<T> *n1, Node<T> *n2, bool directed) {
		Edge<T> *edge = new Edge<T>(n1, n2);
		// check if the edge is already existant
		// TODO: consider directed parameter
		auto isExistantIt = std::find_if(edges.begin(), edges.end(), [edge](Edge<T>* p) { return (*edge == *p); });

		if(isExistantIt == edges.end()) {
			edges.push_back(edge);

			isExistantIt = edges.end();
			--isExistantIt;
		}

		return *isExistantIt;
	}


	/**
	 * @brief Add a new, undirected edge between two given nodes if not existant
	 * @param n1 is the target node
	 * @param n2 is the destination node
	 * @return a pointer to the edge
     */
	Edge<T> *addEdge(Node<T> *n1, Node<T> *n2) {
		return addEdge(n1, n2, false);
	}

	/**
	 * @brief Get all nodes of the graph
	 * @return a vector containing pointers to all nodes of the graph
     */
	std::vector<Node<T>*> getNodes() const {
		return nodes;
	}

	/**
	 * @brief Get all edges of the graph
	 * @return a vector containing pointers to all edges of the graph
	 */
	std::vector<Edge<T>*> getEdges() const {
		return edges;
	}

	bool contains(Node<T> *node) {
		return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
	}

};

#endif
