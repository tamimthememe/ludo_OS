#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int BOARD_SIZE = 17;
char board[BOARD_SIZE][BOARD_SIZE];

// Colour Codes
const string RED = "\033[41m";
const string GREEN = "\033[42m";
const string YELLOW = "\033[43m";
const string BLUE = "\033[44m";
const string RESET = "\033[0m";
const string WHITE_BG_BLACK_TEXT = "\033[47;30m";
const string OUTLINE = "\033[40;37m";
const string LIGHT_RED = "\033[91m";
const string LIGHT_GREEN = "\033[92m";
const string LIGHT_BLUE = "\033[94m";
const string LIGHT_YELLOW = "\033[93m";

void initializeOutline()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board[0][i] = 'o';
        board[BOARD_SIZE - 1][i] = 'o';
        board[i][0] = 'o';
        board[i][BOARD_SIZE - 1] = 'o';
    }
}

void initializeHomes()
{
    for (int i = 1; i < 7; i++)
    {
        board[6][i] = 'o';
        board[i][6] = 'o';
    }

    for (int i = 10; i < 16; i++)
    {
        board[i - 10][10] = 'o';
        board[6][i] = 'o';
    }

    for (int i = 10; i < 16; i++)
    {
        board[10][i] = 'o';
        board[i][10] = 'o';
    }

    for (int i = 1; i < 7; i++)
    {
        board[10][i] = 'o';
        board[i + 10][6] = 'o';
    }

    for (int i = 2; i < 7; i++)
    {
        board[i][8] = 'G';
    }

    for (int i = 2; i < 7; i++)
    {
        board[8][i] = 'R';
    }

    for (int i = 0; i < 5; i++)
    {
        board[10 + i][8] = 'B';
    }

    for (int i = 0; i < 5; i++)
    {
        board[8][10 + i] = 'Y';
    }

    for (int i = 7; i < 10; i++)
    {
        for (int j = 7; j < 10; j++)
        {
            board[i][j] = 'h';
        }
    }
}

void initializeSafety()
{
    board[7][2] = 'R';
    board[9][3] = 'R';

    board[2][9] = 'G';
    board[3][7] = 'G';

    board[9][14] = 'Y';
    board[7][13] = 'Y';

    board[14][7] = 'B';
    board[13][9] = 'B';
}

void displayBoard()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'R')
                cout << RED << " R " << RESET;
            else if (board[i][j] == 'G')
                cout << GREEN << " G " << RESET;
            else if (board[i][j] == 'Y')
                cout << YELLOW << " Y " << RESET;
            else if (board[i][j] == 'B')
                cout << BLUE << " B " << RESET;
            else if (board[i][j] == '0')
                cout << RED << " 1 " << RESET;
            else if (board[i][j] == '1')
                cout << RED << " 2 " << RESET;
            else if (board[i][j] == '2')
                cout << RED << " 3 " << RESET;
            else if (board[i][j] == '3')
                cout << RED << " 4 " << RESET;
            else if (board[i][j] == '4')
                cout << GREEN << " 1 " << RESET;
            else if (board[i][j] == '5')
                cout << GREEN << " 2 " << RESET;
            else if (board[i][j] == '6')
                cout << GREEN << " 3 " << RESET;
            else if (board[i][j] == '7')
                cout << GREEN << " 4 " << RESET;
            else if (board[i][j] == '8')
                cout << YELLOW << " 1 " << RESET;
            else if (board[i][j] == '9')
                cout << YELLOW << " 2 " << RESET;
            else if (board[i][j] == '=')
                cout << YELLOW << " 3 " << RESET;
            else if (board[i][j] == '+')
                cout << YELLOW << " 4 " << RESET;
            else if (board[i][j] == '-')
                cout << BLUE << " 1 " << RESET;
            else if (board[i][j] == '*')
                cout << BLUE << " 2 " << RESET;
            else if (board[i][j] == ';')
                cout << BLUE << " 3 " << RESET;
            else if (board[i][j] == '/')
                cout << BLUE << " 4 " << RESET;
            else if (board[i][j] == 'r')
                cout << OUTLINE << " o " << RESET;
            else if (board[i][j] == 'g')
                cout << OUTLINE << " o " << RESET;
            else if (board[i][j] == 'y')
                cout << OUTLINE << " o " << RESET;
            else if (board[i][j] == 'b')
                cout << OUTLINE << " o " << RESET;
            else if (board[i][j] == 'o')
                cout << OUTLINE << " o " << RESET;
            else if (board[i][j] == 'h')
                cout << OUTLINE << " o " << RESET;
            else
                cout << "   ";
        }
        cout << endl;
    }
}
