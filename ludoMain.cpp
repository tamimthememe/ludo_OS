// i222449_Tamim_Ahmad    i222409_Eman_Khalid    i221511_Huzaifa_Imran

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "board.h"

using namespace std;

pthread_mutex_t boardMutex = PTHREAD_MUTEX_INITIALIZER;
struct Dice
{
    int num;
    //* OS CONCEPT: MUTEX to ensure one thread at a time gets a turn
    pthread_mutex_t mutex;

    Dice()
    {
        num = 0;
        pthread_mutex_init(&mutex, NULL);
        srand(time(0));
    }

    int getNumber(int playerId)
    {
        pthread_mutex_lock(&mutex);

        num = rand() % 6 + 1;
        // To test 3 consecutive 6 validation
        // num = 2;
        num = 6;
        cout << "Player " << playerId << " rolled: " << num << std::endl;
        pthread_mutex_unlock(&mutex);
        return num;
    }
};

struct Token
{
    char color;
    int id;
    int i;
    int j;
    bool inPlay;
    char symbol;
    //*OS Concept: Safe usage of token with usage of binary semaphores
    sem_t sem;

    Token(char playerColor, int tokenId, int k, int j, char sym)
    {
        color = playerColor;
        id = tokenId;
        inPlay = false;
        i = k;
        this->j = j;
        symbol = sym;
        sem_init(&sem, 0, 1);
    }

    ~Token()
    {
        sem_destroy(&sem);
    }
};

struct Player
{
    int id;
    char color;
    string name;
    int tokensAtHome;
    vector<Token> tokens;
    bool hasWon;
    int hits;

    Player(char playerColor, int playerId, string nam) : color(playerColor), id(playerId), name(nam)
    {
        tokensAtHome = 0;
        hits = 0;
        hasWon = false;
        for (int i = 0; i < 4; i++)
        {
            int k, j;
            char symbol;
            switch (i + 1)
            {
            case 1:
            {
                if (color == 'R')
                {
                    k = 2;
                    j = 2;
                    symbol = '0';
                }
                else if (color == 'G')
                {
                    k = 2;
                    j = 12;
                    symbol = '4';
                }
                else if (color == 'Y')
                {
                    k = 12;
                    j = 12;
                    symbol = '8';
                }
                else
                {
                    k = 12;
                    j = 2;
                    symbol = '-';
                }

                break;
            }
            case 2:
            {
                if (color == 'R')
                {
                    k = 2;
                    j = 4;
                    symbol = '1';
                }
                else if (color == 'G')
                {
                    k = 2;
                    j = 14;
                    symbol = '5';
                }
                else if (color == 'Y')
                {
                    k = 12;
                    j = 14;
                    symbol = '9';
                }
                else
                {
                    k = 12;
                    j = 4;
                    symbol = '*';
                }

                break;
            }
            case 3:
            {
                if (color == 'R')
                {
                    k = 4;
                    j = 2;
                    symbol = '2';
                }
                else if (color == 'G')
                {
                    k = 4;
                    j = 12;
                    symbol = '6';
                }
                else if (color == 'Y')
                {
                    k = 14;
                    j = 12;
                    symbol = '=';
                }
                else
                {
                    k = 14;
                    j = 2;
                    symbol = ';';
                }

                break;
            }
            case 4:
            {
                if (color == 'R')
                {
                    k = 4;
                    j = 4;
                    symbol = '3';
                }
                else if (color == 'G')
                {
                    k = 4;
                    j = 14;
                    symbol = '7';
                }
                else if (color == 'Y')
                {
                    k = 14;
                    j = 14;
                    symbol = '+';
                }
                else
                {
                    k = 14;
                    j = 4;
                    symbol = '/';
                }

                break;
            }
            }

            tokens.push_back(Token(playerColor, i + 1, k, j, symbol));
        }
    }

