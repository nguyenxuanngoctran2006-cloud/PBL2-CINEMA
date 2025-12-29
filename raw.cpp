#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <limits>
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#pragma execution_character_set("utf-8")
#ifdef byte
#undef byte
#endif

#include "User.h"
#include "Customer.h"
#include "Staff.h"
#include "ManageFilm.h"
#include "ManageTicket.h"
#include "ManageRoom.h"
#include "ManageShowtime.h"

using namespace std;

void inTieuDe() {
    const int width = 94;
    cout << string(width, '=') << "\n";

    cout << setw((width + 27) / 2) << right << "PBL2: DU AN CO SO LAP TRINH" << "\n";
    cout << setw((width + 33) / 2) << right << "DE TAI: UNG DUNG QUAN LY RAP CHIEU PHIM" << "\n";

    cout << string(width, '=') << "\n";

    cout << "| " << left << setw(31) << "Ho va Ten"
         << "| " << left << setw(25) << "MSSV"
         << "| " << left << setw(31) << "Lop hoc phan" << "|\n";
    cout << string(width, '=') << "\n";

    cout << "| " << left << setw(31) << "Hoang Bao Nguyen"
         << "| " << left << setw(25) << "102240385"
         << "| " << left << setw(31) << "99B" << "|\n";
    cout << string(width, '=') << "\n";

    cout << "| " << left << setw(31) << "Nguyen Xuan Ngoc Tran"
         << "| " << left << setw(25) << "102240405"
         << "| " << left << setw(31) << "99B" << "|\n";
    cout << string(width, '=') << "\n";
}


void showMenuOptions(string options[], int n, int highlight) {
    system("cls");
    inTieuDe();

    const int width = 94;   
    const int boxWidth = 50;   
    int startX = (width - boxWidth) / 2;


    cout << string(startX, ' ') << "+" << string(boxWidth - 2, '-') << "+\n";

    string title = "MENU";
    int leftPad = (boxWidth - 2 - (int)title.length()) / 2;
    int rightPad = (boxWidth - 2) - leftPad - (int)title.length();
    cout << string(startX, ' ') << "|" << string(leftPad, ' ') << title << string(rightPad, ' ') << "|\n";

    cout << string(startX, ' ') << "+" << string(boxWidth - 2, '-') << "+\n";

    for (int i = 0; i < n; i++) {
        string marker = (i == highlight) ? "-> " : "   ";
        string text = marker + options[i];
        if ((int)text.length() > boxWidth - 2) text = text.substr(0, boxWidth - 5) + "...";
        int padR = (boxWidth - 2) - (int)text.length();
        cout << string(startX, ' ') << "|" << text << string(padR, ' ') << "|\n";
    }

    cout << string(startX, ' ') << "+" << string(boxWidth - 2, '-') << "+\n";

    cout << string(startX, ' ') << "(Dung phim UP/DOWN de di chuyen - ENTER de chon)\n";
}


int getMenuChoice(string options[], int n){
    int highlight=0;
    char ch;
    while(true){
        showMenuOptions(options,n,highlight);
        ch=_getch();
        if(ch==-32){ 
            ch=_getch();
            if(ch==72) highlight--; 
            if(ch==80) highlight++; 
            if(highlight<0) highlight=n-1;
            if(highlight>=n) highlight=0;
        } else if(ch==13) { 
            return highlight+1;
        }
    }
}

int mainMenu(User* currentUser){
    if(currentUser==nullptr){
        string options[] = {"Dang ky tai khoan","Dang nhap","Thoat"};
        return getMenuChoice(options,3);
    } else{
        Customer* c = dynamic_cast<Customer*>(currentUser);
        Staff* s = dynamic_cast<Staff*>(currentUser);
        if(c!=nullptr){
            string options[]={"Dat ve xem phim","Xem lich chieu chi tiet","Xem phim yeu thich nhat","Huy ve","Dang xuat"};
            return getMenuChoice(options,5);
        } else if(s!=nullptr){
            string options[]={"Quan ly phim","Quan ly phong chieu","Quan ly suat chieu","Quan ly nguoi dung","Xem bao cao doanh thu","Dang xuat"};
            return getMenuChoice(options,6);
        }
    }
    return 0;
}

