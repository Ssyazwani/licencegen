#include <random>
#include <iostream>
#include <stdio.h>
using namespace std;


int main () {


    int licencenumber=1;
    char b;
    int x;

    cout << "Press Y to generate new licence number\n";
    cin >> b;

    while (true) {
        switch (b) {
        case 'Y':
        case 'y':
             cout << "\nYour licence number is " << licencenumber << 'M' << endl;
            licencenumber++; 
            break;
        case 'x':
        case 'X':
            cout << "\nExiting program";
            return 0;
        default:
            cout << "\n Invalid input. Please input Y for new licence number\n";
            break;
        }

        cout << "Press Y to generate new licence number\n";
        cin >> b;
    }






}