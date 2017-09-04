#ifndef __EDGE_HPP_
#define __EDGE_HPP_

#include "Node.hpp"

template<typename T, bool directed = false>
class Edge {
  Node<T> *n1;
  Node<T> *n2;
public:
  Edge(Node<T> *first, Node<T> *second) : n1(first), n2(second) { }

  Node<T> *getFirstNode() {
    return n1;
  }

  Node<T> *getSecondNode() {
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
};

#endif
