#include <iostream>
#include "Film.h"
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

Film::Film() {}

Film::Film(string ID, string name, string gen, string tg, string date) {
    IDfilm = ID;
    namefilm = name;
    genre = gen;
    time = tg;
    showdate = date;
}

string Film::getIDfilm() {
    return IDfilm;
}

string Film::getNamefilm() {
    return namefilm;
}

string Film::getGenre() {
    return genre;
}

string Film::getTime() {
    return time;
}

string Film::getShowdate() {
        return showdate;
}

void Film::show() {
  const int ID_WIDTH = 7;
    const int NAME_WIDTH = 35;
    const int GENRE_WIDTH = 25; 
    const int TIME_WIDTH = 12; 
    const int DATE_WIDTH = 15;

    cout << "|" << left 
         << setw(ID_WIDTH - 1) << IDfilm << "|" 
         << setw(NAME_WIDTH - 1) << namefilm << "|" 
         << setw(GENRE_WIDTH - 1) << genre << "|" 
         << setw(TIME_WIDTH - 1) << time << "|" 
         << setw(DATE_WIDTH - 1) << showdate << "|";
}
