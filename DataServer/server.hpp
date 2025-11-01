#ifndef ENTERPRISE_SERVER_HPP
#define ENTERPRISE_SERVER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <limits>
#include <cctype>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>

class EnterpriseServer {
private:
    struct User {
        int age;
        std::string email;
        std::string phone;
        std::time_t created_at;
        std::time_t last_modified;
        
        User();
    };
    
    std::map<std::string, User> database;
    int total_operations;
    std::time_t server_start_time;
    
    void logEvent(const std::string& event);
    void loadData();
    void saveData();
    void backupData();
    std::string formatPhone(const std::string& phone);
    std::string generateUserId();
    void displayUser(const std::string& name, const User& user);
    void displayServerStats();

public:
    EnterpriseServer();
    ~EnterpriseServer();
    void start();
};

#endif
