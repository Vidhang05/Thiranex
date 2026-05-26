/**
 * Mini Game Project
 * Features: Tic Tac Toe & Snake Game
 * Developed using C++ OOP Concepts (Classes, Polymorphism, Encapsulation)
 * Platform: Windows Console (supports smooth rendering & non-blocking inputs)
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <limits>

// ==========================================
//          CONSOLE UTILITY FUNCTIONS        
// ==========================================

/**
 * Moves the console cursor to a specific (x, y) coordinate.
 * This is used for double-buffering style screen updates in Snake Game
 * to prevent the standard system("cls") flickering.
 */
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
 * Hides the blinking console cursor.
 */
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

/**
 * Shows the blinking console cursor.
 */
void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

/**
 * Clears any stray keystrokes from the standard keyboard buffer.
 * Essential for transition between real-time keyboard gaming and menu prompts.
 */
void flushInputBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

// ==========================================
//            BASE GAME INTERFACE            
// ==========================================

/**
 * Game interface demonstrating OOP polymorphism.
 */
class Game {
public:
    virtual ~Game() {} // Virtual destructor for proper inheritance cleanup
    virtual void showInstructions() = 0; // Pure virtual function
    virtual void play() = 0;             // Pure virtual function
};

// ==========================================
//             TIC TAC TOE GAME              
// ==========================================

class TicTacToe : public Game {
private:
    char board[3][3];    // 3x3 game board grid
    char currentMarker;  // Current marker ('X' or 'O')
    int currentPlayer;   // Current player (1 or 2)

    /**
     * Resets the game board to grid index characters '1' through '9'.
     */
    void resetBoard() {
        char val = '1';
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = val++;
            }
        }
        currentPlayer = 1;
        currentMarker = 'X';
    }

    /**
     * Renders the Tic Tac Toe board dynamically on the screen.
     */
    void drawBoard() {
        std::cout << "\n";
        std::cout << "     |     |     \n";
        std::cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "  \n";
        std::cout << "_____|_____|_____\n";
        std::cout << "     |     |     \n";
        std::cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "  \n";
        std::cout << "_____|_____|_____\n";
        std::cout << "     |     |     \n";
        std::cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "  \n";
        std::cout << "     |     |     \n";
        std::cout << "\n";
    }

    /**
     * Attempts to place the active player's marker in the specified slot (1-9).
     * Returns true if placement was successful, false if slot is invalid or already taken.
     */
    bool placeMarker(int slot) {
        if (slot < 1 || slot > 9) {
            return false;
        }
        int row = (slot - 1) / 3;
        int col = (slot - 1) % 3;

        // Check if the slot is not already marked by X or O
        if (board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = currentMarker;
            return true;
        }
        return false;
    }

    /**
     * Evaluates the board to check if the current player has won.
     * Returns the player number (1 or 2) if won, 0 otherwise.
     */
    int checkWinner() {
        // Check Rows
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                return currentPlayer;
            }
        }
        // Check Columns
        for (int i = 0; i < 3; ++i) {
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                return currentPlayer;
            }
        }
        // Check Diagonals
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            return currentPlayer;
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            return currentPlayer;
        }

        return 0;
    }

    /**
     * Check if all spots are filled, meaning the game ends in a draw.
     */
    bool checkDraw() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    return false; // Found an unmarked slot
                }
            }
        }
        return true;
    }

    /**
     * Alternates the active player turn.
     */
    void switchPlayer() {
        if (currentPlayer == 1) {
            currentPlayer = 2;
            currentMarker = 'O';
        } else {
            currentPlayer = 1;
            currentMarker = 'X';
        }
    }