    void resetToken(Token &token)
    {
        cout << token.symbol << endl;
        switch (token.symbol)
        {
        case '0':
            token.i = 2;
            token.j = 2;
            break;
        case '1':
            token.i = 2;
            token.j = 4;
            break;
        case '2':
            token.i = 4;
            token.j = 2;
            break;
        case '3':
            token.i = 4;
            token.j = 4;
            break;
        case '4':
            token.i = 2;
            token.j = 12;
            break;
        case '5':
            token.i = 2;
            token.j = 14;
            break;
        case '6':
            token.i = 4;
            token.j = 12;
            break;
        case '7':
            token.i = 4;
            token.j = 14;
            break;
        case '8':
            token.i = 12;
            token.j = 12;
            break;
        case '9':
            token.i = 12;
            token.j = 14;
            break;
        case ';':
            token.i = 14;
            token.j = 2;
            break;
        case '/':
            token.i = 14;
            token.j = 4;
            break;
        case '-':
            token.i = 12;
            token.j = 2;
            break;
        case '*':
            token.i = 12;
            token.j = 4;
            break;
        case '=':
            token.i = 14;
            token.j = 12;
            break;
        case '+':
            token.i = 14;
            token.j = 14;
            break;
        }
        token.inPlay = false;
        pthread_mutex_lock(&boardMutex);
        board[token.i][token.j] = token.symbol;
        pthread_mutex_unlock(&boardMutex);
    }
};

struct HitManager
{
    vector<Player> *players;
    pthread_mutex_t mutex;

    void placeTokens()
    {
        for (int i = 0; i < players->size(); i++)
        {
            for (int j = 0; j < (*players)[i].tokens.size(); j++)
            {
                pthread_mutex_lock(&boardMutex);
                board[(*players)[i].tokens[j].i][(*players)[i].tokens[j].j] = (*players)[i].tokens[j].symbol;
                pthread_mutex_unlock(&boardMutex);
            }
        }
    }

    void hitToken(int i, int j, Player &currentPlayer)
    {
        pthread_mutex_lock(&mutex);
        for (auto &player : *players)
        {
            if (player.id != currentPlayer.id)
            {
                for (int m = 0; m < player.tokens.size(); m++)
                {
                    if (player.tokens[m].i == i && player.tokens[m].j == j)
                    {
                        currentPlayer.hits++;
                        // Handle token being hit
                        int fd = open("ludoPipe", O_WRONLY);
                        if (fd == -1)
                        {
                            perror("open");
                            return;
                        }
                        string message;

                        message = currentPlayer.name + " has hit " + player.name;
                        if (write(fd, message.c_str(), message.size() + 1) == -1)
                        {
                            perror("write");
                        }
                        close(fd);

                        player.resetToken(player.tokens[m]);
                        return;
                    }
                }
            }
            placeTokens();
        }
        pthread_mutex_unlock(&mutex);
    }
};

//! Global Variables / Shared Variables
//! The board is also global in board.h
Dice dice;
pthread_mutex_t turnMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cinMutex = PTHREAD_MUTEX_INITIALIZER;
int turn = 1;
int totalPlayers;
HitManager hitManager;

void initializeGameBoard()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = '.';
        }
    }
    initializeOutline();
    initializeHomes();
    initializeSafety();
}

