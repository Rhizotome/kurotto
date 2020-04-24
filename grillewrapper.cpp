#include <boost/python.hpp>
#include "grille.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(grille_mt){
    class_<Grille>("grille", init<unsigned short int>())
        .def("ajouterContrainte", &Grille::ajouterContrainte)
        .def("getCase", &Grille::getCase)
        .def("setCase", &Grille::setCase)
        .def("resoudre", &Grille::resoudre)
        ;
    enum_<Etat>("etat")
        .value("noir",noir)
        .value("blanc",blanc)
        .value("croix",croix)
        ;
}
