#include "formule.hpp"
#include <forward_list>
#include <utility>
using namespace std;

void Formule::resoudre1()
{
    grilleResolue.clear();
    CInt<litt> a{};
    grilleResolue.push_back(a);
    for (auto i : grille) {
        CExt<CInt<litt>> grilleResolueStep{};
        for (auto j : grilleResolue) {
            for (auto k : i) {
                auto j2(j);
                j2.insert(k.begin(),k.end());
                for (auto l : k) {
                    if (j.contains(-l))
                        goto label;
                }
                grilleResolueStep.push_back(j2);
label:
                ;
            }
        }
        grilleResolue = move(grilleResolueStep);
    }
}

ostream& operator<<(ostream& os, const Formule& item)
{
    for (auto i : item.grilleResolue) {
        for (auto j : i) {
            os << j << " ";
        }
        os << endl;
    }
    return os;
}