void moveToken(int &i, int &j, int moves, char symbol, char color, Player &player)
{
    if ((i == 8 && color == 'R' && j + moves > 7) || (j == 8 && color == 'G' && i + moves > 7) || (i == 8 && color == 'Y' && j - moves > 7) || (j == 8 && color == 'R' && i - moves > 7))
    {
        return;
    }

    for (int k = 0; k < moves; k++)
    {

        initializeSafety();
        initializeHomes();
        hitManager.placeTokens();
        board[i][j] = '.';

        if ((i == 8 && j + moves == 7 && color == 'R') || (j == 8 && color == 'G' && i + moves == 7) || (i == 8 && color == 'Y' && j - moves == 7))
        {
            player.tokensAtHome++;
        }

        if (i == 8 && j >= 1 && j < 7 && color == 'R')
            j++;
        else if (i == 8 && j <= 15 && j > 9 && color == 'Y')
            j--;
        else if (j == 8 && i >= 1 && color == 'G' && i < 7)
            i++;
        else if (j == 8 && i <= 15 && color == 'B' && i > 9)
            i--;
        else if ((i == 7 && j < 6) || (j >= 10 && i == 7 && j < 15) || (i == 1 && j < 9))
        {
            j++;
        }
        else if (j == 6 && i == 7)
        {
            j++;
            i--;
        }
        else if ((j == 7 && i <= 6 && i > 1) || (j == 7 && i <= 15 && i > 10) || (j == 1 && i < 10 && i > 7))
        {
            i--;
        }
        else if ((j == 9 && i < 6) || (j == 15 && i >= 7 && i < 9) || (j == 15 && i <= 15 && i >= 10) || (j == 9 && i > 9 && i < 15))
        {
            i++;
        }
        else if ((i == 9 && j <= 15 && j >= 11) || (i == 9 && j < 7 && j > 1) || (j > 7 && j < 10 && i == 15))
        {
            j--;
        }
        else if (j == 9 && i == 6)
        {
            j++;
            i++;
        }
        else if (i == 10 && j == 7)
        {
            j--;
            i--;
        }
        else if (j == 10 && i == 9)
        {
            i++;
            j--;
        }
        pthread_mutex_lock(&boardMutex);
        board[i][j] = symbol;
        pthread_mutex_unlock(&boardMutex);

        initializeSafety();
        initializeHomes();
        hitManager.placeTokens();

        system("clear");
        displayBoard();
        sleep(1);
    }

    if (player.tokensAtHome == 4)
    {
        player.hasWon = true;
        cout << "Player : " << player.color << " has won." << endl;
        sleep(10);
    }

    if (!((i == 7 && j == 2) || (i == 9 && j == 3) || (i == 2 && j == 9) || (i == 3 && j == 7) || (i == 7 && j == 13) || (i == 9 && j == 14) || (i == 14 && j == 7) || (i == 13 && j == 9)))
    {
        hitManager.hitToken(i, j, player);
        system("clear");
        displayBoard();
    }
}

