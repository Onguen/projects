#include <iostream>  
#include <fstream>   
#include <sstream>   
#include <vector>    
#include <string>    
#include <algorithm> // Permet d'utiliser des algorithmes standard (comme remove)
#include <json/json.h> // Bibliothèque JsonCpp pour manipuler des fichiers JSON
#include "fonction.h"  

using namespace std; 

// Fonction pour deviner le séparateur utilisé dans un fichier CSV
// Recherche les séparateurs courants : ';', '\t' (tabulation) ou ','
char detecterSeparateur(const string& ligne) {

    if (ligne.find(';') != string::npos) return ';';  // Si un point-virgule est trouvé dans la ligne
    if (ligne.find('\t') != string::npos) return '\t'; // Si une tabulation est trouvée
    return ',';  // Sinon, on suppose une virgule
}

// Fonction pour lire un fichier et enlever un BOM s'il est présent
void enleverBOM(ifstream& fichier) {

    const char BOM[3] = { (char)0xEF, (char)0xBB, (char)0xBF }; // Le BOM UTF-8
    char buffer[3];

    fichier.read(buffer, 3);

    if (buffer[0] == BOM[0] && buffer[1] == BOM[1] && buffer[2] == BOM[2]) {

        // Si BOM détecté, ignorer
        fichier.seekg(0);
    }
}

// Fonction principale pour convertir un fichier CSV en fichier JSON
void convertirCSVenJSON(const string& fichierCSV, const string& fichierJSON) {

    ifstream fichierEntree(fichierCSV); // Ouverture du fichier CSV en lecture

    if (!fichierEntree.is_open()) { // Vérification de l'ouverture du fichier
        cout << "Erreur : impossible d'ouvrir le fichier " << fichierCSV << "\n";
        return;
    }

    enleverBOM(fichierEntree); // Enlever le BOM si présent

    string ligne; // Variable pour stocker chaque ligne du fichier CSV
    vector<string> entetes; // Contient les en-têtes des colonnes du CSV

    Json::Value tableauJSON(Json::arrayValue); // Création d'un tableau JSON vide

    // Lire la première ligne pour extraire les noms de colonnes
    if (getline(fichierEntree, ligne)) {
        char separateur = detecterSeparateur(ligne); // Détection du séparateur
        entetes = diviser(ligne, separateur); // Diviser la ligne pour récupérer les noms de colonnes
    } else {
        cout << "Erreur : fichier vide ou format incorrect.\n";
        return;
    }

    // Lire les lignes suivantes et les transformer en objets JSON
    while (getline(fichierEntree, ligne)) {

        char separateur = detecterSeparateur(ligne); // Détection du séparateur pour chaque ligne
        vector<string> valeur = diviser(ligne, separateur); // Division de la ligne en valeurs

        // Vérifier la cohérence entre les entêtes et les valeurs
        if (valeur.size() != entetes.size()) {
            cout << "Avertissement : ligne ignorée, format incorrect : << " << ligne << " >> \n";
            continue; // Ignorer la ligne si le format est incorrect
        }

        Json::Value objetJSON(Json::objectValue); // Création d'un objet JSON pour chaque ligne

        for (unsigned int i = 0; i < entetes.size(); i++) {
            objetJSON[entetes[i]] = Json::Value(nettoyerTexte(valeur[i])); // Ajouter les valeurs aux clés correspondantes
        }

        tableauJSON.append(objetJSON); // Ajouter l'objet JSON au tableau
    }

    fichierEntree.close(); // Fermer le fichier d'entrée

    ofstream fichierSortie(fichierJSON); // Ouvrir le fichier de sortie en écriture

    if (!fichierSortie.is_open()) { // Vérification de l'ouverture du fichier de sortie
        cout << "Erreur : impossible d'écrire dans << " << fichierJSON << " >> \n";
        return;
    }

    // Configurer l'écriture JSON avec indentation
    Json::StreamWriterBuilder writer;

    writer["indentation"] = "    "; // Utiliser 4 espaces pour l'indentation

    fichierSortie << Json::writeString(writer, tableauJSON); // Écrire le tableau JSON dans le fichier

    fichierSortie.close(); // Fermer le fichier de sortie

    cout << "Conversion reussie ! fichier JSON enregistre dans << " << fichierJSON << " >> \n";
}

// Fonction pour diviser une ligne en fonction du séparateur
// Gère aussi les valeurs entre guillemets
vector<string> diviser(const string& ligne, char delimiter) {

    vector<string> champs; // Contiendra les valeurs de chaque champ
    string champ; // Variable temporaire pour chaque champ
    bool entreGuillemets = false; // Indicateur pour savoir si nous sommes dans une valeur entre guillemets
    
    // Parcourir chaque caractère de la ligne
    for (char c : ligne) {

        if (c == '"') { // Si nous rencontrons un guillemet, alterner l'état
            entreGuillemets = !entreGuillemets;
        } else if (c == delimiter && !entreGuillemets) { // Si nous trouvons un séparateur en dehors des guillemets
            champs.push_back(nettoyerTexte(champ)); // Ajouter le champ nettoyé
            champ.clear(); // Réinitialiser le champ
        } else {
            champ += c; // Ajouter le caractère au champ en cours
        }
    }

    champs.push_back(nettoyerTexte(champ)); // Ajouter le dernier champ

    return champs; // Retourner la liste des champs
}

// Fonction pour nettoyer un champ de texte
// Supprime les espaces inutiles, les retours à la ligne et les guillemets entourant
string nettoyerTexte(string texte) {

    texte.erase(remove(texte.begin(), texte.end(), '\r'), texte.end()); // Enlever les retours chariot
    texte.erase(remove(texte.begin(), texte.end(), '\n'), texte.end()); // Enlever les nouvelles lignes

    if (!texte.empty() && texte.front() == '"' && texte.back() == '"') { // Si la chaîne commence et se termine par des guillemets
        texte = texte.substr(1, texte.size() - 2); // Enlever les guillemets
    }

    return texte; // Retourner la chaîne nettoyée
}