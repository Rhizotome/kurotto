from copy import copy, deepcopy
OR = True
AND = False
NOT = False

class Litteral :
    def __init__(self, valeur, flag = True):
        self.valeur = valeur
        self.flag = flag
        self.logique = None

    def __eq__(self, other):
        if type(self) == type(other):
            return self.flag == other.flag and self.valeur == other.valeur

    def simplifier(self):
        pass

    def isLitteral(self):
        return True

    def __str__(self):
        if self.valeur == True and self.flag == NOT:
            return("False")
        return ("" if self.flag else "!") + str(self.valeur)

    def profondeur(self):
        return 0

    def inverse(self):
        return Litteral(self.valeur, not self.flag)

    def reduire(self):
        pass

littTrue = Litteral(True)
littFalse = Litteral(True, NOT)

class Formule:
    def __init__(self, sousFormules, logique, flag = True):
        self.sousFormules = sousFormules
        self.logique = logique
        self.flag = flag

    def __getitem__(self,key):
        if type(key) is slice:
            return Formule(self.sousFormules[key],self.logique,self.flag)
        return self.sousFormules[key]

    def __setitem__(self,key, valeur): # si setitem reçoit une slice en argument, retourne une liste et pas une formule
        self.sousFormules[key] = valeur

    def __len__(self):
        return len(self.sousFormules)

    def append(self, item):
        if not item in self:
            self.sousFormules.append(item)

    def isLitteral(self):
        return False
        
    def __str__(self):
        représentation = ""
        if self.flag == NOT :
            représentation += "!"
        représentation += "("
        if len(self) == 0:
            return "()"
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

    def __contains__(self, item):
        if type(self) == type(item):
            for i in item :
                if not i in self :
                    return False
            return True
        for i in self:
            if i == item:
                return True
        return False

    def __eq__(self, other):
        if type(other) != type(self):
            return False
        if len(self) != len(other):
            return False
        for i in self:
            if not (i in other):
                return False
        return True

    def remove(self, item):
        return self.sousFormules.remove(item)

    def count(self,item):
        return self.sousFormules.count(item)

    def simplifier(self):
        # Première chose :
        if len(self) == 0 :
            self.append(littFalse if self.logique == OR else littTrue)
            return
        # Loi de Morgan
        if self.flag == NOT:
            self.flag = True
            self.logique = not self.logique
            for i in self :
                i.flag = not i.flag
        # Récursion
        for i in self :
            i.simplifier()
        # Repli des ou sur les ou et des et sur les et
        for i in self[:] : #op
            if i.logique == self.logique or (not i.isLitteral() and len(i) == 1):
                for j in i :
                    self.append(j)
                self.remove(i)
        # Déjà, quand même, voilà quoi, on commence par simplifier les formules contenant True ou False
        while littTrue in self and len(self)>1:
            if self.logique == OR :
                self.sousFormules = [littTrue]
                return
            else :
                self.remove(littTrue)
        while littFalse in self and len(self)>1:
            if self.logique == AND :
                self.sousFormules = [littFalse]
                return
            else :
                self.remove(littFalse)
        # Suppression des doublons
        for i in self[:] : #op
            while self.sousFormules.count(i) > 1:
                self.remove(i)
        # Suppression des inverses
        for i in filter(lambda item : item.isLitteral(), self):
            if i.inverse() in self :
                if self.logique == AND :
                    self.sousFormules = [littFalse]
                else :
                    self.sousFormules = [littTrue]
                return
        # Suppression des sousFormules comprenant d'autres sousFormules
        toRemove = []
        for i in range(len(self)):
            for j in range(i+1, len(self)):
                if i in toRemove:
                    break
                if not self[i].isLitteral():
                    if self[j] in self[i] :
                        toRemove.append(i)
        toRemove.sort(reverse=True)
        for i in toRemove:
            del self.sousFormules[i]

    def profondeur(self):
        return max([i.profondeur() for i in self]) + 1

    def distribuer(self):
        if len(self) == 1:
            self.sousFormules = self[0].sousFormules
            self.logique = not self.logique
            for i in range(len(self)) :
                self[i] = Formule([self[i]],not self.logique)
        else : # len(self) > 1
            rec = self[1:len(self)]
            if len(rec) >= 1:
                rec.distribuer()
            newFormule = Formule([], self.logique)
            for i in self[0] :
                for j in rec :
                    j2 = deepcopy(j)
                    j2.append(i)
                    newFormule.append(j2)
            self.sousFormules=newFormule.sousFormules
            self.logique = not self.logique

    def reduire(self):
        self.simplifier()
        if self.profondeur() == 1 :
            return
        elif self.profondeur() == 2 :
            for i in range(len(self)):
                if self[i].isLitteral() :
                    self[i] = Formule([self[i]], not self.logique)
            self.distribuer()
            self.simplifier()
        else : # profondeur() > 2
            for i in self :
                i.reduire()
            self.reduire()

    # Testing grounds
# print(Litteral("a",NOT))
# print(Formule([Litteral("a",NOT),Litteral("b")],OR))
# a = Formule([Formule([Litteral("a"),Litteral("a"),Litteral("a"),Litteral("e"),Litteral("h")],AND),Formule([Litteral("c"),Litteral("d"),Litteral("f")],OR),Formule([Litteral("k"),Litteral("j"),Formule([Litteral("a"),Litteral("b")],OR),Formule([Litteral("c"),Litteral("l")],OR)],OR),Formule([Formule([Litteral("b")],AND),Litteral("j"),Litteral("k")],OR)],AND)
# print(a)
# print(a.profondeur())
# a.simplifier()
# print(a)
# print(a.profondeur())
# a.reduire()
# print(a)
