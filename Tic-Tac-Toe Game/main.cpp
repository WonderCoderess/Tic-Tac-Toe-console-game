#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


enum GameState { PLAYING, WIN, DRAW, QUIT };

int** createIntArray2D(const int rows, const int cols, int defaultValue = 0);
void printIntArray2D(int** array2D, const int rows, const int cols);
void deleteIntArray2D(int** array2D, const int rows);
int choosePlayerSymbol();
bool isValidMove(int** board, int rows, int cols, int move_row, int move_column, bool showMessages);
void recordMove(int** board, int move_row, int move_column, int currentPlayer);
int swapPlayer(int currentPlayer);
bool processInputPlayer(int** board, int rows, int cols, int currentPlayer);
void processInputComputer(int** board, int rows, int cols, int currentPlayer);
GameState updateState(int** board, const int rows, const int cols, int& winner);



/* -- GAME RULES AND SETTINGS --

BOARD:
Board cell values: 0 = empty, 1 = X, 2 = O.
The board size is dynamic. The player chooses the number of rows
and columns, with each dimension ranging from 3 to 10.
The player chooses X or O, and the computer receives the other symbol.
X always makes the first move.

WIN:
A horizontal winning line must fill an entire row from the left edge
to the right edge of the board.
A vertical winning line must fill an entire column from the top edge
to the bottom edge of the board.
A diagonal winning line may start and end on any two board edges;
it does not have to start or end in a corner.
Diagonals containing fewer than three cells are ignored.

DRAW:
The game ends in a draw when every cell is occupied and neither
the player nor the computer has created a winning line.

QUIT:
Enter 0 0 during the player's turn to quit the game.

*/

int main()
{
	srand(static_cast<unsigned int>(time(nullptr))); // seed the random number generator

	GameState gameState = PLAYING;

	// -- 1. INITIALIZE -- entering the board dimensions, choosing the player’s symbol, initializing the board, and printing the board.

	/* -- 1.1. Prompts the user to enter the number of rows and columns for the board.
	Validates both values to ensure they are whole numbers between 3 and 10,
	then displays the selected board dimensions.*/

	int boardRows = 0;

	cout << "========================================" << endl;
	cout << "           TIC-TAC-TOE GAME" << endl;
	cout << "========================================" << endl;
	cout << endl;

	cout << "Welcome to Tic-Tac-Toe!" << endl;
	cout << "You will play against the computer." << endl;
	cout << endl;

	cout << "GAME RULES:" << endl;
	cout << "- Choose a board size from 3 to 10 rows and columns." << endl;
	cout << "- Choose your symbol: X or O." << endl;
	cout << "- X always makes the first move." << endl;
	cout << "- Fill an entire row, column, or valid diagonal to win." << endl;
	cout << "- A diagonal must connect two board edges and contain" << endl;
	cout << "  at least three cells." << endl;
	cout << "- If the board is full and nobody wins, the game ends" << endl;
	cout << "  in a draw." << endl;
	cout << "- Enter 0 0 during your turn to quit the game." << endl;
	cout << endl;

	cout << "Good luck!" << endl;
	cout << "========================================" << endl;
	cout << endl;
	cout << "Choose the board size: " << endl;

	while (true)
	{
		cout << "Enter number of rows (3-10): ";
		cin >> boardRows;

		if (cin.fail() || boardRows < 3 || boardRows > 10)
		{
			cout << "Incorrect value. Enter a whole number from 3 to 10." << endl;

			cin.clear();
			cin.ignore(1000, '\n');
		}
		else
		{
			cin.ignore(1000, '\n');   // line-flush after so leftover characters can't leak into the command
			break;
		}
	}

	int boardColumns = 0;

	while (true)
	{
		cout << "Enter number of columns (3-10): ";
		cin >> boardColumns;

		if (cin.fail() || boardColumns < 3 || boardColumns > 10)
		{
			cout << "Incorrect value. Enter a whole number from 3 to 10." << endl;

			cin.clear();
			cin.ignore(1000, '\n');
		}
		else
		{
			cin.ignore(1000, '\n');
			break;
		}
	}

	cout << "Board size: " << boardRows << " x " << boardColumns << endl;

	// -- 1.2. Choosing the player’s symbol X/O - values 0 = empty, 1 = player X, 2 = player O.

	int playerSymbol = choosePlayerSymbol();

	int currentPlayer = 1; // X always starts

	// -- 1.3. Initializing the board and printing the board

	int** board = createIntArray2D(boardRows, boardColumns, 0);
	printIntArray2D(board, boardRows, boardColumns);
	cout << endl;

	if (playerSymbol == 1)
	{
		cout << "You start the game" << endl;
	}
	else
	{
		cout << "Computer starts the game." << endl;
	}

	// -- 2. GET INPUT -- Read player’s move ReadMove(), validate the move IsmoveValid, record the move on the board recordMove() and print the board PrintIntArray2D()

	// if it is the player’s turn, read the player’s move; otherwise, generate a random move for the computer.
	while (gameState == PLAYING)
	{
		if (currentPlayer == playerSymbol)
		{
			cout << "Your move!" << endl;
			if (!processInputPlayer(board, boardRows, boardColumns, currentPlayer))
			{
				gameState = QUIT;
				break;                    // player quit: don't evaluate the board
			}
		}
		else
		{
			cout << "Computer's move!" << endl;
			processInputComputer(board, boardRows, boardColumns, currentPlayer);
		}

		// -- 3. UPDATE LOGIC -- 

		printIntArray2D(board, boardRows, boardColumns);

		int winner = 0;
		gameState = updateState(board, boardRows, boardColumns, winner);

		// Report Result
		if (gameState == WIN)
		{
			if (winner == playerSymbol)
				cout << "You won!" << endl;
			else
				cout << "Computer won!" << endl;
		}
		else if (gameState == DRAW)
		{
			cout << "It's a draw!" << endl;
		}

		if (gameState == PLAYING)
		{
			currentPlayer = swapPlayer(currentPlayer);
		}
	}

	if (gameState == QUIT)
		cout << "Game aborted by the player." << endl;

	// -- 4. SHUTDOWN -- releasing the memory and ending the program
	deleteIntArray2D(board, boardRows);
	return 0;
}


