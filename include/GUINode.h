#ifndef GUINODE_H
#define GUINODE_H

#include <armadillo>
#include "../Node.hpp"

template <class T>
class GUINode : public Node<T>
{
    public:
        GUINode(T value) : Node<T>(value) { };
        GUINode(T value, std::initializer_list<GUINode<T>*> initlist) : Node<T>(value) {
            // cannot pass initlist to parent constructor; need to add nodes manually
            for(Node<T> *node : initlist) {
                Node<T>::addAdjacentNode(node);
            }
        }

        virtual ~GUINode() = default;

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


    private:
        arma::vec position;
        std::string pathToImage;
};

#endif // GUINODE_H
