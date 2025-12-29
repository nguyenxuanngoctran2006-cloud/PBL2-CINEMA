#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "User.h"
#include "Logger.h"
#include "Customer.h"
#include "Staff.h"
#include "ManageTicket.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

User::User(const string &n, const string &u, const string &p, const string &r, const string &pnum) : name(n), username(u), password(p), role(r), pnumber(pnum) {}

void User::savetofile(string filename, const string &n, const string &u, const string &p, string &r, string &id, const string &pnum){
    if (!fs::exists("Database")) {
        fs::create_directories("Database");
    }
    ofstream fout(filename, ios::app);
    if (fout.is_open()) {
        fout << id << ";" << n << ";" << u << ";" << p << ";" << r << ";"<< pnum << "\n";
        fout.close();
    }
}

string User::getName(){
    return name;
}

string User::getRole(){
    return role;
}
string User::getNumber(){
    return pnumber;
}

bool User::isValidPhoneNumber(const string& s) {\
    if (s.length() != 10) {
        return false;
    }
    if (s[0] != '0') {
        return false;
    }
    for (char const &c : s) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void User::registerUser() {
    string n, u, p, r, filename, id, pnum;
    cout << "Dang ky tai khoan:" << endl;

    cout << "Hay nhap ten nguoi dung: ";
    getline(cin, n);

    cout << "Hay nhap ten tai khoan: ";
    cin >> u;

    cout << "Hay nhap mat khau: ";
    cin >> p;

    while (true){
        cout << "Hay nhap so dien thoai: ";
        cin >> pnum;
        if (User::isValidPhoneNumber(pnum)) {
        break; 
        } else {
            cout << "Dinh dang so dien thoai khong hop le.\n";
            cout << "1. Nhap lai\n2. Huy bo\nLua chon: ";
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 1;
            }else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (choice == 2) {
                pnum = "";
                cout << "Da huy bo nhap so dien thoai. Huy dang ky.\n";
                return; 
            }
        }
    }
    r = "customer";
    filename = "Database/Customer.txt";
    id = generateNewID(filename, "KH");
    savetofile(filename, n, u, p, r, id, pnum);
    cout << "Dang ky thanh cong!" << endl;
    Logger::getInstance().info("Dang ky tai khoan moi: " + id + " - " + n);
}


bool User::checkFileLogin(const string &filename, const string &u, const string &p, string &foundrole, string &foundname, string &foundid, string &foundnum){
    ifstream fs(filename);
    if (!fs.is_open()){
        return false; //không mở được file
    }
    string check;
    while (getline(fs, check)){   
        if (check.empty()) continue;//bỏ qua dòng trống
        stringstream ss(check);

        string name, user, pass, role, id, pnum;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, user, ';'); 
        getline(ss, pass, ';');
        getline(ss, role, ';'); 
        getline(ss, pnum);

        if (user == u && pass == p){
                foundname = name;
                foundrole = role;
                foundid = id;  
                foundnum = pnum;
                fs.close();
                return true;
        }
    }
        fs.close();
        return false;

} 

User* User::login(){ 
    string u, p, foundrole, foundname, foundid, foundnum;
    cout << "Dang nhap tai khoan: " << endl;
    bool check = false;
    while(!check){
        cout << "Hay nhap ten tai khoan: ";
        cin >> u;
        cout << "Hay nhap mat khau: ";
        cin >> p;
        if (!fs::exists("Database")) {
        fs::create_directories("Database");
        }
        if (checkFileLogin("Database/Customer.txt", u, p, foundrole, foundname, foundid, foundnum)){
            cout << "Dang nhap thanh cong !" << endl;
            Logger::getInstance().info("Khach hang dang nhap: " + foundid + " - " + foundname);
            cout << "Chao mung khach hang " << foundname << " den voi he thong !" << endl;
            check = true;
            return new Customer(foundname, u, p, foundrole, foundid, foundnum);
        }   
        else if (checkFileLogin("Database/Staff.txt", u, p, foundrole, foundname, foundid, foundnum)){
            cout << "Dang nhap thanh cong !" << endl;
            Logger::getInstance().info("Nhan vien dang nhap: " + foundid + " - " + foundname);
            cout << "Chao mung nhan vien " << foundname << " den voi he thong !" << endl;
            check = true;
            return new Staff(foundname, u, p, foundrole, foundid, foundnum);
        } 
        else {
            cout << "Dang nhap that bai!" << endl << "Tiep tuc (1)" << endl << "Thoat (0)" << endl;
            int check1;
            cin >> check1;
            if (check1 == 0) check = true;
            else continue;
        }
    }
    return nullptr;
}

