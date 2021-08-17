#ifndef FUNKCJE_H
#define FUNKCJE_H 
#include <vector>
using namespace std;

/**
 * @brief Pobiera miasta obecne w grafie 
 * @param nazwa_pliku nazwa pliku z drogami
 * @return set z miastami obecnymi w grafie
*/

vector<string> vector_z_miastami(string nazwa_pliku);

/**
 * @brief Pobranie tras, ktore trzeba wyznaczyc 
 * @param nazwa_pliku nazwa pliku z trasami, ktore trzeba wyznaczyc
 * @return wektor par string string, ktorego pierwszym elementem pary jest miasto startowe, a drugim koncowe
*/

vector<pair<string, string>> pobranie_tras(const string& nazwa_pliku);

/**
 * @brief Funkcja zwracajaca indeks miasta w wektorze 'miasta'
 * @param miasta wektor z miastami obecnymi w grafie
 * @param miasto miasto, ktorego indeks chcemy otrzymac
 * @return indeks miasta w wektorze 'miasta'
*/

int zwroc_indeks(vector<string> miasta, string miasto);

/**
 * @brief Funkcja zapisujaca polaczenia miedzy miastami do grafu 
 * @param nazwa_pliku nazwa pliku tekstowego z miastami
 * @param miasta vector miast obecnych w grafie
 * @param graf graf prezentujacy polaczenia miedzy miastami
 * @return true or false, w zaleznosci, czy plik jest pusty czy nie 
*/

bool zapisanie_do_grafu(string nazwa_pliku, vector<string> miasta, int** graf);

/**
 * @brief Funkcja zwracajaca indeks miasta z najnizsza odlegloscia
 * @param odleglosci tablica zawierajaca odleglosci 
 * @param sprawdzone tablica bool zawierajaca wierzcholki (sprawdza czy sa odwiedzone czy tez nie)
 * @param miasta vector miast (sluzy tutaj do wyznaczenia ilosci wierzcholkow w grafie)
 * @return zwraca indeks miasta z najnizsza odlegloscia (waga)
*/

int zwroc_najblizsze_miasto(int* odleglosci, bool* sprawdzone, vector<string>miasta);

/**
 * @brief Glowny algorytm odpowiadajacy za wyznaczenie najkrotszych drog z miasta startowego do reszty miast
 * @param m_start miasto startowe
 * @param m_koniec miasto koncowe
 * @param miasta wektor miast obecnych w grafie
 * @param graf macierz sasiedztwa (graf)
 * @param nazwa_pliku_wyjsc nazwa pliku wyjsciowego, do ktorego zapiszemy trasy
*/

void dijkstra(string m_start, string m_koniec, vector<string>miasta, int** graf, string nazwa_pliku_wyjsc);

/**
 * @brief Drukowanie sciezki rekurencyjnie od miasta startowego do miasta koncowego
 * @param p lista poprzednikow
 * @param odl graf z odleglosciami
 * @param miasta vector miast w grafie
 * @param m miasto koncowe
 * @param nazwa_pliku nazwa pliku wyjsciowego, do ktorego zapisujemy dana trase
*/

void drukuj_sciezke(int* p, int** odl, vector<string> miasta, int m, string nazwa_pliku);
#endif
