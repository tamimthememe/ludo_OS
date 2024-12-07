#include <iostream>

using namespace std;

int main() {
    cout << "==============================" << endl;
    cout << "           SETTINGS           " << endl;
    cout << "==============================" << endl;

    cout << "1. Change Player Names" << endl;
    cout << "2. Change Player Colors" << endl;
    cout << "3. Set Difficulty Level" << endl;
    cout << "4. Back to Main Menu" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "change player names" << endl;
            break;
        case 2:
            cout << "change player colors" << endl;
            break;
        case 3:
            cout << "set difficulty level coming soon!" << endl;
            break;
        case 4:
            cout << "Returning to main menu..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
    }

    return 0;
}
