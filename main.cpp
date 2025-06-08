#include <iostream>
#include <sqlite3.h>
using namespace std;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

int main() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

   
    rc = sqlite3_open("licenses.db", &db);
    if (rc) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << "\n";
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
    }

    
    string username = "Kenty";
    string license = "LI-2025-ABCD-1234";
    string insertSQL = 
        "INSERT INTO licenses (username, licenseKey) VALUES ('" + username + "', '" + license + "');";

    rc = sqlite3_exec(db, insertSQL.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Insert error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    
    const char* selectSQL = "SELECT * FROM licenses;";
    rc = sqlite3_exec(db, selectSQL, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Read error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return 0;
}
