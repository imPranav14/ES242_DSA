#include<stdio.h>
#include <stdint.h>
#include<assert.h>
/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
typedef struct {//0 for draw , 1 for win , -1 for lose
    int col;
    int score;
} move_t;
int ord_board[31]={0,1,2,4,5,7,8,13,14,16,17,22,23,25,26,40,41,43,44,49,50,52,53,67,68,70,71,76,77,79,80};
enum {
    EMPTY = 0,
    RED,   // 1
    BLUE,  // 2
};
typedef char board_t[4][5];
typedef char player_t;
#define MAX_ORD (28629150)
uint8_t computed_moves[MAX_ORD+1];
uint8_t encode_move(move_t m)
{
    // W D L C O L----> 6 bits required
    uint8_t a=0;
    assert(0 <= m.col && m.col <= 4);
    a|=m.col;
    
    switch(m.score){
        case -1: a|=1<<3;break; 
        case 0: a|=1<<4;break; 
        case 1: a|=1<<5;break; 
    }
    return a;
}

move_t decode_move(uint8_t b)
{
    move_t m;
    m.col= b & ((1<<2)|(1<<1)|(1));
    if((b>>3)==1){
        m.score=-1;
    }
    if((b>>4)==1){
        m.score=0;
    }
    if((b>>5)==1){
        m.score=1;
    }
    return m;
}
int ord_col(board_t board,int col){   //MIN ord no. 0 when (0000)-->Whole column is empty. 
    int p=27;                          //MAX ord no. 80=(3^0+3^1+3^2+3^3)*2 when (2222)--->Whole col is BLUE
    int i=0;
    int d=0;
    for(int row=0;row<4;row++){
        switch(board[row][col]){
            case EMPTY: d=0;break; 
            case RED: d=1;break; 
            case BLUE: d=2;break; 
        }
        i+=p*d;
        p/=3;
    }
    for(int j=0;j<31;j++){
        if (i==ord_board[j])
            return j;
    }
    printf("%d %d \n",i,col);
    assert(0);
}


int ord(board_t board){  //MAX posssible ord=43,584,805*80=3,486,784,400 when 
                        //all ord_col below return 80 -->(81^0+81^1+81^2+81^3+81^4)*80= 
    int col1,col2,col3,col4,col5;
    col1=ord_col(board,0);
    col2=ord_col(board,1);
    col3=ord_col(board,2);
    col4=ord_col(board,3);
    col5=ord_col(board,4);
    int i=col1+col2*31+col3*961+col4*29791+col5*923521;
    return i;
}

player_t other_player(player_t player){
    switch (player)
    {
    case RED:
        return BLUE;
    
    case BLUE:
        return RED;
    }
}

void add_column(board_t board, int col,player_t player){
    for (int row=3;row>=0;row--){
        if (board[row][col]==0){
            board[row][col]=player;
            return;
        }
    }
    assert(0);
}
void remove_column(board_t board,int col, player_t player){
    int row;
    for (row=0;row<4;row++){
        if(board[row][col]==player){
            break;
        }
    }
    board[row][col]=EMPTY;
    return;
}

int has_won(board_t board, player_t player)
{
    for(int col=0;col<5;col++){
        int row=0;
        for(row;row<4;row++){
            if(board[row][col]!=player){
                break;
            }
        }
        if(row==4){
            return 1;
        }
    }
    
    for(int row=0;row<4;row++){
        int col=0;
        for(col;col<4;col++){
            if(board[row][col]!=player){
                break;
            }
        }
        if(col==4){
            return 1;
        }
    }
    
    for(int row=0;row<4;row++){
        int col=1;
        for(col;col<5;col++){
            if(board[row][col]!=player){
                break;
            }
        }
        if(col==5){
            return 1;
        }
    }

    int col,row;
    col=0;
    row=0;
    for(int i=0;i<4;i++,row++,col++){
        if(board[row][col]!=player){
            break;
        }
    }
    if(row==4){
        return 1;
    }

    col=1;
    row=0;
    for(int i=0;i<4;i++,col++,row++){
        if(board[row][col]!=player){
            break;
        }
    }
    if(row==4){
        return 1;
    }

    col=4;
    row=0;
    for(int i=0;i<4;i++,col--,row++){
        if(board[row][col]!=player){//103223411
            break;
        }
    }
    if(row==4){
        return 1;
    }

    col=3;
    row=0;
    for(int i=0;i<4;i++,col--,row++){
        if(board[row][col]!=player){
            break;
        }
    }
    if(row==4){
        return 1;
    }

    return 0;
}

int is_full(board_t board)
{
    for(int row=0;row<4;row++){
        for(int col=0;col<5;col++){
            if(board[row][col]==EMPTY){
                return 0;
            }
        }
    }
    return 1;
}

move_t best_move(board_t board, player_t player)
{   
    // print_board(board);
    move_t response;
    move_t candidate;
    int no_candidate = 1;
    int o = ord(board);
    // printf("ord no. %d\n",o);
    if (computed_moves[o]) {
        return decode_move(computed_moves[o]);
    }

    for(int col=0;col<5;col++){
        if(board[0][col]==EMPTY){
            add_column(board,col,player);
            if(has_won(board,player)){
                remove_column(board,col,player);    
                candidate=(move_t){
                    .col=col,
                    .score=1
                };
                computed_moves[o]=encode_move(candidate);
                return candidate;
            }
            remove_column(board,col,player);
        }
    }
    
        for(int col=0;col<5;col++){
            if(board[0][col]==EMPTY){
                add_column(board,col,player);
                if (is_full(board)) {
                    remove_column(board,col,player);
                    computed_moves[o] = encode_move(candidate = (move_t) {
                        .col = col,
                        .score = 0
                        });
                    return candidate;
                }
                response=best_move(board,other_player(player));
                remove_column(board,col,player);
                if(response.score==-1){
                   candidate=(move_t){
                    .col=col,
                    .score=1
                };
                computed_moves[o]=encode_move(candidate);
                return candidate; 
                }
                else if(response.score==0){
                    candidate=(move_t){
                        .col=col,
                        .score=0
                    };
                    no_candidate=0;
                }
                else{
                    // +1 i.e. otherplayer is winning and we are losing
                    if(no_candidate){
                        candidate=(move_t){
                            .col=col,
                            .score=-1
                        };
                    }
                    no_candidate=0;
                }
                
            }
        }
    
    computed_moves[o]=encode_move(candidate);
    return candidate;
}

void print_board(board_t board)
{
    for (int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
    return;
}

int main()
{
    board_t board;
    move_t response;
    player_t player,current;
    char c;
    printf("Select a player---> RED or BLUE\n");
    scanf("%s",&c);
    switch(c){
        case 'R':player=RED;break;
        case 'B':player=BLUE;break;
        default: player=RED;
    }
    
    current=player;
    // Initiating Board
    for (int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            board[i][j]=0;
        }
    }
    int col;
    while(1){
        print_board(board);
        printf("\n");
        if (current==player){
            printf("Enter Your move: ");
            scanf("%d",&col);
            // assert(!(board[col]));
            add_column(board,col,current);
        }
        else{
            printf("\n");
            response=best_move(board,current);
            printf("Computer choose to play: %d\n",response.col);
            add_column(board,response.col,current);
        }
        if(has_won(board,current)){
            print_board(board);
            printf("Player %s has won!\n", (current==RED) ? "RED":"BLUE");
            break;
        }

        else if(is_full(board)){
            print_board(board);
            printf("Draw.\n");
            break;
        }
        current=other_player(current);
    }

    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    return 0;
}
