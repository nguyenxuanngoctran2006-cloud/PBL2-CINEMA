#pragma once
#include "Film.h"
#include "Room.h"     
using namespace std;

class Ticket;

class Showtime {
private:
    string showID;
    string showDate;
    string showTime;
    double ticketPrice;
    char seatStatus[5][10];
    Ticket* tickets[50];
    Room* room;        
    Film* film;
    int ticketCount;

public:
    Showtime();
    Showtime(
        const string &showID,
        Film* filmPtr,
        Room* roomPtr,       
        const string &date,
        const string &time,
        const double &price,
        const int &ticketCount
    );

    string getDate();
    double getPrice();
    string getTime();
    Film* getFilm();
    string getShowID();
    Room* getRoom();         

    void addTicket(Ticket* ticket);
    int getTicketCount();
    Ticket* getTicket(int i);

    bool isFull();
    bool isWeekend(const string &date);
    void display();
    bool checkseat();

    bool bookSeat(const char row, const int column);
    bool cancelSeat(const char row, const int column);

    void saveSeatStatus(const string &showID);
    void loadSeatStatus(const string &showID);

    static string generateNewID(const string &filename, const string &prefix);
    static double calculatePriceByDate(const string& date);
};
