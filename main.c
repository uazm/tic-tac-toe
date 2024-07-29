#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef unix
const char *clear = "clear";
#endif
#ifdef _WIN32
const char *clear = "cls";
#endif

#define max(_a, _b) (((_a) > (_b)) ? (_a) : (_b))
#define min(_a, _b) (((_a) < (_b)) ? (_a) : (_b))

#define BOARD_SIZE 9
#define PlayerX 'X'
#define PlayerO 'O'

void printmenu() {
  system(clear);

  printf("\t _____ _        _____            _____\n"
         "\t|_   _(_)      |_   _|          |_   _|\n"
         "\t | |  _  ___    | | __ _  ___    | | ___   ___\n"
         "\t | | | |/ __|   | |/ _` |/ __|   | |/ _ \\ / _ \\\n"
         "\t | | | | (__    | | (_| | (__    | | (_) |  __/\n"
         "\t \\_/ |_|\\___|   \\_/\\__,_|\\___|   \\_/\\___/ \\___|\n");

  printf(" ******************************************************************\n"
         " *                                                                *\n"
         " *  The game is played on a 3x3 grid.                             *\n"
         " *  Players take turns marking a square.                          *\n"
         " *  The player who succeeds in placing three of their marks in a  *\n"
         " *  horizontal, vertical, or diagonal row wins the game.          *\n"
         " *                                                                *\n"
         " ******************************************************************"
         "\n\n");

  printf("\t\t\t1. Vs Human\n"
         "\t\t\t2. Vs Computer\n"
         "\t\t\t3. Exit\n\n");
}

void printBoard(char *board, int turn, char player) {
  system(clear);

  printf("\nPlayer: %c\n", player);
  printf("Turn: %d\n\n", turn);

  printf("\t\t %c | %c | %c\n", board[0], board[1], board[2]);
  printf("\t\t-----------\n");
  printf("\t\t %c | %c | %c\n", board[3], board[4], board[5]);
  printf("\t\t-----------\n");
  printf("\t\t %c | %c | %c\n", board[6], board[7], board[8]);
}

bool isMoveValid(char *board, int move) {
  int is_square_available = (board[move] == PlayerX || board[move] == PlayerO);
  int is_valid_square = (move < 0 || move > BOARD_SIZE);
  if (is_square_available || is_valid_square) {
    return false;
  }
  return true;
}

bool checkWin(char *board, char player) {
  // check rows
  if (board[0] == player && board[1] == player && board[2] == player) {
    return true;
  } else if (board[3] == player && board[4] == player && board[5] == player) {
    return true;
  } else if (board[6] == player && board[7] == player && board[8] == player) {
    return true;
  }

  // check columns
  if (board[0] == player && board[3] == player && board[6] == player) {
    return true;
  } else if (board[1] == player && board[4] == player && board[7] == player) {
    return true;
  } else if (board[2] == player && board[5] == player && board[8] == player) {
    return true;
  }

  // check diagonals
  if (board[0] == player && board[4] == player && board[8] == player) {
    return true;
  } else if (board[2] == player && board[4] == player && board[6] == player) {
    return true;
  }
  return false;
}

bool checkTie(char *board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] != PlayerX && board[i] != PlayerO) {
      return false;
    }
  }
  return true;
}

void clearBoard(char *board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    board[i] = '1' + i;
  }
}

int getMovePlayer(char *board, char player, int turn) {
  int move;
  do {
    printBoard(board, turn, player);
    printf("\nPlayer %c turn (1-9): ", player);
    scanf("%d", &move);
    move--;
  } while (!isMoveValid(board, move));
  return move;
}

int getComputerMoveEasy(char *board) {
  int move;

  // pass unix epoch as a seed to rand()
  srand(time(NULL));

  do {
    move = rand() % (BOARD_SIZE - 1);
  } while (!isMoveValid(board, move));

  return move;
}

int minimax(char *board, int alpha, int beta, bool isMaximizing) {
  if (checkWin(board, PlayerO)) {
    return 1;
  }
  if (checkWin(board, PlayerX)) {
    return -1;
  }
  if (checkTie(board)) {
    return 0;
  }

  int MaxEval;
  int player;

  if (isMaximizing) {
    MaxEval = -1000;
    player = PlayerO;
  } else {
    MaxEval = 1000;
    player = PlayerX;
  }

  int eval = MaxEval;
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (isMoveValid(board, i)) {
      board[i] = player;
      eval = minimax(board, alpha, beta, !isMaximizing);
      board[i] = '1' + i;
      if (isMaximizing) {
        MaxEval = max(eval, MaxEval);
        alpha = max(alpha, MaxEval);
      } else {
        MaxEval = min(eval, MaxEval);
        beta = min(beta, MaxEval);
      }
    }
    if (beta <= alpha) {
      break;
    }
  }

  return MaxEval;
}

int getComputerMoveHard(char *board) {
  // using minimax with alpha-beta pruning algorithm to find the best move
  int bestMove = -1;
  int MaxEval = -1000;
  int eval = MaxEval;

  for (int i = 0; i < BOARD_SIZE; i++) {
    if (isMoveValid(board, i)) {
      board[i] = PlayerO;
      eval = minimax(board, -9999, 9999, false);
      board[i] = '1' + i;

      if (eval > MaxEval) {
        MaxEval = eval;
        bestMove = i;
      }
    }
  }

  return bestMove;
}

int getComputerMove(char *board, int difficulty, int turn) {
  return (difficulty == 2 && turn != 1) ? getComputerMoveHard(board)
                                        : getComputerMoveEasy(board);
}

void computerSetup(int *difficulty, char *player) {
  // get difficulty from player
  printf("\nDifficulty\n"
         "1. Easy\n"
         "2. Hard\n"
         "\nChoose option (1-2): ");
  scanf("%d", difficulty);
  if (*difficulty >= 2) {
    *difficulty = 2;
  }

  // check if player wants to play first
  char choice;
  printf("\nDoes the Computer take the first turn? (y/n) ");
  scanf("%s", &choice);
  if (choice == 'y') {
    *player = PlayerO;
  }
}

void gameloop(char *board, int computer) {
  int move;
  char player = PlayerX;
  int difficulty = 1;
  int turn = 1;

  if (computer) {
    computerSetup(&difficulty, &player);
  }

  while (true) {

    if (computer) {
      move = (player == PlayerX) ? getMovePlayer(board, player, turn)
                                 : getComputerMove(board, difficulty, turn);
    } else {
      move = getMovePlayer(board, player, turn);
    }

    board[move] = player;
    printBoard(board, turn, player);
    if (checkWin(board, player)) {
      if (player == PlayerO && computer) {
        printf("\nComputer wins!\n");
      } else {
        printf("\nPlayer %c wins!\n", player);
      }
      break;

    } else if (checkTie(board)) {
      printf("\nIt's a tie!\n");
      break;
    }

    // swap player
    player = (player == PlayerX) ? PlayerO : PlayerX;

    turn++;
  }
  printf("\nThe Game took %d turns to finish!\n", turn);
}

int main() {
  char board[BOARD_SIZE];
  int choice = 0;

  while (true) {
    clearBoard(board);
    printmenu();

    printf("Choose Option (1-3): ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      gameloop(board, false);
      break;
    case 2:
      gameloop(board, true);
      break;
    case 3:
      return 0;
    default:
      printf("\nInvalid Option\n");
      break;
    }

    // press enter to continue
    printf("\n\nPress Enter to Continue...");
    getchar();
    getchar();
  }
}
