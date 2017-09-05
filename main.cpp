#include <SFML/Graphics.hpp>
#include <random>
#include <map>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <chrono>
#include "Graph.hpp"
#include "GUINode.h"
#include "ExpandingGraphManager.h"


#define WIDTH 700
#define HEIGHT 700
#define RADIUS 10

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Self expanding graph");
sf::CircleShape shape(RADIUS);

template<typename NODEVAL, typename EDGEVAL, bool isDirected = false,
            template<typename> typename NODETYPE = Node,
            template<typename, class, bool> typename EDGETYPE = Edge>
void drawGraph(Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE> &graph) {

    using NODE = NODETYPE<NODEVAL>;
    using EDGE = EDGETYPE<EDGEVAL, NODE, isDirected>;

    for(NODE *node : graph.getNodes()) {

        shape.setFillColor(node->getValue());
        shape.setPosition(node->getPosition().at(0), node->getPosition().at(1));
        window.draw(shape);
    }


    for(NODE *node : graph.getNodes()) {
        for(Node<sf::Color>* adjacentNode : node->getAdjacentNodes()) {
            GUINode<sf::Color> *adjacentGUINode = dynamic_cast<GUINode<sf::Color>*>(adjacentNode);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(node->getPosition().at(0) + RADIUS, node->getPosition().at(1)+ RADIUS)),
                sf::Vertex(sf::Vector2f(adjacentGUINode->getPosition().at(0) + RADIUS, adjacentGUINode->getPosition().at(1) + RADIUS))
            };

            window.draw(line, 2, sf::Lines);
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}



int main()
{
    Graph<sf::Color, bool, false, GUINode> graph;

    auto openstack = graph.addNode(sf::Color::Green);
    auto ubuntu = graph.addNode(sf::Color::Green, {openstack});
    auto java = graph.addNode(sf::Color::Green, {ubuntu});
    auto tomcat = graph.addNode(sf::Color::Green, {ubuntu, java});
    auto wso2bp2 = graph.addNode(sf::Color::Green, {ubuntu, java});
    auto container = graph.addNode(sf::Color::Green, {wso2bp2, java, tomcat, ubuntu});
    auto admin = graph.addNode(sf::Color::Green, {container, tomcat});
    auto winery = graph.addNode(sf::Color::Green, {container, tomcat});
    auto modeler = graph.addNode(sf::Color::Green, {tomcat, winery});
    auto vinothek = graph.addNode(sf::Color::Green, {tomcat, container});



    /*auto a = graph.addNode(sf::Color::Green);
    auto b = graph.addNode(sf::Color::Blue, {a});
    auto c = graph.addNode(sf::Color::Yellow, {b});
    auto d = graph.addNode(sf::Color::Cyan, {a, c});*/

    //std::cout << a->getPosition() << std::endl;
    ExpandingGraphManager<sf::Color, bool, false, GUINode> gm(graph, WIDTH, HEIGHT, RADIUS);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::MouseButtonReleased) {
            //    auto x = graph.addNode(sf::Color(255, 255, 255), {a, b, c, d});
            //    x->setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }
        }

        window.clear();
        drawGraph(graph);
        gm.update();
        window.display();
    }

    return 0;
}




