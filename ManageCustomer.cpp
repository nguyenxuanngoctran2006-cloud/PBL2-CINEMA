#include "ManageCustomer.h"
#include <fstream>
#include <sstream>
#include <limits> 
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

ManageCustomer::ManageCustomer(){
    n = 0;
}

Customer* ManageCustomer::getCustomer(const string& id) {
    for (int i = 0; i < n; ++i) {
        if (list[i].getid() == id) { 
            return &list[i]; 
        }
    }
    return nullptr;
}

void ManageCustomer::readFile(const string& filename) {
    fs::create_directories("Database");
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Canh bao: Khong the mo file " << filename << ". Danh sach Customer se rong.\n";
        return;
    }
    
    n = 0;
    std::string line;
    while (getline(fin, line) && n < MAX_CUSTOMERS) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string name, user, pass, role, id, pnum;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, user, ';'); 
        getline(ss, pass, ';');
        getline(ss, role, ';'); 
        getline(ss, pnum);
        list[n++] = Customer(name, user, pass, role, id, pnum);
    }
    fin.close();
}