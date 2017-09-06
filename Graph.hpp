#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include "Node.hpp"
#include "Edge.hpp"

template<typename NODEVAL, typename EDGEVAL, bool isDirected = false,
            template<typename> typename NODETYPE = Node,
            template<typename, class, bool> typename EDGETYPE = Edge>
class Graph
{
	using NODE = NODETYPE<NODEVAL>;
	using EDGE = EDGETYPE<EDGEVAL, NODE, isDirected>;

	// Vector containing pointers to all nodes of this graph
	std::vector<NODE*> nodes;
	std::vector<EDGE*> edges;
public:
	Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE>() = default;

	Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE>
		(Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE> &copyGraph)
	{
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
	NODE* addNode(NODEVAL value) {
		NODE* newNode = new NODE(value);
		this->nodes.push_back(newNode);
		return newNode;
	}

	/**
	 * @brief Add a new node with a given value and adjacent nodes to the graph.
	 * @param value is the value of the node
	 * @param adjacentNodes an initializer list containing pointers to adjacent nodes
	 * @return a pointer to the new node
   */
	NODE* addNode(NODEVAL value, std::initializer_list<NODE*> adjacentNodes) {
	 	NODE* newNode = new NODE(value, adjacentNodes);
		this->nodes.push_back(newNode);
		if(!isDirected) {
			for(auto adjacentNode : adjacentNodes) {
				adjacentNode->addAdjacentNode(newNode);
			}
		}
		for(auto adjacentNode : adjacentNodes) {
			edges.push_back(new EDGE(newNode, adjacentNode));
		}
		return newNode;
	}

	/**
	 * @brief Add an existing node to the graph
	 * @param node a pointer to the node that should be added
	 * @return false if the node is already a part of the graph, else true.
     */
	bool addNode(NODE *node) {
		auto isExistantIt = std::find_if(nodes.begin(), nodes.end(), [node](NODE* p) { return (*node == *p); });
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
	EDGE *addEdge(NODE *n1, NODE *n2, bool directed) {
		EDGE *edge = new EDGE(n1, n2);
		// check if the edge is already existant
		// TODO: consider directed parameter
		auto isExistantIt = std::find_if(edges.begin(), edges.end(), [edge](EDGE* p) { return (*edge == *p); });

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
	EDGE *addEdge(NODE *n1, NODE *n2) {
		return addEdge(n1, n2, false);
	}

	/**
	 * @brief Get all nodes of the graph
	 * @return a vector containing pointers to all nodes of the graph
     */
	std::vector<NODE*> getNodes() const {
		return nodes;
	}

	/**
	 * @brief Get all edges of the graph
	 * @return a vector containing pointers to all edges of the graph
	 */
	std::vector<EDGE*> getEdges() const {
		return edges;
	}

	bool contains(NODE *node) {
		return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
	}

	bool removeNode(NODE *delnode) {
        auto it = std::find(nodes.begin(), nodes.end(), delnode);
        if(it != nodes.end()) {
            nodes.erase(it);
            for(auto node : nodes) {
               node->removeAdjacentNode(delnode);
            }
            return true;
        }
        return false;
	}

};

#endif