// -- FUNCTIONS -- 

// -- F1. Initialize --

/* -- F1.1. createIntArray2D --
Creates a dynamic two-dimensional integer array,
initializes every cell with the specified default value,
and returns a pointer to the created array.
 */

int** createIntArray2D(const int rows, const int cols, int defaultValue)
{
	int** array2D = new int* [rows];
	for (int i = 0; i < rows; i++)
	{
		array2D[i] = new int[cols];
		for (int j = 0; j < cols; j++)
		{
			array2D[i][j] = defaultValue;
		}
	}
	return array2D;
}

/* -- F1.2 printIntArray2D --
Prints the game board.
Empty cells are displayed as "_", player 1 as "X", and player 2 as "O".
*/

void printIntArray2D(int** array2D, const int rows, const int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (array2D[i][j] == 0)
			{
				cout << "_ ";
			}
			else if (array2D[i][j] == 1)
			{
				cout << "X ";
			}
			else if (array2D[i][j] == 2)
			{
				cout << "O ";
			}
		}
		cout << endl;
	}
}

/* -- F1.3 choosePlayerSymbol --
Asks the player to choose X or O and returns the corresponding numeric symbol. */

int choosePlayerSymbol()
{
	char choice;

	while (true)
	{
		cout << "Choose your symbol (X/O): ";
		cin >> choice;
		cin.ignore(1000, '\n');   // discard the rest of the line

		if (choice == 'X' || choice == 'x')
		{
			return 1; // X
		}
		else if (choice == 'O' || choice == 'o')
		{
			return 2; // O
		}
		else
		{
			cout << "Incorrect choice. Enter X or O.\n";
		}
	}
}

// -- F2. Get Input -- 

/* -- F2.1. processInputPlayer --
 Gets the player's row and column input, validates the move,
 records it on the board if it is valid, and returns true.
 Returns false if the player enters 0 0 to quit the game. */

bool processInputPlayer(int** board, int rows, int cols, int currentPlayer)
{
	while (true)
	{
		int move_row, move_column;

		cout << "Enter row and column (enter 0 0 to quit): ";

		if (!(cin >> move_row >> move_column))
		{
			cin.clear();
			cin.ignore(10000, '\n');

			cout << "Please enter two whole numbers." << endl;
			continue;
		}

		cin.ignore(10000, '\n');   // discard the rest of the line

		// Quit the game
		if (move_row == 0 && move_column == 0)
		{
			return false;
		}

		// Convert to 0-based index 
		move_row--;
		move_column--;

		// return false if the move is invalid, and ask for input again
		if (!isValidMove(board, rows, cols, move_row, move_column, true))
		{
			continue;
		}

		recordMove(board, move_row, move_column, currentPlayer);

		return true;
	}
}

/* -- F2.2. processInputComputer --
Generates random board coordinates until it finds an empty cell,
then records the computer's move in that cell. */

void processInputComputer(int** board, int rows, int cols, int currentPlayer)
{
	while (true)
	{
		int move_row = rand() % rows;
		int move_column = rand() % cols;

		if (!isValidMove(board, rows, cols, move_row, move_column, false))
			continue;

		recordMove(board, move_row, move_column, currentPlayer);
		return;
	}
}

