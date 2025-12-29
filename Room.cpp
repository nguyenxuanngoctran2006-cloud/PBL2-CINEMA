#include "Room.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

Room::Room() {}

Room::Room(string id, string name, int seats)
    : roomID(id), roomName(name), seatCount(seats) {}

string Room::getRoomName() {
    return roomName;
}

string Room::getRoomID() {
    return roomID;
}

int Room::getSeatCount() {
    return seatCount;
}
