/******************************************
 * An Edge class intended for graph usage.
 * Last edited: 09.09.2017
 */


#ifndef __EDGE_HPP_
#define __EDGE_HPP_

#include <memory>
#include "Node.hpp"

template<typename T, class NODE, bool directed = false>
class Edge {
    /**
    * private variables
    */
    std::shared_ptr<NODE> n1;
    std::shared_ptr<NODE> n2;
    T value;
public:
    // default constructor
    Edge(std::shared_ptr<NODE> first, std::shared_ptr<NODE> second) : n1(first), n2(second) { };


    /** \brief get the source node of the edge.
    * \return a pointer to the first node.
    */
    std::shared_ptr<NODE> getFirstNode() {
    return n1;
    }

    /** \brief get the target node of the edge.
    * \return a pointer to the target node.
    */
    std::shared_ptr<NODE> getSecondNode() {
    return n2;
    }

    /** \brief get the value of the edge.
     * \return the value of the edge.
     */
    T getValue() const {
        return value;
    }

    /** \brief set the value of the edge.
     * \param the value of the edge.
     */
    void setValue(T &val) {
        this->value = val;
    }


    /** \brief Overwritten equals operator.
    * If the graph is directed, two edges are equal, if their source and target node are the same.
    * Else, the order of source- and target node don't matter.
    */
    bool operator==(Edge &e) {
    if(directed) {
      return e.getFirstNode()->getId() == n1->getId() && e.getSecondNode()->getId() == n2->getId();
    }

    return e.getFirstNode()->getId() == n1->getId() && e.getSecondNode()->getId() == n2->getId() ||
             e.getFirstNode()->getId() == n2->getId() && e.getSecondNode()->getId() == n1->getId();

    }

    /** \brief Overwritten less operator. Needed for use with maps.
    */
    bool operator<(Edge const &e) {
      if(directed) {
        return e.getFirstNode()->getId() < n1->getId() && e.getSecondNode()->getId() < n2->getId();
      } else {
        return e.getFirstNode()->getId() < n1->getId() && e.getSecondNode()->getId() < n2->getId() ||
               e.getFirstNode()->getId() < n2->getId() && e.getSecondNode()->getId() < n1->getId();
      }
    }
};

#endif
