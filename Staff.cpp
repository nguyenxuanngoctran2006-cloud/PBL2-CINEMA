#include <iostream>
#include "Staff.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <limits>
#include <iomanip>

Staff::Staff(const string &n, const string &u, const string &p, const string r, const string &id, const string &pnum) : User(n, u, p, r, pnum), id(id){}
void Staff::ShowInfo() {
    int width = 94;
    cout << "\n" << string(width, '=') << "\n";

    string title = "THONG TIN NHAN VIEN";
    cout << string((width - title.length())/2, ' ') << title << "\n";
    cout << string(width, '=') << "\n";

    cout << string((width - ("Ma nhan vien: " + id).length())/2, ' ') << "Ma nhan vien: " << id << "\n";
    cout << string((width - ("Ten nhan vien: " + name).length())/2, ' ') << "Ten nhan vien: " << name << "\n";
    cout << string((width - ("Username: " + username).length())/2, ' ') << "Username: " << username << "\n";
    cout << string((width - ("So dien thoai: " + pnumber).length())/2, ' ') << "So dien thoai: " << pnumber << "\n";

    cout << string(width, '=') << "\n\n";
}

void Staff::addstaff() {
    string n, u, p, r, id, pnum;
    cout << "Nhap ho va ten nhan vien hoac nhap '0' de thoat: ";
    if (cin.fail() || cin.peek() == '\n') { 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    getline(cin, n); 
    if (n == "0") return;
    cout << "Tao ten tai khoan: ";
    cin >> u; 
    cout << "Tao mat khau: ";
    cin >> p; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    r = "staff";

    while (true) {
        cout << "Hay nhap so dien thoai: ";
        cin >> pnum;
        if (User::isValidPhoneNumber(pnum)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        } else {
            cout << "Dinh dang so dien thoai khong hop le.\n";
            cout << "1. Nhap lai\n2. Huy bo\nLua chon: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 1;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (choice == 2) {
                cout << "Da huy bo nhap so dien thoai. Huy dang ky.\n";
                return;
            }
        }
    }

    string filename = "Database/Staff.txt";
    id = generateNewID(filename, "NV");
    savetofile(filename, n, u, p, r, id, pnum);

    cout << "Da them nhan vien thanh cong!\n";
}
