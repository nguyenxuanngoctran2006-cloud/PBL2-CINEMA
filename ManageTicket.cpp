#include "ManageTicket.h"
#include "ManageFilm.h"
#include "ManageShowtime.h"
#include "Room.h"
#include "ManageRoom.h"
#include "Customer.h"
#include "ManageCustomer.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
ManageTicket::ManageTicket(){
    n = 0;
}

void ManageTicket::readFile(const string& filename, ManageShowtime& MST, ManageRoom& MR, ManageFilm& MF, ManageCustomer& MC) {
    fs::create_directories("Database");
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }
    MC.readFile("Database/Customer.txt");
    n = 0;
    string line;
    while (getline(fin, line) && n < MAX_TICKETS) {
        if (line.empty()) continue;

        stringstream ss(line);
        string cID, tID, fID, stID, rID, chID, time, count;
        getline(ss, cID, ';');
        getline(ss, tID, ';');
        getline(ss, fID, ';');
        getline(ss, stID, ';');
        getline(ss, rID, ';');
        getline(ss, count, ';');
        getline(ss, chID, ';');
        getline(ss, time);
        int cnt = stoi(count);
        Showtime* suatPtr = MST.getshowtimeByID(stID);
        Film* filmPtr = MF.getfilm(fID);
        Room* roomPtr = MR.getRoom(rID);
        Customer* cPtr = MC.getCustomer(cID);
        if (suatPtr == nullptr || filmPtr == nullptr || roomPtr == nullptr) {
            continue;
        }
        list[n++] = Ticket(tID, filmPtr, suatPtr, chID, roomPtr, cPtr, cnt, time);
        if (cPtr = nullptr) list[n++] = Ticket(tID, filmPtr, suatPtr, chID, roomPtr, cPtr, cnt, time);
    }    
    fin.close();
}

void ManageTicket::saveFile(const string &filename){
    fs::create_directories("Database/Bill");
    ofstream fout(filename);
    if (!fout.is_open()){
        cout << "Can not open file " << filename << " to write\n";
        return;
    }
    for (int i = 0; i < n; ++i){
        fout << list[i].getCustomer()->getid() << ";"
             << list[i].getTicketID() << ";"
             << list[i].getFilm()->getIDfilm() << ";"
             << list[i].getShowtime()->getShowID() << ";"
             << list[i].getRoom()->getRoomID() << ";"
             << list[i].getcount() << ";"
             << list[i].getChairID() << ";"
             << list[i].gettime() << '\n';
            }
    fout.close();
}

void ManageTicket::Order(ManageFilm& MF, ManageShowtime& MST, ManageRoom& MR, Customer& C){
    MF.displayfilm(MST);
    cout << "Nhap ma phim muon dat hoac nhap '0' de thoat: ";
    string idFilm, idsuat;
    cin >> idFilm;
    if (idFilm == "0") return;
    while(!MST.findFilm(idFilm)){
        cout << "Vui long nhap lai ma phim hoac nhap '0' de thoat: ";
        cin >> idFilm;
        if (idFilm == "0") return;
    }
    Film* f = MF.getfilm(idFilm);
    cout << "Nhap ma suat chieu muon xem: ";
    cin >> idsuat;

    Showtime* suatPtr = MST.getshowtimeByID(idsuat);
    Room* r = suatPtr->getRoom();

    suatPtr->loadSeatStatus(idsuat);
    suatPtr->display();

    cout << "\nNhap nhieu ghe cung luc cach nhau boi dau ',':\n";
    string line;
    cout << "Nhap: ";
    cin.ignore();
    getline(cin, line);
    int cnt = 0;

    stringstream ss(line);
    string ghe;
    string tmpghe = "";
    string danhSachGhe = "";

    while (getline(ss, ghe, ',')) { 
        stringstream gh(ghe);
        int cot;
        char hang;
        if (gh >> hang >> cot) {
            while (!suatPtr->bookSeat(hang, cot)) {
                cout << "Ghe [" << hang << "" << cot << "] da duoc dat hoac khong hop le!\n";
                cout << "Nhap lai ghe (hang cot) bi loi: ";
                string reGhe;
                getline(cin, reGhe);
                stringstream ghRe(reGhe);
                ghRe >> hang >> cot;
                tmpghe += (hang + cot);
            }
            cnt++;
            if (!danhSachGhe.empty()) {
                danhSachGhe += ",";
            }
            danhSachGhe += string(1, hang) + "-" + to_string(cot);
        }
    }
    cout << "\nTrang thai sau khi dat:\n";
    suatPtr->display();
    suatPtr->saveSeatStatus(idsuat);
    cout << "Ban da dat ve thanh cong! Cam on da su dung dich vu\n";

    ofstream fs("Database/Ticket.txt", ios::app);
    if (!fs.is_open()) {
        cout << "Khong mo duoc file Ticket.txt\n";
        Logger::getInstance().error("Khong mo duoc file Ticket.txt de luu ve!");
        return;
    }
    string ticketid = Ticket::generateTicketID("Database/Ticket.txt", "MV");
    Customer* cPtr = &C;
    string time = Ticket::getCurrentTime();
    Ticket newTicket(ticketid, f, suatPtr, danhSachGhe, r, cPtr, cnt, time);
    suatPtr->addTicket(&newTicket);
    list[n++] = newTicket;
    fs << cPtr->getid() << ";" << ticketid << ";" << idFilm << ";" << idsuat << ";" << r->getRoomID() << ";" << cnt << ";" << danhSachGhe << ";" << time << "\n";
    fs.close();
    newTicket.PrintTicket();
    Logger::getInstance().info("Khach " + cPtr->getid() + " da dat " + to_string(cnt) + " ghe (" + danhSachGhe + ")" + " cho phim " + idFilm + " tai suat " + idsuat + ".");
}

