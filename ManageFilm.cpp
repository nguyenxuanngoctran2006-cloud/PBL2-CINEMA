#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "ManageFilm.h"
#include "ManageShowtime.h"
#include "Logger.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

bool validFilmID(const string &id) {
    if (id.size() != 4) return false;
    if (id[0] != 'P') return false;
    for (int i = 1; i < 4; i++) {
        if (!isdigit(id[i])) return false;
    }
    return true;
}

void ManageFilm::readFile(string filename) {
    fs::create_directories("Database");
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Khong the mo file!" << endl;
        return;
    }

    n = 0;
    string line;
    while (getline(fin, line) && n < 100) {
        stringstream ss(line);
        string ID, name, genre, time, date;

        getline(ss, ID, ';');
        getline(ss, name, ';');
        getline(ss, genre, ';');
        getline(ss, time, ';');
        getline(ss, date);

        if (!ID.empty() && ID[0] == ' ') ID.erase(0, 1);
        if (!name.empty() && name[0] == ' ') name.erase(0, 1);

        list[n++] = Film(ID, name, genre, time, date);
    }

    fin.close();
}

void ManageFilm::saveFile(string filename) {
    fs::create_directories("Database");
    ofstream fout(filename);
    for (int i = 0; i < n; i++) {
        fout << list[i].getIDfilm() << ";"
             << list[i].getNamefilm() << ";"
             << list[i].getGenre() << ";"
             << list[i].getTime() << ";"
             << list[i].getShowdate() << "\n";
    }
    fout.close();
}

void ManageFilm::addfilm() {
    if (n >= 100) {
        cout << "Danh sach da day!\n";
        Logger::getInstance().warn("Them phim that bai: Danh sach da day");

        return;
    }

    string ID, name, genre, time, date;
    while (true) {
        cout << "Nhap ma phim (dang Pxxx, vi du: P001): ";
        getline(cin, ID);
        if (ID == "0") {
            return;
        }
        if (!validFilmID(ID)) {
            cout << "Loi: Ma phim khong hop le! Vui long nhap lai hoac nhap '0' de thoat.\n";
            Logger::getInstance().warn("Nhap sai ID phim: " + ID);
            continue;
        }
        bool duplicate = false;
        for (int i = 0; i < n; i++) {
            if (list[i].getIDfilm() == ID) {
                cout << "Loi: Ma phim da ton tai! Vui long nhap lai.\n";
                Logger::getInstance().warn("Trung ID phim khi them: " + ID);
                duplicate = true;
                break;
            }
        }

        if (!duplicate) break;
    }

    do {
        cout << "Nhap ten phim: ";
        getline(cin, name);
        if (name.empty()) cout << "Ten phim khong duoc de trong!\n";
    } while (name.empty());

    do {
        cout << "Nhap the loai: ";
        getline(cin, genre);
        if (genre.empty()) cout << "The loai khong duoc de trong!\n";
    } while (genre.empty());

    do {
        cout << "Nhap thoi luong: ";
        getline(cin, time);
        if (time.empty()) cout << "Thoi luong khong duoc de trong!\n";
    } while (time.empty());

    do {
        cout << "Nhap ngay ra mat: ";
        getline(cin, date);
        if (date.empty()) cout << "Ngay ra mat khong duoc de trong!\n";
    } while (date.empty());

    list[n++] = Film(ID, name, genre, time, date);
    saveFile("Database/Film.txt");
    Logger::getInstance().info("Them phim thanh cong: " + ID + " - " + name);
    cout << "Them phim thanh cong!\n";
}


