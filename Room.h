#pragma once
#include <iostream>
using namespace std;

class Ticket;

class Room {
private:
    string roomID;
    string roomName;
    int seatCount;

public:
    Room();
    Room(string id, string name, int seats);

    string getRoomName();
    string getRoomID();
    int getSeatCount();
};
