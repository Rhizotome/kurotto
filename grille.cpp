#include <iostream>
#include <fstream>
#include "grille.hpp"
#include <algorithm>
#include <cmath>
using namespace std;
using coordonnee = array<unsigned short int,2>;

void Grille::clear()
{
    contraintes.clear();
}

Grille::Grille(unsigned short int l, vector<array<int,3>> c): nbSolutions(0), taille(l), contraintes(c), cases(l)
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
                if (cases.at(coordVoisine[0]).at(coordVoisine[1]) == noir) { // at throw une exception si lescoordonnées sont incorrectes, dans quel cas on ne fait rien
                    centre.emplace(coordonnee{(unsigned short)coordVoisine[0],(unsigned short)coordVoisine[1]});
                }
            }
            catch(const out_of_range&) {}
        }
    }
    return centre.size() == tailleCentreInitiale ? tailleCentreInitiale : tailleZone(centre); // appel récurrent
}

Formule Grille::toFormule()const
{
    CExt<CExt<CInt<litt>>> contenu;
    for (auto contrainte : contraintes) {
        CExt<CInt<litt>> contrainteSousFormule;
        if (contrainte[2] == -1) {
            contrainteSousFormule = {{-toLitt({contrainte[0],contrainte[1]})}};
            contenu.push_back(contrainteSousFormule);
            continue;
        }
        if (contrainte[2] == 0) {
            contrainteSousFormule = {{-toLitt({contrainte[0],contrainte[1]})}};
            vector<coordonnee> voisins;
            if (contrainte[0] >= 1)
                voisins.push_back({contrainte[0]-1,contrainte[1]});
            if (contrainte[0] < taille-1)
                voisins.push_back({contrainte[0]+1,contrainte[1]});
            if (contrainte[1] >= 1)
                voisins.push_back({contrainte[0],contrainte[1]-1});
            if (contrainte[1] < taille - 1)
                voisins.push_back({contrainte[0],contrainte[1]+1});
            for (auto k : voisins) {
                contrainteSousFormule.back().insert({-toLitt(k)});
            }
            contenu.push_back(contrainteSousFormule);
            continue;
        }
        for (auto i : forme({{{contrainte[0],contrainte[1]}}},contrainte[2])) {
            contrainteSousFormule.push_back({});
            auto bordureI = bordure(i);
            i.erase({contrainte[0],contrainte[1]});
            bordureI.insert({contrainte[0],contrainte[1]});
            for (auto j : i) {
                contrainteSousFormule.back().insert({toLitt(j)});
            }
            for (auto j : bordureI) {
                contrainteSousFormule.back().insert({-toLitt(j)});
            }
        }
        contenu.push_back(contrainteSousFormule);
    }
    return Formule{contenu,{}};
}

// le littéral i représente le fait que la ième case (en comptant depuis en haut à gauche et en passant à la ligne à chaque fin de ligne) est noircie
// Cette fonction renvoie l'ensemble des ensembles de points comprenant centre et de taille nbCases
set<set<coordonnee>> Grille::forme(set<set<coordonnee>> centre, unsigned short nbCases)const
{
    if (nbCases <= 0) {
        return centre;
    }
    auto ensembleRec = forme(centre, nbCases - 1);
    set<set<coordonnee>> retour;
    for (auto i : ensembleRec) {
        for (auto j : i) {
            vector<coordonnee> voisins;
            if (j[0] >= 1)
                voisins.push_back({j[0]-1,j[1]});
            if (j[0] < taille-1)
                voisins.push_back({j[0]+1,j[1]});
            if (j[1] >= 1)
                voisins.push_back({j[0],j[1]-1});
            if (j[1] < taille - 1)
                voisins.push_back({j[0],j[1]+1});
            for (auto k : voisins) {
                auto temp = i;
                temp.insert({k});
                if (temp.size() == nbCases + 1) {
                    retour.insert({temp});
                }
            }
        }
    }
    return retour;
}

// Cette fonction renvoie l'ensemble des cellules qui sont en contact avec centre et ne sont pas dans centre 
set<coordonnee> Grille::bordure(const set<coordonnee> &centre)const
{
    set<coordonnee> retour{};
    for (auto i : centre) {
        vector<coordonnee> voisins;
        if (i[0] >= 1)
            voisins.push_back({i[0]-1,i[1]});
        if (i[0] < taille-1)
            voisins.push_back({i[0]+1,i[1]});
        if (i[1] >= 1)
            voisins.push_back({i[0],i[1]-1});
        if (i[1] < taille- 1)
            voisins.push_back({i[0],i[1]+1});
        for (auto j : voisins) {
            if (centre.find(j)==centre.end())
                retour.insert({j});
        }
    }
    return retour;
}

// le littéral i représente le fait que la ième case (en comptant depuis en haut à gauche et en passant à la ligne à chaque fin de ligne) est noircie
litt Grille::toLitt(coordonnee c)const
{
    return (unsigned short)(c[0] + taille * c[1] + 1);
}

coordonnee Grille::toCoord(litt l)const
{
    if (l < 0) {
        l = -l;
    }
    return {(l - 1) % taille, (l - 1) / taille};
}

// On trie les contraintes par ordre de p croissants et on appelle resoudre sur une formule créée à partir de la grille puis on place une des réponses dans la grille
void Grille::resoudre()
{
    sort(contraintes.begin(),contraintes.end(),[](array<int,3>a, array<int,3>b) {
        return a[2] < b[2];
    });
    auto formule = toFormule();
    formule.resoudre();
    nbSolutions = 0;
    // On détermine le nombre de solution possible pour chaque solution trouvée à la formule
    std::for_each(formule.grilleResolue.begin(),formule.grilleResolue.end(),[&](auto i) {
        this->nbSolutions += pow(2.0,(pow(this->taille, 2) - i.size()));
    });
    if (formule.grilleResolue.size() >= 1) {
        for (auto i : formule.grilleResolue[0]) {
            auto c = this->toCoord(i);
            cases[c[0]][c[1]] = (i < 0 ? blanc : noir);
        }
    }
}

unsigned long long int Grille::nombreSolutions()const
{
    return nbSolutions;
}

void Grille::fromFile(string path)
{
    contraintes.clear();
    ifstream fichier (path);
    if(!fichier.is_open()) {
        throw("fichier introuvable");
    }
    int i, j, p;
    while (fichier >> i) {
        fichier >> j;
        fichier >> p;
        appendContrainte({j - 1,i - 1,p});
    }
}