void User::erasefileLine(string path, string eraseline){
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()){
       cout << "Loi : Khong the mo file " << path << " de doc" << '\n'; 
       return;
    }
    ofstream temp;
    temp.open("Database/temp.txt");
    string line;
    bool firstLine = true;
    while(getline(fin, line)){
        if (!line.empty() && line!= eraseline){
            if (!firstLine){
                temp << '\n';
            }
            temp << line;
            firstLine = false;
        }
    }

    temp.close();
    fin.close();

    const char *p = path.c_str();
    if (remove(p) != 0){
        cout << "Khong xoa duoc file goc" << '\n';
    }
    if (rename("Database/temp.txt", p) != 0){
        cout << "Khong the doi ten file tam thoi " << '\n';
    }
}

void User::delUser(ManageTicket& T){
    string id, filename;
    int res;
    cout << "Nhap loai nguoi dung ban muon xoa hoac nhap '0' de thoat:\n";
    cout << "1. Khach hang\n2. Nhan vien\nLua chon: ";
    cin >> res;
    if (res == 0) return;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    switch (res) {
        case 1: filename = "Database/Customer.txt"; break;
        case 2: filename = "Database/Staff.txt"; break;
        default: 
            cout << "Lua chon khong hop le!\n";
            return;
    }
    if (!fs::exists(filename)) {
        cout << "Du lieu khong ton tai (File chua duoc tao)!\n";
        return;
    }
    cout << "Nhap ID nguoi dung can xoa: ";
    getline(cin, id);
    ifstream fin(filename);
    if (filename == "Database/Customer.txt"){
        T.delcustomerticket(id);
    }
    ofstream temp("Database/temp.txt");
    if (!fin.is_open() || !temp.is_open()) {
        cout << "Loi: Khong the mo file database!\n";
        return;
    }
    string line;
    bool found = false;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t pos = line.find(';');
        if (pos != string::npos) {
            string currentID = line.substr(0, pos);
            if (currentID == id) {
                found = true;
            } else {
                temp << line << '\n';
            }
        } else {
            temp << line << '\n';
        }
    }
    fin.close();
    temp.close();

    if (remove(filename.c_str()) != 0) {
        cout << "Loi xoa file cu!\n";
    } else if (rename("Database/temp.txt", filename.c_str()) != 0) {
        cout << "Loi doi ten file moi!\n";
    } else {
        if (found) {
            cout << "Da xoa thanh cong nguoi dung co ID: " << id << "\n";
            Logger::getInstance().info("Da xoa nguoi dung co ID: " + id);
        } else {
            cout << "Khong tim thay nguoi dung co ID: " << id << "\n";
        }
    }
}


