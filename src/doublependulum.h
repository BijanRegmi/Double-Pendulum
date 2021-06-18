#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdlib.h>

class Dpendulum: public sf::Sprite{
private:
    //Params defining the state of pendulum
    float angle1, angle2;
    float length1, length2;
    float angularAccel1, angularAccel2;
    float angularVel1, angularVel2;
    float mass1, mass2;
    float posX1, posX2;
    float posY1, posY2;
    bool hold = false;
    bool moving1 = false;
    bool moving2 = false;
    bool showrod = true;

    //Render objects
    sf::RenderTexture textr;
    sf::Vertex rodVertices[3];
    sf::VertexBuffer vb;
    sf::CircleShape bob1;
    sf::CircleShape bob2;
    std::vector<sf::Vertex> trails;

    //Functions
    void updatexy();
    int bobclicked(sf::Vector2i mousepos);
public:
    //Constructors
    Dpendulum(float length1, float length2, float mass1, float mass2, float angle1, float angle2);
    void setupRenderobjects(float width, float height);


    void update();
    void render();


    void changestate();
    void movebob(sf::Vector2i mousepos);
    void clicked(sf::Vector2i mousepos);

    void rodvisibility();
};




#endif // DOUBLEPENDULUM_H


