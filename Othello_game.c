#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void flip(char *x) {  //performs the function of changing the X to O or vice versa
    if(*x=='X') {
        *x='O';
    } else if(*x=='O') {
        *x='X';
    }
}

int check_valid(char (*arr)[8], char x, int row, int col) {  //checks sandwich conditions and flips accordingly
    if (row >= 0 && row < 8 && col >= 0 && col < 8 && arr[row][col] != ' ' && arr[row][col] != x) {
        return 1;
    }
    return 0;
}

int func(char (*arr)[8], char x, int row, int col, int m, int n, int k) {
    int j = col + n;
    int i = row + m;
    if((i + m) >= 0 && (i + m) < 8 && (j + n) >= 0 && (j + n) < 8) {
        if (check_valid(arr, x, i, j) == 0) {
            return 0;
        }
        if(arr[i + m][j + n] != ' ') {
            if(arr[i + m][j + n] == x) {
                if(k == 0) {
                    flip(&arr[i][j]);
                }
                return 1;
            } else if(arr[i + m][j + n] != x) {
                if(func(arr, x, i, j, m, n, k) == 1) {
                    if(k == 0) {
                        flip(&arr[i][j]);
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

int declare_result(char (*a)[8]) {  //declares result, counts the scores
    int count_x = 0;
    int count_o = 0;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(a[i][j] == 'X') {
                count_x++;
            } else if(a[i][j] == 'O') {
                count_o++;
            }
        }
    }

    if(count_x + count_o == 64) {
        char winner;
        if(count_o > count_x) {
            winner = 'O';
        } else if(count_x > count_o) {
            winner = 'X';
        } else {
            winner = 'T';
        }

        printf("\033[5;31m");  //changes color to red whose code is 31
        printf("GAME OVER\n");
        printf("\033[0m");   //reverts the color back to white
        printf("\033[5;32m");
        printf("X has %d points\n", count_x);
        printf("\033[0m");
        printf("\033[5;32m");
        printf("O has %d points\n", count_o);
        printf("\033[0m");
        printf("\033[33m");
        printf("WINNER IS %c\n", winner);
        printf("\033[0m");
        return 1;
    }

    printf("\033[5;33m");  //color change
    printf("\nSCORES: X = %d  O = %d", count_x, count_o);  //prints scores
    printf("\033[37m");
    return 0;
}

int endgame(char (*a)[8], int turn) {  //checks and declares result when the entire board is occupied by only X or O
    int count_x = 0;
    int count_o = 0;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(a[i][j] == 'X') {
                count_x++;
            }
            if(a[i][j] == 'O') {
                count_o++;
            }
        }
    }

    if(turn > 2) {  //initial condition, as board is unfilled
        if(count_x == 0) {
            printf("\033[36m");
            printf("O is the winner.");
            printf("\033[0m");
            return 0;
        }
        if(count_o == 0) {
            printf("\033[36m");
            printf("X is the winner.");
            printf("\033[0m");
            return 0;
        }
    }
    return 1;
}

int main() {
    int p = 0;
    char a[8][8], y;
    int row, col;

    char play1[100], play2[100];

    printf("\033[5;36m");
    printf("Welcome to this Othello adventure:\n");
    printf("Please enter player-1: ");
    scanf("%s", play1);
    printf("\nPlease enter player-2: ");
    scanf("%s", play2);
    printf("\033[0m");

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            a[i][j] = ' ';  //fills each cell with a space
        }
    }

    a[3][3] = 'O';
    a[4][4] = 'O';
    a[3][4] = 'X';
    a[4][3] = 'X';
    int turn = 1;
    char x;
    while(turn <= 100) {
        if(turn % 2 == 1) {
            x = 'X';
            printf("\033[5;36m");
            printf("\n\n%s's Turn (X)\n", play1);
        } else {
            x = 'O';
            printf("\n\n%s's Turn (O)\n", play2);
            printf("\033[0m");
        }
        printf("\033[0m");  //part which prints the box
        printf("     1    2    3    4    5    6    7    8   \n");
        printf("   ---- ---- ---- ---- ---- ---- ---- ----  \n");
        for(int i = 0; i < 8; i++) {
            printf("%d ", i + 1);
            for(int j = 0; j < 8; j++) {
                printf("| %c  ", a[i][j]);
            }
            printf("|\n");
            printf("   ---- ---- ---- ---- ---- ---- ---- ----  \n");
        }
        printf("\033[32m");
        printf("Enter your input (row col): ");   //input part which takes the place where x or o will be chosen to be placed
        printf("\033[0m");
        scanf("%d %d", &row, &col);
        row--;
        col--;

        if (row >= 0 && row < 8 && col >= 0 && col < 8 && a[row][col] == ' ') {   //condition to increase turn
            a[row][col] = x;
            turn++;
        } else {         //incase of invalid move which is a move which is already occupied
            printf("\033[31m");
            printf("Invalid move!!!!\n");
            printf("\033[0m");
            continue;
        }

        // Eight conditions to check sandwich and flip if possible
        int A = func(a, x, row, col, 1, 0, 0),    //row check direction in right
            B = func(a, x, row, col, -1, 0, 0),   //row check direction in left
            C = func(a, x, row, col, 0, 1, 0),    //column check in direction of positive y
            D = func(a, x, row, col, 0, -1, 0),   //column check in direction of negative y
            E = func(a, x, row, col, 1, 1, 0),    //diagonally northeast
            F = func(a, x, row, col, -1, -1, 0),  //diagonally south-west
            G = func(a, x, row, col, 1, -1, 0),   //diagonally south-east
            H = func(a, x, row, col, -1, 1, 0);   //diagonally north-west

        if((A == 0 && B == 0 && C == 0 && D == 0 && E == 0 && F == 0 && G == 0 && H == 0)) {  //when all these conditions are returning 0, means no sandwich occurs and hence no flipping occurs
            //makes the move invalid
            printf("\033[31m");
            printf("\nPLAYER HAS ENTERED INVALID MOVE\nENTER AGAIN.\n");
            printf("\033[0m");
            a[row][col] = ' ';
            turn--; //reduces the turn as we are keeping a count of only valid turns
            continue; //continues
        }

        if(declare_result(a) == 1) {  //when declare result returns 1 means game has ended
            printf("\033[0m");
            printf("     1     2     3     4     5     6     7     8  \n");
            printf("   ---- ---- ---- ---- ---- ---- ---- ----  \n");
            for(int i = 0; i < 8; i++) {
                printf("%d ", i + 1);
                for(int j = 0; j < 8; j++) {
                    printf("| %c  ", a[i][j]);
                }
                printf("|\n");
                printf("   ---- ---- ---- ---- ---- ---- ---- ----  \n");
            }
            break;
        }
        if(endgame(a, turn) == 0) {
            break;
        }
    }
    return 0;
}