//*OS Concept: Threading for each player's turn according to rubric
void *playerTurn(void *arg)
{
    Player *player = (Player *)arg;

    while (true)
    {

        pthread_mutex_lock(&turnMutex);
        if (player->id == turn)
        {
            pthread_mutex_unlock(&turnMutex);
            pthread_mutex_lock(&cinMutex);
            system("clear");
            displayBoard();
            if (player->hasWon)
            {
                turn = ((turn) % totalPlayers) + 1;
                continue;
            }
            cout << "Player " << player->color << ", type 'r' to roll the dice: ";
            char input;
            cin >> input;

            if (input == 'r' || input == 'R')
            {
                vector<int> rollResult;
                int result;
                do
                {
                    result = dice.getNumber(player->id);
                    rollResult.push_back(result);

                    int fd = open("ludoPipe", O_WRONLY);
                    if (fd == -1)
                    {
                        perror("open");
                        continue;
                    }
                    string message;

                    message = player->name + " has rolled " + to_string(rollResult[0]);
                    if (write(fd, message.c_str(), message.size() + 1) == -1)
                    {
                        perror("write");
                    }
                    close(fd);

                    if (rollResult.size() == 3 && rollResult[2] == 6)
                    {
                        cout << "Cannot Play Turn" << endl;
                        rollResult.clear();
                        sleep(2);
                        break;
                    }
                } while (result == 6);
                cout << player->color << " rolled " << rollResult[0] << endl;
                pthread_mutex_lock(&turnMutex);

                for (int i = 0; i < rollResult.size(); i++)
                {
                    cout << "choose Token: [1] [2] [3] [4]" << endl;
                    int tokenInput;
                    cin >> tokenInput;

                    cout << player->tokens[tokenInput - 1].inPlay << endl;

                    if (player->tokens[tokenInput - 1].inPlay == true)
                    {
                        sem_wait(&player->tokens[tokenInput - 1].sem);
                        moveToken(player->tokens[tokenInput - 1].i, player->tokens[tokenInput - 1].j, rollResult[i], player->tokens[tokenInput - 1].symbol, player->color, *player);
                        int fd = open("ludoPipe", O_WRONLY);
                        if (fd == -1)
                        {
                            perror("open");
                            return nullptr;
                        }
                        string message;

                        message = player->name + " has moved token: " + to_string(tokenInput) + ", " + to_string(rollResult[i]) + " places ";
                        if (write(fd, message.c_str(), message.size() + 1) == -1)
                        {
                            perror("write");
                        }
                        close(fd);
                        sem_post(&player->tokens[tokenInput - 1].sem);
                    }
                    else
                    {
                        pthread_mutex_lock(&boardMutex);
                        switch (tokenInput)
                        {
                        case 1:
                        {
                            if (player->color == 'R' && rollResult[i] == 6 && board[2][2] == '0')
                            {
                                board[7][2] = '0';
                                board[2][2] = '.';
                                player->tokens[tokenInput - 1].i = 7;
                                player->tokens[tokenInput - 1].j = 2;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'G' && rollResult[i] == 6 && board[2][12] == '4')
                            {
                                board[2][9] = '4';
                                board[2][12] = '.';
                                player->tokens[tokenInput - 1].i = 2;
                                player->tokens[tokenInput - 1].j = 9;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'Y' && rollResult[i] == 6 && board[12][12] == '8')
                            {
                                board[9][14] = '8';
                                board[12][12] = '.';
                                player->tokens[tokenInput - 1].i = 9;
                                player->tokens[tokenInput - 1].j = 14;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'B' && rollResult[i] == 6 && board[12][2] == '-')
                            {
                                board[14][7] = '-';
                                board[12][2] = '.';
                                player->tokens[tokenInput - 1].i = 14;
                                player->tokens[tokenInput - 1].j = 7;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            break;
                        }
                        case 2:
                        {
                            if (player->color == 'R' && rollResult[i] == 6 && board[2][4] == '1')
                            {
                                board[7][2] = '1';
                                board[2][4] = '.';
                                player->tokens[tokenInput - 1].i = 7;
                                player->tokens[tokenInput - 1].j = 2;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'G' && rollResult[i] == 6 && board[2][14] == '5')
                            {
                                board[2][9] = '5';
                                board[2][14] = '.';
                                player->tokens[tokenInput - 1].i = 2;
                                player->tokens[tokenInput - 1].j = 9;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'Y' && rollResult[i] == 6 && board[12][14] == '9')
                            {
                                board[9][14] = '9';
                                board[12][14] = '.';
                                player->tokens[tokenInput - 1].i = 9;
                                player->tokens[tokenInput - 1].j = 14;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'B' && rollResult[i] == 6 && board[12][4] == '*')
                            {
                                board[14][7] = '*';
                                board[12][4] = '.';
                                player->tokens[tokenInput - 1].i = 14;
                                player->tokens[tokenInput - 1].j = 7;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            break;
                        }
                        case 3:
                        {
                            if (player->color == 'R' && rollResult[i] == 6 && board[4][2] == '2')
                            {
                                board[7][2] = '2';
                                board[4][2] = '.';
                                player->tokens[tokenInput - 1].i = 7;
                                player->tokens[tokenInput - 1].j = 2;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'G' && rollResult[i] == 6 && board[4][12] == '6')
                            {
                                board[2][9] = '6';
                                board[4][12] = '.';
                                player->tokens[tokenInput - 1].i = 2;
                                player->tokens[tokenInput - 1].j = 9;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'B' && rollResult[i] == 6 && board[14][2] == ';')
                            {
                                board[14][7] = ';';
                                board[14][2] = '.';
                                player->tokens[tokenInput - 1].i = 14;
                                player->tokens[tokenInput - 1].j = 7;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'Y' && rollResult[i] == 6 && board[14][12] == '=')
                            {
                                board[9][14] = '=';
                                board[14][12] = '.';
                                player->tokens[tokenInput - 1].i = 9;
                                player->tokens[tokenInput - 1].j = 14;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            break;
                        }
                        case 4:
                        {
                            if (player->color == 'R' && rollResult[i] == 6 && board[4][4] == '3')
                            {
                                board[7][2] = '3';
                                board[4][4] = '.';
                                player->tokens[tokenInput - 1].i = 7;
                                player->tokens[tokenInput - 1].j = 2;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'G' && rollResult[i] == 6 && board[4][14] == '7')
                            {
                                board[2][9] = '7';
                                board[4][14] = '.';
                                player->tokens[tokenInput - 1].i = 2;
                                player->tokens[tokenInput - 1].j = 9;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'B' && rollResult[i] == 6 && board[14][4] == '/')
                            {
                                board[14][7] = '/';
                                board[14][4] = '.';
                                player->tokens[tokenInput - 1].i = 14;
                                player->tokens[tokenInput - 1].j = 7;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            else if (player->color == 'Y' && rollResult[i] == 6 && board[14][14] == '+')
                            {
                                board[9][14] = '+';
                                board[14][14] = '.';
                                player->tokens[tokenInput - 1].i = 9;
                                player->tokens[tokenInput - 1].j = 14;
                                player->tokens[tokenInput - 1].inPlay = true;
                            }
                            break;
                        }
                        }
                        pthread_mutex_unlock(&boardMutex);
                    }
                }

                turn = ((turn) % totalPlayers) + 1;
                pthread_mutex_unlock(&turnMutex);
            }
            pthread_mutex_unlock(&cinMutex);
        }
        else
        {
            pthread_mutex_unlock(&turnMutex);
        }
        usleep(100000);
    }

    return nullptr;
}

//! Creating a MasterThread to overlook the entire game
//* OS CONCEPT USED: THREADING

void *masterThread(void *arg)
{
    vector<Player> playerArray = *((vector<Player> *)arg);
    hitManager.players = &playerArray;

    if (mkfifo("ludoPipe", 0666) == -1)
    {
        perror("mkfifo");
    }

    //* Thread for each player
    pthread_t *players = new pthread_t[playerArray.size()];

    for (int i = 0; i < playerArray.size(); ++i)
    {
        pthread_create(&players[i], nullptr, playerTurn, (void *)&playerArray[i]);
    }

    for (int i = 0; i < playerArray.size(); ++i)
    {
        pthread_join(players[i], nullptr);
    }

    cout << "Game Over! All players have finished their turns." << endl;

    delete[] players;
    return nullptr;
}

int main()
{
    initializeGameBoard();

    int NUM_PLAYERS = 3;
    pthread_t master; //! Master Thread for the whole game
    vector<Player> players;

    // Create players
    int choice = 0;

    while (choice < 1 || choice > 4)
    {
        cout << "Enter Number of players: " << endl;
        cin >> choice;
    }

    vector<char> colors = {'R',
                           'G',
                           'Y',
                           'B'};

    for (int i = 0; i < choice; i++)
    {
        char color = 'Z';
        string name;
        cout << "Enter name: ";
        cin >> name;

        bool found = true;
        while (found)
        {
            cout << "Choose Colour: ";
            for (int i = 0; i < colors.size(); i++)
            {
                cout << "[" << colors[i] << "] ";
            }
            cout << endl;
            cin >> color;
            for (int i = 0; i < colors.size(); i++)
            {
                if (colors[i] == color)
                {
                    colors.erase(colors.begin() + i);
                    found = false;
                }
            }
        }

        Player player(color, i + 1, name);
        players.push_back(player);
    }

    totalPlayers = players.size();

    for (int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < players[i].tokens.size(); j++)
        {
            board[players[i].tokens[j].i][players[i].tokens[j].j] = players[i].tokens[j].symbol;
        }
    }

    hitManager.players = &players;

    pthread_create(&master, nullptr, masterThread, (void *)&players);

    pthread_join(master, NULL);

    pthread_mutex_destroy(&dice.mutex);

    displayBoard();

    return 0;
}
