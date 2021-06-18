#include "doublependulum.h"

int main(){
    sf::RenderWindow win(sf::VideoMode(600, 600), "Double Pendulum");
    win.setFramerateLimit(FPS);
    sf::Event ev;

    Dpendulum pend(1, 0.8, 10, 4, 90*PI/180, 90*PI/180);
    pend.setupRenderobjects(600, 600);

    while(win.isOpen()){
        //Poll Event
        while(win.pollEvent(ev)){
            if (ev.type == sf::Event::Closed)
                win.close();

            if (ev.type == sf::Event::MouseButtonPressed){
                 pend.clicked(sf::Mouse::getPosition(win));
            }

            if (ev.type == sf::Event::MouseMoved){
                pend.movebob(sf::Mouse::getPosition(win));
            }

            if (ev.type == sf::Event::KeyPressed){
                switch(ev.key.code){
                case sf::Keyboard::Escape:
                    win.close();
                    break;
                case sf::Keyboard::Space:
                    pend.changestate();
                    break;
                case sf::Keyboard::R:
                    pend.rodvisibility();
                    break;
                }
            }
        }

        pend.update();
        pend.render();

        win.clear();

        win.draw(pend);

        win.display();
    }
}
