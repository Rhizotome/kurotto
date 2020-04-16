#include <iostream>
#include "grille.hpp"
using namespace std;

Grille::Grille(unsigned short int l, vector<array<int,3>> c): taille(l), contraintes(c), cases(l)
{
    for (auto i : c) {
        if (i[0] < 0 || i[0] >= taille || i[1] < 0 || i[1] >= taille || i[2] <= -1)
            throw("contrainte incorrecte");
    }
    vector<Etat> ligne(taille);
    fill(ligne.begin(),ligne.end(),blanc);
    fill(cases.begin(),cases.end(),ligne);
};

vector<Etat>& Grille::operator[](int i)
{
    assert(i >= 0 && i < taille);
    return cases[i];
};

void Grille::ajouterContrainte(array<int,3> newContrainte)
{
    if (newContrainte[0] < 0 || newContrainte[0] >= taille || newContrainte[1] < 0 || newContrainte[1] >= taille || newContrainte[2] < -1)
        throw("contrainte incorrecte");
    contraintes.push_back(newContrainte);
}
