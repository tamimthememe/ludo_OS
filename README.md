# Ludo Game 🎲

This is a Ludo game developed as part of my **Operating Systems final project**. The project demonstrates key concepts of process synchronization, inter-process communication, and threading in the context of an interactive, multiplayer game.

## Features
- **Multiplayer Support**: Play with up to 4 players.
- **Process Synchronization**: Ensures turn-based gameplay.
- **Inter-Process Communication (IPC)**: Implements communication between game threads/processes.
- **Threading**: Utilizes multithreading for smooth gameplay.
- **User-Friendly Interface**: Designed for an engaging game experience.

## How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ludo-game.git
   cd ludo-game

3. Compile all files
    ```bash
   g++ -o settings settings.cpp
   g++ -o ludoMain ludoMain.cpp
   g++ -o leaderboard leaderboard.cpp
   g++ -o menu menu.cpp
   g++ -o commentary commentary.cpp

5. Run the executables
In the first and second terminal respectively:
```bash
   ./menu
   ./commentary

