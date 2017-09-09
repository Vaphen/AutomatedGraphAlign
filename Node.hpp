#ifndef __NODE_H_
#define __NODE_H_

#include <set>
#include <initializer_list>
#include <algorithm>


template<class T>
class Node
{
    // node members
	T value;
	std::set<std::shared_ptr<Node<T> > > adjacentNodes;

	// unique ids
	static unsigned instanceCount; // initialized at the end of this file
	unsigned privateId;

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
	Node(T value, std::initializer_list<std::shared_ptr<Node<T> > > nodes) : value(value), adjacentNodes(nodes), privateId(instanceCount) {
		instanceCount++;
	}

	/**
	 * @brief Virtual destructor
	 */
	virtual ~Node() = default;

	/**
	 * @brief Add a new adjacent node to this one
	 * @param a pointer to the new adjacent node
     */
	void addAdjacentNode(std::shared_ptr<Node<T> >node) {
		adjacentNodes.insert(node);
	}

    /** \brief Add multiple adjacent nodes to this one
     * \param nodes an initializer list containing all new adjacent nodes
     */
	void addAdjacentNodes(std::initializer_list<Node<T>*> nodes) {
	    for(Node<T> *node : nodes) {
            adjacentNodes.insert(node);
	    }
	}

    /** \brief Remove an adjacent node
     * \param delnode a pointer to the node that should be deleted
     * \return bool true if deletion was successful, else false.
     */
	bool removeAdjacentNode(std::shared_ptr<Node<T> > delnode) {
	    auto delIt = std::find(adjacentNodes.begin(), adjacentNodes.end(), delnode);
        if(delIt != adjacentNodes.end()) {
            adjacentNodes.erase(delIt);
            return true;
        }
        return false;
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
	std::set<std::shared_ptr<Node<T> > > &getAdjacentNodes() {
		return adjacentNodes;
	}

	/**
	 * @brief get the unique id
	 * @return an unsigned int as id
     */
	unsigned getId() {
		return privateId;
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

    /** \brief Overwritten equals operator; two nodes are equal if their id is equal.
     */
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
