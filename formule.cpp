#include "formule.hpp"
#include <forward_list>
#include <utility>
#include <thread>
#include <functional>
using namespace std;

void Formule::resoudre()
{
    const auto procCount = std::thread::hardware_concurrency(); //nombre de cœurs supportés par le système
    if (procCount == 0)
        throw("Multithreading indisponible");
    grilleResolue.clear();
    CInt<litt> a{};
    grilleResolue.push_back(a);
    for (auto i : grille) {
        CExt<CInt<litt>> grilleResolueStep{};
        vector<thread> threads(procCount);
        auto iter = threads.begin();
        for (auto j : grilleResolue) {
            if (iter->joinable())
                iter->join();
            *iter = thread(concurrentStep,j,ref(grilleResolueStep),i);
            iter++;
            if (iter == threads.end())
                iter = threads.begin();
        }
        for (int i(0) ; i < procCount ; i++) {
            iter++;
            if (iter == threads.end())
                iter = threads.begin();
            if (iter->joinable())
                iter->join();
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
        os << " 0"<< endl;
    }
    return os;
}

mutex Formule::vectorLock;

void Formule::concurrentStep(const CInt<litt> &sousFormuleGrilleResolue, CExt<CInt<litt>> &grilleResolueStep,const CExt<CInt<litt>>& sousFormule)
{
    for (auto k : sousFormule) {
        bool flag = false;
        for (auto l : k) {
            flag = flag || sousFormuleGrilleResolue.contains(-l);
        }
        if (flag) continue;
        auto j(sousFormuleGrilleResolue);
        for (auto l : k) {
            j.insert({l});

        }
        vectorLock.lock();
        grilleResolueStep.push_back(j);
        vectorLock.unlock();
    }
}
