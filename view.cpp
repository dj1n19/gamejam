#include "view.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

void View::displayLevel(const vector<vector<char>>& level, int playerX, int playerY, int collectedItems, int points, int fuel) {
    // Mise à jour de la première ligne du fichier ../config.txt
    fstream configFile("config.txt", ios::in | ios::out);

    if (configFile.is_open()) {
        string line;
        streampos pos = configFile.tellg(); // Position de début du fichier

        // Convertir playerX et playerY en strings avec un zéro devant si nécessaire
        ostringstream formattedPlayerX, formattedPlayerY;
        formattedPlayerX << setw(2) << setfill('0') << playerX; // Ajoute un 0 si playerX < 10
        formattedPlayerY << setw(2) << setfill('0') << playerY; // Ajoute un 0 si playerY < 10

        // Concaténer les deux strings
        string concatenated = formattedPlayerX.str() + formattedPlayerY.str();

        // Lire la première ligne (pas nécessaire pour la logique ici, mais conserve la cohérence)
        getline(configFile, line);

        // Revenir au début du fichier pour la réécriture
        configFile.seekp(pos);

        // Écrire la valeur concaténée dans la première ligne
        configFile << concatenated << endl;

        // Calcul de la deuxième ligne en fonction du niveau de fuel (de 0.1 à 10)
        float fuelFactor = 0.1f + (10.0f - 0.1f) * (1.0f - fuel / 100.0f); // Valeur de 0.1 à 10 selon le fuel
        ostringstream formattedFuel;
        formattedFuel << fixed << setprecision(1) << fuelFactor; // Formater pour une décimale

        // Lire la deuxième ligne
        getline(configFile, line);

        // Revenir au début du fichier pour réécrire la deuxième ligne
        configFile.seekp(pos);

        // Ignorer la première ligne (concatenated) et réécrire la deuxième ligne avec le fuelFactor
        configFile << concatenated << endl << formattedFuel.str() << endl;

        // Fermer le fichier
        configFile.close();
    } else {
        cerr << "Erreur : Impossible d'ouvrir le fichier ../config.txt" << endl;
    }

    // Affichage de la carte et des informations
    cout << "MAP:\n";
    for (const auto& row : level) {
        for (char ch : row) {
            cout << ch;
        }
        cout << "\n";
    }
    cout << "Player is at (" << playerX << ", " << playerY << ")\n";
    cout << "Items collected: " << collectedItems << "\n";
    cout << "Points: " << points << "\n";
    cout << "Fuel: " << fuel << "\n";
}

