# Description succinte du projet

Le projet consiste en la simulation d'un système T9 qu'on retrouvait sur les anciens nokia

## Compilation

`g++ -c main.cpp` puis `g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system` 
(puis `./sfml-app` pour l'exécution).

## Fonctionnalités

- fond d'écran nokia 1110
- récupération des entrées clavier : écriture, effacement et affichage de texte à l'écran
- navigation fluide dans le texte affiché à l'aide des flèches directionnelles
- attribution d'une liste de mots possibles à chaque groupe de chiffres entrés au clavier
- tri de cette liste pour montrer en priorité le mot le plus fréquent
- navigation entre les différents mots proposés à l'aide des flèches directionnelles
- remplacement de chaque groupe de chiffres par le mot proposé sélectionné