void ManageTicket::readBills(Bill* &bills, int &billCount) {
    billCount = 0;
    for (int m = 1; m <= 12; m++) {
        fs::create_directories("Database/Bill");
        string filename = "Database/Bill/Bill_" + to_string(m) + ".txt";
        ifstream fin(filename);
        if (!fin) continue;
        string check;
        while(getline(fin, check)){
            if (check.empty()) continue;
            stringstream ss(check);

            string id, name, moneyStr, date;
            getline(ss, id, ';');
            getline(ss, name, ';');
            getline(ss, moneyStr, ';');
            getline(ss, date);

            try {
                bills[billCount].ticketid = id;
                bills[billCount].money = stod(moneyStr);
                bills[billCount].day   = stoi(date.substr(0, 2));
                bills[billCount].month = stoi(date.substr(3, 2));
                bills[billCount].year  = stoi(date.substr(6, 4));
                
                bills[billCount].date = date;
                bills[billCount].name = name;
                billCount++;
                
            } catch (const std::invalid_argument& e) {
                cerr << "LỖI ĐỌC BILL: Dữ liệu dòng này không phải số (" << e.what() << "). Dòng:m " << check << endl;
                continue; 
            } catch (const std::out_of_range& e) {
                cerr << "LỖI ĐỌC BILL: Dữ liệu ngày tháng không đúng định dạng. Dòng: " << check << endl;
                continue; 
            }
        }
        fin.close();
        }
}

void ManageTicket::saveBills(Bill* &bills, int& billCount){
    fs::create_directories("Database/Bill");
    for (int m = 1; m <= 12; m++) {
        string filename = "Database/Bill/Bill_" + to_string(m) + ".txt";
        ofstream fout(filename);
        
        for (int i = 0; i < billCount; ++i){
            if (bills[i].month == m) {
                fout << bills[i].ticketid << ";" 
                     << bills[i].name << ";" 
                     << bills[i].money << ";" 
                     << bills[i].date << "\n";
            }
        }
        fout.close();
    }
}

