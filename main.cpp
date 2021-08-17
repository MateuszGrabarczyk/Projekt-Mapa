#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <limits.h>
#include "funkcje.h" // dolaczenie naglowka z funkcjami

using namespace std;
int main(int argc, char** argv) {

	string plik_drog, plik_z_trasami, plik_wynikowy; // Nazwy plikow do uzycia
	vector <string> nazwy_miast; // Vector przechowujacy nazwy miast

	for (int i = 1; i < argc - 1; i++) { // przyporzadkowanie nazw do zmiennych string
		string str = argv[i];

		if (str == "-d") {
			plik_drog = argv[i + 1];
		}
		else if (str == "-t") {
			plik_z_trasami = argv[i + 1];
		}
		else if (str == "-o") {
			plik_wynikowy = argv[i + 1];
		}

	}

	if (plik_drog == "" or plik_z_trasami == "" or plik_wynikowy == "") {
		cout << "Podane argumenty sa zle" << endl;
		return 0;
	}

	


	vector<string> miasta = vector_z_miastami(plik_drog); // Zapisanie miast do wektora, miasta sa unikalne
	int** graf = new int* [miasta.size()]; // Utworzenie grafu (macierz sasiedztwa), tablica dwuwymiarowa

	for (int i = 0; i < miasta.size(); ++i) 
	{
		graf[i] = new int[miasta.size()]; //alokacja pamieci
		for (int j = 0; j < miasta.size(); ++j) // zaznaczenie wszystkich tras jako odleglosc 0
			graf[i][j] = 0;
	}

	bool ok = zapisanie_do_grafu(plik_drog, miasta, graf); // zapisanie do grafu wszystkich mozliwych drog np. Katowice Gliwice 30 = graf[x][y] = 30

	if (!ok) {
		return -1;
	}
	else {
		/* DLA TESTU
		for (int i = 0; i < miasta.size(); i++) { // wypisanie miast, ktore sa w grafie i pokazanie ich indeksow
			cout << i << ". " << miasta[i] << endl;
		}
		*/

		cout << endl;
		cout << endl;

		// --- WYSWIETLENIE GRAFU --- DLA TESTU
		/*
		for (int i = 0; i < miasta.size(); i++) {
			for (int j = 0; j < miasta.size(); j++) {
				cout << graf[i][j] << " ";
			}
			cout << endl;
		}
		*/

		cout << endl;
		cout << endl;
		vector <pair<string, string>> trasy = pobranie_tras(plik_z_trasami); // pobranie tras, ktore maja byc wyznaczone i nastepnie zapisane do pliku wyjsciowego
		
		if (trasy.size() == 0) { // sprawdzenie czy plik z trasami do wyznaczenia jest pusty
			return -1;
		}

		for (auto& element : trasy) { // jesli oba miasta znajduja sie w wektorze z miastami, wtedy pusc dijkstre
			if (find(miasta.begin(), miasta.end(), element.first) != miasta.end()
				and find(miasta.begin(), miasta.end(), element.second) != miasta.end()) {
				dijkstra(element.first, element.second, miasta, graf, plik_wynikowy);
			}
			else {
				ofstream plik(plik_wynikowy, ios::app);
				if (plik) { // jezeli ich nie ma, wtedy zapisz do pliku instrukcje widoczna ponizej:
					plik << "Trasa: " << element.first << " --> " << element.second << " : " << endl;
					plik << "TRASA NIEMOZLIWA DO WYZNACZENIA!!! " << endl;
					plik << endl;
				}
				plik.close();
			}
		}






		for (int i = 0; i < miasta.size(); i++) { // zwolnienie pamieci z tablicy dynamicznej 'graf'
			delete[] graf[i];
		}
		delete[] graf;
		graf = NULL;
	}
}
	