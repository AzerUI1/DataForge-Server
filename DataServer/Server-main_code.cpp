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

using namespace std;

const string DATA_FILE = "enterprise_database.dat";
const string BACKUP_FILE = "enterprise_backup.dat";
const string LOG_FILE = "system_log.txt";

class EnterpriseServer {
private:
    struct User {
        int age;
        string email;
        string phone;
        time_t created_at;
        time_t last_modified;
        
        User() : age(0), created_at(0), last_modified(0) {}
    };
    
    map<string, User> database;
    int total_operations;
    time_t server_start_time;
    
    void logEvent(const string& event) {
        ofstream log(LOG_FILE, ios::app);
        time_t now = time(nullptr);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        log << "[" << timestamp << "] " << event << endl;
        log.close();
    }
    
    void loadData() {
        ifstream file(DATA_FILE);
        if (!file.is_open()) {
            logEvent("SYSTEM: Database file not found. Initializing new database.");
            return;
        }
        
        string line;
        int loaded_count = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            User user;
            string created_str, modified_str;
            
            if (ss >> name >> user.age >> user.email >> user.phone >> created_str >> modified_str) {
                user.created_at = stol(created_str);
                user.last_modified = stol(modified_str);
                database[name] = user;
                loaded_count++;
            }
        }
        file.close();
        logEvent("SYSTEM: Database loaded successfully. Records: " + to_string(loaded_count));
    }
    
    void saveData() {
        backupData();
        
        ofstream file(DATA_FILE);
        if (!file.is_open()) {
            logEvent("ERROR: Failed to open database file for writing");
            cout << "ERROR: Cannot save to database file" << endl;
            return;
        }
        
        time_t current_time = time(nullptr);
        
        for (auto &entry : database) {
            file << entry.first << " " 
                 << entry.second.age << " "
                 << entry.second.email << " "
                 << entry.second.phone << " "
                 << entry.second.created_at << " "
                 << current_time << "\n";
        }
        file.close();
    }
    
    void backupData() {
        ofstream backup(BACKUP_FILE);
        if (backup.is_open()) {
            for (auto &entry : database) {
                backup << entry.first << "|" << entry.second.age << "|" 
                       << entry.second.email << "|" << entry.second.phone << "|"
                       << entry.second.created_at << "\n";
            }
            backup.close();
        }
    }
    
    string formatPhone(const string& phone) {
        if (phone.length() == 10) {
            return "+1-" + phone.substr(0, 3) + "-" + phone.substr(3, 3) + "-" + phone.substr(6, 4);
        }
        return phone;
    }
    
    string generateUserId() {
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
        
        string id;
        for (int i = 0; i < 8; ++i) {
            id += alphanum[dis(gen)];
        }
        return id;
    }
    
    void displayUser(const string& name, const User& user) {
        cout << "\n┌─────────────── USER PROFILE ───────────────┐" << endl;
        cout << "│  Name:    " << left << setw(30) << name << "│" << endl;
        cout << "│  Age:     " << setw(30) << user.age << "│" << endl;
        cout << "│  Email:   " << setw(30) << user.email << "│" << endl;
        cout << "│  Phone:   " << setw(30) << formatPhone(user.phone) << "│" << endl;
        
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&user.created_at));
        cout << "│  Created: " << setw(30) << buffer << "│" << endl;
        cout << "└─────────────────────────────────────────────┘" << endl;
    }
    
    void displayServerStats() {
        time_t now = time(nullptr);
        double uptime = difftime(now, server_start_time);
        int hours = static_cast<int>(uptime) / 3600;
        int minutes = (static_cast<int>(uptime) % 3600) / 60;
        int seconds = static_cast<int>(uptime) % 60;
        
        cout << "\n┌────────────── SERVER STATISTICS ──────────────┐" << endl;
        cout << "│  Total Users:      " << right << setw(24) << database.size() << "  │" << endl;
        cout << "│  Total Operations: " << setw(24) << total_operations << "  │" << endl;
        cout << "│  Server Uptime:    " << setw(24) << "  │" << endl;
        cout << "│                    " << setw(2) << hours << "h " << setw(2) << minutes << "m " << setw(2) << seconds << "s" << setw(8) << "  │" << endl;
        cout << "│  Data File:        " << setw(24) << DATA_FILE << "  │" << endl;
        cout << "└────────────────────────────────────────────────┘" << endl;
    }

