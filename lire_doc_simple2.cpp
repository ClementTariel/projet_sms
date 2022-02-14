#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
#include <chrono>


void lire_doc_simple2(std::string doc) {
    auto start = std::chrono::system_clock::now(); //mesurer le temps d'éxécution du programme
    auto mots = std::ifstream(doc);

    if (mots) {
        std::string mot;
        std::string type;
        int freq;
        int position = mots.tellg();
        int erreur = 0; //prendra la valeur 1 si le mot contient une lettre non identifiée
        mots.seekg(0, std::ios::end);  //On se déplace à la fin du fichier
        int longueur_fichier = mots.tellg();
        int nombre_lignes_traitees = 0;

        mots.seekg(0, std::ios::beg); // on revient au début

        //while (position < 2000) {
        while (position < longueur_fichier - 1) {
            mots >> mot;    //Lit un mot depuis le fichier
            mots >> type;
            mots >> freq;
            //remplacer le mot par son code en 1 nombre
            int longueur = mot.length();
            int a = 0;

            if (longueur < 10 ) {  //le max des int est 2147483647 // || (longueur == 10 && mot[0] == 'a')

                for (int i = 0; i < longueur; i++) {

                    if (mot[i] == 'a' || mot[i] == 'b' || mot[i] == 'c' || mot[i] == 'à') {
                        a += 2 * pow(10, longueur - 1 - i);  //les lettres ci-dessus correspondent à la touche "2" sur le Nokia
                    }
                    else if (mot[i] == 'd' || mot[i] == 'e' || mot[i] == 'f') {
                        a += 3 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 'g' || mot[i] == 'h' || mot[i] == 'i') {
                        a += 4 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 'j' || mot[i] == 'k' || mot[i] == 'l') {
                        a += 5 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 'm' || mot[i] == 'n' || mot[i] == 'o') {
                        a += 6 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 'p' || mot[i] == 'q' || mot[i] == 'r' || mot[i] == 's') {
                        a += 7 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 't' || mot[i] == 'u' || mot[i] == 'v') {
                        a += 8 * pow(10, longueur - 1 - i);
                    }
                    else if (mot[i] == 'w' || mot[i] == 'x' || mot[i] == 'y' || mot[i] == 'z') {
                        a += 9 * pow(10, longueur - 1 - i);
                    }
                    else {
                        erreur = 1;
                        break;
                    }
                }

                if (erreur == 0) {  //si le mot a bien été saisi, on l'écrit dans le fichier
                    auto fichier_struct = std::ofstream("fichier_struct.txt", std::ios::app);   //on l'ouvre  en ajoutant à la fin du fichier les lignes suivantes
                    if (nombre_lignes_traitees == 0) {   //sauf en 1er, un ouverture en mode normal la 1ere fois permet de supprimer ce qui était déjà écrit
                        auto fichier_struct = std::ofstream("fichier_struct.txt");
                    }

                    if (fichier_struct) {
                        fichier_struct << mot << " " << freq << " " << a << "\n";   //écriture dans le fichier
                        fichier_struct.close();
                    }
                    else {
                        std::cout << "erreur; impossible d'ouvrir le fichier" << std::endl;
                    }
                    fichier_struct.close();
                    nombre_lignes_traitees += 1;

                }
                else {   // le mot n'est pas bon (accentué...)
                    erreur = 0;
                }

            }

            

            position = mots.tellg();
        }
        std::cout << nombre_lignes_traitees << std::endl;

    }


    else
    {
        std::cout << "erreur; impossible d'ouvrir le fichier" << std::endl;
    }


    mots.close();
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    std::cout << "le temps que cela a pris est " << elapsed << " secondes." << '\n';
}