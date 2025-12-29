#pragma once
#include <iostream>
#include <string>

using namespace std;

class Film;
class Room;
class Showtime;
class Customer;

class Ticket{
    private:
        string TicketID;
        Film* F;
        Showtime* SC;
        string ChairID;
        Room* R;
        Customer* C;
        int count;
        string time;
    public:
        Ticket();
        ~Ticket();
        Ticket(const string& id, Film* f, Showtime* sc, string chair, Room* r, Customer* c, int cnt, string time);
        string getTicketID();
        string getChairID();
        string gettime();
        int getcount();
        Film* getFilm();
        Showtime* getShowtime();
        Room* getRoom();
        Customer* getCustomer();
        static string generateTicketID(const string &filename, const string &prefix);
        void PrintTicket();
        static string getCurrentTime();
};

