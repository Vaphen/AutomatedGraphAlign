/******************************************
 * An Node class intended for graphical use
 * of nodes.
 * Last edited: 09.09.2017
 */

#ifndef GUINODE_H
#define GUINODE_H

#include <armadillo>
#include <initializer_list>
#include "../Node.hpp"

template <class T>
class GUINode : public Node<T>
{
    private:
        /**
         * Private variables
         */
        arma::vec position;
        std::string pathToImage;

    public:
        /**
         *  Constructors and destructors
         */
        GUINode(T value) : Node<T>(value) { };

        GUINode(T value, std::initializer_list<std::shared_ptr<GUINode> > initlist) : Node<T>(value) {
            // cannot pass initlist to parent constructor; need to add nodes manually
            for(auto node : initlist) {
                Node<T>::addAdjacentNode(node);
            }
        }

        virtual ~GUINode() = default;

        /**
         * Getter and setter
         */
        arma::vec getPosition() const
        {
            return this->position;
        }

        void setPosition(double x, double y, double z)
        {
            this->position = {x, y, z};
        }

        void setPosition(arma::vec vec)
        {
            this->position = vec;
        }

        void setPathToImage(std::string path) {
            this->pathToImage = path;
        }

        std::string getPathToImage() const {
            return this->pathToImage;
        }

};

#endif // GUINODE_H
