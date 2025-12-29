#pragma once
#include <iostream>
#include <fstream>
#include "Film.h"
#include "ManageShowtime.h"
using namespace std;

class ManageFilm {
public:
    Film list[100]; 
    int n;        
public:
    ManageFilm() { 
    n = 0; 
}
    void readFile(string readFile);
    void saveFile(string nameFile);
    void addfilm();
    void deletefilm(ManageShowtime& MS);
    void displayfilm(ManageShowtime& MS);
    Film* getfilm(string const &id);
};
