#pragma once
#include <vector>
#include <array>
#include <cassert>
enum Etat {
    noir,
    blanc
};
class Grille {
protected:
    unsigned short int taille;
    std::vector<std::array<int,3>> contraintes;
    std::vector<std::vector<Etat>> cases;
public:
    Grille(unsigned short int, std::vector<std::array<int,3>> = {});
    std::vector<Etat>& operator[](int);
    void ajouterContrainte(std::array<int,3>);
    const std::vector<std::array<int,3>>& getContraintes()const;
};