public:
    EnterpriseServer() : total_operations(0), server_start_time(time(nullptr)) {
        logEvent("SYSTEM: Enterprise Server initialized");
    }
    
    ~EnterpriseServer() {
        logEvent("SYSTEM: Enterprise Server shutdown completed");
    }
    
    void start() {
        // Load existing data
        loadData();
        
        // Professional startup sequence
        cout << "\nInitializing Enterprise Data Server ";
        for(int i = 0; i < 3; i++) {
            cout << ".";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(300));
        }
        cout << " ONLINE" << endl;
        
        cout << "\n╔══════════════════════════════════════════════════╗" << endl;
        cout << "║           ENTERPRISE DATA SERVER v2.1            ║" << endl;
        cout << "║             Production Environment               ║" << endl;
        cout << "╚══════════════════════════════════════════════════╝" << endl;
        
        cout << "\nSystem Status:" << endl;
        cout << "  ✓ Database: " << database.size() << " records loaded" << endl;
        cout << "  ✓ Backup System: Active" << endl;
        cout << "  ✓ Audit Logging: Enabled" << endl;
        cout << "  ✓ Ready for operations" << endl << endl;
        
        string command, name;
        
        while (true) {
            cout << "cmd> ";
            cin >> command;
            
            // Convert to uppercase for command matching
            transform(command.begin(), command.end(), command.begin(), ::toupper);
            total_operations++;
            
            if (command == "ADD" || command == "CREATE") {
                User user;
                cout << "Enter user details:" << endl;
                cout << "  Name: ";
                cin >> name;
                cout << "  Age: ";
                cin >> user.age;
                cout << "  Email: ";
                cin >> user.email;
                cout << "  Phone: ";
                cin >> user.phone;
                
                if (user.age < 1 || user.age > 120) {
                    cout << " Validation Error: Age must be between 1-120" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                
                user.created_at = time(nullptr);
                user.last_modified = user.created_at;
                database[name] = user;
                saveData();
                logEvent("CREATE: User '" + name + "' added to database");
                cout << "User '" << name << "' registered successfully" << endl;
            }
            else if (command == "GET" || command == "FIND") {
                cin >> name;
                if (database.count(name)) {
                    cout << "User record found:" << endl;
                    displayUser(name, database[name]);
                    logEvent("RETRIEVE: User '" + name + "' data accessed");
                } else {
                    cout << " User '" << name << "' not found in database" << endl;
                }
            }
            else if (command == "DELETE" || command == "REMOVE") {
                cin >> name;
                if (database.count(name)) {
                    database.erase(name);
                    saveData();
                    logEvent("DELETE: User '" + name + "' removed from database");
                    cout << "User '" << name << "' deleted successfully" << endl;
                } else {
                    cout << " User '" << name << "' not found" << endl;
                }
            }
            else if (command == "LIST" || command == "SHOWALL") {
                if (database.empty()) {
                    cout << "Database is currently empty" << endl;
                } else {
                    cout << "\nRegistered Users (" << database.size() << " records):" << endl;
                    cout << "┌──────────────┬─────┬────────────────────────────┬──────────────┐" << endl;
                    cout << "│ Name         │ Age │ Email                      │ Phone        │" << endl;
                    cout << "├──────────────┼─────┼────────────────────────────┼──────────────┤" << endl;
                    
                    for (auto &entry : database) {
                        string phone_display = formatPhone(entry.second.phone);
                        if (phone_display.length() > 12) phone_display = phone_display.substr(0, 9) + "...";
                        
                        cout << "│ " << left << setw(12) << (entry.first.length() > 12 ? entry.first.substr(0, 9) + "..." : entry.first)
                             << " │ " << setw(3) << entry.second.age
                             << " │ " << setw(26) << (entry.second.email.length() > 26 ? entry.second.email.substr(0, 23) + "..." : entry.second.email)
                             << " │ " << setw(12) << phone_display << " │" << endl;
                    }
                    cout << "└──────────────┴─────┴────────────────────────────┴──────────────┘" << endl;
                    logEvent("LIST: Displayed all " + to_string(database.size()) + " user records");
                }
            }
            else if (command == "UPDATE" || command == "EDIT") {
                cin >> name;
                if (database.count(name)) {
                    User& user = database[name];
                    cout << "Editing user: " << name << endl;
                    cout << "Current age: " << user.age << " → New age: ";
                    cin >> user.age;
                    cout << "Current email: " << user.email << " → New email: ";
                    cin >> user.email;
                    cout << "Current phone: " << user.phone << " → New phone: ";
                    cin >> user.phone;
                    
                    user.last_modified = time(nullptr);
                    saveData();
                    logEvent("UPDATE: User '" + name + "' profile modified");
                    cout << " User profile updated successfully" << endl;
                } else {
                    cout << " User '" << name << "' not found" << endl;
                }
            }
            else if (command == "STATS" || command == "STATISTICS") {
                displayServerStats();
                logEvent("SYSTEM: Server statistics accessed");
            }
            else if (command == "SEARCH") {
                string term;
                cin >> term;
                cout << "Searching for: '" << term << "'" << endl;
                vector<string> results;
                
                for (auto &entry : database) {
                    if (entry.first.find(term) != string::npos || 
                        entry.second.email.find(term) != string::npos ||
                        entry.second.phone.find(term) != string::npos) {
                        results.push_back(entry.first);
                    }
                }
                
                if (results.empty()) {
                    cout << "No matching records found" << endl;
                } else {
                    cout << "Found " << results.size() << " result(s):" << endl;
                    for (size_t i = 0; i < results.size(); i++) {
                        cout << "  " << (i+1) << ". " << results[i] << endl;
                    }
                }
            }
            else if (command == "BACKUP") {
                backupData();
                cout << " Backup created: " << BACKUP_FILE << endl;
                logEvent("SYSTEM: Manual backup created");
            }
            else if (command == "GENERATE" || command == "GEN") {
                int count;
                cout << "Number of test records to generate: ";
                cin >> count;
                
                if (count < 1 || count > 1000) {
                    cout << " Error: Please enter number between 1-1000" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                
                for (int i = 0; i < count; i++) {
                    string random_name = "TEST_" + generateUserId();
                    User random_user;
                    random_user.age = 20 + (rand() % 50);
                    random_user.email = "testuser" + to_string(i) + "@company.com";
                    random_user.phone = to_string(1000000000 + rand() % 9000000000);
                    random_user.created_at = time(nullptr);
                    random_user.last_modified = random_user.created_at;
                    
                    database[random_name] = random_user;
                }
                saveData();
                cout << " Generated " << count << " test records" << endl;
                logEvent("SYSTEM: Generated " + to_string(count) + " test records");
            }
            else if (command == "CLEAR" || command == "WIPE") {
                cout << " SECURITY ALERT: This will PERMANENTLY delete ALL records!" << endl;
                cout << "Type 'CONFIRM' to proceed: ";
                string confirmation;
                cin >> confirmation;
                
                if (confirmation == "CONFIRM") {
                    int deleted_count = database.size();
                    database.clear();
                    saveData();
                    logEvent("SECURITY: Database wiped - " + to_string(deleted_count) + " records deleted");
                    cout << " Database cleared. " << deleted_count << " records removed." << endl;
                } else {
                    cout << "Operation cancelled" << endl;
                }
            }
            else if (command == "EXIT" || command == "QUIT") {
                cout << "\nInitiating shutdown sequence..." << endl;
                cout << "Saving database... ";
                saveData();
                cout << "Complete" << endl;
                cout << "Writing logs... ";
                logEvent("SYSTEM: Normal shutdown initiated");
                cout << "Complete" << endl;
                cout << "Enterprise Server shutdown complete." << endl;
                break;
            }
            else if (command == "HELP") {
                cout << "\nAvailable Commands:" << endl;
                cout << "  ADD      - Register new user" << endl;
                cout << "  GET      - Retrieve user details" << endl;
                cout << "  UPDATE   - Modify user information" << endl;
                cout << "  DELETE   - Remove user" << endl;
                cout << "  LIST     - Display all users" << endl;
                cout << "  SEARCH   - Search users" << endl;
                cout << "  STATS    - Server statistics" << endl;
                cout << "  BACKUP   - Create backup" << endl;
                cout << "  GENERATE - Create test data" << endl;
                cout << "  CLEAR    - Clear database (requires confirmation)" << endl;
                cout << "  HELP     - This help message" << endl;
                cout << "  EXIT     - Shutdown server" << endl;
            }
            else {
                cout << " Unknown command. Type HELP for available commands." << endl;
            }
            
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;
        }
    }
};

int main() {
    EnterpriseServer server;
    server.start();
    return 0;
}
