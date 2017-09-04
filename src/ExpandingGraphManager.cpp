#include "ExpandingGraphManager.h"

tmeplate <class T>
ExpandingGraphManager::ExpandingGraphManager(Graph<T> &graph)
{
    this->graph = graph;
}

template <class T>
ExpandingGraphManager::~ExpandingGraphManager()
{
    //dtor
}
