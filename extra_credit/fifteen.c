/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void greet(void);
void init(int** board, int d);
void draw(int** board, int d);
short move(int tile, int** board, int d);
short won(int** board, int d);
int get_int();


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    int d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // allocates memory for board
    int** board = malloc(d * sizeof(int*));
    for (int i = 0; i < d; i++) {
        board[i] = malloc(d * sizeof(int));
    }

    // greets player
    greet();

    // initializes the board
    init(board, d);

    // allows moves until game is won
    while (1)
    {
        // draws the board
        draw(board, d);

        if (won(board, d))
        {
            printf("ftw!\n");
            break;
        }

        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // move if possible, else report illegality
        if (!move(tile, board, d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // frees memory 
    for (int i = 0; i < d; i++) {
        free(board[i]);
    }
    free(board);

    // success
    return 0;
}

/**
 * Get an non-negative integer from user input
 * If the input is not non-negative integer, return INT_MAX
 */
 int get_int()
 {
    int input = 0;
    short invalid = 0;

    char c = getchar();
    if (c == '\n')
        return INT_MAX;

    while (c != '\n') 
    {
        if ((c >= '0') && ( c <= '9'))
        {
            input = input * 10 + (c - '0');
        } else {
            invalid = 1;
        }

        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else 
        return input;
 }

/**
 * Greets player.
 */
void greet(void)
{    
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}


/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(int** board, int d)
{
    int count = d * d - 1;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = count;
            count--;
        }
    }

    //swap 2 and 1 if d is even
    if (d % 2 == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int** board, int d)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _ ");
            }
            else
            {
                printf("%2d ", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else returns false. 
 */
short move(int tile, int** board, int d)
{
    // find tile location
    int tile_i = -1, tile_j = -1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                tile_i = i;
                tile_j = j;
                break;
            }
        }
    }

    // check if tile can be moved
    if (tile_i != -1 && tile_j != -1)
    {
        if (tile_i > 0 && board[tile_i - 1][tile_j] == 0) // check up
        {
            board[tile_i - 1][tile_j] = tile;
            board[tile_i][tile_j] = 0;
            return 1;
        }
        else if (tile_i < d - 1 && board[tile_i + 1][tile_j] == 0) // check down
        {
            board[tile_i + 1][tile_j] = tile;
            board[tile_i][tile_j] = 0;
            return 1;
        }
        else if (tile_j > 0 && board[tile_i][tile_j - 1] == 0) // check left
        {
            board[tile_i][tile_j - 1] = tile;
            board[tile_i][tile_j] = 0;
            return 1;
        }
        else if (tile_j < d - 1 && board[tile_i][tile_j + 1] == 0) // check right
        {
            board[tile_i][tile_j + 1] = tile;
            board[tile_i][tile_j] = 0;
            return 1;
        }
    }

    return 0;
}

short won(int** board, int d)
{
    int count = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (i == d - 1 && j == d - 1)
            {
                if (board[i][j] != 0)
                {
                    return 0;
                }
            }
            else if (board[i][j] != count)
            {
                return 0;
            }
            count++;
        }
    }
    return 1;
}
