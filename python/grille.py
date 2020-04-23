##############################################################################
## Auteurs : Damis El Alami, Tanguy Freycon, Lorris Sahli, Muaad Tamtam      #
## Grille représente les contrainte initiales d'un problème ou une solution  #
##############################################################################

import os
import sys
from formule import *
from copy import copy
import profile
class Grille:
    def __init__(self, l=0, *args): # prend en arguments un la largeur voulue pour la grille et un certain nombre de triplet de la forme (i,j,p), avec i,j l'emplacement du cercle contenant p si p>=0 ou rien si p=-1
        if l < 0:
            raise ValueError("l doit être positif")
        self.l = l 
        self.T = []
        for i in args:
            self.ajouter(i[0],i[1],i[2])

    def __str__(self): # retourne une représentation de la grille en texte
        grille = [["*" for i in range(self.l)] for i in range(self.l)]
        for i,j,p in self.T:
            if p != -1 :
                grille[i-1][j-1] = str(p)
            else :
                grille[i-1][j-1] = "o"
        représentation = ""
        for l in grille:
            for c in l:
                représentation += c + " "
            représentation += '\n'
        return représentation

    def ajouter(self,i,j,p):# ajoute le triplet i,j,p à T
        if p < -1:
            raise ValueError("le contenu d'un cercle doit être -1 ou un entier naturel")
        if i <= 0 or j <= 0 or i > self.l or j > self.l :
            raise ValueError("les coordonnées d'un des cercles sont incorrectes")
        self.T.append((i,j,p)) 

    def toFormule(self) : # on représente n_{i,j} par (i,j)
        contraintes = Formule([], AND)
        for i in self.T :
            contraintes.append(Litteral((i[0],i[1]),flag = NOT))
            if i[2] == -1 :
                continue
            else :
                contraintesFormes = Formule([],OR)
                candidats = découpages((i[0],i[1]),i[2],self.l)
                for j in candidats :
                    j.remove((i[0],i[1]))
                    listej = list(j)
                    for k in range(len(j)) :
                        listej[k] = Litteral(listej[k])
                    for k in bordure(j,self.l) :
                        listej.append(Litteral(k,NOT))
                    contraintesFormes.append(Formule(listej,AND))
                contraintes.append(contraintesFormes)
        return contraintes

def découpages(centre, taille, l): # renvoie un ensemble contenant l'ensemble des coordonnées des formes de taille taille et de centre centre dans la grille de taille l
    if taille <= 0 :
        return [{centre}]
    else :
        ensembleRéccurent = découpages(centre, taille - 1, l)
        formes = []
        for i in ensembleRéccurent :
            for j in i :
                voisins = {(j[0]-1,j[1]),(j[0]+1,j[1]),(j[0],j[1]-1),(j[0],j[1]+1)}
                for k in voisins :
                    if k[0] > 0 and k[1] > 0 and k[0] <= l and k[1] <= l :
                        temp = copy(i)
                        temp.add(k)
                        if len(temp) == taille + 1 and not temp in formes : 
                            formes.append(temp)
        return formes

def bordure(forme,l):
    retour = []
    for i in forme :
        voisins = {(i[0]-1,i[1]),(i[0]+1,i[1]),(i[0],i[1]-1),(i[0],i[1]+1)}
        for j in voisins :
            if j[0] > 0 and j[1] > 0 and j[0] <= l and j[1] <= l and not j in forme :
                retour.append(j)
    return retour

a = Grille(3, (1,1,2), (3,2,1))
sys.setrecursionlimit(1000000)
print(a)
b=a.toFormule()
print(b,"\n \n \n")
# b = Formule([Litteral("c"),Formule([Litteral("a"),Litteral("b")],OR)],AND)
# print(b)
# b.reduire()
# print(b)
# print(b)
# print(a.toFormule().réduire()
