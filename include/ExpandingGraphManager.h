#ifndef EXPANDINGGRAPHMANAGER_H
#define EXPANDINGGRAPHMANAGER_H

#include <armadillo>
#include "../Graph.hpp"

/** \brief This class handles the automated expansion of nodes until they
 * reach a predefined distance between each other. Useful for graphical
 * display of graphs.
 */
template<typename NODEVAL, typename EDGEVAL, bool isDirected = false,
            template<typename> typename NODETYPE = Node,
            template<typename, typename, bool> typename EDGETYPE = Edge>
class ExpandingGraphManager
{
    using TypedGraph = Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE>;
    using NODE = NODETYPE<NODEVAL>;
    using EDGE = EDGETYPE<EDGEVAL, NODE, isDirected>;

    public:
        /** \brief Constructor for initialization
         *
         * \param graph Graph<T>& the graph to handle
         * \param WIDTH unsigned the WIDTH of the window
         * \param HEIGHT unsigned the HEIGHT of the window
         * \param RADIUS unsigned the RADIUS of the nodes
         *
         */
        ExpandingGraphManager(TypedGraph &graph, unsigned WIDTH, unsigned HEIGHT, unsigned RADIUS) :
            graph(graph),
            WIDTH(WIDTH),
            HEIGHT(HEIGHT),
            RADIUS(RADIUS)
        {

            if(graph.getNodes().size() == 0) return;


            positionNodes();
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
            std::map<NODE*, arma::vec> newPositions;
            for(NODE *node : graph.getNodes()) {

                std::map<NODE*, double> distancesToCurNode = getDistancesToNode(node);

                arma::vec deltaVec = {0, 0, 0};

                // rejection
                for(auto nodeDistancePair : distancesToCurNode) {
                    arma::vec directionVec = calculateDirectionVectorFromTo(nodeDistancePair.first, node);

                    // prevent division by 0 just in case
                    double divisorBiggerZero =  std::pow(nodeDistancePair.second, 0.1);

                    if(divisorBiggerZero == 0 ) continue;

                    deltaVec += (1/divisorBiggerZero) * directionVec;

                }
                //attraction
                for(Node<NODEVAL> *adjPrimitiveNode : node->getAdjacentNodes()) {
                    NODE *adjNode = dynamic_cast<NODE*>(adjPrimitiveNode);
                    arma::vec directionVec = calculateDirectionVectorFromTo(node, adjNode);


                    deltaVec += (distancesToCurNode[adjNode] / attractionFactor) * directionVec;

                       // if the graph is directed, we have to implement the reversed attraction
                    // manually.
                    if(isDirected == true) {
                        arma::vec newPos = adjNode->getPosition() + (distancesToCurNode[adjNode] / attractionFactor) * directionVec;
                        adjNode->setPosition(newPos);
                    }
                }


                arma::vec newPosition = deltaVec + node->getPosition();

              /*  newPosition.at(0) -= WIDTH / 2;
                newPosition.at(2) -= WIDTH / 2;

                arma::vec turnedVec = turnVectorXDegree(newPosition, 0.01);

                turnedVec.at(0) += WIDTH / 2;
                turnedVec.at(2) += WIDTH / 2;

                turnedVec.at(1) -= WIDTH / 2;
                turnedVec.at(2) -= WIDTH / 2;
                turnedVec = turnVectorYDegree(turnedVec, 0.01);
                turnedVec.at(1) += WIDTH / 2;
                turnedVec.at(2) += WIDTH / 2;*/

                newPositions[node] = newPosition;
            }

            for(auto x : newPositions) {
                x.first->setPosition(x.second);
            }
        }

        void setDeltaZ(int delta) {
            if(attractionFactor + delta <= 0) return;

            attractionFactor += delta;
        }

        void turnGraphYForDegree(float degree) {
            for(auto node : graph.getNodes()) {
                arma::vec posVec = node->getPosition();
                posVec.at(0) -= WIDTH / 2;
                posVec.at(2) -= WIDTH / 2;
                arma::vec turnedVec = turnVectorYDegree(posVec, degree);
                turnedVec.at(0) += WIDTH / 2;
                turnedVec.at(2) += WIDTH / 2;
                node->setPosition(turnedVec);
            }
        }

