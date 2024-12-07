

#include <iostream>
#include <iomanip>  
#include <fstream>
#include <ctime> 
#include <sstream> 
using namespace std;

int main() {
    char b;
    int licenseNumber = 1;
    string dateOfApplication;
    string activity;

    ofstream outFile("license_numbers.csv");

    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return 1;
    }

    outFile << "License Number,Date of Application,Activities\n";

    cout << "Press Y to generate a new license number or X to exit.\n";
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

            dateStream << setw(2) << setfill('0') << ltm.tm_mday << "-" 
                << setw(2) << setfill('0') << (1 + ltm.tm_mon) << "-"  
                << 1900 + ltm.tm_year;
            dateOfApplication = dateStream.str();

            activity;
            cout << "Enter activity for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            cin.ignore();
            getline(cin, activity);


            cout << "Generated license number: " << setw(3) << setfill('0') << licenseNumber << 'M' << endl;


            outFile << setw(3) << setfill('0') << licenseNumber << ","
                << dateOfApplication << ","
                << activity << "\n";

            licenseNumber++;
            break;

        case 'X':
        case 'x':
            cout << "\nExiting program.\n";
            return 0;

        default:
            cout << "\nInvalid input. Please input Y to generate a new license number or X to exit.\n";
            break;
        }


        if (licenseNumber <= 999) {
            cout << "Press Y to generate a new license number or X to exit.\n";
            cin >> b;
        }
    }
}
