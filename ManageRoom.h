#pragma once
#include "Room.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ManageShowtime.h"

using namespace std;

class ManageRoom{
    private:
        Room list[3];
        int n;
    public:
        ManageRoom();
        void readFile(const string& filename);
        void saveFile(const string& filename);
        void addRoom();
        void showRoom();
        void deleteRoom(ManageShowtime& MS);
        Room* getRoom(const string &id);
};