int main(){
    User* currentUser=nullptr;
    ManageFilm MF;
    ManageRoom MR;
    ManageShowtime MST;
    ManageTicket T;
    ManageCustomer MC;
    Bill* bills = new Bill[10000];
    int billCount=0;

    MF.readFile("Database/Film.txt");
    MR.readFile("Database/Room.txt");
    MST.readFile("Database/Showtime.txt",MF,MR);
    MC.readFile("Database/Customer.txt");
    T.readFile("Database/Ticket.txt",MST,MR,MF,MC);
    T.readBills(bills,billCount);
    inTieuDe();
    while(true){
        int choice = mainMenu(currentUser);
        if(currentUser==nullptr){
            switch(choice){
                case 1:{
                    User::registerUser(); 
                    cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                    break;

                }
                case 2:{
                    currentUser = User::login();
                    if(currentUser!=nullptr){
                        currentUser->ShowInfo();
                        cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                    } else{
                        cout<<"Ten dang nhap hoac mat khau khong dung.\n";
                        cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                    }
                    break;
                }
                case 3: 
                    cout<<"Cam on da su dung he thong. Tam biet!\n";
                    delete[] bills;
                    return 0;
            }
        } else{
            Customer* c = dynamic_cast<Customer*>(currentUser);
            Staff* s = dynamic_cast<Staff*>(currentUser);

            if(c!=nullptr){
                switch(choice){
                    case 1: {
                        T.Order(MF,MST,MR,*c);
                        cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                        break;
                    }
                    case 2: /*MF.displayfilm(MST);*/ MST.display(); cout<<"Nhan phim bat ky de tiep tuc..."; _getch(); break;
                    case 3: {
                        int m; cout<<"Nhap thang: "; cin>>m; cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        T.mostBookedFilmStatistics(m,MF);
                        cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                        break;
                    }
                    case 4: T.cancelTicket(c); cout<<"Nhan phim bat ky de tiep tuc..."; _getch(); break;
                    case 5: delete currentUser; currentUser=nullptr; break;
                }
            } else if(s!=nullptr){
                switch(choice){
                    case 1:{
                        int sub;
                        do{
                            string options[]={"Them phim","Xoa phim","Quay lai"};
                            sub = getMenuChoice(options,3);
                            cin.ignore(numeric_limits<streamsize>::max(),'\n');
                            if(sub==1){MF.displayfilm(MST); MF.addfilm(); cout<<"Phim da duoc them. Nhan phim bat ky de tiep tuc..."; _getch();}
                            else if(sub==2){MF.displayfilm(MST); MF.deletefilm(MST); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                        }while(sub!=3);
                        break;
                    }
                    case 2:{
                        int sub;
                        do{
                            string options[]={"Them phong chieu","Xoa phong chieu","Quay lai"};
                            sub = getMenuChoice(options,3);
                            if(sub==1){MR.showRoom(); MR.addRoom(); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                            else if(sub==2){MR.showRoom(); MR.deleteRoom(MST); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                        }while(sub!=3);
                        break;
                    }
                    case 3:{
                        int sub;
                        do{
                            string options[]={"Them suat chieu","Xoa suat chieu","Quay lai"};
                            sub = getMenuChoice(options,3);
                            if(sub==1){MF.displayfilm(MST);/*** MST.display(); */ MST.addShowtime(MF,MR); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                            else if(sub==2){/*MF.displayfilm(MST);*/ MST.display(); MST.deleteShowtime(); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                        }while(sub!=3);
                        break;
                    }
                    case 4:{
                        int sub;
                        do{
                            string options[]={"Them nguoi quan ly moi","Xoa nguoi dung","Quay lai"};
                            sub = getMenuChoice(options,3);
                            if(sub==1) { User::viewAllStaff(); Staff::addstaff(); }
                            else if(sub==2) { User::viewAllCustomer(); User::viewAllStaff(); User::delUser(T); cout<<"Nhan phim bat ky de tiep tuc..."; _getch();}
                        }while(sub!=3);
                        break;
                    }
                    case 5:{
                        int sub;
                        string year = Ticket::getCurrentTime();
                        int y = stoi(year.substr(6,4));
                        do{
                            string options[]={"Theo ngay","Theo thang","Theo nam","Quay lai"};
                            sub = getMenuChoice(options,4);
                            if(sub==1){
                                int d,m; cout<<"Ngay: "; cin>>d; cout<<"Thang: "; cin>>m;
                                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                                cout<<"Doanh thu: "<<T.revenueByDay(d,m,y,bills,billCount)<<endl;
                                cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                            }
                            else if(sub==2){
                                int m; cout<<"Thang: "; cin>>m; cin.ignore(numeric_limits<streamsize>::max(),'\n');
                                cout<<"Doanh thu: "<<T.revenueByMonth(m,y,bills,billCount)<<endl;
                                cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                            }
                            else if(sub==3){
                                cout<<"Doanh thu: "<<T.revenueByYear(y,bills,billCount)<<endl;
                                cout<<"Nhan phim bat ky de tiep tuc..."; _getch();
                            }
                        }while(sub!=4);
                        break;
                    }
                    case 6: delete currentUser; currentUser=nullptr; break;
                }
            }
        }
    }

    delete[] bills;
    return 0;
}

//g++ raw.cpp ManageFilm.cpp Film.cpp ManageShowtime.cpp Showtime.cpp ManageRoom.cpp Room.cpp Ticket.cpp ManageTicket.cpp Customer.cpp User.cpp Staff.cpp ManageCustomer.cpp -o raw.exe