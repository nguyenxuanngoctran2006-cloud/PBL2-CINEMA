#include "ManageFilm.h"
#include "ManageShowtime.h"
#include "ManageRoom.h"
#include "ManageTicket.h"
#include "Ticket.h"
#include "Film.h"
#include "Logger.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <iostream>
#include <filesystem>


namespace fs = std::filesystem;
ManageShowtime::ManageShowtime(){
    n = 0;
}

void ManageShowtime::readFile(const string& filename, ManageFilm& MF, ManageRoom& MR){
    fs::create_directories("Database");
    ifstream fin(filename);
    if (!fin.is_open()){ 
        cout << "Can not open file " << filename << "\n";
        return;
    }
    n = 0;
    string line;
    while (getline(fin, line) && n < 100){
        if (line.empty()) continue; 

        stringstream ss(line);
        string masuat, maphim, maphong, ngay, gio;
        double gia;

        getline(ss, masuat, ';');
        getline(ss, maphim, ';');
        getline(ss, maphong, ';');
        getline(ss, ngay, ';');
        getline(ss, gio, ';');
        ss >> gia;
        Film* f = MF.getfilm(maphim);
        Room* r = MR.getRoom(maphong);
        if (f != nullptr && r != nullptr) {
        list[n++] = Showtime(masuat, f, r, ngay, gio, gia, 50);
        } else {
            cout << "Canh bao: Khong tim thay Film (" << maphim << ") hoac Room (" << maphong << ") cho suat chieu " << masuat << "\n";
            Logger::getInstance().warn("Khong tim thay Film " + maphim + " hoac Room " + maphong +" khi doc suat chieu " + masuat + ".");
        }
    }
    fin.close();
}

bool cmp(Showtime x, Showtime y){
        if (ManageShowtime::toComparableString(x.getDate(), x.getTime()) > ManageShowtime::toComparableString(y.getDate(), y.getTime()))
        return false;
    else
        return true;
}

void ManageShowtime::display() {
    if (n == 0) {
        cout << "Khong co suat chieu nao!\n";
        return;
    }

    sort(list, list + n, cmp);
    Logger::getInstance().info("Hien thi danh sach suat chieu hien tai");

    const int STT_WIDTH      = 5;
    const int MASUAT_WIDTH   = 9;
    const int MAPHIM_WIDTH   = 10;
    const int TENPHIM_WIDTH  = 28;
    const int MAPHONG_WIDTH  = 11;
    const int NGAY_WIDTH     = 12;
    const int GIO_WIDTH      = 8;
    const int GIA_WIDTH      = 10;

    int totalWidth = STT_WIDTH + MASUAT_WIDTH + MAPHIM_WIDTH + TENPHIM_WIDTH
                   + MAPHONG_WIDTH + NGAY_WIDTH + GIO_WIDTH + GIA_WIDTH + 1;

    cout << "\n" << string(totalWidth, '=') << endl;
    cout << "                                DANH SACH SUAT CHIEU HIEN TAI\n";
    cout << string(totalWidth, '=') << endl;

    cout << "|" << left
         << setw(STT_WIDTH - 1) << "STT" << "|"
         << setw(MASUAT_WIDTH - 1) << "MA SUAT" << "|"
         << setw(MAPHIM_WIDTH - 1) << "MA PHIM" << "|"
         << setw(TENPHIM_WIDTH - 1) << "TEN PHIM" << "|"
         << setw(MAPHONG_WIDTH - 1) << "MA PHONG" << "|"
         << setw(NGAY_WIDTH - 1) << "NGAY" << "|"
         << setw(GIO_WIDTH - 1) << "GIO" << "|"
         << setw(GIA_WIDTH - 1) << "GIA (VND)" << "|"
         << endl;

    cout << string(totalWidth, '=') << endl;

    int stt = 1;
    for (int i = 0; i < n; i++) {
        if (ManageShowtime::isPastDateTime(list[i].getDate(), list[i].getTime())) {
            continue;
        }

        cout << "|" << left
             << setw(STT_WIDTH - 1) << stt++ << "|"
             << setw(MASUAT_WIDTH - 1) << list[i].getShowID() << "|"
             << setw(MAPHIM_WIDTH - 1) << list[i].getFilm()->getIDfilm() << "|"
             << setw(TENPHIM_WIDTH - 1) << list[i].getFilm()->getNamefilm() << "|"
             << setw(MAPHONG_WIDTH - 1) << list[i].getRoom()->getRoomID() << "|"
             << setw(NGAY_WIDTH - 1) << list[i].getDate() << "|"
             << setw(GIO_WIDTH - 1) << list[i].getTime() << "|"
             << right << setw(GIA_WIDTH - 1)
             << fixed << setprecision(0) << list[i].getPrice()
             << left << "|"
             << endl;
    }

    if (stt == 1) {
        cout << "Khong co suat chieu hop le (tat ca deu da chieu xong)!\n";
    }

    cout << string(totalWidth, '=') << endl;
}

