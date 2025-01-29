#ifndef __FONCTION_H__
#define __FONCTION_H__

#include <vector>
#include <string>  
#include <fstream>

using namespace std;

string nettoyerTexte(string texte);
void enleverBOM(ifstream& fichier);  
void convertirCSVenJSON(const string& fichierCSV, const string& fichierJSON);
vector<string> diviser(const string& ligne, char delimiter);

#endif