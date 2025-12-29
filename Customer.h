#pragma once
#include <iostream>
#include "User.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

using namespace std;

class Customer : public User{
    private: 
        string id;
    public:
        Customer();    
        Customer(const string &n, const string &u, const string &p, const string r, const string &id, const string &pnum);
        void ShowInfo() override;
        string getid();
};