void ManageShowtime::saveFile(const string& filename){
    fs::create_directories("Database");
    ofstream fout(filename);
    if (!fout.is_open()){
        cout << "Can not open file " << filename << " to write\n";
        return;
    }
    for (int i = 0; i < n; ++i){
        fout << list[i].getShowID() << ";"
             << list[i].getFilm()->getIDfilm() << ";"
             << list[i].getRoom()->getRoomID() << ";"
             << list[i].getDate() << ";"
             << list[i].getTime() << ";"
             << list[i].getPrice() << "\n";
    }
    fout.close();
}

bool ManageShowtime::isDuplicateShowtime(ManageRoom& MR,const string& idroom,const string& day, const string& time){
    for (int i = 0; i < n; ++i){
        if (list[i].getRoom()->getRoomID() == idroom && list[i].getDate() == day && list[i].getTime() == time){
            return true;
        }
    }
    return false;
}

int ManageShowtime::toDateInt(const string& d) {
    // d = dd-MM-yyyy
    int dd = stoi(d.substr(0,2));
    int mm = stoi(d.substr(3,2));
    int yy = stoi(d.substr(6,4));
    return yy * 10000 + mm * 100 + dd;
}

bool ManageShowtime::isBeforeReleaseDate(const string& release, const string& day) {
    string r = release.substr(6,4) + release.substr(3,2) + release.substr(0,2);
    string d = day.substr(6,4)     + day.substr(3,2)     + day.substr(0,2);
    return d < r;
}

string ManageShowtime::toComparableString(const string& date, const string& time) {
    return date.substr(6, 4)   
        + date.substr(3, 2)   
        + date.substr(0, 2)  
        + time.substr(0, 2)   
        + time.substr(3, 2);  
}

bool ManageShowtime::isPastDateTime(string day, string time){
    replace(day.begin(), day.end(), '-', '/');
    string now = Ticket::getCurrentTime();
    string today = now.substr(0, 10);        
    string curTime = now.substr(11, 5);      

    int d1, m1, y1;
    int d2, m2, y2;

    sscanf(day.c_str(), "%d/%d/%d", &d1, &m1, &y1);
    sscanf(today.c_str(), "%d/%d/%d", &d2, &m2, &y2);

    if (y1 < y2) return true;
    if (y1 > y2) return false;

    if (m1 < m2) return true;
    if (m1 > m2) return false;

    if (d1 < d2) return true;
    if (d1 > d2) return false;

    if (time < curTime) return true;

    return false;
}

void ManageShowtime::addShowtime(ManageFilm& MF, ManageRoom& MR){
    string ma, day, time, room;
    double gia;
    while (true){
        cout << "Nhap ma phim: ";
        cin >> ma;
        Film* f = MF.getfilm(ma);
        if (!f) {
            cout << "Loi: Khong tim thay ma phim " << ma << ".\n";
            Logger::getInstance().error("Khong tim thay ma phim: " + ma);
            return;
        }
        while(findFilm(ma)){
            break;
        }
        cout << "Nhap ngay chieu (DD-MM-YYYY): ";
        cin >> day;
        string release = f->getShowdate();
        if (isBeforeReleaseDate(release, day)) {
            cout << "Loi: Suat chieu truoc ngay phat hanh phim (" << release << ")!\n";
            Logger::getInstance().error("Suat chieu " + day + " truoc ngay phat hanh phim " + release);
            return;
        }
        cout << "Chon gio chieu (1-3):\n1. 08:00\n2. 14:00\n3. 19:00\nLua chon: ";
        int chon; 
        cin >> chon; 
        cin.ignore();
        if (chon == 1) time = "08:00";
        else if (chon == 2) time = "14:00";
        else time = "19:00"; 
        if (ManageShowtime::isPastDateTime(day, time)) {
        cout << "Loi: Suat chieu " << day << " " << time << " da qua thoi gian hien tai. Khong the them!\n";
        Logger::getInstance().error("Them suat chieu that bai do ngay gio da qua: " + day + " " + time);
        return;
        }
        cout << "Chon phong chieu: \n";
        MR.showRoom();
        cout << "Nhap ma phong chieu: ";
        cin >> room;
        if (isDuplicateShowtime(MR, room, day, time)) {
            cout << "Loi: da co suat chieu trung (ngay: " << day << ", gio: " << time << ").\n";
            Logger::getInstance().error("Them suat chieu that bai do trung suat: phong " + room + ", ngay " + day + ", gio " + time);
            cout << "1. Nhap lai\n2. Huy\nLua chon: ";
            int opt; 
            cin >> opt; 
            cin.ignore();
            if (opt == 2) return; 
        } else {
            break;
        }
    }
    gia = Showtime::calculatePriceByDate(day);
    string id = Showtime::generateNewID("Database/Showtime.txt", "SC");
    Film* f = MF.getfilm(ma);
    Room* r = MR.getRoom(room);
    if (f == nullptr) {
        cout << "Loi: Khong tim thay ma phim " << ma << ".\n";
        Logger::getInstance().error("Khong tim thay ma phim: " + ma);
        return;
    }
    if (r == nullptr) {
        cout << "Loi: Khong tim thay ma phong " << room << ".\n";
        Logger::getInstance().error("Khong tim thay ma phong chieu: " + room);
        return;
    }
    list[n++] = Showtime(id, f, r, day, time, gia, 0);
    saveFile("Database/Showtime.txt");
    Logger::getInstance().info("Da them suat chieu " + id + " cho phim " + ma + " vao ngay " + day + " luc " + time + " tai phong " + r->getRoomID() + ".");
    cout << "Them suat chieu thanh cong: " << ma << " " << day << " " << time << "\n";
}

