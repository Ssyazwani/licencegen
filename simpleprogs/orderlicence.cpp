#include <iostream>
#include <iomanip>  
#include <fstream>
#include <ctime> 
#include <sstream> 
#include <string>
using namespace std;

int main() {
    char b;
    int licenseNumber = 1;
    string dateOfApplication;
    string activity;
    string UENumber;
    string type;
    string paymentDate;
    string Oic;

    ofstream outFile("license_numbers.csv");

    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return 1;
    }

    outFile << "License Number,Date of Application,Activities,UEN,Type,Payment Date,OIC\n";

    cout << "Press Y to generate a new license number, M to view the license. Press X to exit.\n";
    cin >> b;

    time_t now = time(0);
    tm ltm = {};
    localtime_s(&ltm, &now);
    stringstream dateStream;

    while (true) {
        switch (b) {
        case 'Y':
        case 'y':
            if (licenseNumber > 999) {
                cout << "All license numbers from 001M to 999M have been generated.\n";
                return 0;
            }

            dateStream.str(""); 
            dateStream << setw(2) << setfill('0') << ltm.tm_mday << "-"
                       << setw(2) << setfill('0') << (1 + ltm.tm_mon) << "-"  
                       << (1900 + ltm.tm_year); 
            dateOfApplication = dateStream.str();

            cout << "Enter activity for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            cin.ignore();
            getline(cin, activity);

            cout << "Enter UEN for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            getline(cin, UENumber);

            cout << "Enter Type for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            getline(cin, type);

            cout << "Enter payment date if they have paid fees for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            getline(cin, paymentDate);

            cout << "Enter Officer In Charge if app is assigned " << setw(3) << setfill('0') << licenseNumber << ": ";
            getline(cin, Oic);

            cout << "Generated license number: " << setw(3) << setfill('0') << licenseNumber << 'M' << endl;
            outFile << setw(3) << setfill('0') << licenseNumber << 'M' << ","
                    << "\"" << dateOfApplication << "\","
                    << "\"" << activity << "\","
                    << "\"" << UENumber << "\","
                    << "\"" << type << "\","
                    << "\"" << paymentDate << "\","
                    << "\"" << Oic << "\""
                    << "\n";

            licenseNumber++;
            break;

        case 'X':
        case 'x':
            cout << "\nExiting program.\n";
            return 0;

        case 'M':
        {
            ifstream inFile("license_numbers.csv");

            if (!inFile) {
                cout << "Error opening file for reading.\n";
                return 1;
            }

            cout << "Existing license numbers:\n";
            cout << left << setw(20) << "License Number"
                << setw(20) << "Date of Application"
                << setw(30) << "Activities"
                << setw(40) << "UEN"
                << setw(50) << "Type"
                << setw(60) << "Payment Date"
                << setw(70) << "Assigned Officer"
                << endl;
            cout << string(70, '-') << endl;

            string line;
            getline(inFile, line);  

            while (getline(inFile, line)) {
                stringstream ss(line);

                string licenseNumStr;
                string dateOfApplication;
                string activity;
                string UENumber;
                string type;
                string paymentDate;
                string Oic;

                if (getline(ss, licenseNumStr, ',') &&
                    getline(ss, dateOfApplication, ',') &&
                    getline(ss, activity, ',') &&
                    getline(ss, UENumber, ',') &&
                    getline(ss, type, ',') &&
                    getline(ss, paymentDate, ',') &&
                    getline(ss, Oic)) {

                    cout << left << setw(20) << licenseNumStr
                        << setw(20) << dateOfApplication
                        << setw(30) << activity
                        << setw(40) << UENumber
                        << setw(50) << type
                        << setw(60) << paymentDate
                        << setw(70) << Oic << endl;
                }
                else {
                    cout << "Warning: Skipping malformed line\n";
                }
            }

            inFile.close();
        }
        break;

        default:
            cout << "\nInvalid input. Press Y to generate a new license number, M to view the license. Press X to exit.\n";
            break;
        }

        if (licenseNumber <= 999) {
            cout << "Press Y to generate a new license number, M to view the license. Press X to exit.\n";
            cin >> b;
        }
    }
}
