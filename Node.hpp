#ifndef __NODE_H_
#define __NODE_H_

#include <vector>
#include <initializer_list>
#include <ostream>
#include <algorithm>

template<class T>
class Node
{
    // node members
	T value;
	std::vector<Node<T>*> adjacentNodes;

	// unique ids
	static unsigned instanceCount; // initialized at the end of this file
	unsigned privateId;

	// position for gui use
    std::pair<float, float> position;





public:
	/**
	 * @brief Constructor for value only instantiation
     */
	Node(T val) : value(val), privateId(instanceCount) {
		instanceCount++;
	}

	/**
	 * @brief Constructor for value and adjacent node instantiation
     */
	Node(T value, std::initializer_list<Node<T>*> nodes) : value(value), adjacentNodes(nodes), privateId(instanceCount) {
		instanceCount++;
	}

	/**
	 * @brief Add a new adjacent node to this one
	 * @param a pointer to the new adjacent node
     */
	void addAdjacentNode(Node<T> *node) {
		adjacentNodes.push_back(node);
	}

	/**
	 * @brief Get the value
	 * @return the value of this node
     */
    T getValue() {
		return value;
	}

	/**
	 * @brief get all adjacent nodes
	 * @return a reference to a vector containing pointers to all adjacent nodes
     */
	std::vector<Node<T>*> &getAdjacentNodes() {
		return adjacentNodes;
	}

	/**
	 * @brief get the unique id
	 * @return an unsigned int as id
     */
	unsigned getId() {
		return privateId;
	}

	std::pair<float, float> getPosition() {
        return position;
    }

    void setPosition(float x, float y) {
        this->position = std::pair<float, float>(x, y);
    }

	/**
	 * @brief Overwritten operator used by std::cout to output nodes in a readable way
     */
	friend std::ostream& operator<<(std::ostream &os, Node<T> &n)
	{
	    os << n.privateId << "[" << n.value << "]" << "{ ";
			for(auto a : n.getAdjacentNodes()) {
				os << a->getId() << ",";
			}
			os << "\b }";
	    return os;
	}

	bool operator==(Node &n) {
		return n.getId() == privateId;
	}

	/*bool equals(Node<T> *n) {
		if(n->getValue() != this->value) return false;
		auto workAdjacentList = adjacentNodes;
		for(auto adjNode : n->getAdjacentNodes()) {
			auto foundIt = std::find_if(workAdjacentList.begin(), workAdjacentList.end(),
				[&adjNode](Node<T> *curNode) { return adjNode->getValue() == curNode->getValue(); });
			if(foundIt != workAdjacentList.end()) {
				workAdjacentList.erase(foundIt);
			}
		}
		return workAdjacentList.empty();
	}*/
};

template<class T>
unsigned Node<T>::instanceCount = 0;
#endif