        void turnGraphXForDegree(float degree) {
            for(auto node : graph.getNodes()) {
                arma::vec posVec = node->getPosition();
                posVec.at(1) -= WIDTH / 2;
                posVec.at(2) -= WIDTH / 2;
                arma::vec turnedVec = turnVectorXDegree(posVec, degree);
                turnedVec.at(1) += WIDTH / 2;
                turnedVec.at(2) += WIDTH / 2;
                node->setPosition(turnedVec);
            }
        }

    private:
        /**
         * Variables
         */
        TypedGraph &graph;
        const unsigned WIDTH, HEIGHT, RADIUS;
        int attractionFactor = 10000;


        arma::vec turnVectorYDegree(arma::vec &pos, double degree) {
            arma::mat rotationMatrix = { { std::cos(degree), 0, std::sin(degree) },
                                         { 0, 1, 0 },
                                         { -std::sin(degree), 0, std::cos(degree) } };

            return rotationMatrix * pos;
        }

        arma::vec turnVectorXDegree(arma::vec &pos, double degree) {
            arma::mat rotationMatrix = { { 1, 0, 0 },
                                         { 0, std::cos(degree), -std::sin(degree) },
                                         { 0, std::sin(degree), std::cos(degree) } };

            return rotationMatrix * pos;
        }

        arma::vec turnVectorZDegree(arma::vec &pos, double degree) {
            arma::mat rotationMatrix = { { std::cos(degree), -std::sin(degree), 0 },
                                         { std::sin(degree), std::cos(degree), 0 },
                                         { 0, 0, 1 } };

            return rotationMatrix * pos;
        }


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


        /** \brief Get the euclidean distance between two nodes.
         *
         * \param node1 Node<T>* a pointer to the first node
         * \param node2 Node<T>* a pointer to the second node
         * \return unsigned the distance between the two nodes
         *
         */
        double getDistance(NODE *node1, NODE *node2) {
            arma::vec directionVec = node2->getPosition() - node1->getPosition();

            // euclidean distance
            double distance = 0.f;
            directionVec.for_each([&distance](double x) {distance += x*x;});

            // Pythagoras
            return distance;
        }


        /** \brief Get a normalized direction vector between two given nodes.
         *
         * \param node1 Node<T>* the starting node
         * \param node2 Node<T>* the end node
         * \return std::pair<double, double> a 2D-vector (pair) containing the normalized direction vector.
         *
         */
        arma::vec calculateDirectionVectorFromTo(NODE *node1, NODE *node2) {

            arma::vec direction = node2->getPosition() - node1->getPosition();
            return arma::normalise(direction);

        }


        /** \brief Get a map containing the distances between a given node and all other nodes.
         * The given Node is not included in the returned distance map.
         * \param node Node<T>* a pointer to the node from which all distances should calculated
         * \return std::map<Node<T>*, unsigned> a map with pointers to all other nodes as index and the distance
         *  as value.
         */
        std::map<NODE*, double> getDistancesToNode(NODE *node) {
            std::map<NODE*, double> distances;
            for(NODE *curNode : graph.getNodes()) {
                if(node == curNode) continue;
                distances[curNode] = getDistance(node, curNode);
            }
            return distances;
        }


        /** \brief Set the position of all nodes. Outgoing from a given node,
         * the positions of the child nodes are recursively are set.
         * \param node Node<T>* the starting node. The position of this node must be already set.
         */
        void positionNodes() {
            for(NODE *node : graph.getNodes()) {
                node->setPosition(getRandomBetween(WIDTH / 2 - 100, WIDTH / 2 + 100), getRandomBetween(HEIGHT / 2 - 100, HEIGHT / 2 + 100), getRandomBetween(WIDTH / 2 - 100, WIDTH / 2 + 100));
            }
        }
};

#endif // EXPANDINGGRAPHMANAGER_H
