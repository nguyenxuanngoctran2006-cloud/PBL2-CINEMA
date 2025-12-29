#pragma once
#include <iostream>
using namespace std;
class Ticket;

class Film
{
private:
    string IDfilm;
    string namefilm;
    string genre;
    string time;
    string showdate;
public:
    Film();
    Film(string ID, string name, string gen, string tg, string date);

    string getIDfilm();
    string getNamefilm();
    string getGenre();
    string getTime();
    string getShowdate();

    void show();
};


