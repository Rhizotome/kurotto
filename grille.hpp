#pragma once
#include <vector>
#include <set>
#include <array>
#include <cassert>
#include <unordered_set>
enum Etat {
    noir,
    blanc,
    croix
};
class Grille {
protected:
    unsigned short int taille;
    std::vector<std::array<int,3>> contraintes;
    std::vector<std::vector<Etat>> cases;
    unsigned short int tailleZone(unsigned short int, unsigned short int)const; // taille de la zone noire autour de la case
    unsigned short int tailleZone(std::set<std::array<unsigned short int,2>>)const; // taille de la zone noire autour de la zone
public:
    Grille(unsigned short int, std::vector<std::array<int,3>> = {}); // largeur de la grille, ensemble de contraintes
    std::vector<Etat>& operator[](int);
    void appendContrainte(std::array<int,3>);
    const std::vector<std::array<int,3>>& getContraintes()const;
    bool isResolue()const;
};
