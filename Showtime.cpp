#include "Film.h"
#include "Room.h"       
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "Showtime.h"      
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

Showtime::Showtime() {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 10; j++)
            seatStatus[i][j] = '0';
    ticketCount = 0;
}

Showtime::Showtime(const string &showID, Film* fPtr, Room* rPtr,
                   const string &date, const string &time,
                   const double &price, const int &count)
    : showID(showID), film(fPtr), room(rPtr),
      showDate(date), showTime(time), ticketPrice(price), ticketCount(0)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 10; j++)
            seatStatus[i][j] = '0';
}

string Showtime::getDate() {
    return showDate;
}

double Showtime::getPrice() {
    return ticketPrice;
}

string Showtime::getTime() {
    return showTime;
}

Film* Showtime::getFilm() {
    return film;
}

string Showtime::getShowID() {
    return showID;
}

Room* Showtime::getRoom() {
    return room;
}

void Showtime::addTicket(Ticket* ticket) {
    if (ticketCount < 50) {
        tickets[ticketCount++] = ticket;
    }
}

int Showtime::getTicketCount() {
    return ticketCount;
}

bool Showtime::checkseat(){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 10; j++){
            if (seatStatus[i][j] == 'X') return false;
        }
    }
    return true;
}

bool Showtime::isFull() {
    if (ticketCount == 50) {
        cout << "The showtime is full! Please select another show.\n";
        return true;
    }
    return false;
}

Ticket* Showtime::getTicket(int i) {
    if (i >= 0 && i < ticketCount)
        return tickets[i];
    return nullptr;
}

bool Showtime::isWeekend(const string &date) {
    int d, m, y;

    if (sscanf(date.c_str(), "%d-%d-%d", &d, &m, &y) != 3)
        return false;

    tm time_in = { 0, 0, 0, d, m - 1, y - 1900 };
    time_t time_temp = mktime(&time_in);
    tm *time_out = localtime(&time_temp);

    int weekday = time_out->tm_wday;
    return (weekday == 0 || weekday == 6);
}

string Showtime::generateNewID(const string &filename, const string &prefix) {
    ifstream fs(filename);
    if (!fs.is_open()) {
        return prefix + "001";
    }

    int maxID = 0;
    string line;

    while (getline(fs, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id;
        getline(ss, id, ';');

        if (id.rfind(prefix, 0) == 0) {
            try {
                string numStr = id.substr(prefix.length());
                int num = stoi(numStr);
                if (num > maxID) maxID = num;
            } catch (...) {}
        }
    }

    fs.close();
    maxID++;

    stringstream newID;
    newID << prefix << setw(3) << setfill('0') << maxID;

    return newID.str();
}

double Showtime::calculatePriceByDate(const string& date) {
    Showtime tmp;
    return tmp.isWeekend(date) ? 100000.0 : 80000.0;
}

void Showtime::display() {
    const int COLS = 10;
    const int ROWS = 5;
    const int totalWidth = 94;
    const int SEAT_WIDTH = 3;
    int spaceBetween = (totalWidth - 4 - (COLS * SEAT_WIDTH)) / (COLS - 1);
    if (spaceBetween < 1) spaceBetween = 1;

    string space(spaceBetween, ' ');
    string title = "SO DO GHE";
    cout << "\n" << string(totalWidth, '=') << "\n";
    cout << string((totalWidth - title.length()) / 2, ' ') << title << "\n";
    cout << string(totalWidth, '=') << "\n";
    cout << "    ";
    for (int j = 1; j <= COLS; j++) {
        cout << " " << j << " ";
        if (j < COLS) cout << space;
    }
    cout << "\n" << string(totalWidth, '-') << "\n";
    for (int i = 0; i < ROWS; i++) {
        cout << " " << (char)('A' + i) << "  ";

        for (int j = 0; j < COLS; j++) {
            char c = seatStatus[i][j];
            cout << " " << c << " ";
            if (j < COLS - 1) cout << space;
        }
        cout << "\n";
    }

    cout << string(totalWidth, '=') << "\n";
}

bool Showtime::bookSeat(char row, int col) {
    row = toupper(row);
    int r = row - 'A';

    if (r < 0 || r >= 5 || col < 1 || col > 10) {
        cout << "Invalid position!\n";
        return false;
    }

    col--;

    if (seatStatus[r][col] == 'X') return false;

    seatStatus[r][col] = 'X';
    return true;
}

bool Showtime::cancelSeat(char row, int col) {
    int r = row - 'A';

    if (r < 0 || r >= 5 || col < 1 || col > 10) {
        cout << "Invalid position!\n";
        return false;
    }

    col--;

    if (seatStatus[r][col] == '0') {
        cout << "Seat is already empty.\n";
        return false;
    }

    seatStatus[r][col] = '0';
    return true;
}

void Showtime::saveSeatStatus(const string &id) {
    fs::create_directories("Database/Seats");
    string filename = "Database/Seats/SeatStatus_" + id + ".txt";
    ofstream fout(filename);

    if (!fout) return;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++)
            fout << seatStatus[i][j] << " ";
        fout << "\n";
    }

    fout.close();
}

void Showtime::loadSeatStatus(const string &id) {
    fs::create_directories("Database/Seats");
    string filename = "Database/Seats/SeatStatus_" + id + ".txt";
    ifstream fin(filename);

    if (!fin) return;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 10; j++)
            fin >> seatStatus[i][j];

    fin.close();
}
