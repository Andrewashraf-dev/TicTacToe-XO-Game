#include <iostream>
#include <limits>
using namespace std;

class TicTacToe {
private:
    char board[3][3];
    char humanPlayer, aiPlayer;

public:
    TicTacToe() {
        humanPlayer = 'X';
        aiPlayer = 'O';
        initializeBoard();
    }

    void initializeBoard() {
        char num = '1';
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                board[i][j] = num++;
    }

    void printBoard() {
        cout << "\n";
        for (int i = 0; i < 3; i++) {
            cout << " ";
            for (int j = 0; j < 3; j++) {
                cout << board[i][j];
                if (j < 2) cout << " | ";
            }
            cout << "\n";
            if (i < 2) cout << "---+---+---\n";
        }
        cout << "\n";
    }

    bool isMovesLeft() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] != 'X' && board[i][j] != 'O')
                    return true;
        return false;
    }

    // Check if player has won
    bool checkWin(char player) {
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == player &&
                board[i][1] == player &&
                board[i][2] == player) ||
                (board[0][i] == player &&
                    board[1][i] == player &&
                    board[2][i] == player))
                return true;
        }
        if ((board[0][0] == player &&
            board[1][1] == player &&
            board[2][2] == player) ||
            (board[0][2] == player &&
                board[1][1] == player &&
                board[2][0] == player))
            return true;

        return false;
    }

    int evaluate() {
        if (checkWin(aiPlayer))
            return +10;
        else if (checkWin(humanPlayer))
            return -10;
        else
            return 0;
    }

    int minimax(int depth, bool isMax) {
        int score = evaluate();

        if (score == 10) return score - depth;    // Prefer faster wins
        if (score == -10) return score + depth;   // Prefer slower losses

        if (!isMovesLeft()) return 0;

        if (isMax) {
            int best = numeric_limits<int>::min();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] != 'X' && board[i][j] != 'O') {
                        char backup = board[i][j];
                        board[i][j] = aiPlayer;
                        best = max(best, minimax(depth + 1, false));
                        board[i][j] = backup;
                    }
                }
            }
            return best;
        }
        else {
            int best = numeric_limits<int>::max();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] != 'X' && board[i][j] != 'O') {
                        char backup = board[i][j];
                        board[i][j] = humanPlayer;
                        best = min(best, minimax(depth + 1, true));
                        board[i][j] = backup;
                    }
                }
            }
            return best;
        }
    }

    pair<int, int> findBestMove() {
        int bestVal = numeric_limits<int>::min();
        pair<int, int> bestMove = { -1, -1 };

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char backup = board[i][j];
                    board[i][j] = aiPlayer;

                    int moveVal = minimax(0, false);

                    board[i][j] = backup;

                    if (moveVal > bestVal) {
                        bestMove = { i, j };
                        bestVal = moveVal;
                    }
                }
            }
        }

        return bestMove;
    }

    bool makeHumanMove() {
        int move;
        cout << "Enter your move (1-9): ";
        cin >> move;

        if (move < 1 || move > 9) {
            cout << "Invalid move! Try again.\n";
            return false;
        }

        int row = (move - 1) / 3;
        int col = (move - 1) % 3;

        if (board[row][col] == 'X' || board[row][col] == 'O') {
            cout << "Spot taken! Try another.\n";
            return false;
        }

        board[row][col] = humanPlayer;
        return true;
    }

    void playGame() {
        cout << "You are 'X', AI is 'O'\n";
        while (true) {
            printBoard();

            // Human move
            if (!makeHumanMove())
                continue;

            if (checkWin(humanPlayer)) {
                printBoard();
                cout << "You win! Congrats!\n";
                break;
            }

            if (!isMovesLeft()) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }

            // AI move
            pair<int, int> aiMove = findBestMove();
            board[aiMove.first][aiMove.second] = aiPlayer;
            cout << "AI plays move: " << (aiMove.first * 3 + aiMove.second + 1) << "\n";

            if (checkWin(aiPlayer)) {
                printBoard();
                cout << "AI wins! Better luck next time.\n";
                break;
            }

            if (!isMovesLeft()) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }
        }
    }
};