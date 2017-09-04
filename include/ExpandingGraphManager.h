#ifndef EXPANDINGGRAPHMANAGER_H
#define EXPANDINGGRAPHMANAGER_H

/** \brief This class handles the automated expansion of nodes until they
 * reach a predefined distance between each other. Useful for graphical
 * display of graphs.
 */
template <class T, bool DIRECTED = false>
class ExpandingGraphManager
{
    public:
        /** \brief Constructor for initialization
         *
         * \param graph Graph<T>& the graph to handle
         * \param WIDTH unsigned the WIDTH of the window
         * \param HEIGHT unsigned the HEIGHT of the window
         * \param RADIUS unsigned the RADIUS of the nodes
         *
         */
        ExpandingGraphManager(Graph<T, DIRECTED> &graph, unsigned WIDTH, unsigned HEIGHT, unsigned RADIUS) :
            graph(graph),
            WIDTH(WIDTH),
            HEIGHT(HEIGHT),
            RADIUS(RADIUS)
        {

            if(graph.getNodes().size() == 0) return;

            graph.getNodes().back()->setPosition(WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS);
            positionAllNodes(graph.getNodes().back());
        }

        /*void positionNodes() {
            for(auto node : graph.getNodes()) {
                node->setPosition(getRandomBetween(0, WIDTH), getRandomBetween(0, HEIGHT));
            }
        }*/

        /** \brief Update the positions of all nodes. The update of the position is just a small change
         * which is useful for animations; however, this function must be called many times to get the
         * optimal result.
         */
        void update()
        {
            for(Node<T> *node : graph.getNodes()) {

                std::map<Node<T>*, float> distancesToCurNode = getDistancesToNode(node);

                float deltaX = 0;
                float deltaY = 0;

                // rejection
                for(auto nodeDistancePair : distancesToCurNode) {
                    std::pair<float, float> directionVec = calculateDirectionVectorFromTo(nodeDistancePair.first, node);

                    // prevent division by 0
                    //float xBiggerZero =  * directionVec.first;
                    //float yBiggerZero = nodeDistancePair.second * directionVec.second;

              //      if(xBiggerZero == 0 || yBiggerZero == 0) continue;


                    deltaX += (1/std::pow(nodeDistancePair.second, 0.8)) * directionVec.first;
                    deltaY += (1/std::pow(nodeDistancePair.second, 0.8)) * directionVec.second;


                }

                //attraction
                for(Node<T> *adjNode : node->getAdjacentNodes()) {
                    std::pair<float, float> directionVec = calculateDirectionVectorFromTo(node, adjNode);


                    float xVec = distancesToCurNode[adjNode] / 1000 * directionVec.first;
                    float yVec = distancesToCurNode[adjNode] / 1000 * directionVec.second;


                    deltaX += xVec;
                    deltaY += yVec;

                       // if the graph is directed, we have to implement the reversed attraction
                    // manually.
                    if(DIRECTED == true) {
                        auto adjacentPosition = adjNode->getPosition();
                        float newAdjXPos = adjacentPosition.first + distancesToCurNode[adjNode] / 1000 * directionVec.first;
                        float newAdjYPos = adjacentPosition.second + distancesToCurNode[adjNode] / 1000 * directionVec.second;
                        adjNode->setPosition(newAdjXPos,
                                                  newAdjYPos);
                    }
                }

                node->setPosition(node->getPosition().first + deltaX, node->getPosition().second + deltaY);
            }
        }

    private:
        /**
         * Variables
         */
        Graph<T, DIRECTED> &graph;
        const unsigned WIDTH, HEIGHT, RADIUS;


        /** \brief Get a random value including both sides of the given range.
         *
         * \param x int lower value
         * \param y int upper value
         * \return int a random value between (including) lower and upper value
         *
         */
        int getRandomBetween(int x, int y) {
            std::mt19937 rng;
            rng.seed(std::random_device()());
            std::uniform_int_distribution<std::mt19937::result_type> dist(x, y);

            return dist(rng);
        }


        /** \brief Get the distance between two nodes.
         *
         * \param node1 Node<T>* a pointer to the first node
         * \param node2 Node<T>* a pointer to the second node
         * \return unsigned the distance between the two nodes
         *
         */
        float getDistance(Node<T> *node1, Node<T> *node2) {
            float deltaX = std::abs(node1->getPosition().first - node2->getPosition().first);
            float deltaY = std::abs(node1->getPosition().second - node2->getPosition().second);

            // Pythagoras
            return std::sqrt(deltaX*deltaX + deltaY*deltaY);
        }


        /** \brief Get a normalized direction vector between two given nodes.
         *
         * \param node1 Node<T>* the starting node
         * \param node2 Node<T>* the end node
         * \return std::pair<float, float> a 2D-vector (pair) containing the normalized direction vector.
         *
         */
        std::pair<float, float> calculateDirectionVectorFromTo(Node<T> *node1, Node<T> *node2) {
            float n1x = node1->getPosition().first;
            float n1y = node1->getPosition().second;
            float n2x = node2->getPosition().first;
            float n2y = node2->getPosition().second;

            float directionX = n2x - n1x;
            float directionY = n2y - n1y;

            // Pythagoras
            float length = std::sqrt(directionX*directionX + directionY*directionY);

            // Normalization of values
            float normDirectionX = directionX / length;
            float normDirectionY = directionY / length;


            return std::pair<float, float>(normDirectionX, normDirectionY);
        }


        /** \brief Get a map containing the distances between a given node and all other nodes.
         * The given Node is not included in the returned distance map.
         * \param node Node<T>* a pointer to the node from which all distances should calculated
         * \return std::map<Node<T>*, unsigned> a map with pointers to all other nodes as index and the distance
         *  as value.
         */
        std::map<Node<T>*, float> getDistancesToNode(Node<T> *node) {
            std::map<Node<T>*, float> distances;
            for(Node<T> *curNode : graph.getNodes()) {
                if(node == curNode) continue;
                distances[curNode] = getDistance(node, curNode);
            }
            return distances;
        }


        std::map<Node<sf::Color>*, bool> positionedNodes;
        /** \brief Set the position of all nodes. Outgoing from a given node,
         * the positions of the child nodes are recursively are set.
         * \param node Node<T>* the starting node. The position of this node must be already set.
         */
        void positionAllNodes(Node<T> *node) {

            bool changedNewNode = false;
            for(Node<T> *adjacentNode : node->getAdjacentNodes()) {
                if(positionedNodes[adjacentNode] == true) continue;
                changedNewNode = true;
                adjacentNode->setPosition(node->getPosition().first + getRandomBetween(-50, 50),
                                          node->getPosition().second + getRandomBetween(-50, 50));
                positionedNodes[adjacentNode] = true;
                positionAllNodes(adjacentNode);
            }
            if(changedNewNode == false) return;
        }
};

#endif // EXPANDINGGRAPHMANAGER_H
