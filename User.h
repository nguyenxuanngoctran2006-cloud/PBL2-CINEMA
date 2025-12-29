#pragma once
#include <iostream>
#include <string>

using namespace std;

class ManageTicket;

class User{
    protected:
        string name, username, password, role, pnumber;
    public:
        User(const string &n = "", const string &u = "", const string &p = "", const string &r = "", const string &pnum = "");
        virtual void ShowInfo() = 0; 
        virtual ~User(){} 
        string getRole();
        string getName();
        string getNumber();
        static bool isValidPhoneNumber(const string& s);
        static bool checkFileLogin(const string &filename, const string &u, const string &p, string &foundrole, string &foundname, string &foundid, string &foundnum);  
        static void savetofile(string filename, const string &n, const string &u, const string &p, string &r, string &id, const string &pnum);
        static void registerUser();
        static User* login();
        static void erasefileLine(string path, string eraseline);
        static void delUser(ManageTicket& T);
        static string generateNewID(const string &filename, const string &prefix);
        static User* findbyID();
        static void viewAllCustomer();
        static void viewAllStaff();
};
