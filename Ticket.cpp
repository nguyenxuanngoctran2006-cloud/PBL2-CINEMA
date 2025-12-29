#include <iostream>
#include "Ticket.h"
#include "Customer.h"
#include "Film.h"
#include "Showtime.h"
#include "Room.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

Ticket::Ticket(){}

Ticket::~Ticket(){
    F = nullptr;
    SC = nullptr;
    R = nullptr;
    C = nullptr;
}

Ticket::Ticket(const string& id, Film* f, Showtime* sc, string chair, Room* r, Customer* c, int cnt, string time) : TicketID(id), F(f), SC(sc), ChairID(chair), R(r), C(c), count(cnt), time(time) {}

string Ticket::getTicketID(){
    return TicketID;
}

string Ticket::getChairID(){
    return ChairID;
}

int Ticket::getcount(){
    return count;
}

string Ticket::gettime(){
    return time;
}

Film* Ticket::getFilm(){
    return F;
}

Showtime* Ticket::getShowtime(){
    return SC;
}

Room* Ticket::getRoom(){
    return R;
}

Customer* Ticket::getCustomer(){
    return C;
}

string Ticket::generateTicketID(const string &filename, const string &prefix){
    ifstream fs(filename);
    if (!fs.is_open()){
        return prefix + "001";
    }

    int maxID = 0;
    string line;
    
    while(getline(fs, line)){
        if (line.empty()) continue;
        
        stringstream ss(line);
        string cID, tID;
        getline(ss, cID, ';'); 
        getline(ss, tID, ';'); 
        if (tID.rfind(prefix, 0) == 0){
            try {
                string numStr = tID.substr(prefix.length());
                int num = stoi(numStr);
                
                if (num > maxID){
                    maxID = num;
                }
            } catch (const std::exception& e) {
                continue; 
            }
        }
    }
    
    fs.close();
    maxID++;
    stringstream newIDss;
    newIDss << prefix << std::setw(3) << std::setfill('0') << maxID;
    
    return newIDss.str();
}

string Ticket::getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);

    tm *timeinfo = localtime(&now_c);

    stringstream ss;
    ss << put_time(timeinfo, "%d/%m/%Y %H:%M:%S");
    return ss.str();
}
void Ticket::PrintTicket() {
    const int width = 94;
    cout << "\n" << string(width, '=') << "\n";
    string title = "HOA DON MUA VE XEM PHIM";
    cout << string((width - title.length())/2, ' ') << title << "\n";
    cout << string(width, '=') << "\n\n";

    cout << left << setw(45) << "                         Khach Hang:" << C->getName() << "\n";
    cout << left << setw(45) << "                         So Dien Thoai:" << C->getNumber() << "\n";
    cout << left << setw(45) << "                         Ten Phim:" << F->getNamefilm() << "\n";
    cout << left << setw(45) << "                         Ngay Xem:" << SC->getDate() << "\n";
    cout << left << setw(45) << "                         Suat Xem:" << SC->getTime() << "\n";
    cout << left << setw(45) << "                         Phong Chieu:" << R->getRoomName() << "\n";

    cout << "\n" << string(width, '-') << "\n";
    cout << left << setw(40) << "                         Danh Sach Vi Tri:\n";

    stringstream ss(ChairID);
    string chair;
    while (getline(ss, chair, ',')) {
        cout << left << setw(45) << "                                " << chair << "\n";
    }

    cout << string(width, '-') << "\n";
    double total = getcount() * SC->getPrice();
    cout << left << setw(45) << "                         THANH TIEN:" << total << " VND\n";

    string day = Ticket::getCurrentTime();
    cout << left << setw(45) << "                         Hoa don in luc:" << day << "\n";
    cout << string(width, '=') << "\n";

    // Lưu hóa đơn
    fs::create_directories("Database/Bill");
    string month = day.substr(3, 2);
    string filename = "Database/Bill/Bill_" + month + ".txt";
    ofstream fs(filename, ios::app);
    fs << getTicketID() << ";" << C->getName() << ";" << total << ";" << day << "\n";
}
