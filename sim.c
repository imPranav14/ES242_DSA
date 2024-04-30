#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
 * The status of a line.
 */
enum
{
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

void init_board(board_t board)
{
    for (int i = 0; i < 15; i++)
    {
        board[i] = 0;
    }
}

player_t other_player(player_t player)
{
    switch (player)
    {
    case RED:
        return BLUE;
    case BLUE:
        return RED;
    default:
        return 0;
    }
}

int has_loose(board_t board, player_t player)
{
    int c = 4;
    for (int i = 0; i < 4; i++)
    {
        if ((board[i] != player))
        {
            c = c + 3 - i;
            continue;
        }
        for (int j = i + 1; j < 5; j++)
        {
            if (board[j] == board[i])
            {

                if (board[i] == board[j + c])
                {
                    return 1;
                }
            }
        }
        c = c + 3 - i;
    }

    c = 3;
    for (int i = 5; i < 8; i++)
    {
        if ((board[i] != player))
        {
            c = c + 7 - i;
            continue;
        }
        for (int j = i + 1; j <= 8; j++)
        {
            if (board[j] == board[i])
            {
                if (board[i] == board[j + c])
                {
                    return 1;
                }
            }
        }
        c = c + 7 - i;
    }

    c = 2;
    for (int i = 9; i < 11; i++)
    {
        if ((board[i] != player))
        {
            c = c + 10 - i;
            continue;
        }
        for (int j = i + 1; j <= 11; j++)
        {
            if (board[j] == board[i])
            {
                if (board[i] == board[j + c])
                {
                    return 1;
                }
            }
        }
        c = c + 10 - i;
    }
    if (board[12] == player)
    {
        if ((board[12] == board[13]) && (board[12] == board[14]))
        {
            return 1;
        }
    }
    return 0;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i)
    {
        switch (board[i])
        {
        case RED:
            printf("R  ");
            break;
        case BLUE:
            printf("B  ");
            break;
        case NO:
            printf("0  ");
            break;
        }
    }
    printf("\n");
}

void print_num()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%d  ", i);
    }
    for (int i = 10; i < 15; i++)
    {
        printf("%d ", i);
    }

    printf("\n");
}

typedef struct
{
    int line;  /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

move_t best_move(board_t board, player_t player)
{

    move_t response;
    int no_candidate = 1;
    move_t candidate;

    for (int i = 0; i < 15; i++)
    {
        if (board[i] == 0)
        {
            board[i] = player;
            if (!has_loose(board, player))
            {
                no_candidate = 0;
                board[i] = other_player(player);
                if (!has_loose(board, other_player(player)))
                {
                    board[i] = 0;
                    candidate = (move_t){
                        i,
                        1};
                    return candidate;
                }
            }
            else
            {
                if (no_candidate)
                {
                    candidate = (move_t){
                        i,
                        -1};
                }
            }
            board[i] = 0;
        }
    }
    if (no_candidate)
    {
        return candidate;
    }

    for (int i = 0; i < 15; i++)
    {
        if (board[i] == 0)
        {
            board[i] = player;
            if (!has_loose(board, player))
            {
                response = best_move(board, other_player(player));
                board[i] = 0;

                if (response.score == -1)
                {
                    return (move_t){
                        i,
                        1};
                }
                else
                {
                    candidate = (move_t){
                        .line = i,
                        .score = -1};
                }
            }
            else
            {
                board[i] = 0;
            }
        }
    }
    return candidate;
}

int main()
{
    /* Your game play logic. */
    /*arr[15] is created to keep track of empty spaces*/
    int arr[15];
    for(int i = 0; i < 15; i++){
        arr[i] = -1;
    }
    int line;
    board_t board;
    move_t response;
    player_t player, current;
    char c;
    printf("Select a player---> RED or BLUE\n");
    scanf("%s", &c);
    switch (c)
    {
    case 'R':
        player = RED;
        break;
    case 'B':
        player = BLUE;
        break;
    default:
        player = RED;
    }
    init_board(board);

    current = player;
    while (1)
    {
        if (current == 1)
        {
            print_board(board);
            print_num();
            printf("Enter your move:- \n");
            scanf("%d", &line);
            while(1){
            if(arr[line]==-1){
                arr[line] = line;
                board[line] = current;
                break;
            }
            else{
                printf("It is invalid move\n");
                printf("Enter your move:- \n");
                scanf("%d", &line);

            }
        }

            

            
            
        }
        else
        {
            response = best_move(board, current);
            board[response.line] = current;
            arr[response.line] = response.line; 
            
        }

        if (has_loose(board, current))
        {
            print_board(board);
            print_num();
            printf("Player %s has won!\n", (other_player(current) == RED) ? "RED" : "BLUE");
            break;
        }

        current = other_player(current);
    }

    return 0;
}

