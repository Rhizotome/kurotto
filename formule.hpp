#ifndef FORMULE_H
#define FORMULE_H
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <set>
#include <mutex>
#include "sparsepp/sparsepp/spp.h"

using litt = unsigned short int;
template <class T>
using CInt = spp::sparse_hash_set<T>;
template <class T>
using CExt = std::vector<T>;

// Représente une formule profondeur 3
class Formule {
public:
    CExt<CExt<CInt<litt>>> grille; 
    CExt<CInt<litt>> grilleResolue;
    void resoudre();
    static void concurrentStep(const CInt<litt>&, CExt<CInt<litt>>&, const CExt<CInt<litt>>&);
    static std::mutex vectorLock;
};

std::ostream& operator<<(std::ostream&, const Formule&);

#endif
