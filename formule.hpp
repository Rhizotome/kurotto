#ifndef FORMULE_H
#define FORMULE_H
#include <vector>
#include <iostream>
#include <unordered_set>
#include <set>
using litt = char;
template <class T>
using CInt = std::unordered_set<T>;
template <class T>
using CExt = std::vector<T>;

class Formule {
public:
    CExt<CExt<CInt<litt>>> grille;
    CExt<CInt<litt>> grilleResolue;
    void resoudre1();
};

std::ostream& operator<<(std::ostream&, const Formule&);

#endif
