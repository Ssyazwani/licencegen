
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
    string line;

    ofstream outFile("license_numbers.csv");

    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return 1;
    }

    outFile << "License Number,Date of Application,Activities\n";

    cout << "Press Y to generate a new license number, M to view the license. Press X to exit.\n";
    cin >> b;

    time_t now = time(0);
    tm ltm = {};
    localtime_s(&ltm, &now);
    stringstream dateStream;
    stringstream ss(line);
    string cell;

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

            cout << "Enter activity for license number " << setw(3) << setfill('0') << licenseNumber << ": ";
            cin.ignore();
            getline(cin, activity);

            cout << "Generated license number: " << setw(3) << setfill('0') << licenseNumber << 'M' << endl;

            // Store as string in the file
            outFile << setw(3) << setfill('0') << licenseNumber << 'M' << ","
                << dateOfApplication << ","
                << activity << "\n";

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

            // Print the table header with formatting
            cout << "Existing license numbers:\n";
            cout << left << setw(20) << "License Number"
                << setw(20) << "Date of Application"
                << setw(30) << "Activities" << endl;
            cout << string(70, '-') << endl; // Separator line

            // Skip the header line
            string line;
            getline(inFile, line);  // This will skip the first line (header)

            // Read and display each line from the file
            while (getline(inFile, line)) {  // Read each line from the file
                stringstream ss(line);

                string licenseNumStr;
                string dateOfApplication;
                string activity;

                // Extract the license number, date of application, and activity
                if (getline(ss, licenseNumStr, ',') && // First value: License Number
                    getline(ss, dateOfApplication, ',') && // Second value: Date
                    getline(ss, activity)) { // Third value: Activity

                    // Ensure the data is not empty or malformed
                    if (licenseNumStr.empty() || dateOfApplication.empty() || activity.empty()) {
                        cout << "Warning: Skipping malformed line\n";
                        continue; // Skip malformed lines
                    }

                    // Display the data in a formatted manner
                    cout << left << setw(20) << licenseNumStr
                        << setw(20) << dateOfApplication
                        << setw(30) << activity << endl;
                }
                else {
                    cout << "Warning: Skipping malformed line\n";
                }
            }

            inFile.close();  // Close the file after reading
        }
        break;


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
