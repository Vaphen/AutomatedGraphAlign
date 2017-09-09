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


#define WIDTH 1000
#define HEIGHT 1000
#define RADIUS 10

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Self expanding graph");

template<typename NODEVAL, typename EDGEVAL, bool isDirected = false,
            template<typename> typename NODETYPE = Node,
            template<typename, class, bool> typename EDGETYPE = Edge>
void drawGraph(Graph<NODEVAL, EDGEVAL, isDirected, NODETYPE, EDGETYPE> &graph) {

    using NODE = NODETYPE<NODEVAL>;

    for(std::shared_ptr<NODE> node : graph.getNodes()) {
        for(std::shared_ptr<Node<sf::Color> > adjacentNode : node->getAdjacentNodes()) {
            std::shared_ptr<GUINode<sf::Color> > adjacentGUINode = std::dynamic_pointer_cast<GUINode<sf::Color> >(adjacentNode);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(node->getPosition().at(0) + RADIUS, node->getPosition().at(1)+ RADIUS)),
                sf::Vertex(sf::Vector2f(adjacentGUINode->getPosition().at(0) + RADIUS, adjacentGUINode->getPosition().at(1) + RADIUS))
            };

            window.draw(line, 2, sf::Lines);
        }
    }


    std::vector<std::shared_ptr<NODE>> sortedNodes = graph.getNodes();
    std::sort(sortedNodes.begin(), sortedNodes.end(), [](std::shared_ptr<NODE> node1, std::shared_ptr<NODE> node2) { return node1->getPosition().at(2) < node2->getPosition().at(2); });
    for(std::shared_ptr<NODE> node : sortedNodes) {
        sf::Texture texture;
        texture.loadFromFile(node->getPathToImage());
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setColor(sf::Color(255, 255, 255, 255));
        sprite.setPosition(node->getPosition().at(0), node->getPosition().at(1));

        window.draw(sprite);
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

    openstack->setPathToImage("image/openstack_small.png");
    ubuntu->setPathToImage("image/ubuntu_small.png");
    java->setPathToImage("image/java_small.png");
    tomcat->setPathToImage("image/tomcat_small.png");
    wso2bp2->setPathToImage("image/gear_small.png");
    container->setPathToImage("image/container_small.png");
    admin->setPathToImage("image/tosca_small.png");
    winery->setPathToImage("image/winery_small.jpg");
    modeler->setPathToImage("image/topology_small.png");
    vinothek->setPathToImage("image/vinothek_small.png");


    ExpandingGraphManager<sf::Color, bool, false, GUINode> gm(graph, WIDTH, HEIGHT, RADIUS);

    int firstX = 0, firstY = 0;
    bool clicked = false;
    std::shared_ptr<GUINode<sf::Color> > addedNode = nullptr;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::MouseButtonReleased) {
                clicked = false;
                firstX = 0;
                if(event.mouseButton.button == sf::Mouse::Right) {
                    if(!graph.removeNode(addedNode)) {
                        addedNode = graph.addNode(sf::Color(255, 255, 255), {admin, winery, modeler, vinothek});
                        addedNode->setPathToImage("image/watchdog_small.png");
                        addedNode->setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0);
                    }

                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                gm.adjustRejectionFactor(event.mouseWheel.delta);
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(!clicked) {
                firstX = sf::Mouse::getPosition(window).x;
                firstY = sf::Mouse::getPosition(window).y;
            }
            clicked = true;
            int newXPos = sf::Mouse::getPosition(window).x;
            int newYPos = sf::Mouse::getPosition(window).y;
            if(firstX != newXPos || firstY != newYPos) {

                gm.turnGraphYForDegree((float)(newXPos - firstX) * 0.01);
                gm.turnGraphXForDegree(-(float)(newYPos - firstY) * 0.01);
                firstX = newXPos;
                firstY = newYPos;
                clicked = false;
            }
        }





        window.clear();
        drawGraph(graph);
        gm.update();
        window.display();
    }

    return 0;
}

