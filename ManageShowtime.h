#pragma once
#include <iostream>
#include <fstream>
#include "Showtime.h"
#include "Film.h"
using namespace std;

class ManageFilm; 
class ManageRoom;

class ManageShowtime{
    private:
        Showtime list[100];
        int n;
    public:
        ManageShowtime();
        void readFile(const string& filename, ManageFilm& MF, ManageRoom& MR);
        void saveFile(const string& Filename);
        void display();
        void addShowtime(ManageFilm& MR, ManageRoom& MF);
        void deleteShowtime();
        void deleteShowtimebyFilm(Film& f);
        void deleteShowtimebyRoom(Room& p);
        void deleteSeatfile(const string& STid);
        bool isDuplicateShowtime(ManageRoom& MR,const string& idroom,const string& day, const string& time);
        bool findFilm(const string &idfilm);
        bool findFilmm(const string &idfilm);
        bool findRoom(const string &idroom);
        int toDateInt(const string& d);
        static bool isPastDateTime(string day, string time);
        bool isBeforeReleaseDate(const string& release, const string& day);
        Showtime* getshowtimeByID(const string& id);
        int getSoLuong() const;  
        static string toComparableString(const string& date, const string& time);
};