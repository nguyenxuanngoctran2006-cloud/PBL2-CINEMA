#pragma once
#include "Customer.h"
#include <string>
#include <iostream>
using namespace std;

const int MAX_CUSTOMERS = 1000; 

class ManageCustomer {
private:
    Customer list[MAX_CUSTOMERS];
    int n;

public:
    ManageCustomer();
    void readFile(const string& filename);
    Customer* getCustomer(const string& id);
    int getSoLuong() const { return n; } 
};