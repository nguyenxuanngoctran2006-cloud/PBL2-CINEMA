#pragma once
#include "Ticket.h"
#include "ManageShowtime.h"
#include "ManageCustomer.h"
#include <string>
using namespace std;

constexpr int MAX_TICKETS = 10000;

struct Bill {
    string ticketid, date, name;
    double money;
    int day, month, year;
};

class ManageTicket {
private:
    Ticket list[MAX_TICKETS];
    int n;

public:
    ManageTicket();
    void readFile(const std::string& filename, ManageShowtime& MST, ManageRoom& MR, ManageFilm& MF, ManageCustomer& MC);
    void saveFile(const string &filename);
    void Order(ManageFilm& MF, ManageShowtime& MST, ManageRoom& MR, Customer& C);

    int getTotalTickets() const { return n; }
    void readBills(Bill* &bills, int &billCount);
    void saveBills(Bill* &bills, int &billCount);
    void deletebill(Bill* &bills, const string& id, int &billCount);

    string revenueByDay(int d, int m, int y, Bill* bills, int billCount);
    string revenueByMonth(int m, int y, Bill* bills, int billCount);
    string revenueByYear(int y, Bill* bills, int billCount);

    string formatMoney(double money);

    void mostBookedFilmStatistics(int month, ManageFilm& MF);

    static bool isCancel(const string& day, const string& time);
    void delcustomerticket(const string& customerid);
    void cancelTicket(Customer *c);
    void display(Customer *c);
    void deleteticket(const string &id);
    Ticket* getTicketbyID(const string &id);
};
