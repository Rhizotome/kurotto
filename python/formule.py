from copy import copy, deepcopy
# Le but c'est que tout fonctionne par copie à l'exception des constructeurs et modifications simples si la fonction renvoie un objet, cet objet est cependant une copie
OR = True
AND = False
NOT = False

class Littéral :
    def __init__(self, valeur, flag = True):
        self.valeur = valeur
        self.logique = None
        self.flag = flag

    def __eq__(self, other) :
        return self.__dict__ == other.__dict__

    def simplifier(self) :
        return copy(self)

    def isLittéral(self):
        return True

    def __str__(self) :
        représentation = ""
        if self.flag == NOT :
            représentation += "!"
        représentation += str(self.valeur)
        return représentation

    def profondeur(self) :
        return 0

class Formule :
    def __init__(self, valeur, logique, flag = True) :
        self.logique = logique
        self.flag = flag
        self.sousFormules = valeur

    def __eq__(self, other) :
        if type(self) == type(other) :
            return self.__dict__ == other.__dict__
        else :
            return False

    def __str__(self):
        représentation = ""
        if self.flag == NOT :
            représentation += "!"
        représentation += "("
        if len(self) == 0:
            raise ValueError("Formule de taille nulle")
        else :
            for i in range(len(self.sousFormules)-1) :
                représentation += str(self[i])
                if self.logique == OR :
                    représentation += " or "
                elif self.logique == AND :
                    représentation += " and "
        représentation += str(self[len(self)-1])
        représentation += ")"
        return représentation

    def append(self, a) :
        self.sousFormules.append(a)
    
    def __len__(self) :
        return len(self.sousFormules)

    def profondeur(self) :
        profondeur = 0
        for i in self :
            profondeur = max(profondeur, i.profondeur() + 1)
        return profondeur

    def __add__(self, a) :
        if not (type(a) is Formule) :
            raise ValueError("ajouter autre chose qu'une formule à une formule impossible")
        elif a.logique != self.logique :
            raise ValueError("impossible d'ajouter des formules aux logiques différentes")
        elif a.flag != self.flag :
            raise ValueError("impossible d'ajouter des formules aux flags différents")
        else :
            nouvelleFormule = deepcopy(self)
            for i in range(0, len(a)) :
                nouvelleFormule.append(deepcopy(a[i]))
            return nouvelleFormule

    def __getitem__(self, key) :
        if type(key) is int :
            return deepcopy(self.sousFormules[key])
        elif type(key) is slice :
            nouvelleFormule = Formule([], self.logique, flag = self.flag)
            for i in range(key.start,key.stop) :
                nouvelleFormule.append(deepcopy(self[i]))
            return nouvelleFormule

    def __iter__(self) :
        return iter(self.sousFormules)

    def __setitem__(self, key, value) :
        if not(type(key) is int) :
            raise ValueError("setitem prend un int")
        else :
            self.sousFormules[key] = value

    def isLittéral(self) :
        return False

    def __contains__(self, item) :
        return item in self.sousFormules

    def simplifier(self) :
        temp = deepcopy(self)
        # Loi de Morgan
        if temp.flag == NOT :
            temp.flag = not temp.flag
            temp.logique = not temp.logique
            for i in temp : 
                i.flag = not i.flag
        # Récursion
        for i in range(len(temp)) :
            temp[i] = temp[i].simplifier()
        # Concaténation de formules de même nature : (a && (b && c)) devient (a && b && c)
        nouvelleFormule = Formule([], temp.logique, flag = temp.flag)
        for i in temp :
            if i.logique != nouvelleFormule.logique and (i.isLittéral() or len(i) > 1) : # N'est pas vérifiée que si i est de la même logique que nouvelleFormule ou est une formule de taille 1
                nouvelleFormule.append(i)
            else :
                i.logique = nouvelleFormule.logique
                nouvelleFormule += i
        # Suppression de littéraux ou formules en double
        nouvelleFormule.suppressionDoublons()
        return nouvelleFormule

    def suppressionDoublons(self):
        formuleSansDoubles = Formule([], self.logique)
        for i in self :
            if not i in formuleSansDoubles :
                formuleSansDoubles.append(i)
        self = formuleSansDoubles

    def distribuer(self) :
        if self.flag == NOT :
            raise ValueError("une formule doit être simplifiée avant d'être développée")
        elif len(self) == 1 :
            nouvelleFormule = Formule([], not self.logique)
            for i in self[0] :
                nouvelleFormule.append(Formule([copy(i)], self.logique))
            return nouvelleFormule
        elif len(self) > 1 :
            formuleRéccurence = self[1:len(self)].distribuer()
            print(formuleRéccurence)
            nouvelleFormule = Formule([], not self.logique)
            for i in self[0] :
                for j in formuleRéccurence :
                    nouvelleSousFormule = Formule(copy(j.sousFormules), self.logique)
                    if not i in nouvelleSousFormule :
                        nouvelleSousFormule.append(i)
                    nouvelleFormule.append(nouvelleSousFormule)
            nouvelleFormule.suppressionDoublons()
            return nouvelleFormule

    def réduire(self) :
        temp = self.simplifier() # On utilise le fait que simplifier produise une deepcopy
        if temp.profondeur() == 1 :
            return temp
        elif temp.profondeur() == 2 :
            for i in range(len(temp)) :
                if temp[i].isLittéral() :
                    temp[i] = Formule([temp[i]],not temp.logique)
            print("temp : " + str(temp))
            return temp.distribuer().simplifier()
        elif temp.profondeur() > 2 :
            for i in range(len(temp)) :
                if temp[i].profondeur() > 1 :
                    temp[i] = temp[i].réduire()
            temp = temp.réduire()
            return temp

    def FNC(self) :
        temp = self.simplifier()
        if temp.logique == OR :
            temp = temp.réduire()
        return temp

# print(c.profondeur())
# print(c.simplifier().profondeur())
# print(c.simplifier())
# a = Formule([Formule([Littéral("a"),Littéral("a"),Littéral("e"),Littéral("h")],OR),Formule([Littéral("c"),Littéral("d"),Littéral("f")],OR),Formule([Littéral("j"),Littéral("k")],OR),Formule([Littéral("j"),Littéral("k")],OR)],AND)
# print(a)
# print(a.simplifier())
# print(a.FNC())
# print(a.réduire())
# b = a.réduire()
# print(b.réduire())
# print(a.réduire().distribuer())
# print(a)
# print(a.profondeur())
# print(b)

# a = Formule([Formule([Littéral("a"), Formule([Littéral("b"),Littéral("c")],AND,flag = AND)],OR), Littéral("e")], AND)
# print(a)
# print(a.réduire())
# print(a.FNC())

 # print(c)
 # print(c.simplifier())
 # a = Proposition(Logique.AND, Proposition(Logique.OR, Littéral("a"),Littéral("b"),Littéral("c")), Proposition(Logique.OR,     Littéral("c"),Littéral("d")))
 # print(a)
 # b = a.réduire()
 # print(b)