void ManageShowtime::deleteShowtime(){
    if (n == 0){
        cout << "Danh sach trong!\n";
        return;
    } 
    string ID;
    bool found = false;
    do {
        cout << "Nhap ma suat chieu de xoa hoac nhap '0' de thoat: ";
        cin >> ID;
        if (ID == "0") return;
        Showtime ST;
        for (int i = 0; i < n; i++){
            if (list[i].getShowID() == ID){
                string day = list[i].getDate();
                string time = list[i].getTime();
                if (ManageTicket::isCancel(day, time)){
                    if (list[i].checkseat()){
                        for (int j = i; j < n - 1; j++){
                            list[j] = list[j + 1];
                        }      
                        n--;
                        Logger::getInstance().info("Da xoa suat chieu " + ID + ".");
                        cout << "Da xoa suat chieu co ma: " << ID << endl;
                        found = true;
                        break;
                    }else{
                        cout << "Suat chieu khong the huy vi da co nguoi dat ";
                        Logger::getInstance().warn("Khong the huy suat chieu co ma " + ID + " vi da co nguoi dat");
                        return;
                    }
                }
                else{
                    cout << "Suat chieu khong the huy vi sap chieu ";
                    Logger::getInstance().warn("Khong the huy suat chieu co ma " + ID + " vi sap chieu");
                    return;
                }
            }
        }
        if (!found) {
            cout << "Khong tim thay suat chieu co ma " << ID << ". Vui long nhap lai!\n";
            Logger::getInstance().warn("Khong co suat chieu nao cho ma " + ID);

        }
    } while (!found);
    if (found) {
        saveFile("Database/Showtime.txt");
    }
}

bool ManageShowtime::findFilm(const string &idfilm){
    if (n == 0) {
        cout << "Danh sach trong!\n";
        Logger::getInstance().warn("Tim suat chieu nhung danh sach rong");
        return false;
    }

    bool check = false;

    for (int i = 0; i < n; ++i){
        Film* filmPtr = list[i].getFilm();
        Room* roomPtr = list[i].getRoom();

        if (ManageShowtime::isPastDateTime(list[i].getDate(), list[i].getTime())) {
            continue;
        }

        if (filmPtr != nullptr && roomPtr != nullptr && filmPtr->getIDfilm() == idfilm){
            Logger::getInstance().info("Tim thay suat chieu " + list[i].getShowID() + " cua phim " + idfilm);
            if (!check) {
                cout << "\nCo suat chieu:\n";
                cout << string(94, '=') << "\n";

                cout << left 
                    << "| " << setw(14) << "Ma suat"
                    << "| " << setw(20) << "Phong"
                    << "| " << setw(12) << "Ma phong"
                    << "| " << setw(12) << "Ngay"
                    << "| " << setw(12)  << "Gio"
                    << "| " << setw(11) << "Gia"
                    << "|\n";

                cout << string(94, '=') << "\n";
            }
            cout << left
                << "| " << setw(14) << list[i].getShowID()
                << "| " << setw(20) << roomPtr->getRoomName()
                << "| " << setw(12) << roomPtr->getRoomID()
                << "| " << setw(12) << list[i].getDate()
                << "| " << setw(12)  << list[i].getTime()
                << "| " << setw(11) << (int)list[i].getPrice()
                << "|\n";

            check = true;
        }
    }

    if (!check){
        cout << "Khong co suat chieu nao cho ma phim " << idfilm << "\n";
        Logger::getInstance().warn("Khong co suat chieu nao cho ma phim " + idfilm);
        return false;
    }

    cout << string(94, '=') << "\n";
    return true;
}


