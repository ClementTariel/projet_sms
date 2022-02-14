#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
//#include "lire_doc.cpp"
//#include "lire_doc_debug.cpp"
#include "lire_doc_simple2.cpp"



/*char* mots_possibles(std::vector<int> tableau) {
    // on va cherche stoutes les possibilités des mots
    const char* lettre2 = "abc";
    const char* lettre3 = "def";
    const char* lettre4 = "ghi";
    const char* lettre5 = "jkl";
    const char* lettre6 = "mno";
    const char* lettre7 = "pqrs";
    const char* lettre8 = "tuv";
    const char* lettre9 = "wxyz";

    for (int i = 0; i < std::size(tableau); i++) {
        
    }
}*/


/* COMPLILATION : g++ -Wall -Werror sms.cpp */





////// compter le nombre de chiffres dans un nombre/////////////////////////
template<typename T>
size_t countDigits(T n)
{
    std::string tmp;

    tmp = std::to_string(n);
    return tmp.size();
}


void ajouter(std::string mots_possibles[3], int freq_possibles[3], std::string mot, int freq) {
    if (freq > freq_possibles[2]) {
        if (freq > freq_possibles[1]) {
            if (freq > freq_possibles[0]) {
                freq_possibles[2] = freq_possibles[1];
                mots_possibles[2] = mots_possibles[1];
                freq_possibles[1] = freq_possibles[0];
                mots_possibles[1] = mots_possibles[0];
                freq_possibles[0] = freq;
                mots_possibles[0] = mot;
                //std::cout << "un 1er trouvé : " << mot << std::endl;
            }
            else {
                freq_possibles[2] = freq_possibles[1];
                mots_possibles[2] = mots_possibles[1];
                freq_possibles[1] = freq;
                mots_possibles[1] = mot;
                //std::cout << "un 2e trouvé : " << mot << std::endl;

            }
        }
        else {
            freq_possibles[2] = freq;
            mots_possibles[2] = mot;
        }
    }
}


int main(int argc, char* argv[]){  //on aura un tableau de int avec les n° correspondant aux touches de l'utilisateur 
/* !!!! il faut mettre des espaces entre les différents chiffres !!!!!!!! */

    /*std::vector <int> tableau{};  //on doit utiliser un tableau dynamique car argc est constant (sinon erreur de compilation)
    for (int i = 1; i < argc; i++) {
        tableau.push_back (atoi(argv[i]));
        std::cout << tableau[i-1] << " ";
    }
    std::cout << std::endl;*/

    int nombre = atoi(argv[1]);
    std::cout << nombre << std::endl;
    int longueur_nombre = countDigits(nombre);

    std::string doc = "liste_mots_alphabetique_frequence.txt";
    lire_doc_simple2(doc); //associer à chaque mot son nombre correspondant, et regrouper ça avec sa fréquence dans un struct MyWord
    



    std::string mots_possibles[3]; //contiendra les 3 mots possibles les plus probables avec leur fréquence
    int freq_possibles[3];

    auto start = std::chrono::system_clock::now();
    auto mots = std::ifstream("fichier_struct.txt");

    if (mots) {
        std::string Mot;
        std::string type;
        int freq;
        int trad;
        int position = mots.tellg();
        mots.seekg(0, std::ios::end);
        int longueur_fichier = mots.tellg();
        mots.seekg(0, std::ios::beg);
        //std::cout << position << std::endl;

        while (position < longueur_fichier - 1) { 
            //std::cout << "entrée boucle ";
            mots >> Mot;
            mots >> freq;
            mots >> trad;
            int longueur_mot = Mot.length();
            //std::cout << Mot << " " << longueur_mot << " vs " << longueur_nombre << std::endl;
            int diff = longueur_mot - longueur_nombre;
            //std::cout << diff << " ";
            if (diff >= 0) {
                int trad_corr = (int)trad / pow(10, diff);
                //std::cout << trad_corr;
                if (trad_corr == nombre) {
                    //char mot = *Mot.c_str();
                    //std::cout << "un de trouvé : " << Mot << " ie " << mot << std::endl;
                    ajouter(mots_possibles, freq_possibles, Mot, freq);
                    //std::cout << "un ajouté " << std::endl;
                }
            }
            position = mots.tellg();
            //std::cout << position << std::endl;
            //position += 1;

        }

        std::cout << mots_possibles[0] << " " << mots_possibles[1] << " " << mots_possibles[2] << std::endl;



    }
    else
    {
        std::cout << "erreur; impossible d'ouvrir le fichier" << std::endl;
    }


    mots.close();
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    std::cout << "le temps que cela a pris est " << elapsed << " secondes." << '\n';
    //char* mots_possibles = mots_possibles(tableau);
    //afficher mots les plus probables
    return 0;
}
