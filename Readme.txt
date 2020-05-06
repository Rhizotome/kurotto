Attention, ce programme a tendance a consommer très vite de très grandes quantités de RAM, il est donc conseillé au moins d'avoir un swapfile, vous pouvez appuyer sur ctrl + c pour interrompre le processus si il consomme trop de ressources ou trop de temps.
Pour lancer le programme après compilation, ouvrez un émulateur de terminal dans ce dossier, et tapez "./Kurotto" (sans les guillemets), puis entrez la taille souhaitée pour la grille (par exemple 5 pour une grille 5*5.
Une fenêtre affichant une grille vide devrait s'ouvrir, avec laquelle vous pourrez interagir des manières suivantes :
* Effectuez un clic droit sur une des cases, puis entrez un nombre naturel ou -1 dans le terminal afin de créer une contrainte
* Effectuez un clic gauche sur une des cases pour changer la couleur de la case. Notre programme n'implémente pas de vérification des solutions de l'utilisateur, mais cela peut être utile pour tenter de résoudre une grille
* Appuyez sur espace pour que le programme affiche une résolution correspondante au contraintes sur la grille
* Appuyez sur Backspace afin d'effacer toutes les contraintes de la grille
* Appuyez sur F pour entrer dans le terminal un chemin vers un fichier contenant des triplets de nombres séparés par des espaces et/ou des sauts à la ligne afin de construire les contraintes de la grille à partir de ce fichier de la manière suivante :
    -Toutes les contraintes existantes sont effacées
    -Chaque triplet i j p du fichier est interprété comme la contrainte (i,j,p)
    -Vous pouvez entrer exemple1.txt dans une grille 5x5 ou exemple2.txt dans une grille 6x6 ou exemple3.txt, ce sont des ensembles de contraintes qui produisent chacun une seule solution
