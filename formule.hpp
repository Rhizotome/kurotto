#ifndef FORMULE_H
#define FORMULE_H
#include <vector>
#include <iostream>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <mutex>
#include <boost/container_hash/hash.hpp>
using litt = char;
template <class T>
using CInt = std::unordered_set<T>;
template <class T>
using CExt = std::vector<T>;

template <> struct std::hash<std::array<unsigned short, 4>>{
    std::size_t operator()(const std::array<unsigned short,4>& a)const noexcept
    {
        return boost::hash_range(a.begin(),a.end());
    }
};

class Formule {
public:
    Formule(CExt<CExt<CInt<litt>>>);
    std::unordered_map<std::array<unsigned short, 4>,bool> mapu;
    CExt<CExt<CInt<litt>>> grille;
    CExt<CInt<litt>> grilleResolue;
    void resoudre1();
    bool testCoherence(std::array<unsigned short int,4>); // teste si la [2] eme forme du [1] ensemble est compatible avec la [4] eme du [3]
};

std::ostream& operator<<(std::ostream&, const Formule&);

#endif