void ManageFilm::deletefilm(ManageShowtime& MS) {
    if (n == 0) {
        cout << "Danh sach trong!\n";
        Logger::getInstance().warn("Xoa phim nhung danh sach phim trong");
        return;
    }
    string ID;
    bool found = false;
    while (true) {
        cout << "Nhap ma phim de xoa hoac nhap '0' de thoat: ";
        getline(cin, ID);
        if (ID == "0") return;
        for (int i = 0; i < n; i++) {
            if (list[i].getIDfilm() == ID) {
                if (MS.findFilmm(ID)){
                    cout << "Khong the xoa phim vi van con suat chieu! ";
                    Logger::getInstance().warn("Khong the xoa phim vi van con suat chieu, ma phim: " + ID);
                    return;
                }
                else{
                    MS.deleteShowtimebyFilm(list[i]); 
                    for (int j = i; j < n - 1; j++) {
                        list[j] = list[j + 1];
                    }
                    n--;
                    Logger::getInstance().info("Xoa phim: " + ID);
                    cout << "Da xoa phim co ma: " << ID << endl;
                    found = true;
                    break; 
                }
            }
        }
        if (found){
            saveFile("Database/Film.txt");
            break;
        }
        else {cout << "Khong tim thay phim co ma " << ID << ". Vui long nhap lai!\n";
             Logger::getInstance().warn("Nhap sai ma phim khi xoa: " + ID);
        }
    }
}


void ManageFilm::displayfilm(ManageShowtime &MS) {
    if (n == 0) {
        cout << "Khong co phim trong danh sach!\n";
        Logger::getInstance().info("Hien thi phim nhung danh sach rong");
        return;
    }

    Logger::getInstance().info("Hien thi danh sach phim");

    const int ID_WIDTH = 6;
    const int NAME_WIDTH = 25;
    const int GENRE_WIDTH = 20;
    const int TIME_WIDTH = 10;
    const int DATE_WIDTH = 12;
    const int STATUS_WIDTH = 14;

    const int totalWidth = 
        ID_WIDTH + NAME_WIDTH + GENRE_WIDTH +
        TIME_WIDTH + DATE_WIDTH + STATUS_WIDTH + 7;

    cout << "\n" << string(totalWidth, '=') << "\n";

    string title = "DANH SACH PHIM";
    int pad = (totalWidth - title.length()) / 2;
    cout << string(pad, ' ') << title << "\n";

    cout << "|" << left
         << setw(ID_WIDTH)      << "MA"         << "|"
         << setw(NAME_WIDTH)    << "TEN PHIM"   << "|"
         << setw(GENRE_WIDTH)   << "THE LOAI"   << "|"
         << setw(TIME_WIDTH)    << "THOI LUONG" << "|"
         << setw(DATE_WIDTH)    << "NGAY CHIEU" << "|"
         << setw(STATUS_WIDTH)  << "TRANG THAI" << "|"
         << "\n";

    cout << string(totalWidth, '=') << "\n";

    for (int i = 0; i < n; i++) {

        cout << "|" << left
             << setw(ID_WIDTH)    << list[i].getIDfilm()   << "|"
             << setw(NAME_WIDTH)  << list[i].getNamefilm() << "|"
             << setw(GENRE_WIDTH) << list[i].getGenre()    << "|"
             << setw(TIME_WIDTH)  << list[i].getTime()     << "|"
             << setw(DATE_WIDTH)  << list[i].getShowdate()      << "|";

        if (MS.findFilmm(list[i].getIDfilm()))
            cout << setw(STATUS_WIDTH) << "CO SUAT" << "|\n";
        else 
            cout << setw(STATUS_WIDTH) << "KHONG CO SUAT" << "|\n";
    }

    cout << string(totalWidth, '=') << "\n";
}


Film* ManageFilm::getfilm(const string& id) {
    if (n == 0){
        cout << "Danh sach trong!\n";
        Logger::getInstance().warn("Tim phim nhung danh sach rong");
        return nullptr; 
    }
    for (int i = 0; i < n; ++i) {
        if (list[i].getIDfilm() == id) {
            return &list[i];
        }
    }
    cout << "Khong tim thay phim co ma " << id << endl;
    Logger::getInstance().warn("Khong tim thay phim: " + id);
    return nullptr;
}
