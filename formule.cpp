#include "formule.hpp"
#include <forward_list>
#include <utility>
#include <thread>
#include <functional>
using namespace std;

// Place la formule grille résolue dans grilleResolue; expliquée plus en détail dans le projet
void Formule::resoudre()
{
    auto procCount = std::thread::hardware_concurrency(); //nombre de threads supportés par le système
    if (procCount == 0)
        procCount = 1;
    grilleResolue.clear();
    CInt<litt> a{};
    grilleResolue.push_back(a);
    vector<thread> threads(procCount);
    for (auto i : grille) {
        CExt<CInt<litt>> grilleResolueStep{};
        // ici, on répartit les tâches entre les threads
        auto iter = threads.begin();
        for (auto &j : grilleResolue) {
            if (iter->joinable())
                iter->join();
            *iter = thread(concurrentStep,j,ref(grilleResolueStep),i);
            iter++;
            if (iter == threads.end())
                iter = threads.begin();
        }
        // On attends que tous les threads soient terminés
        for (auto k(0u) ; k < procCount ; k++) {
            iter++;
            if (iter == threads.end())
                iter = threads.begin();
            if (iter->joinable())
                iter->join();
        }
        grilleResolue = move(grilleResolueStep);
    }
}

// Affiche grille résolue dans un format similaire à un format dimacs
ostream& operator<<(ostream& os, const Formule& item)
{
    for (auto i : item.grilleResolue) {
        for (auto j : i) {
            os << j << " ";
        }
        os << " 0"<< endl;
    }
    return os;
}

mutex Formule::vectorLock;

void Formule::concurrentStep(const CInt<litt> &sousFormuleGrilleResolue, CExt<CInt<litt>> &grilleResolueStep,const CExt<CInt<litt>>& sousFormule)
{
    for (auto &k : sousFormule) {
        bool flag = false;
        // On vérifie que les deux formules sont cohérentes (ie que leur conjonction ne soit pas toujours fausse)
        for (auto l : k) {
            flag = flag || (sousFormuleGrilleResolue.find(-l) != sousFormuleGrilleResolue.end());
        }
        if (flag) continue;
        auto j(sousFormuleGrilleResolue);
        // Fusion des sousFormules
        for (auto l : k) {
            j.insert({l});
        }
        vectorLock.lock();
        grilleResolueStep.push_back(j);
        vectorLock.unlock();
    }
}
