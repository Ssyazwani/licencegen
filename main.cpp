#include <iostream>
#include <sqlite3.h>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    cout << "\n";
    return 0;
}

string generateLicenseKey(const string& name, const string& org) {
    time_t now = time(0);
    stringstream ss;
    ss << "LIC-" << name.substr(0, 3) << "-" << org.substr(0, 3) << "-" << now;
    return ss.str();
}

void ensureColumn(sqlite3* db, const std::string& tableName, const std::string& columnName, const std::string& columnType) {
    std::string checkColumnSQL = "PRAGMA table_info(" + tableName + ");";
    bool hasColumn = false;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkColumnSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (colName == columnName) {
                hasColumn = true;
                break;
            }
        }
        sqlite3_finalize(stmt);
    }

    if (!hasColumn) {
        std::string alterTableSQL = "ALTER TABLE " + tableName + " ADD COLUMN " + columnName + " " + columnType + ";";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, alterTableSQL.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to add '" << columnName << "' column: " << errMsg << "\n";
            sqlite3_free(errMsg);
        } else {
            std::cout << "'" << columnName << "' column added to " << tableName << " table.\n";
        }
    }
}


void createLicense(sqlite3* db) {
    string name, org, activity, date, email;
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your organization: ";
    getline(cin, org);
    cout << "Enter your activity: ";
    getline(cin, activity);
    cout << "Enter your date: ";
    getline(cin, date);
    cout << "Enter your email: ";
    getline(cin, email);

    string licenseKey = generateLicenseKey(name, org);

    const char* sql = "INSERT INTO licenses (username, organization, activity, date, email, licenseKey) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, org.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, activity.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, licenseKey.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            cerr << "Insert error: " << sqlite3_errmsg(db) << "\n";
        } else {
            cout << "License created successfully:\n";
            cout << "Name: " << name << ", Organization: " << org << ", Activity: " << activity 
                 << ", Date: " << date << ", Email: " << email << ", License Key: " << licenseKey << "\n\n";
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare statement\n";
    }
}

void searchLicense(sqlite3* db) {
    string keyword;
    cout << "Enter name or organization to search: ";
    getline(cin, keyword);

    string sql = "SELECT * FROM licenses WHERE username LIKE '%" + keyword + "%' OR organization LIKE '%" + keyword + "%';";
    char* errMsg;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Search error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void editLicense(sqlite3* db) {
    string inputEmail;
    cout << "Enter the email to edit: ";
    getline(cin, inputEmail);

    const char* selectSQL = "SELECT username, organization, activity, date, licenseKey, email FROM licenses WHERE email = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare select statement\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, inputEmail.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        cout << "Email not found.\n";
        sqlite3_finalize(stmt);
        return;
    }

    string currentName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    string currentOrg = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    string currentActivity = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    string currentDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    string licenseKey = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    string currentEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    sqlite3_finalize(stmt);

    cout << "License Key: " << licenseKey << "\n";
    cout << "Current values (press enter to keep unchanged):\n";

    cout << "Name [" << currentName << "]: ";
    string name; 
    getline(cin, name);
    if (name.empty()) name = currentName;

    cout << "Organization [" << currentOrg << "]: ";
    string org; 
    getline(cin, org);
    if (org.empty()) org = currentOrg;

    cout << "Activity [" << currentActivity << "]: ";
    string activity;
    getline(cin, activity);
    if (activity.empty()) activity = currentActivity;

    cout << "Date [" << currentDate << "]: ";
    string date;
    getline(cin, date);
    if (date.empty()) date = currentDate;

    cout << "Email [" << currentEmail << "]: ";
    string newEmail;
    getline(cin, newEmail);
    if (newEmail.empty()) newEmail = currentEmail;

    const char* updateSQL = "UPDATE licenses SET username = ?, organization = ?, activity = ?, date = ?, email = ? WHERE licenseKey = ?;";
    if (sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare update statement\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, org.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, activity.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, newEmail.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, licenseKey.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Update error: " << sqlite3_errmsg(db) << "\n";
    } else {
        cout << "License updated successfully.\n";
    }

    sqlite3_finalize(stmt);
}
    void viewLicenseKeyByEmail(sqlite3* db) {
    string email;
    cout << "Enter the email to view license key: ";
    getline(cin, email);

    const char* selectSQL = "SELECT licenseKey FROM licenses WHERE email = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement.\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* licenseKey = sqlite3_column_text(stmt, 0);
        cout << "License Key: " << licenseKey << "\n";
    } else {
        cout << "No license key found for that email.\n";
    }

    sqlite3_finalize(stmt);
}





int main() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("licenses.db", &db);
    if (rc) {
        cerr << "Can't open DB: " << sqlite3_errmsg(db) << "\n";
        return 1;
    }

    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS licenses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "activity TEXT NOT NULL,"
        "date TEXT NOT NULL,"
        "email TEXT NOT NULL,"
        "licenseKey TEXT NOT NULL);"; 

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

   
    ensureColumn(db, "licenses", "organization", "TEXT");
    ensureColumn(db, "licenses", "activity", "TEXT");
    ensureColumn(db, "licenses", "date", "TEXT");
    ensureColumn(db, "licenses", "email", "TEXT");


    int choice;
    do {
        cout << "1. Create new license\n2. Search licenses \n3. Search license number by email\n4. Edit Licence Details\n5. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1:
                createLicense(db);
                break;
            case 2:
                searchLicense(db);
                break;
            case 3:
                viewLicenseKeyByEmail(db);
                break;
            case 4:
                editLicense(db);
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    sqlite3_close(db);
    return 0;
}
