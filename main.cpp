#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "grille.hpp"
using namespace std;
//CONSTANTES
constexpr int TAILLE = 10; //taille de la grille, l
constexpr int DIMCASE = 60; //largeur d'une case en pixels
constexpr int DIMX = TAILLE * DIMCASE; // dimension x de la fenêtre
constexpr int DIMY = TAILLE * DIMCASE; // dimension y de la fenêtre

sf::Vector2<unsigned short> positionToCoord(sf::Vector2i);
int main()
{
    try {
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



        cout << "Entrer des valeurs pour les éléments de T (entre un nombre négatif pour i ou j pour sortir de la boucle)." << endl;
        while (true) {
            array<int,3> contrainte;
            cout << "i : ";
            cin >> contrainte[0];
            if (contrainte[0] < 0) break;
            cout << "j : ";
            cin >> contrainte[1];
            if (contrainte[1] < 0) break;
            cout << "p : ";
            cin >> contrainte[2];
            grille.appendContrainte(contrainte);
        }

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

        sf::RenderWindow window(sf::VideoMode(DIMY,DIMX),"test");
        //BOUCLE PRINCIPALE
        while(window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    auto coord = positionToCoord(sf::Mouse::getPosition(window));// si il y a un clic gauche, noircir la case dans la grille
                    grille[coord.x][coord.y] = noir;
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    auto coord = positionToCoord(sf::Mouse::getPosition(window));// si il y a clic droit, blanchir la case
                    grille[coord.x][coord.y] = blanc;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    cout << (grille.isResolue() ? "résolue" : "erreur") << endl;
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
