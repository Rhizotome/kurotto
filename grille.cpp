#include <iostream>
#include "grille.hpp"
using namespace std;
using coordonnee = array<unsigned short int,2>;

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

void Grille::appendContrainte(array<int,3> newContrainte)
{
    if (newContrainte[0] < 0 || newContrainte[0] >= taille || newContrainte[1] < 0 || newContrainte[1] >= taille || newContrainte[2] < -1)
        throw("contrainte incorrecte");
    contraintes.push_back(newContrainte);
}

const vector<array<int,3>>& Grille::getContraintes()const
{
    return contraintes;
}

bool Grille::isResolue()const
{
    for (auto c : contraintes) {
        if (cases[c[0]][c[1]] != blanc || (c[2] != -1 && tailleZone(c[0],c[1]) - 1 != c[2]))
            return false;
    }
    return true;
}

unsigned short Grille::tailleZone(unsigned short a, unsigned short b)const
{
    return this->tailleZone(set<coordonnee> {coordonnee{a,b}});
}

unsigned short Grille::tailleZone(set<coordonnee> centre)const
{
    unsigned short tailleCentreInitiale = centre.size();
    for (auto c : centre) {
        for (auto coordVoisine : array<array<int,2>,4> {{{c[0]-1,c[1]},{c[0]+1,c[1]},{c[0],c[1]-1},{c[0],c[1]+1}}}) {
            try {
                if (cases.at(coordVoisine[0]).at(coordVoisine[1]) == noir) {
                    centre.emplace(coordonnee{(unsigned short)coordVoisine[0],(unsigned short)coordVoisine[1]});
                }
            }
            catch(const out_of_range&) {}
        }
    }
    return centre.size() == tailleCentreInitiale ? tailleCentreInitiale : tailleZone(centre);
}
