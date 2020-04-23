#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "grille.hpp"
using namespace std;
int DIMX;
int DIMY;
int TAILLE;
int DIMCASE;
sf::Vector2<unsigned short> positionToCoord(sf::Vector2i);
int main()
{
    try {
        bool modifiee = false;
        cin>>TAILLE;
        DIMCASE = 80; //largeur d'une case en pixels
        DIMX = TAILLE * DIMCASE; // dimension x de la fenêtre
        DIMY = TAILLE * DIMCASE; // dimension y de la fenêtre

        //SETUP
        Grille grille(TAILLE); // on crée la grille principale sans contraintes aucunes

        // Création de la texture du background
        sf::Texture backgroundTexture;
        backgroundTexture.create(DIMCASE,DIMCASE);
        sf::Uint8 tex[DIMCASE * DIMCASE * 4];
        for (int i(0); i < DIMCASE * DIMCASE; i++) {
            if (i < DIMCASE || i % DIMCASE == 0 || i % DIMCASE == DIMCASE - 1 || DIMCASE * DIMCASE - i < DIMCASE) {
                tex[4 * i] = sf::Color::Black.r;
                tex[4 * i + 1] = sf::Color::Black.g;
                tex[4 * i + 2] = sf::Color::Black.b;
                tex[4 * i + 3] = sf::Color::Black.a;
            }
            else {
                tex[4 * i] = sf::Color::White.r;
                tex[4 * i + 1] = sf::Color::White.g;
                tex[4 * i + 2] = sf::Color::White.b;
                tex[4 * i + 3] = sf::Color::White.a;
            }
        }
        backgroundTexture.update(tex);
        backgroundTexture.setRepeated(true);
        sf::Sprite background(backgroundTexture,sf::IntRect(0,0,DIMX,DIMY));


        sf::RectangleShape caseNoire(sf::Vector2f(DIMCASE,DIMCASE));
        caseNoire.setFillColor(sf::Color::Black); //on crée un sprite de carré noir pour couvrir les cases noircies

        sf::Text texte;
        texte.setCharacterSize(DIMCASE-DIMCASE/4);
        sf::Font arial;
        if (!arial.loadFromFile("arial.ttf")) {
            throw ("erreur de chargement de police");
        }
        texte.setFont(arial);
        texte.setFillColor(sf::Color::Black);

        sf::RenderWindow window(sf::VideoMode(DIMY,DIMX),"test",sf::Style::Close);
        //BOUCLE PRINCIPALE
        while(window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    modifiee = true;
                    auto coord = positionToCoord(sf::Mouse::getPosition(window));// si il y a un clic gauche, noircir la case dans la grille
                    if (grille[coord.x][coord.y] == noir) {
                        grille[coord.x][coord.y] = blanc;
                    }
                    else {
                        grille[coord.x][coord.y] = noir;
                    }
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    modifiee = true;
                    auto coord = positionToCoord(sf::Mouse::getPosition(window));// si clic droit, créer contrainte
                    array<int,3> contrainte{coord.x,coord.y,-1};
                    cout<< "p : ";
                    cin>>contrainte[2];
                    grille.appendContrainte(contrainte);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    if(modifiee) {
                        grille.resoudre();
                    }
                }
                window.clear(sf::Color::White);
                window.draw(background);
                for (int i(0) ; i < TAILLE ; i++) {
                    for (int j(0) ; j < TAILLE ; j++) {
                        if (grille[i][j] == noir) {
                            caseNoire.setPosition(i*DIMCASE, j*DIMCASE);
                            window.draw(caseNoire);
                        }
                    }
                }
                const vector<array<int,3>> contraintes = grille.getContraintes();
                for (auto c : contraintes) {
                    texte.setString(to_string(c[2]));
                    texte.setPosition(c[0] * DIMCASE + (c[2] / 10 >= 1 ? DIMCASE / 20 :  2 * DIMCASE / 7), c[1] * DIMCASE);
                    window.draw(texte);
                }
                window.display();
            }
        }
    }
    catch (char const *c) {
        cout << c;
    }
    return 0;
}

sf::Vector2<unsigned short> positionToCoord(sf::Vector2i position)
{
    sf::Vector2<unsigned short> coord;
    coord.x = position.x < 0 ? 0 : (position.x >= TAILLE*DIMCASE ? TAILLE - 1 : position.x/DIMCASE);
    coord.y = position.y < 0 ? 0 : (position.y >= TAILLE*DIMCASE ? TAILLE - 1 : position.y/DIMCASE);
    return coord;
}
