#include <random>
#include <iostream>
#include <stdio.h>
using namespace std;


int main() {

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int>dist(1, 1000);
    int licencenumber;
    char b;

    cout << "Press Y to generate new licence number\n";
    cin >> b;

    while (true) {
        switch (b) {
        case 'Y':
        case 'y':
            licencenumber = dist(gen);
            cout << "\n Your licence number is " << licencenumber << 'M' << endl;
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