bool ManageShowtime::findFilmm(const string &idfilm){
    if (n == 0) {
        Logger::getInstance().warn("Tim suat chieu nhung danh sach rong");
        return false;
    } 
    bool check = false;
    for (int i = 0; i < n; ++i){
        Film* filmPtr = list[i].getFilm();
        Room* roomPtr = list[i].getRoom();
        if (isPastDateTime(list[i].getDate(), list[i].getTime())) {
            continue;
        }
        if (filmPtr != nullptr && roomPtr != nullptr && filmPtr->getIDfilm() == idfilm){
            check = true;
        }
    }
    if (!check){
        Logger::getInstance().warn("Khong co suat chieu nao cho ma phim " + idfilm);
        return false;
    }
    return true;
}

bool ManageShowtime::findRoom(const string &idroom){
    if (n == 0) {
        cout << "Danh sach trong!\n";
        Logger::getInstance().warn("Tim suat chieu nhung danh sach rong");
        return false;
    } 
    bool check = false;
    for (int i = 0; i < n; ++i){
        Film* filmPtr = list[i].getFilm();
        Room* roomPtr = list[i].getRoom();
        if (isPastDateTime(list[i].getDate(), list[i].getTime())) {
            continue;
        }
        if (roomPtr != nullptr && roomPtr->getRoomID() == idroom && filmPtr !=  nullptr){
            check = true;
        }
    }
    if (!check){
        Logger::getInstance().warn("Khong co suat chieu nao cho ma phong " + idroom);
        return false;
    }
    return true;
}


Showtime* ManageShowtime::getshowtimeByID(const string& id) {
    if (n == 0) {
        cout << "Danh sach trong!\n";
        return nullptr; 
    }
    for (int i = 0; i < n; ++i) {
        if (list[i].getShowID() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

int ManageShowtime::getSoLuong() const {
    return n;
}

void ManageShowtime::deleteShowtimebyFilm(Film& f) {
    if (n == 0) {
        cout << "Danh sach suat chieu trong!\n";
        Logger::getInstance().warn("Xoa suat chieu theo phim that bai - danh sach rong");
        return;
    }
    string filmID = f.getIDfilm();
    bool deleted = false;
    for (int i = n - 1; i >= 0; --i) {
        Film* filmPtr = list[i].getFilm(); 
        Logger::getInstance().info("Da xoa suat chieu " + list[i].getShowID() + " cua phim " + filmID + ".");
        if (filmPtr != nullptr && filmPtr->getIDfilm() == filmID) {
            cout << "Da xoa suat chieu: " << list[i].getShowID() << " cua phim " << filmID << endl;
            for (int j = i; j < n - 1; j++) {
                list[j] = list[j + 1];
            }
            n--;
            deleted = true;
        }
    }

    if (deleted) {
        saveFile("Database/Showtime.txt");
        cout << "Da cap nhat file Showtime.txt sau khi xoa!\n";
    } else {
        cout << "Khong co suat chieu nao lien quan den phim " << filmID << " duoc tim thay.\n";
        Logger::getInstance().warn("Khong co suat chieu nao lien quan den phim " + filmID);
    }
}

void ManageShowtime::deleteShowtimebyRoom(Room& p) {
    if (n == 0) {
        cout << "Danh sach suat chieu trong!\n";
        Logger::getInstance().warn("Tim suat chieu theo phim that bai - danh sach rong");
        return;
    }
    string roomID = p.getRoomID(); 
    
    bool deleted = false;
    for (int i = n - 1; i >= 0; --i) {
        Room* roomPtr = list[i].getRoom(); 
        if (roomPtr != nullptr && roomPtr->getRoomID() == roomID) {
            
            cout << "Da xoa suat chieu: " << list[i].getShowID() << " cua phong " << roomID << endl;
            for (int j = i; j < n - 1; j++) {
                list[j] = list[j + 1];
            }
            n--;
            deleted = true;
        }
        Logger::getInstance().info("Da xoa suat chieu " + list[i].getShowID() + " cua phim co ma phong " + roomID + ".");
    }

    if (deleted) {
        saveFile("Database/Showtime.txt");
        cout << "Da cap nhat file Showtime.txt sau khi xoa cac suat chieu cua phong " << roomID << "!\n";
    } else {
        cout << "Khong co suat chieu nao lien quan den phong " << roomID << " duoc tim thay.\n";
        Logger::getInstance().warn("Khong co suat chieu nao lien quan den phong " + roomID);

    }
}
