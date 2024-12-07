#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream file("leaderboard.txt"); 
    if (!file.is_open()) {
        cout << "Error opening leaderboard file." << endl;
        return 1;
    }

    cout << "==============================" << endl;
    cout << "         LEADERBOARD          " << endl;
    cout << "==============================" << endl;

    string line;
    while (getline(file, line)) {
        cout << line << endl; //linebyline print
    }

    cout << "==============================" << endl;

    file.close();
    return 0;
}
