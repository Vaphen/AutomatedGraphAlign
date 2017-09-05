#ifndef __EDGE_HPP_
#define __EDGE_HPP_

#include "Node.hpp"

template<typename T, class NODE, bool directed = false>
class Edge {
  NODE *n1;
  NODE *n2;
  T value;
public:
  Edge(NODE *first, NODE *second) : n1(first), n2(second) { }

  NODE *getFirstNode() {
    return n1;
  }

  NODE *getSecondNode() {
    return n2;
  }

  bool operator==(Edge &e) {
    if(directed) {
      return e.getFirstNode()->getId() == n1->getId() && e.getSecondNode()->getId() == n2->getId();
    }

    return e.getFirstNode()->getId() == n1->getId() && e.getSecondNode()->getId() == n2->getId() ||
             e.getFirstNode()->getId() == n2->getId() && e.getSecondNode()->getId() == n1->getId();

  }


  bool operator<(Edge const &e) {
      if(directed) {
        return e.getFirstNode()->getId() < n1->getId() && e.getSecondNode()->getId() < n2->getId();
      } else {
        return e.getFirstNode()->getId() < n1->getId() && e.getSecondNode()->getId() < n2->getId() ||
               e.getFirstNode()->getId() < n2->getId() && e.getSecondNode()->getId() < n1->getId();
      }
    }

    T &getValue() const {
        return value;
    }

    void setValue(T &val) {
        this->value = val;
    }
};

#endif
