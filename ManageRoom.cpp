#include "ManageRoom.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
using namespace std;


namespace fs = std::filesystem;
ManageRoom::ManageRoom(){
    n = 0;
}

void ManageRoom::readFile(const string& filename){
    fs::create_directories("Database");
    ifstream fin(filename);
    if (!fin.is_open()){
        cout << "Can not open file " << filename << "\n";
        return;
    }
    n = 0;
    string line;
    while (getline(fin, line) && n < 3){
        if (line.empty()) continue;
        stringstream ss(line);
        string maphong, tenphong;
        int sl;
        getline(ss, maphong, ';');
        getline(ss, tenphong, ';');
        ss >> sl;

        list[n++] = Room(maphong, tenphong, sl);
    }
    fin.close();
}

void ManageRoom::saveFile(const string &filename){
    fs::create_directories("Database");
    ofstream fout(filename);
    if (!fout.is_open()){
        cout << "Can not open file " << filename << "to write\n";
        return;
    }
    for (int i = 0; i < n; ++i){
        fout << list[i].getRoomID() << ";"
             << list[i].getRoomName() << ";"
             << list[i].getSeatCount() << "\n";
    }
    fout.close();
}

void ManageRoom::showRoom() {
    if (n == 0) {
        cout << "Khong co phong chieu nao!\n";
        Logger::getInstance().warn("Hien thi phong - danh sach rong");
        return;
    }
        Logger::getInstance().info("Hien thi danh sach phong");

    const int MAPHONG_WIDTH = 28;
    const int TENPHONG_WIDTH = 33;
    const int SOGHE_WIDTH = 33;  
    int totalWidth = MAPHONG_WIDTH + TENPHONG_WIDTH + SOGHE_WIDTH + 1; 
    cout << "\n" << string(totalWidth, '=') << endl;
    cout << "                          DANH SACH PHONG CHIEU\n";
    cout << string(totalWidth, '=') << endl; 
    cout << "|" << left 
         << setw(MAPHONG_WIDTH - 1) << "MA PHONG" << "|"
         << setw(TENPHONG_WIDTH - 1) << "TEN PHONG" << "|"
         << setw(SOGHE_WIDTH - 1) << "SO GHE" << "|"
         << endl;
         
    cout << string(totalWidth, '=') << endl; 
    for (int i = 0; i < n; i++) {
        cout << "|" << left 
             << setw(MAPHONG_WIDTH - 1) << list[i].getRoomID() << "|"
             << setw(TENPHONG_WIDTH - 1) << list[i].getRoomName() << "|"
             << setw(SOGHE_WIDTH - 1) << list[i].getSeatCount() << "|"
             << endl;
    }
    cout << string(totalWidth, '=') << endl;
}

void ManageRoom::addRoom(){
    if (n == 3){
        cout << "So luong phong da day!\n";
        Logger::getInstance().warn("Khong the them phong moi - Danh sach phong da day");
        return;
    }
    string name, id;
    int count;
    cout << "Nhap thong tin phong chieu: " << endl << "Ma phong chieu: ";
    cin >> id;
    cout << "Ten phong chieu: ";
    cin >> name;
    cout << "So luong ghe: ";
    cin >> count;
    list[n++] = Room(id, name, count);
    saveFile("Database/Room.txt");
    Logger::getInstance().info("Them phong " + id + "  Ten: " + name + "  Ghe: " + to_string(count));
    cout << "Them phong thanh cong!";
}

void ManageRoom::deleteRoom(ManageShowtime& MS){
    if (n == 0){
        cout << "Danh sach phong trong!\n";
        Logger::getInstance().warn("Xoa phong that bai - danh sach phong trong");
        return;
    }
    string ID;
    bool found = false;
    while(true){
        cout << "Nhap ma phong de xoa hoac nhap '0' de thoat: ";
        getline(cin, ID);
        if (ID == "0") return;
        for (int i = 0; i < n; ++i){
            if (list[i].getRoomID() == ID){
                if (MS.findRoom(ID)){
                    cout << "Khong the xoa phong vi van con suat chieu! ";
                    Logger::getInstance().warn("Khong the xoa phong vi van con suat chieu, ma phong: " + ID);
                    return;
                }
                else{
                    MS.deleteShowtimebyRoom(list[i]);
                    for (int j = i; j < n - 1; ++j){ 
                        list[j] = list[j + 1];
                    }
                    n--; 
                    Logger::getInstance().warn("Xoa phong " + ID + " Da xoa cac suat chieu lien quan");
                    cout << "Da xoa phong chieu co ma: " << ID << endl;
                    found = true;
                    break;
                }
            }
        }
        
        if (found){
            saveFile("Database/Room.txt");
            break;
        }
        else {
            cout << "Khong tim thay phong chieu co ma " << ID << ". Vui long nhap lai\n";
            Logger::getInstance().error("Khong tim thay phong co ma " + ID);
        }
    }
}

Room* ManageRoom::getRoom(const string& id) {
    if (n == 0){
        cout << "Danh sach trong!\n";
        Logger::getInstance().warn("Tim phong nhung danh sach rong");
        return nullptr; 
    }

    for (int i = 0; i < n; ++i) {
        if (list[i].getRoomID() == id) {
            return &list[i];
        }
    }
    cout << "Khong tim thay phong chieu co ma " << id << endl;
    Logger::getInstance().error("Khong tim thay phong chieu " + id);
    return nullptr;
}

