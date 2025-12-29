#include <iostream>
#include "Customer.h"
#include "ManageFilm.h"
#include "Showtime.h"
#include "Film.h"
#include "Ticket.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

using namespace std;

Customer::Customer() : User() {
}

Customer::Customer(const string &n, const string &u, const string &p, const string r, const string &id, const string &pnum) : User(n, u, p, "customer", pnum), id(id){}

string Customer::getid(){
    return id;
}

void Customer::ShowInfo() {
    int width = 94;
    cout << "\n" << string(width, '=') << "\n";

    string title = "THONG TIN KHACH HANG";
    cout << string((width - title.length())/2, ' ') << title << "\n";
    cout << string(width, '=') << "\n";

    cout << string((width - ("Ma khach hang: " + id).length())/2, ' ') << "Ma khach hang: " << id << "\n";
    cout << string((width - ("Ten khach hang: " + name).length())/2, ' ') << "Ten khach hang: " << name << "\n";
    cout << string((width - ("Username: " + username).length())/2, ' ') << "Username: " << username << "\n";
    cout << string((width - ("So dien thoai: " + pnumber).length())/2, ' ') << "So dien thoai: " << pnumber << "\n";

    cout << string(width, '=') << "\n\n";
}