string User::generateNewID(const string &filename, const string &prefix){
    fs::create_directories("Database");
    ifstream fs(filename);
    if (!fs.is_open()){
        return prefix + "001";
    }

    int maxID = 0;
    string line;
    
    while(getline(fs, line)){
        if (line.empty()) continue;
        
        stringstream ss(line);
        string currentID;
        getline(ss, currentID, ';'); 
        if (currentID.rfind(prefix, 0) == 0){
            try {
                string numStr = currentID.substr(prefix.length());
                int num = std::stoi(numStr);
                
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
    string newID = newIDss.str();
    Logger::getInstance().info("Tao ID moi: " + newID);
    return newID;
}

User* User::findbyID(){
    string target, filename;
    int res;
    cout << "Nhap thong tin nguoi dung ban muon tim kiem :\n";
    cout << "1. Khach hang" << endl;
    cout << "2. Nhan vien" << endl;
    cin >> res;

    if (res == 1){
        cout << "Nhap id khach hang ban muon tim kiem: ";
        cin >> target;
        filename = "Database/Customer.txt";
        ifstream fs(filename);
        if (!fs.is_open()){
            return nullptr;
        }
        string line;
        while(getline(fs, line)){
            if (line.empty()) continue;
            stringstream ss(line);
            string name, user, pass, role, id, pnum;
            getline(ss, id, ';');
            getline(ss, name, ';');
            getline(ss, user, ';');
            getline(ss, pass, ';');
            getline(ss, role, ';');
            getline(ss, pnum);
            if (id == target){
                return new Customer(name, user, pass, role, id, pnum);
            }
        }
        cout << "Khong tim thay khach hang co ma la: " << target << endl;
        return nullptr;
    } else if (res == 2){
        cout << "Nhap id nhan vien ban muon tim kiem: ";
        cin >> target;
        filename = "Database/Staff.txt";
        ifstream fs(filename);
        if (!fs.is_open()){
            return nullptr;
        }
        string line;
        while(getline(fs, line)){
            if (line.empty()) continue;
            stringstream ss(line);
            string name, user, pass, role, id, pnum;
            getline(ss, id, ';');
            getline(ss, name, ';');
            getline(ss, user, ';');
            getline(ss, pass, ';');
            getline(ss, role, ';');
            getline(ss, pnum);
            if (id == target){
                return new Staff(name, user, pass, role, id, pnum);
            }
        }
        cout << "Khong tim thay nhan vien co ma la: " << target << endl;
        return nullptr;
    } else {
        cout << "Khong the tim nguoi dung khong co vai tro !" << endl;
        return nullptr;
    }
}

void User::viewAllCustomer() {
    ifstream fin("Database/Customer.txt");
    if (!fin.is_open()) {
    cout << "Khong the mo file Customer.txt\n";
    return;
    }
    Logger::getInstance().info("Xem danh sach khach hang");

    int width = 94;
    cout << "\n" << string(width, '=') << "\n";
    string title = "DANH SACH KHACH HANG";
    cout << string((width - title.length())/2, ' ') << title << "\n";
    cout << string(width, '=') << "\n";

    cout << "| " << left << setw(8)  << "Ma KH"
        << "| " << left << setw(28) << "Ten KH"
        << "| " << left << setw(23) << "Username"
        << "| " << left << setw(26) << "So dien thoai"
        << "|\n";
    cout << string(width, '=') << "\n";

    string line;
    bool empty = true;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        empty = false;

        stringstream ss(line);
        string id, name, user, pass, role, phone;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, user, ';');
        getline(ss, pass, ';');
        getline(ss, role, ';');
        getline(ss, phone, ';');

        cout << "| " << left << setw(8)  << id
            << "| " << left << setw(28) << name
            << "| " << left << setw(23) << user
            << "| " << left << setw(26) << phone
            << "|\n";
    }

    if (empty) {
        string msg = "Khong co du lieu khach hang.";
        cout << "|" << string((width - msg.length())/2, ' ')
            << msg
            << string((width - msg.length() + 1)/2, ' ')
            << "|\n";
    }

    cout << string(width, '=') << "\n";
    fin.close();
}

void User::viewAllStaff() {
    ifstream fin("Database/Staff.txt");
    if (!fin.is_open()) {
    cout << "Khong the mo file Staff.txt\n";
    return;
    }
    Logger::getInstance().info("Xem danh sach nhan vien");
    int width = 94;
    cout << "\n" << string(width, '=') << "\n";
    string title = "DANH SACH NHAN VIEN";
    cout << string((width - title.length())/2, ' ') << title << "\n";
    cout << string(width, '=') << "\n";

    cout << "| " << left << setw(8)  << "Ma NV"
        << "| " << left << setw(28) << "Ten NV"
        << "| " << left << setw(23) << "Username"
        << "| " << left << setw(26) << "So dien thoai"
        << "|\n";
    cout << string(width, '=') << "\n";

    string line;
    bool empty = true;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        empty = false;

        stringstream ss(line);
        string id, name, user, pass, role, phone;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, user, ';');
        getline(ss, pass, ';');
        getline(ss, role, ';');
        getline(ss, phone, ';');

        cout << "| " << left << setw(8)  << id
            << "| " << left << setw(28) << name
            << "| " << left << setw(23) << user
            << "| " << left << setw(26) << phone
            << "|\n";
    }

    if (empty) {
        string msg = "Khong co du lieu nhan vien.";
        cout << "|" << string((width - msg.length())/2, ' ')
            << msg
            << string((width - msg.length() + 1)/2, ' ')
            << "|\n";
    }

    cout << string(width, '=') << "\n";
    fin.close();
}