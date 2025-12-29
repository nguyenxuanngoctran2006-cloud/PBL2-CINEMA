#pragma once
#include "User.h"
#include <string>

using namespace std;

class Staff : public User{
    private: 
        string id;
    public:
        Staff(const string &n, const string &u, const string &p, const string r, const string &id, const string &pnum);
        void ShowInfo() override;
        static void addstaff();
};