/* -- F2.3. isValidMove --
 Checks whether the selected move is within the board boundaries
 and whether the selected cell is empty.
 Returns true if the move is valid; otherwise, returns false. */

bool isValidMove(int** board, int rows, int cols, int move_row, int move_column, bool showMessages)
{
	if (move_row < 0 || move_row >= rows || move_column < 0 || move_column >= cols)
	{
		if (showMessages)
		{
			cout << "Move is out of bounds. Please try again." << endl;
		}
		return false;
	}

	if (board[move_row][move_column] != 0)
	{
		if (showMessages)
		{
			cout << "Cell is already occupied. Please try again." << endl;
		}
		return false;
	}
	else
	{
		return true;
	}
}

/* -- F2.4. recordMove --
Stores the current player's symbol in the selected valid cell on the board. */

void recordMove(int** board, int move_row, int move_column, int currentPlayer)
{
	board[move_row][move_column] = currentPlayer;
}

/* -- F2.5. swapPlayer --
Switches the current player to the other player. */

int swapPlayer(int currentPlayer)
{
	return (currentPlayer == 1) ? 2 : 1;
}

/* -- F3. Update Logic --
 Inspects the board and reports whether the game is over.
GameState updateState(int** board, int& winner)
*/

GameState updateState(int** board, const int rows, const int cols, int& winner)
{
	winner = 0;

	// 1. ROWS – the line must be filled from the left edge to the right edge
	for (int i = 0; i < rows; i++)
	{
		int symbol = board[i][0];
		if (symbol != 0)
		{
			bool full = true;
			for (int j = 1; j < cols; j++)
			{
				if (board[i][j] != symbol)
				{
					full = false; // if any cell in the row is not the same symbol, it's not a winning line
					break;
				}
			}
			if (full)
			{
				winner = symbol;
				return WIN;
			}
		}
	}

	// 2. COLUMNS – the line must be filled from the top edge to the bottom edge
	for (int j = 0; j < cols; j++)
	{
		int symbol = board[0][j];
		if (symbol != 0)
		{
			bool full = true;
			for (int i = 1; i < rows; i++)
			{
				if (board[i][j] != symbol)
				{
					full = false;
					break;
				}
			}
			if (full)
			{
				winner = symbol;
				return WIN;
			}
		}
	}

	// 3a. DIAGONALS going down-right (row+1, col+1).
	//     A valid diagonal starts on the top or left edge and ends on another edge.
	for (int startRow = 0; startRow < rows; startRow++)
	{
		for (int startCol = 0; startCol < cols; startCol++)
		{
			if (startRow != 0 && startCol != 0)   // keep only starts on the top/left edge
				continue;

			// number of cells until we run off an edge
			int len = (rows - startRow < cols - startCol) ? (rows - startRow) : (cols - startCol);
			if (len < 3)                           // diagonals shorter than 3 are ignored
				continue;

			int symbol = board[startRow][startCol];
			if (symbol == 0)
				continue;

			bool full = true;
			for (int k = 1; k < len; k++)
			{
				if (board[startRow + k][startCol + k] != symbol)
				{
					full = false;
					break;
				}
			}
			if (full)
			{
				winner = symbol;
				return WIN;
			}
		}
	}

	// 3b. DIAGONALS going down-left (row+1, col-1).
	//     A valid diagonal starts on the top or right edge and ends on another edge.
	for (int startRow = 0; startRow < rows; startRow++)
	{
		for (int startCol = 0; startCol < cols; startCol++)
		{
			if (startRow != 0 && startCol != cols - 1)   // keep only starts on the top/right edge
				continue;

			int len = (rows - startRow < startCol + 1) ? (rows - startRow) : (startCol + 1);
			if (len < 3)
				continue;

			int symbol = board[startRow][startCol];
			if (symbol == 0)
				continue;

			bool full = true;
			for (int k = 1; k < len; k++)
			{
				if (board[startRow + k][startCol - k] != symbol)
				{
					full = false;
					break;
				}
			}
			if (full)
			{
				winner = symbol;
				return WIN;
			}
		}
	}

	// 4. No winner – is there any empty cell left?
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (board[i][j] == 0) //  if only one cell is empty, the game is still ongoing
				return PLAYING;
		}
	}

	return DRAW;
}

// -- F4. Shutdown -- 

/* -- F4.1. deleteIntArray2D --
Frees the memory allocated for a dynamic two-dimensional array.*/

void deleteIntArray2D(int** array2D, const int rows)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] array2D[i];
	}
	delete[] array2D;
}