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
              char *licencenum[] = { "001", "002", "003", 
                        "004", "005", "006", "007",
                        "008", "009", "010",
                        "011", "012", "013" };
  int i;
  for (i=0; i<13; i++) {
    printf("licencenum[%d] = %s\n", i, licencenum[i]);
  }
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