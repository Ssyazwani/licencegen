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

void ensureOrganizationColumn(sqlite3* db) {
    const char* checkColumnSQL = "PRAGMA table_info(licenses);";
    bool hasOrganization = false;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkColumnSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (colName == "organization") {
                hasOrganization = true;
                break;
            }
        }
        sqlite3_finalize(stmt);
    }

    if (!hasOrganization) {
        const char* alterTableSQL = "ALTER TABLE licenses ADD COLUMN organization TEXT;";
        char* errMsg;
        int rc = sqlite3_exec(db, alterTableSQL, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "Failed to add 'organization' column: " << errMsg << "\n";
            sqlite3_free(errMsg);
        } else {
            cout << "'organization' column added to licenses table.\n";
        }
    }
}

void createLicense(sqlite3* db) {
    string name, org;
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your organization: ";
    getline(cin, org);

    string licenseKey = generateLicenseKey(name, org);
    string sql = "INSERT INTO licenses (username, organization, licenseKey) VALUES ('" 
                 + name + "', '" + org + "', '" + licenseKey + "');";

    char* errMsg;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Insert error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    } else {
        cout << "License created successfully:\n";
        cout << "Name: " << name << ", Organization: " << org << ", License Key: " << licenseKey << "\n\n";
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
        "licenseKey TEXT NOT NULL);"; 

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

   
    ensureOrganizationColumn(db);

    int choice;
    do {
        cout << "1. Create new license\n2. Search licenses\n3. Exit\nEnter choice: ";
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
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    sqlite3_close(db);
    return 0;
}
