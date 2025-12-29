#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <filesystem>

class Logger {
private:
    std::ofstream file;
    std::mutex mtx;

    Logger() {
        std::filesystem::create_directory("logs"); 
        file.open("logs/app.log", std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Không thể mở file log!\n";
        }
    }

    ~Logger() {
        if (file.is_open()) file.close();
    }

    std::string currentDateTime() {
    auto now = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now);

    tm *timeinfo = std::localtime(&now_c);

    std::stringstream ss;
    ss << std::put_time(timeinfo, "%d/%m/%Y %H:%M:%S");

    return ss.str();
}


    void write(const std::string &level, const std::string &message) {
    std::lock_guard<std::mutex> lock(mtx);
    if (file.is_open()) {
        file << "[" << currentDateTime() << "] [" << level << "] " << message << "\n";
        file.flush();
    }
}

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void info(const std::string &msg) { write("INFO", msg); }
    void warn(const std::string &msg) { write("WARN", msg); }
    void error(const std::string &msg) { write("ERROR", msg); }
    
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
};
