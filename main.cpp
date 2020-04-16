#include <iostream>
#include <SFML/Graphics.hpp>
#include "grille.hpp"
using namespace std;
//CONSTANTES
constexpr int TAILLE = 20;
constexpr int DIMCASE = 30;
constexpr int DIMX = TAILLE * DIMCASE;
constexpr int DIMY = TAILLE * DIMCASE;

sf::Vector2<unsigned short> positionToCoord(sf::Vector2i);
int main(){
    try{
    //SETUP
    Grille grille(TAILLE);
    sf::Texture backgroundTexture;
    backgroundTexture.create(DIMCASE,DIMCASE);
    sf::Uint8 tex[DIMCASE * DIMCASE * 4];
    for (int i(0); i < DIMCASE * DIMCASE; i++){
        if (i < DIMCASE || i % DIMCASE == 0 || i % DIMCASE == DIMCASE - 1 || DIMCASE * DIMCASE - i < DIMCASE){
            tex[4 * i] = sf::Color::Black.r;
            tex[4 * i + 1] = sf::Color::Black.g;
            tex[4 * i + 2] = sf::Color::Black.b;
            tex[4 * i + 3] = sf::Color::Black.a;
        }
        else{
            tex[4 * i] = sf::Color::White.r;
            tex[4 * i + 1] = sf::Color::White.g;
            tex[4 * i + 2] = sf::Color::White.b;
            tex[4 * i + 3] = sf::Color::White.a;
        }
    }
    backgroundTexture.update(tex);
    backgroundTexture.setRepeated(true);
    sf::Sprite background(backgroundTexture,sf::IntRect(0,0,DIMX,DIMY));
    cout << "Entrer des valeurs pour les éléments de T (entre un nombre négatif pour i ou j pour sortir de la boucle)." << endl;
    while (true){
        array<int,3> contrainte;
        cout << "i : ";
        cin >> contrainte[0];
        if (contrainte[0] < 0) break;
        cout << "j : ";
        cin >> contrainte[1];
        if (contrainte[1] < 0) break;
        cout << "p : ";
        cin >> contrainte[2];
        grille.ajouterContrainte(contrainte);
    }
    sf::RenderWindow window(sf::VideoMode(DIMY,DIMX),"test");
    //BOUCLE PRINCIPALE
    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                auto coord = positionToCoord(sf::Mouse::getPosition(window));
                grille[coord.x][coord.y] = noir;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                auto coord = positionToCoord(sf::Mouse::getPosition(window));
                grille[coord.x][coord.y] = blanc;
            }
            window.clear(sf::Color::White);
            window.draw(background);
            window.display();
        }
    }
    }
    catch (char const *c){
        cout << c;
    }
    return 0;
}

sf::Vector2<unsigned short> positionToCoord(sf::Vector2i position){
    return sf::Vector2<unsigned short>(position.x / DIMCASE, position.y / DIMCASE); 
}
