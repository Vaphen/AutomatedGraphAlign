#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include "Node.hpp"
#include "Edge.hpp"

template<typename NODEVAL, typename EDGEVAL, bool isDirected = false,
            template<typename> typename NODETYPE = Node,
            template<typename, class, bool> typename EDGETYPE = Edge>
class Graph
{
	using NODE = NODETYPE<NODEVAL>;
	using EDGE = EDGETYPE<EDGEVAL, NODE, isDirected>;

	// Vectors containing pointers to all nodes and edges of this graph
	std::vector<std::shared_ptr<NODE> > nodes;
	std::vector<std::shared_ptr<EDGE> > edges;
public:
    // default constructor
	Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE>() = default;

	/**
	 * @brief Add a new node with a given value to the graph.
	 * @param value is the value of the node
	 * @return a pointer to the new node
     */
	std::shared_ptr<NODE> addNode(NODEVAL value) {
		this->nodes.emplace_back(new NODE(value));
		return this->nodes.back();
	}

	/**
	 * @brief Add a new node with a given value and adjacent nodes to the graph.
	 * @param value is the value of the node
	 * @param adjacentNodes an initializer list containing pointers to adjacent nodes
	 * @return a pointer to the new node
     */
	std::shared_ptr<NODE> addNode(NODEVAL value, std::initializer_list<std::shared_ptr<NODE>> adjacentNodes) {
	 	std::shared_ptr<NODE> newNode(new NODE(value, adjacentNodes));
		this->nodes.push_back(newNode);
		if(!isDirected) {
			for(auto adjacentNode : adjacentNodes) {
				adjacentNode->addAdjacentNode(newNode);
			}
		}
		for(auto adjacentNode : adjacentNodes) {
			edges.emplace_back(new EDGE(newNode, adjacentNode));
		}
		return newNode;
	}

	/**
	 * @brief Add an existing node to the graph
	 * @param node a pointer to the node that should be added
	 * @return false if the node is already a part of the graph, else true.
     */
	bool addNode(std::shared_ptr<NODE> node) {
		auto isExistantIt = std::find_if(nodes.begin(), nodes.end(), [node](std::shared_ptr<NODE> p) { return (*node == *p); });
		if(isExistantIt == nodes.end()) {
			this->nodes.push_back(node);
			return true;
		}
		return false;
	}

	/**
	 * @brief Add a new edge between two given nodes if not existant
	 * @param n1 is a pointer to the target node
	 * @param n2 is a pointer to the destination node
 	 * @param directed must be true if the edge should be directed, else false
	 * @return a pointer to the edge
	 */
	std::shared_ptr<EDGE> addEdge(std::shared_ptr<NODE> n1, std::shared_ptr<NODE> n2, bool directed) {
		std::shared_ptr<EDGE> edge(new EDGE(n1, n2));
		// check if the edge is already existant
		// TODO: consider directed parameter
		auto isExistantIt = std::find_if(edges.begin(), edges.end(), [edge](std::shared_ptr<EDGE> p) { return (*edge == *p); });

		if(isExistantIt == edges.end()) {
			edges.push_back(edge);

			isExistantIt = edges.end();
			--isExistantIt;
		}

		return *isExistantIt;
	}


	/**
	 * @brief Add a new, undirected edge between two given nodes if not existent
	 * @param n1 is a pointer to the target node
	 * @param n2 is a pointer to the destination node
	 * @return a pointer to the edge
     */
	std::shared_ptr<NODE> addEdge(std::shared_ptr<NODE> n1, std::shared_ptr<NODE> n2) {
		return addEdge(n1, n2, false);
	}

	/**
	 * @brief Get all nodes of the graph
	 * @return a vector containing pointers to all nodes of the graph
     */
	std::vector<std::shared_ptr<NODE>> getNodes() const {
		return nodes;
	}

	/**
	 * @brief Get all edges of the graph
	 * @return a vector containing pointers to all edges of the graph
	 */
	std::vector<std::shared_ptr<EDGE> > getEdges() const {
		return edges;
	}

    /** \brief Check if a given node is contained in the graph
     * \param node a pointer to the node that should be checked
     * \return bool true if the node is contained in the graph, else false
     *
     */
	bool contains(std::shared_ptr<NODE> node) {
		return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
	}


    /** \brief Remove a given node from the graph.
     * \param delnode a pointer to the node that should be deleted.
     * \return true if the node was existent in the graph and removed successful, else false.
     */
	bool removeNode(std::shared_ptr<NODE> delnode) {
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
