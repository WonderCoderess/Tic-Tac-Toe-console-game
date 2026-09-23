# Dynamic Tic-Tac-Toe in C++

A console-based Tic-Tac-Toe game written in C++. The player competes against a computer on a dynamically allocated board with independently selected dimensions from 3 to 10 rows and columns.

This project was created as a C++ programming exercise focused on functions, pointers, dynamic memory, input validation, enums, and game-state management.

## Features

- Dynamic rectangular board from `3 × 3` to `10 × 10`
- Choice of `X` or `O`
- `X` always makes the first move
- Computer opponent using randomly selected valid moves
- Validation of board size, symbol choice, coordinates, and occupied cells
- Detection of wins, draws, and player exit
- Manual allocation and release of a two-dimensional array
- Clear separation of initialization, input, game logic, and shutdown

## Winning rules

The game uses extended rules designed for boards of different sizes:

- A horizontal win must fill an entire row from the left edge to the right edge.
- A vertical win must fill an entire column from the top edge to the bottom edge.
- A diagonal win must connect two board edges, but it does not need to begin or end in a corner.
- Diagonals shorter than three cells are ignored.
- The game ends in a draw when the board is full and neither side has won.

## How to play

1. Choose the number of rows and columns. Each value must be between `3` and `10`.
2. Choose your symbol: `X` or `O`.
3. Enter the row and column of the cell where you want to place your symbol.
4. Coordinates are numbered starting from `1`.
5. Enter `0 0` during your turn to quit the game.

Example move:

```text
Enter row and column (enter 0 0 to quit): 2 3
```

## Build and run

### Visual Studio

1. Create or open a C++ Console Application project.
2. Add `main.cpp` to the project.
3. Build the solution with `Ctrl + Shift + B`.
4. Run it with `Ctrl + F5`.

### g++

Compile:

```bash
g++ -std=c++11 main.cpp -o tic-tac-toe
```

Run on Windows:

```powershell
.\tic-tac-toe.exe
```

Run on Linux or macOS:

```bash
./tic-tac-toe
```

## Project structure

```text
.
├── main.cpp      # Complete game implementation
└── README.md     # Project documentation
```

## Main components

- `createIntArray2D()` – creates and initializes the dynamic board
- `printIntArray2D()` – displays the current board
- `choosePlayerSymbol()` – validates the player's symbol choice
- `processInputPlayer()` – reads and validates player moves
- `processInputComputer()` – generates a random valid computer move
- `isValidMove()` – checks coordinates and cell availability
- `updateState()` – detects wins, draws, and ongoing play
- `deleteIntArray2D()` – releases dynamically allocated memory

## Possible future improvements

- Smarter computer opponent using the minimax algorithm
- Multiple difficulty levels
- Rematch option without restarting the application
- Score tracking across several games
- Improved board display with numbered rows and columns
