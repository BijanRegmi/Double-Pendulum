#include "doublependulum.h"

Dpendulum::Dpendulum(float length1, float length2, float mass1, float mass2, float angle1, float angle2){
    this->length1 = length1;
    this->length2 = length2;
    this->mass1 = mass1;
    this->mass2 = mass2;
    this->angle1 = angle1;
    this->angle2 = angle2;

    angularAccel1 = 0;
    angularAccel2 = 0;
    angularVel1 = 0;
    angularVel2 = 0;
}

void Dpendulum::setupRenderobjects(float width, float height){
    textr.create(width, height);
    this->setTexture(textr.getTexture());

    vb.create(3);
    vb.setPrimitiveType(sf::LineStrip);

    rodVertices[0].position = sf::Vector2f(width/2, height/2);
    for (int i=0; i<3; i++)
        rodVertices[i].color = sf::Color::Black;

    bob1.setRadius(mass1);
    bob1.setOrigin(bob1.getRadius(), bob1.getRadius());
    bob1.setFillColor(sf::Color::Black);

    bob2.setRadius(mass2);
    bob2.setOrigin(bob2.getRadius(), bob2.getRadius());
    bob2.setFillColor(sf::Color::Black);
}

void Dpendulum::changestate(){
    hold = !hold;
}

void Dpendulum::rodvisibility(){
    showrod = !showrod;
}

void Dpendulum::update(){
    if (hold) return;
    float n11 = -gravity*(2*mass1+mass2)*std::sin(angle1);
    float n12 = -mass2*gravity*std::sin(angle1-2*angle2);
    float n13 = -2*std::sin(angle1-angle2) * mass2;
    float n14 = (angularVel2*angularVel2*length2 + angularVel1*angularVel1*length1*std::cos(angle1-angle2));
    float den = 2*mass1+mass2-mass2*std::cos(2*angle1-2*angle2);
    float n21 = 2*std::sin(angle1-angle2);
    float n22 = angularVel1*angularVel1*length1*(mass1+mass2);
    float n23 = gravity*(mass1+mass2)*std::cos(angle1);
    float n24 = angularVel2*angularVel2*length2*mass2*std::cos(angle1-angle2);

    angularAccel1 = (n11+n12+n13*n14)/(length1*den*FPS*FPS);
    angularAccel2 = (n21*(n22+n23+n24))/(length2*den*FPS*FPS);

    angularVel1 += angularAccel1;
    angularVel2 += angularAccel2;

    angle1 += angularVel1;
    angle2 += angularVel2;

    updatexy();

    angularVel1 *= DAMP1;
    angularVel2 *= DAMP2;
}

void Dpendulum::render(){
    sf::Vector2f endPos1 = sf::Vector2f(posX1*100 + rodVertices[0].position.x, posY1*100 + rodVertices[0].position.y);
    sf::Vector2f endPos2 = sf::Vector2f(posX2*100 + rodVertices[0].position.x, posY2*100 + rodVertices[0].position.y);

    rodVertices[1].position = endPos1;
    rodVertices[2].position = endPos2;
    vb.update(rodVertices);

    bob1.setPosition(endPos1);
    bob2.setPosition(endPos2);

    //Trails
    int s = trails.size();
    if (s<TRAILSIZE){
       trails.push_back(sf::Vertex(endPos2, sf::Color(0, 255, 200, (s+1)*255/TRAILSIZE)));
    } else {
        for (int i=0; i<s-1; i++){
            trails[i].position = trails[i+1].position;
        }
        trails[s-1].position = endPos2;
    }

    textr.clear(sf::Color::White);
    if (showrod) textr.draw(vb);
    textr.draw(bob1);
    textr.draw(bob2);
    textr.draw(&trails[0], trails.size(), sf::LineStrip);
    textr.display();

    std::system("clear");
    std::cout << "Accel 1: " << angularAccel1 << " and Accel 2: " << angularAccel2 << std::endl
              << "Veloc 1: " << angularVel1 << " and Veloc 2: " << angularVel2 << std::endl
              << "Angle 1: " << angle1 << " and Angle 2: " << angle2 << std::endl << std::endl;

}

void Dpendulum::updatexy(){
    posX1 = length1 * std::sin(angle1);
    posY1 = length1 * std::cos(angle1);

    posX2 = posX1 + length2 * std::sin(angle2);
    posY2 = posY1 + length2 * std::cos(angle2);
}

int Dpendulum::bobclicked(sf::Vector2i mousepos){
    if (bob1.getGlobalBounds().contains(mousepos.x, mousepos.y))
        return 1;
    else if (bob2.getGlobalBounds().contains(mousepos.x, mousepos.y))
        return 2;
    else
        return 0;
}

void Dpendulum::clicked(sf::Vector2i mousepos){
    int id = bobclicked(mousepos);
    if (id == 0) return;
    hold = true;

    if (id == 1) moving1 = !moving1;
    if (id == 2) moving2 = !moving2;
}

void Dpendulum::movebob(sf::Vector2i mousepos){
    if (!moving1 && !moving2) return;
    float x = mousepos.x;
    float y = mousepos.y;
    if (moving1){
        float delx = x - rodVertices[0].position.x;
        float dely = y - rodVertices[0].position.y;

        angle1 = std::atan(delx / dely);

        if (dely<0) angle1 += PI;

        length1 = std::sqrt(delx*delx + dely*dely) / 100;

    }

    if (moving2){
        float delx = x - rodVertices[1].position.x;
        float dely = y - rodVertices[1].position.y;

        angle2 = std::atan(delx / dely);

        if (dely<0) angle2 += PI;

        length2 = std::sqrt(delx*delx + dely*dely) / 100;

    }

    angularVel1 = 0;
    angularVel2 = 0;
    updatexy();

}

