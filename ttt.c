#include <stdio.h>

// TODO: rejigger the check function.

typedef struct {
    char symbol;
    size_t id;
} Player;

const size_t ROWS = 3;
const size_t COLS = 3;

char check(char board[ROWS][COLS]);
void update(char board[ROWS][COLS], size_t pos[2], Player* p);
int move_to_pos(char* move, size_t pos[]);
int legal(char board[ROWS][COLS], size_t pos[2]);
void input(size_t id, char* pos);
void initialize(char b[ROWS][COLS]);
void display(char b[ROWS][COLS]);

int main() {
    Player p1;
    Player p2;

    Player *ps[2] = {&p1, &p2};

    char move[2];
    size_t pos[2] = { '?', '?' };

    p1.symbol = 'X';
    p2.symbol = 'O';

    p1.id = 0;
    p2.id = 1;

    size_t turn = p1.id;

    size_t moves = 0;

    char win = 0;

    char board[ROWS][COLS];

    initialize(board);

    // GAME LOOP

    do {
        printf("\n");
        display(board);

        if (moves == ROWS*COLS) {
            printf("Draw.\n");
            break;
        }

        if (moves >= 5) {
            win = check(board);
            if (win) {
                switch (win) {
                    case 'X':
                    printf("Player 1 wins!\n");
                    break;
                    case 'O':
                    printf("Player 2 wins!\n");
                    break;
                    default:
                    printf("?\n");
                }
                break;
            }
        }

        do {
            input(ps[turn]->id, move);
            if (!move_to_pos(move, pos)) {
                printf("Invalid input.\n");
                continue;
            } else {
                if (!legal(board, pos)) {
                    printf("Position filled.\n");
                } else {
                    break;
                }
            }
        } while (1);

        update(board, pos, ps[turn]);

        if (turn == p1.id) {
            turn = p2.id;
        } else {
            turn = p1.id;
        }

        moves++;

    } while(1);

    return 0;
}

char check(char b[ROWS][COLS]) {
    for (size_t i = 0; i < ROWS; i++) {
        char tic = b[i][0];
        if (tic != ' ' && tic == b [i][1] && tic == b[i][2]) {
            return tic;
        }
        tic = b[0][i];
        if (tic != ' ' && tic == b[1][i] && tic == b[2][i]) {
            return tic;
        }
        tic = b[1][1];
        if (tic != ' ') {
            if (b[0][0] == tic && b[0][0] == b[2][2]) {
                return tic;
            }
            if (b[0][2] == tic && b[0][2] == b[2][0]) {
                return tic;
            }
        }
    }
    return 0;
}

void update(char b[ROWS][COLS], size_t pos[2], Player* p) {
    b[pos[0]][pos[1]] = p->symbol;
}

int move_to_pos(char* move, size_t *pos) {
    switch (move[0]) {
        case 'a':
        pos[1] = 0;
        break;
        case 'b':
        pos[1] = 1;
        break;
        case 'c':
        pos[1] = 2;
        break;
        default:
        return 0;
    }
    switch (move[1]) {
        case '3':
        pos[0] = 0;
        break;
        case '2':
        pos[0] = 1;
        break;
        case '1':
        pos[0] = 2;
        break;
        default:
        return 0;
    }
    return 1;
}

int legal(char b[ROWS][COLS], size_t pos[2]) {
    if (b[pos[0]][pos[1]] == ' ') {
        return 1;
    }
    return 0;
}

void input(size_t id, char* move) {
    printf("Player %ld: ", id+1);
    scanf("%s", move);
}

void display(char b[ROWS][COLS]) {
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLS; j++) {
            printf("%c", b[i][j]);
            if (j + 1 == COLS) {
                printf("\n");
                break;
            }
            printf("|");
        }
        if (i + 1 == ROWS) {
            printf("\n");
            break;
        }
        printf("-----\n");
    }
}

void initialize(char b[ROWS][COLS]) {
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLS; j++) {
            b[i][j] = ' ';
        }
    }
}
