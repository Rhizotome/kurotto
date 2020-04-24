#ifndef GRILLE_H
#define GRILLE_H
#include <vector>
#include <set>
#include <array>
#include <cassert>
#include <unordered_set>
#include "formule.hpp"
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
    std::set<std::set<std::array<unsigned short int,2>>> forme(std::set<std::set<std::array<unsigned short int,2>>>, unsigned short int)const; // récursif, renvoie la liste contenant tout les ensembles de coordonnées de taille l+1 contenant l'argument récursif
    std::set<std::array<unsigned short int,2>> bordure(const std::set<std::array<unsigned short int, 2>>&)const; // renvoie l'ensemble des coordonnées en bordure de l'argument
    litt toLitt(std::array<unsigned short int, 2>)const;
    std::array<unsigned short int,2> toCoord(litt l)const;
public:
    Etat getCase(int,int);
    void setCase(int,int,Etat);
    Grille(unsigned short int, std::vector<std::array<int,3>> = {}); // largeur de la grille, ensemble de contraintes
    std::vector<Etat>& operator[](int);
    void appendContrainte(std::array<int,3>);
    void ajouterContrainte(int,int,int);
    const std::vector<std::array<int,3>>& getContraintes()const;
    bool isResolue()const;
    Formule toFormule()const;
    void resoudre();
};
#endif
