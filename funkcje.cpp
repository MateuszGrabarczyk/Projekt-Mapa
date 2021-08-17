#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <limits.h>
#include <stdio.h>
#include <sstream>
using namespace std;

#include "funkcje.h"

void drukuj_sciezke(int* p, int** odl, vector<string> miasta, int m, string nazwa_pliku) {
	ofstream plik(nazwa_pliku, ios_base::app);

	if (plik) {
		if (p[m] != -1) {
			drukuj_sciezke(p, odl, miasta, p[m], nazwa_pliku);
			plik << miasta[p[m]] << " --> " << miasta[m] << " (" << odl[p[m]][m] << " km) " << endl;
		}
		
		
	}
	plik.close();
	
}

vector<string> vector_z_miastami(string nazwa_pliku) {
	vector<string> miasta; //vector przechowujacy miasta obecne w grafie
	ifstream plik(nazwa_pliku);

	string miasto_p, miasto_k;
	string linia;
	int odl;
	if (plik) {
		while (getline(plik, linia)) {
			istringstream iss(linia);
			while (iss >> miasto_p >> miasto_k >> odl) {
				if (find(miasta.begin(), miasta.end(), miasto_p) == miasta.end()) { //sprawdzenie czy miasto_p nie jest w vectorze
					miasta.push_back(miasto_p);
				}
				if (find(miasta.begin(), miasta.end(), miasto_k) == miasta.end()) { //sprawdzenie czy miasto_k nie jest w vectorze
					miasta.push_back(miasto_k);
				}
			}
		}
	}
	else {
		cout << "Nie uzyskano dostepu do pliku! " << endl;
	}

	plik.close();
	return miasta;
}

vector<pair<string, string>> pobranie_tras(const string& nazwa_pliku) {
	ifstream plik(nazwa_pliku);

	vector<pair<string, string>> trasy;
	string start, koniec;
	string linia;
	if (plik) {

		while (getline(plik, linia)) {
			istringstream iss(linia);
			while (iss >> start >> koniec) {
				trasy.push_back(make_pair(start, koniec));
			}
		}
	}
	else {
		cout << "Nie udalo sie otworzyc pliku" << endl;
	}

	plik.close();

	return trasy;
}

int zwroc_indeks(vector<string> miasta, string miasto) {
	int indeks;
	auto itr = find(miasta.begin(), miasta.end(), miasto);

	if (itr != miasta.end()) {
		indeks = itr - miasta.begin();
	}

	return indeks;

}

bool zapisanie_do_grafu(string nazwa_pliku, vector<string> miasta, int** graf) {
	ifstream plik(nazwa_pliku);

	string miasto_p, miasto_k;
	string linia;
	int odl;
	bool ok = false;
	if (plik) {
		
		while (getline(plik, linia)) {
			istringstream iss(linia);
			while (iss >> miasto_p >> miasto_k >> odl) {
				if (odl > 0 and graf[zwroc_indeks(miasta, miasto_p)][zwroc_indeks(miasta, miasto_k)] == 0) {
					graf[zwroc_indeks(miasta, miasto_p)][zwroc_indeks(miasta, miasto_k)] = odl;
					ok = true;
				}
			}
		}
	}
	
	plik.close();
	return ok;
}

int zwroc_najblizsze_miasto(int* odleglosci, bool* sprawdzone, vector<string>miasta) {
	int min_odl = INT_MAX;
	int indeks_min = 0;

	for (int i = 0; i < miasta.size(); i++) {
		if (sprawdzone[i] == false and odleglosci[i] < min_odl) {
			min_odl = odleglosci[i];
			indeks_min = i;
		}
	}

	return indeks_min;
}

void dijkstra(string m_start, string m_koniec, vector<string>miasta, int** graf, string nazwa_pliku_wyjsc) {
	
	int* odl; // wskaznik na tablice int odleglosci od wierzcholka startowego
	bool* spr;// wskaznik na tablice bool zawierajaca informacje o tym, czy wierzcholek zostal juz odwiedzony
	int* poprzedni; // wskaznik na tablice z poprzednikami kazdego z wierzcholkow
	
	odl = new int[miasta.size()];
	spr = new bool[miasta.size()];
	poprzedni = new int[miasta.size()];
	for (int i = 0; i < miasta.size(); i++) { // wszystkie odleglosci oznaczamy jako najwiekszy mozliwy int, a wszystkie wierzcholki jako nieodwiedzone
		odl[i] = INT_MAX;
		spr[i] = false;
		poprzedni[i] = -1;
	}
	odl[zwroc_indeks(miasta, m_start)] = 0; // odleglosc miasta startowego do jego samego zawsze bedzie rowna 0

	for (int i = 0; i < miasta.size() - 1; i++) {
		int miasto = zwroc_najblizsze_miasto(odl, spr, miasta); // pobieramy miasto z najmniejsza waga krawedzi (przy pierwszym przejsciu jest to zawsze indeks startu)
		
		spr[miasto] = true; // oznaczamy miasto jako odwiedzone w tablicy bool 
		
		for (int j = 0; j < miasta.size(); j++) { // sprawdzenie wierzcholkow sasiednich 
			if (spr[j] == false and graf[miasto][j] and odl[miasto] != INT_MAX // jesli miasto nie zostalo jeszcze odwiedzone i istnieje taka krawedz
				and odl[miasto] + graf[miasto][j] < odl[j]) {                  // oraz koszt dojscia jest nizszy
				odl[j] = odl[miasto] + graf[miasto][j];                        // wtedy zaaktualizuj wartosc
				poprzedni[j] = miasto;                                         // oraz zaaktualizuj poprzednika
			}
		}
	}	
	ofstream plik(nazwa_pliku_wyjsc, ios_base::app);
	
	int id_k = zwroc_indeks(miasta, m_koniec); // indeks miasta koncowego, do ktorego wyznaczamy trase

	if (plik) {
		if (odl[id_k] == INT_MAX) { // jesli miasto koncowe po przejsciu algorytmu nadal ma odleglosc ustawiona na INT MAX oznacza to, ze trasy nie da sie wyznaczyc
			plik << "Trasa: " << m_start << " --> " << miasta[id_k] <<" :" << endl;
			plik << "TRASA NIEMOZLIWA DO WYZNACZENIA!!!" << endl; 
			plik << endl;
		}
		else {
			plik << "Trasa: " << m_start << " --> " << miasta[id_k] << " (" << odl[id_k] << " km)" << endl;
			drukuj_sciezke(poprzedni, graf, miasta, id_k, nazwa_pliku_wyjsc);
			plik << endl;
		}
	}
	else {
		cout << "Nie udalo sie otworzyc pliku. ";
	}
	plik.close();
	
	
	delete[] odl; // zwolnienie pamieci trzech tablic dynamicznych
	delete[] spr;
	delete[] poprzedni;
	_CrtDumpMemoryLeaks();
}





