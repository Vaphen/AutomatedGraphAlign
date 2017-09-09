#ifndef EXPANDINGGRAPHMANAGER_H
#define EXPANDINGGRAPHMANAGER_H

#include <armadillo>
#include <memory>
#include <cmath>
#include <map>
#include <random>
#include "../Graph.hpp"
#include "ArmadilloUtils.hpp"

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
            DEPTH(WIDTH),
            RADIUS(RADIUS)
        {

            if(graph.getNodes().size() == 0) return;


            positionNodes();
        }

        /** \brief Update the positions of all nodes. The update of the position is just a small change
         * which is useful for animations; however, this function must be called many times to get the
         * optimal result.
         */
        void update()
        {

            for(auto node : graph.getNodes()) {

                std::map<std::shared_ptr<NODE>, double> distancesToCurNode = getDistancesToNode(node);

                arma::vec deltaVec = {0, 0, 0};

                // rejection
                for(auto nodeDistancePair : distancesToCurNode) {
                    arma::vec directionVec = calculateDirectionVectorFromTo(nodeDistancePair.first, node);

                    // prevent division by 0 just in case
                    if(nodeDistancePair.second == 0) continue;

                    deltaVec += (std::pow(rejectionFactor, 2)/nodeDistancePair.second) * directionVec;

                }


                //attraction
                for(auto adjPrimitiveNode : node->getAdjacentNodes()) {
                    // cast needed because nodes just save Node-class pointer for adjacent nodes
                    std::shared_ptr<NODE> adjNode = std::dynamic_pointer_cast<NODE>(adjPrimitiveNode);
                    arma::vec directionVec = calculateDirectionVectorFromTo(node, adjNode);


                    deltaVec += std::pow(distancesToCurNode[adjNode], 0.5) * directionVec;

                    // if the graph is directed, we have to implement the reversed attraction
                    // manually.
                    if(isDirected == true) {
                        arma::vec newPos = adjNode->getPosition() + std::pow(distancesToCurNode[adjNode], 0.5) * directionVec;
                        adjNode->setPosition(newPos);
                    }
                }

                node->setPosition(deltaVec + node->getPosition());


            }

        }

        /** \brief adjust the attraction factor for all nodes
         * \param delta the delta of the attraction factor
         */
        void adjustRejectionFactor(double delta) {
            rejectionFactor += delta;
        }

        /** \brief turn the whole graph around the Y-axis
         * \param degree the degree the graph should be turned
         */
        void turnGraphYForDegree(double degree) {
            for(auto node : graph.getNodes()) {
                arma::vec posVec = node->getPosition();
                posVec.at(0) -= WIDTH / 2;
                posVec.at(2) -= DEPTH / 2;
                posVec = ArmaUtils::turnVectorYDegree(posVec, degree);
                posVec.at(0) += WIDTH / 2;
                posVec.at(2) += DEPTH / 2;
                node->setPosition(posVec);
            }
        }


        /** \brief turn the whole graph around the X-axis
         * \param degree the degree the graph should be turned
         */
        void turnGraphXForDegree(double degree) {
            for(auto node : graph.getNodes()) {
                arma::vec posVec = node->getPosition();
                posVec.at(1) -= WIDTH / 2;
                posVec.at(2) -= DEPTH / 2;
                posVec = ArmaUtils::turnVectorXDegree(posVec, degree);
                posVec.at(1) += WIDTH / 2;
                posVec.at(2) += DEPTH / 2;
                node->setPosition(posVec);
            }
        }

    private:
        /**
         * Variables
         */
        TypedGraph &graph;
        const unsigned WIDTH, HEIGHT, DEPTH, RADIUS; // depth is currently set to width; can be changed if needed
        double rejectionFactor = 10.0;

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
        double getDistance(std::shared_ptr<NODE> node1, std::shared_ptr<NODE> node2) {
            arma::vec directionVec = node2->getPosition() - node1->getPosition();

            // euclidean distance
            return ArmaUtils::getLength(directionVec);
        }


        /** \brief Get a normalized direction vector between two given nodes.
         *
         * \param node1 Node<T>* the starting node
         * \param node2 Node<T>* the end node
         * \return std::pair<double, double> a 2D-vector (pair) containing the normalized direction vector.
         *
         */
        arma::vec calculateDirectionVectorFromTo(std::shared_ptr<NODE> node1, std::shared_ptr<NODE> node2) {
            return arma::normalise(node2->getPosition() - node1->getPosition());
        }


        /** \brief Get a map containing the distances between a given node and all other nodes.
         * The given Node is not included in the returned distance map.
         * \param node Node<T>* a pointer to the node from which all distances should calculated
         * \return std::map<Node<T>*, unsigned> a map with pointers to all other nodes as index and the distance
         *  as value.
         */
        std::map<std::shared_ptr<NODE>, double> getDistancesToNode(std::shared_ptr<NODE> node) {
            std::map<std::shared_ptr<NODE>, double> distances;
            for(std::shared_ptr<NODE> curNode : graph.getNodes()) {
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
            for(std::shared_ptr<NODE> node : graph.getNodes()) {
                node->setPosition(getRandomBetween(WIDTH / 2 - 100, WIDTH / 2 + 100),
                                  getRandomBetween(HEIGHT / 2 - 100, HEIGHT / 2 + 100),
                                  getRandomBetween(DEPTH / 2 - 100, DEPTH / 2 + 100));
            }
        }
};

#endif // EXPANDINGGRAPHMANAGER_H
