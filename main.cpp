#include <iostream>
#include <string>
#include "fonction.h"

using namespace std;

int main(int argc, char** argv) {

    string fichier_csv, fichier_json;

    // chemin d'accès du fichier d'entrée (CSV)
    cout << "entrez le nom ou chemin d'acces de votre fichier CSV \n";
    cin >> fichier_csv;

    // chemin d'accès du fichier de sortie (JSON)
    cout << "entrez le nom ou chemin d'acces de votre fichier JSON \n";
    cin >> fichier_json;

    // Appel de la fonction pour convertir le fichier CSV en JSON
    convertirCSVenJSON(fichier_csv, fichier_json);

    return 0; 
}