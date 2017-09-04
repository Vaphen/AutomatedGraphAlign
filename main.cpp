#include <SFML/Graphics.hpp>
#include <random>
#include <map>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <chrono>
#include "Graph.hpp"
#include "ExpandingGraphManager.h"


#define WIDTH 700
#define HEIGHT 700
#define RADIUS 10

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Self expanding graph");
sf::CircleShape shape(RADIUS);

template <class T, bool DIRECTED>
void drawGraph(Graph<T, DIRECTED> &graph) {

    for(Node<T> *node : graph.getNodes()) {
        shape.setFillColor(node->getValue());
        shape.setPosition(node->getPosition().first, node->getPosition().second);
        window.draw(shape);
    }


    for(Node<T> *node : graph.getNodes()) {
        for(Node<T> *adjacentNode : node->getAdjacentNodes()) {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(node->getPosition().first + RADIUS, node->getPosition().second+ RADIUS)),
                sf::Vertex(sf::Vector2f(adjacentNode->getPosition().first + RADIUS, adjacentNode->getPosition().second+ RADIUS))
            };

            window.draw(line, 2, sf::Lines);
        }
    }

}



int main()
{
    Graph<sf::Color> graph;
    auto a = graph.addNode(sf::Color::Green);
    auto b = graph.addNode(sf::Color::Blue, {a});
    auto c = graph.addNode(sf::Color::Yellow, {b});
    auto d = graph.addNode(sf::Color::Cyan, {a, c});




    ExpandingGraphManager<sf::Color> gm(graph, WIDTH, HEIGHT, RADIUS);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        drawGraph(graph);
      /*  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            auto x = graph.addNode(sf::Color(255, 255, 255), {a, b});
            x->setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        }*/
        gm.update();
        window.display();
    }

    return 0;
}