void ManageTicket::deletebill(Bill* &bills, const string& id, int &billCount){
    if (billCount == 0){
        cout << "Danh sach trong!\n";
        return;
    } 
    bool found = false;
    string month = to_string(bills[0].month);
    do {
        for (int i = 0; i < billCount; i++){
            if (bills[i].ticketid == id){
                for (int j = i; j < billCount - 1; j++){
                    bills[j] = bills[j + 1];
                }      
                billCount--;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Khong tim thay bill co ma ve " << id << ". Vui long nhap lai!\n";
        }
    } while (!found);
    if (found) {
        saveBills(bills, billCount);
    }
}

string ManageTicket::revenueByDay(int d, int m, int y, Bill* bills, int billCount) {
    readBills(bills, billCount);
    double sum = 0;
    for (int i = 0; i < billCount; i++) {
        if (bills[i].day == d &&
            bills[i].month == m &&
            bills[i].year == y)
        {
            sum += bills[i].money;
        }
    }
    return formatMoney(sum);
}

string ManageTicket::revenueByMonth(int m, int y, Bill* bills, int billCount) {
    readBills(bills, billCount);
    double sum = 0;
    for (int i = 0; i < billCount; i++) {
        if (bills[i].month == m &&
            bills[i].year == y)
        {
            sum += bills[i].money;
        }
    }
    return formatMoney(sum);
}

string ManageTicket::revenueByYear(int y, Bill* bills, int billCount) {
    readBills(bills, billCount);
    double sum = 0;
    for (int i = 0; i < billCount; i++) {
        if (bills[i].year == y)
            sum += bills[i].money;
    }
    return formatMoney(sum);
}

string ManageTicket::formatMoney(double money) {
    string s = to_string((long long)money);
    int n = s.length();
    int count = 0;
    string res = "";

    for (int i = n-1; i >= 0; i--) {
        res = s[i] + res;
        count++;
        if (count % 3 == 0 && i != 0) res = "." + res;
    }
    return res + " VND";
}

void ManageTicket::mostBookedFilmStatistics(int month, ManageFilm& MF) {
    const int MAX_FILMS = 100;
    string filmIDs[MAX_FILMS];
    int counts[MAX_FILMS] = {0};
    int nFilms = 0;
    for (int i = 0; i < n; i++) { 
        string fID = list[i].getFilm()->getIDfilm();
        int cnt = list[i].getcount(); 
        string time = list[i].gettime();

        int t_month = stoi(time.substr(3, 2));
        if (t_month != month) continue;

        bool found = false;
        for (int j = 0; j < nFilms; j++) {
            if (filmIDs[j] == fID) {
                counts[j] += cnt;
                found = true;
                break;
            }
        }
        if (!found) {
            filmIDs[nFilms] = fID;
            counts[nFilms] = cnt;
            nFilms++;
        }
    }

    string maxFilmID = "";
    int maxCount = 0;
    for (int i = 0; i < nFilms; i++) {
        if (counts[i] > maxCount) {
            maxCount = counts[i];
            maxFilmID = filmIDs[i];
        }
    }
    if (maxFilmID == ""){
        cout << "Du lieu thang nay chua duoc cap nhat " << endl;
        Logger::getInstance().warn("Du lieu thang nay chua duoc cap nhat.");
        return;
    }
    Film* f = MF.getfilm(maxFilmID);
    Logger::getInstance().info("Phim dat nhieu nhat thang " + to_string(month) + " la " + f->getNamefilm() + " voi " + to_string(maxCount) + " ve.");
    if (f != nullptr) {
        cout << "Phim duoc dat nhieu nhat trong thang " << month << " la: " << f->getNamefilm()
             << " voi tong so ve: " << maxCount << endl;
    } else{
        cout << "Du lieu thang nay chua duoc cap nhat " << endl;
        Logger::getInstance().warn("Du lieu thang nay chua duoc cap nhat.");
    }
}

Ticket* ManageTicket::getTicketbyID(const string &id){
    if (n == 0) {
        cout << "Danh sach trong!\n";
        return nullptr; 
    }
    for (int i = 0; i < n; ++i) {
        if (list[i].getTicketID() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

void ManageTicket::deleteticket(const string &id){
    if (n == 0){
        cout << "Danh sach trong!\n";
        return;
    } 
    string ID;
    bool found = false;
    do {
        for (int i = 0; i < n; i++){
            if (list[i].getTicketID() == id){
                for (int j = i; j < n - 1; j++){
                    list[j] = list[j + 1];
                }      
                n--;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Khong tim thay ve co ma " << id << ". Vui long nhap lai!\n";
            Logger::getInstance().warn("Khong tim thay ve co ma " + id + " de xoa.");
        }
    } while (!found);
    if (found) {
        saveFile("Database/Ticket.txt");
    }
}

bool ManageTicket::isCancel(const string& day, const string& time) {
    string now = Ticket::getCurrentTime();
    string showtimeDay = day;
    replace(showtimeDay.begin(), showtimeDay.end(), '-', '/');
    string today = now.substr(0, 10);
    string curTime = now.substr(11, 5);
    int d1, m1, y1;
    int h, m; 
    sscanf(showtimeDay.c_str(), "%d/%d/%d", &d1, &m1, &y1);
    sscanf(time.c_str(), "%d:%d", &h, &m);
    if (ManageShowtime::isPastDateTime(showtimeDay, time)) {
        return false;
    }
    int now_d, now_m, now_y;
    int now_H, now_M;
    sscanf(today.c_str(), "%d/%d/%d", &now_d, &now_m, &now_y);
    sscanf(curTime.c_str(), "%d:%d", &now_H, &now_M);
    if (d1 == now_d && m1 == now_m && y1 == now_y) {
        long long stinminute = h * 60 + m;
        long long nowinminute = now_H * 60 + now_M;    
        long long diff = stinminute - nowinminute;
        return diff >= 30;
    }
    return true;
}

void ManageTicket::display(Customer *c){
    if (n == 0) {
        cout << "Khong co ve nao!\n";
        return;
    }

    const int STT_WIDTH = 4;     
    const int MAVE_WIDTH = 10;    
    const int MASUAT_WIDTH = 10;  
    const int IDFILM_WIDTH = 10;   
    const int MAPHONG_WIDTH = 10;  
    const int NGAYGIO_WIDTH = 25;  
    const int GIA_WIDTH = 11;     
    const int CHAIRCOUNT_WIDTH = 6; 
    int totalWidth = 4 + 10 + 10 + 10 + 10 + 25 + 11 + 6 + 9; 
    Logger::getInstance().info("Hien thi danh sach thong tin hoa don");

    cout << "\n" << string(totalWidth, '=') << endl;
    string title = "DANH SACH THONG TIN HOA DON";
    int padding = (totalWidth - title.length()) / 2;
    cout << string(padding, ' ') << title << endl;
    cout << string(totalWidth, '=') << endl;

    cout << "|" << left
         << setw(STT_WIDTH) << "STT" << "|"
         << setw(MAVE_WIDTH) << "MA VE" << "|"
         << setw(MASUAT_WIDTH) << "MA SUAT" << "|"
         << setw(IDFILM_WIDTH) << "MA PHIM" << "|"
         << setw(MAPHONG_WIDTH) << "MA PHONG" << "|"
         << setw(CHAIRCOUNT_WIDTH) << "SO VE" << "|"
         << setw(NGAYGIO_WIDTH) << "NGAY GIO CHIEU" << "|"
         << setw(GIA_WIDTH) << "TONG TIEN" << "|"
         << endl;

    cout << string(totalWidth, '=') << endl;

    int stt = 1;
    for (int i = 0; i < n; i++) {
        if (c->getid() == list[i].getCustomer()->getid()){
            string day = list[i].getShowtime()->getDate();
            string time = list[i].getShowtime()->getTime();
            if (!ManageShowtime::isPastDateTime(day, time)){
                double totalPayment = list[i].getShowtime()->getPrice() * list[i].getcount();
                string dateTime = list[i].getShowtime()->getDate() + " " + list[i].getShowtime()->getTime();

                cout << "|" << left
                    << setw(STT_WIDTH) << stt << "|"
                    << setw(MAVE_WIDTH) << list[i].getTicketID() << "|"
                    << setw(MASUAT_WIDTH) << list[i].getShowtime()->getShowID() << "|"
                    << setw(IDFILM_WIDTH) << list[i].getShowtime()->getFilm()->getIDfilm() << "|"
                    << setw(MAPHONG_WIDTH) << list[i].getShowtime()->getRoom()->getRoomID() << "|"
                    << setw(CHAIRCOUNT_WIDTH) << list[i].getcount() << "|"
                    << setw(NGAYGIO_WIDTH) << dateTime << "|"
                    << right << setw(GIA_WIDTH) << fixed << setprecision(0) << totalPayment << left << "|"
                    << endl;
                ++stt;
            }
        } 
    }

    if (stt == 1) {
        cout << "Khong co ve nao duoc tim thay cho tai khoan nay.\n";
    }
    cout << string(totalWidth, '=') << endl;
}

void ManageTicket::delcustomerticket(const string& customerid) {
    Bill* bills = new Bill[10000]; 
    int billCount = 0;
    readBills(bills, billCount); 
    for (int i = n - 1; i >= 0; i--) { 
        if (customerid == list[i].getCustomer()->getid()) {
            string eraseid = list[i].getTicketID(); 
            Ticket* ticketToCancel = getTicketbyID(eraseid);

            if (ticketToCancel == nullptr) {
                continue; 
            }
            Showtime* showtime = ticketToCancel->getShowtime();
            string stday = showtime->getDate();
            string sttime = showtime->getTime();
            if (isCancel(stday, sttime)) {
                showtime->loadSeatStatus(showtime->getShowID());
                string chair = ticketToCancel->getChairID();
                std::replace(chair.begin(), chair.end(), ',', ' ');
                stringstream ss(chair);
                string onechair;
                while (ss >> onechair) {
                    size_t pos = onechair.find('-');
                    if (pos != string::npos) {
                        char hang = onechair[0];
                        try {
                            int cot_num = stoi(onechair.substr(pos + 1));
                            showtime->cancelSeat(hang, cot_num);
                        } catch (...) {}
                    }
                }
                showtime->saveSeatStatus(showtime->getShowID());
                deletebill(bills, eraseid, billCount);
                deleteticket(ticketToCancel->getTicketID());

                Logger::getInstance().info("Khach hang " + customerid + " da bi huy ve vi xoa tai khoan: " + eraseid);
            } else {
                Logger::getInstance().warn("Ve " + eraseid + " khong the bi huy vi da het han (trong qua trinh xoa user)");
            }
        }
    }
    delete[] bills; 
}

void ManageTicket::cancelTicket(Customer *c){
    display(c);

    cout << "\n" << string(94, '-') << "\n";
    cout << "Nhap ma ve muon huy hoac nhap '0' de thoat: "; 
    string eraseid; 
    cin >> eraseid;
    if (eraseid == "0") return;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Ticket* ticketToCancel = getTicketbyID(eraseid);
    if (ticketToCancel == nullptr){
        cout << "Ve khong ton tai. Vui long kiem tra lai!\n";
        Logger::getInstance().warn("Khach hang " + c->getid() + " thu huy ve khong ton tai: " + eraseid);
        return;
    }

    Showtime* showtime = ticketToCancel->getShowtime();
    string stday = showtime->getDate();
    string sttime = showtime->getTime();
    if (ManageTicket::isCancel(stday, sttime)){
        showtime->loadSeatStatus(showtime->getShowID());
        string chair = ticketToCancel->getChairID();
        std::replace(chair.begin(), chair.end(), ',', ' ');
        stringstream ss(chair);
        string onechair;
        while (ss >> onechair){
            size_t pos = onechair.find('-');
            char hang = onechair[0];
            int cot_num = stoi(onechair.substr(pos + 1));
            showtime->cancelSeat(hang, cot_num);
        }
        showtime->saveSeatStatus(showtime->getShowID());
        deleteticket(ticketToCancel->getTicketID());

        cout << "\n" << string(94, '=') << "\n";
        cout << "Huy ve (Ma: " << eraseid << ") thanh cong!\n";
        cout << string(94, '=') << "\n";

        Logger::getInstance().info("Khach hang " + c->getid() + " da huy ve: " + eraseid);

        Bill* bills = new Bill[10000];
        int billCount;
        readBills(bills, billCount);
        deletebill(bills, eraseid, billCount);
        delete[] bills;
    } else {
        cout << "\n" << string(94, '-') << "\n";
        cout << "Ve khong the huy! Vi suat chieu chi con duoi 30 phut hoac da qua.\n";
        cout << string(94, '-') << "\n";
        Logger::getInstance().warn("Khach hang " + c->getid() + " khong the huy ve: " + eraseid);
    }
}