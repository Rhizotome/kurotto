#include "formule.hpp"
#include <forward_list>
#include <utility>
#include <thread>
#include <functional>
using namespace std;

Formule::Formule(CExt<CExt<CInt<litt>>> a):grille(a){}
void Formule::resoudre1()
{
    grilleResolue.clear();
    vector<unsigned short> combinaison(grille.size());
    fill(combinaison.begin(),combinaison.end(),0);
debutBoucle: while (true){
        for (unsigned short i(0) ; i < grille.size() ; i++){
            for (unsigned short j(i + 1) ; j < grille.size() ; j++){
                if (!testCoherence({i,combinaison[i],j,combinaison[j]})){ // on passe à la combinaison suivante et on reboucle
                    for (unsigned short k(0) ; k < grille.size() ; k++){
                        if (combinaison[k] != grille[k].size() - 1){
                            combinaison[k]++;
                            goto debutBoucle;
                        }
                        else{
                            combinaison[k] = 0;
                        }
                    }
                    return; // le dernière combinaison est atteinte, tout est fini
                }
            }
        }
        // la combinaison fonctionne
        CInt<litt> solution;
        for (unsigned short i(0) ; i < grille.size() ; i++){
            solution.insert(grille[i][combinaison[i]].begin(),grille[i][combinaison[i]].end());
        }
        grilleResolue.push_back(solution);
        return;
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

bool Formule::testCoherence(array<unsigned short,4> key){
    if (mapu.contains(key)){
        return mapu[key];
    }
    else{
        for (litt l : grille[key[0]][key[1]]){
            if (grille[key[2]][key[3]].contains(-l)){
                mapu[key] = false;
                return false;
            }
        }
        mapu[key] = true;
        return true;
    }
}
