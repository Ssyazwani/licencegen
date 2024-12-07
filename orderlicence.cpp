
#include <iostream>
#include <iomanip>  
using namespace std;

int main() {
    char b;
    int licenseNumber = 1;

    cout << "Press Y to generate a new license number or X to exit.\n";
    cin >> b;

    while (true) {
        switch (b) {
        case 'Y':
        case 'y':
            if (licenseNumber > 999) {
                cout << "All license numbers from 001 to 999 have been generated.\n";
                return 0;  
            }
            
            cout << "Generated license number: " << setw(3) << setfill('0') << licenseNumber << 'M' << endl;

           
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