public:
    /**
     * Displays instructions for playing Tic Tac Toe.
     */
    void showInstructions() override {
        system("cls");
        std::cout << "=========================================\n";
        std::cout << "          TIC TAC TOE INSTRUCTIONS       \n";
        std::cout << "=========================================\n\n";
        std::cout << "1. The game is played on a 3x3 grid.\n";
        std::cout << "2. Player 1 is X and Player 2 is O.\n";
        std::cout << "3. Grid cells are numbered 1 to 9:\n";
        std::cout << "     1 | 2 | 3\n";
        std::cout << "    ---+---+---\n";
        std::cout << "     4 | 5 | 6\n";
        std::cout << "    ---+---+---\n";
        std::cout << "     7 | 8 | 9\n";
        std::cout << "4. Players take turns entering a number to place their marker.\n";
        std::cout << "5. The first player to get 3 of their markers in a row\n";
        std::cout << "   (horizontally, vertically, or diagonally) wins.\n";
        std::cout << "6. If all 9 cells are full and no player has 3 in a row,\n";
        std::cout << "   the game is a draw.\n\n";
        std::cout << "Press Enter to start playing...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    /**
     * Core Tic Tac Toe game loop.
     */
    void play() override {
        while (true) {
            resetBoard();
            int winner = 0;
            bool draw = false;

            while (winner == 0 && !draw) {
                system("cls");
                std::cout << "=========================================\n";
                std::cout << "               TIC TAC TOE               \n";
                std::cout << "=========================================\n";
                std::cout << "Player 1 [X]  vs  Player 2 [O]\n";
                
                drawBoard();

                std::string input;
                int choice = 0;
                bool valid = false;

                // Loop for validated input selection
                while (!valid) {
                    std::cout << "Player " << currentPlayer << " (" << currentMarker << "), enter a slot (1-9): ";
                    std::getline(std::cin, input);

                    if (input.length() == 1 && input[0] >= '1' && input[0] <= '9') {
                        choice = input[0] - '0';
                        if (placeMarker(choice)) {
                            valid = true;
                        } else {
                            std::cout << "Slot " << choice << " is already occupied! Try again.\n";
                        }
                    } else {
                        std::cout << "Invalid input! Please enter a single digit between 1 and 9.\n";
                    }
                }

                winner = checkWinner();
                if (winner != 0) {
                    system("cls");
                    std::cout << "=========================================\n";
                    std::cout << "               TIC TAC TOE               \n";
                    std::cout << "=========================================\n";
                    drawBoard();
                    std::cout << "CONGRATULATIONS! Player " << winner << " (" << currentMarker << ") wins!\n\n";
                    break;
                }

                draw = checkDraw();
                if (draw) {
                    system("cls");
                    std::cout << "=========================================\n";
                    std::cout << "               TIC TAC TOE               \n";
                    std::cout << "=========================================\n";
                    drawBoard();
                    std::cout << "It's a DRAW! Well played.\n\n";
                    break;
                }

                switchPlayer();
            }

            // Ask to replay
            std::string replayInput;
            std::cout << "Do you want to play Tic Tac Toe again? (y/n): ";
            std::getline(std::cin, replayInput);
            if (replayInput.empty() || (replayInput[0] != 'y' && replayInput[0] != 'Y')) {
                break;
            }
        }
    }
};

// ==========================================
//               SNAKE GAME                  
// ==========================================

class SnakeGame : public Game {
private:
    struct Point {
        int x, y;
    };

    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

    int width;                  // Width of the snake board
    int height;                 // Height of the snake board
    std::vector<Point> snake;   // Vector containing coordinates of snake pieces (index 0 is head)
    Point food;                 // Food location coordinate
    Direction dir;              // Current movement direction of snake
    int score;                  // Current score
    bool gameOver;              // State checking for active game loop

    /**
     * Randomly places food on the grid, ensuring it doesn't spawn on the snake body.
     */
    void spawnFood() {
        bool onSnake;
        do {
            onSnake = false;
            // Generate food inside boundary walls
            food.x = rand() % (width - 2) + 1;
            food.y = rand() % (height - 2) + 1;

            // Check if coordinates overlap with any part of the snake
            for (const auto& part : snake) {
                if (part.x == food.x && part.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    /**
     * Initial game variables state configuration.
     */
    void initGame() {
        width = 20;
        height = 15;
        gameOver = false;
        dir = STOP;
        score = 0;

        snake.clear();
        Point head = { width / 2, height / 2 };
        snake.push_back(head);

        spawnFood();
    }

    /**
     * Redraws the board dynamically. Using setCursorPosition(0,0) allows
     * for smooth game rendering without clearing console screen flickering.
     */
    void drawBoard() {
        setCursorPosition(0, 0);

        std::cout << "=========================================\n";
        std::cout << "               SNAKE GAME                \n";
        std::cout << "=========================================\n";
        std::cout << "Score: " << score << "   |   Controls: W/A/S/D (X to Pause/Quit)\n\n";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Draw walls
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                    std::cout << "#";
                } else {
                    bool isSnakePart = false;
                    // Draw snake head
                    if (x == snake[0].x && y == snake[0].y) {
                        std::cout << "O";
                        isSnakePart = true;
                    } else {
                        // Draw snake body
                        for (size_t i = 1; i < snake.size(); ++i) {
                            if (x == snake[i].x && y == snake[i].y) {
                                std::cout << "o";
                                isSnakePart = true;
                                break;
                            }
                        }
                    }

                    if (!isSnakePart) {
                        // Draw food
                        if (x == food.x && y == food.y) {
                            std::cout << "*";
                        } else {
                            std::cout << " "; // Empty floor space
                        }
                    }
                }
            }
            std::cout << "\n";
        }
        std::cout << "\nPress X to exit game immediately.\n";
    }

    /**
     * Non-blocking keyboard state check to register changes in direction.
     */
    void handleInput() {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 'a':
                case 'A':
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd':
                case 'D':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w':
                case 'W':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's':
                case 'S':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x':
                case 'X':
                    gameOver = true;
                    break;
            }
        }
    }

    /**
     * Updates snake coordinates, handles food consumption, and collision checking.
     */
    void updateLogic() {
        if (dir == STOP) return;

        // Save position of the head to calculate the next position
        Point nextHead = snake[0];
        switch (dir) {
            case LEFT:  nextHead.x--; break;
            case RIGHT: nextHead.x++; break;
            case UP:    nextHead.y--; break;
            case DOWN:  nextHead.y++; break;
            default: break;
        }

        // Collision with Wall
        if (nextHead.x <= 0 || nextHead.x >= width - 1 || nextHead.y <= 0 || nextHead.y >= height - 1) {
            gameOver = true;
            return;
        }

        // Collision with Self
        for (const auto& part : snake) {
            if (nextHead.x == part.x && nextHead.y == part.y) {
                gameOver = true;
                return;
            }
        }

        // Insert new head coordinates at index 0
        snake.insert(snake.begin(), nextHead);

        // Check if head landed on food position
        if (nextHead.x == food.x && nextHead.y == food.y) {
            score += 10;
            spawnFood();
        } else {
            // Remove the tail position (snake maintains original length)
            snake.pop_back();
        }
    }

