#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

void display()
{
    cout << endl;
    cout << "==============================" << endl;
    cout << "        LUDO GAME MENU        " << endl;
    cout << "==============================" << endl;
    cout << "1. Leaderboard" << endl;
    cout << "2. Play Game" << endl;
    cout << "3. Settings" << endl;
    cout << "4. Exit" << endl;
    cout << "==============================" << endl;
}

void executeProgram(const char *path)
{
    //*OS Concept : Fork used to execute programs
    pid_t pid = fork();

    if (pid == 0)
    {
        execl(path, path, (char *)NULL);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork failed");
    }
}

int main()
{
    int choice;

    do
    {
        system("clear"); // use to clear screen when switching to next menu page
        display();
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            executeProgram("./leaderboard");
            break;
        case 2:
            executeProgram("./ludo");
            break;
        case 3:
            executeProgram("./settings");
            break;
        case 4:
            cout << "Exiting the game. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select a valid option." << endl;
            break;
        }

        if (choice != 4)
        {
            cout << "\nReturning to the menu...";
            sleep(2);
        }

    } while (choice != 4);

    return 0;
}