public:
    /**
     * Displays instructions for Snake.
     */
    void showInstructions() override {
        system("cls");
        std::cout << "=========================================\n";
        std::cout << "           SNAKE GAME INSTRUCTIONS       \n";
        std::cout << "=========================================\n\n";
        std::cout << "1. Control the snake using WASD keys:\n";
        std::cout << "     W -> Move UP\n";
        std::cout << "     A -> Move LEFT\n";
        std::cout << "     S -> Move DOWN\n";
        std::cout << "     D -> Move RIGHT\n";
        std::cout << "2. Eat the food (*) to grow your snake and increase score.\n";
        std::cout << "3. Do not run into the walls (#) or yourself (o).\n";
        std::cout << "4. The game starts in a PAUSED state. Press any movement key\n";
        std::cout << "   (W, A, S, D) to start the snake moving.\n";
        std::cout << "5. Press 'X' at any time to exit the game.\n\n";
        std::cout << "Press Enter to start playing...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    /**
     * Core Snake game loop.
     */
    void play() override {
        while (true) {
            srand(static_cast<unsigned int>(time(nullptr)));
            initGame();
            hideCursor();

            // Perform single full clean before drawing
            system("cls");

            while (!gameOver) {
                drawBoard();
                handleInput();
                updateLogic();
                Sleep(100); // Wait 100 milliseconds to control game tick speed
            }

            showCursor();
            flushInputBuffer(); // Flush any stray keystrokes pressed during gameplay

            system("cls");
            std::cout << "=========================================\n";
            std::cout << "                GAME OVER                \n";
            std::cout << "=========================================\n\n";
            std::cout << "Your Final Score: " << score << "\n\n";
            
            // Ask to replay
            std::string replayInput;
            std::cout << "Do you want to play Snake Game again? (y/n): ";
            std::getline(std::cin, replayInput);
            if (replayInput.empty() || (replayInput[0] != 'y' && replayInput[0] != 'Y')) {
                break;
            }
        }
    }
};

// ==========================================
//             GAME APPLICATION              
// ==========================================

void showWelcomeScreen() {
    system("cls");
    std::cout << "==================================================\n";
    std::cout << " * * * * *  WELCOME TO MINI GAME HUB  * * * * * *\n";
    std::cout << "==================================================\n\n";
    std::cout << "               Featured Games:\n";
    std::cout << "             1. Tic Tac Toe (2-Player)\n";
    std::cout << "             2. Snake Game (Classic)\n\n";
    std::cout << "==================================================\n";
    std::cout << "Created using C++ OOP Concepts\n";
    std::cout << "Press Enter to continue to Main Menu...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void showMainMenu() {
    std::cout << "==================================================\n";
    std::cout << "                   MAIN MENU                      \n";
    std::cout << "==================================================\n";
    std::cout << "1. Play Tic Tac Toe\n";
    std::cout << "2. Play Snake Game\n";
    std::cout << "3. Exit\n";
    std::cout << "==================================================\n";
    std::cout << "Enter your choice (1-3): ";
}

int main() {
    showWelcomeScreen();

    while (true) {
        system("cls");
        showMainMenu();

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        if (input == "1") {
            // Polymorphism: instantiate TicTacToe subclass via Game interface pointer
            Game* game = new TicTacToe();
            game->showInstructions();
            game->play();
            delete game; // Prevent memory leak
        } else if (input == "2") {
            // Polymorphism: instantiate SnakeGame subclass via Game interface pointer
            Game* game = new SnakeGame();
            game->showInstructions();
            game->play();
            delete game; // Prevent memory leak
        } else if (input == "3") {
            system("cls");
            std::cout << "==================================================\n";
            std::cout << "       THANK YOU FOR PLAYING MINI GAME HUB!       \n";
            std::cout << "==================================================\n";
            break;
        } else {
            std::cout << "\nInvalid choice! Press Enter and try again...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }
    return 0;
}
