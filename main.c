#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int COLUMN = 8;
const int ROW = 8;
const int MAXDISCS = 64;

struct Board
{
    char board[8][8];
    int playerTurn;	/*playerTurn's 0 or 1 value indicate the turn of the First Player or Second player respectively */
};

struct Player
{
    char firstPlayerName[50];
    char secondPlayerName[50];

    int firstPlayerScore;
    int secondPlayerScore;

    int firstPlayerMoves;
    int SecondPlayerMoves;
};

void PrintBoard(int r, int c, struct Board *Sboard, struct Player *p);
void InitializeBoard(int r, int c, struct Board *Sboard);
void InitalizePlayers(struct Player *p);
void SaveResult(char data[], char WinnerName[], char score[]);


int UpdateBoard(char[], struct Board *Sboard, int, int);
int UpdateScore(struct Board *b, struct Player *p);
int IsMovesLeft(struct Board *b, int t);
int ShowWinner(struct Player *p);
int CheckInput(char[]);

int main()
{
    struct Board s_B1; /*strcut for Board*/
    struct Player s_P; /*strcut for Players*/
    InitializeBoard(ROW, COLUMN, &s_B1);
    printf("***Welcome to Othello! ***\n");
    InitalizePlayers(&s_P);
    PrintBoard(ROW, COLUMN, &s_B1, &s_P);

    int inGame = 1; /*inGame value is used for ending or starting the game*/
    char playerInput[3]; /*playerInput holds the player input value*/
    

    while (inGame)
    {
        start: if (s_B1.playerTurn == 0)
                printf("%s (Black) Input: ", s_P.firstPlayerName);
            else
                printf("%s (White) Input: ", s_P.secondPlayerName);
        scanf("%s", &playerInput);




        int r; /*used for checking input result*/
        do {
            r = CheckInput(playerInput); /*CheckInput return 1 if the input is valid(p or[1-8][a-h] otherwise 0*/
            if (r != 1)
            {
                printf("Input is wrong, Input Again: ");
                scanf("%s", &playerInput);
            }
        } while (r == 0);




        if (UpdateBoard(playerInput, &s_B1, s_B1.playerTurn, 1) == 1) /*UpdateBoard funtion return 1 if the move by the user is not valid otherwise it effect the board and returns 0*/
        {
            printf("%s is Invalid Move: \n", playerInput);
            goto start;

        }

        if (UpdateScore(&s_B1, &s_P) == 1 ||  (IsMovesLeft(&s_B1, 1) == 0 && IsMovesLeft(&s_B1, 1)==0)) /*UpdateScore Board returns 1 if board is full otherwise it return 1 and in both cases update the score board*/
        { 																		/*IsMovesLeft return 0 if no moves left to play */
            inGame = 0; 	/*Ending the game*/
			system("cls"); 			/*make this line comment if you want to see history of steps taken*/												
            PrintBoard(ROW, COLUMN, &s_B1, &s_P);
            int res=ShowWinner(&s_P);        /*Displaying the Winner statistics*/
            if(res==0) 			
			{
				char  scr[3];
    			snprintf(scr, sizeof(scr), "%d", s_P.secondPlayerScore);
    			scr[2]='\n';											/*converting the int to string*/
				SaveResult("Winner is ", s_P.secondPlayerName, scr ); 	/*saving the data in the file*/
			}
			else if(res==1)
			{
				char  scr[3];
    			snprintf(scr, sizeof(scr), "%d", s_P.firstPlayerScore);
    			scr[2]='\n';
				SaveResult("Winner is => ", s_P.firstPlayerName, scr ); 	/*saving the data in the file*/
			}
			else
			{
				char  scr[3];
    			snprintf(scr, sizeof(scr), "%d", s_P.firstPlayerScore);
    			scr[2]='\n';
				SaveResult("The Match is ", "Tie ", scr); 	/*saving the data in the file*/
			}
	 	} 											
        else
        {
            system("cls"); 			/*make this line comment if you want to see history of steps taken*/
            PrintBoard(ROW, COLUMN, &s_B1, &s_P);

            if (s_B1.playerTurn == 0)
                s_B1.playerTurn = 1;
            else
                s_B1.playerTurn = 0;
        }
    }

    return 0;
}

void InitializeBoard(int r, int c, struct Board *b)
{
    /*
    	initialize the Board with default values, each board element with space
    	Score to zero's and isWinner to zero
    */
    int i, j;

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
            b->board[i][j] = ' ';
    }

    /*setting the starting state of the board*/
    b->board[3][3] = 'W';
    b->board[3][4] = 'B';

    b->board[4][3] = 'B';
    b->board[4][4] = 'W';
    
    b->playerTurn=0;

}

void InitalizePlayers(struct Player *p)
{
    /*Takes the players Struct and ask players to enter names*/
    printf("Enter name of Player 1 (Black): ");
    fgets(p->firstPlayerName, 50, stdin);

    p->firstPlayerName[strlen(p->firstPlayerName) - 1] = '\0'; /*Removing the extra newline character*/

    printf("\nEnter name of Player 2 (White): ");
    fgets(p->secondPlayerName, 50, stdin);
    p->secondPlayerName[strlen(p->secondPlayerName) - 1] = '\0'; /*Removing the extra newline character*/

    p->firstPlayerScore = 2;
    p->secondPlayerScore = 2;

}

void PrintBoard(int r, int c, struct Board *b, struct Player *p)
{
    /*Funtion print the Board*/
    int i, j;

    printf("\tScore: %s (Black) %d:%d %s (White)\n", p->firstPlayerName, p->firstPlayerScore, p->secondPlayerScore, p->secondPlayerName);
    printf("\t    a   b   c   d   e   f   g   h\n");
    printf("\t   --- --- --- --- --- --- --- ---\n");
    for (i = 0; i < r; i++)
    {
        printf("\t%d |", i + 1);
        for (j = 0; j < c; j++)
            printf(" %c |", b->board[i][j]);

        printf("\n");
        printf("\t   --- --- --- --- --- --- --- ---\n");
    }

    printf("\t    a   b   c   d   e   f   g   h\n");

}

int CheckInput(char a[])
{
    /*CheckInput take the user input array and check whether they 
    are valid or not valid character[s] are 
    'p' or[1-8][a-h] in lowercase for characters and 
    return 1 if they are valid otherwise 0
    */

    if (strlen(a) == 1) /*if length is one then must be a single 'p' character otherwise error*/
    {
        if (a[0] == 'p')
            return 1;
        else
            return 0;
    }
    else
    {
       	//checking whether they are in the form of[1-8][a-h]
        if (
            ((a[0] - '0') >= 1 && (a[0] - '0') <= 8) &&

            ((a[1]) >= 'a' && (a[1]) <= 'h')
       )
            return 1;
        else
            return 0;
    }
}

int UpdateScore(struct Board *b, struct Player *p)
{
    /*This function update the board and return 1 if the board is full otherwise it return 0*/
    int whiteCount = 0, blackCount = 0, filled = 0;
    int i, j;	//loop variables
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (b->board[i][j] == 'W')
            {
                whiteCount++;
                filled++;
            }

            if (b->board[i][j] == 'B')
            {
                blackCount++;
                filled++;
            }
        }
    }

    p->firstPlayerScore = blackCount;
    p->secondPlayerScore = whiteCount;
    if (filled == MAXDISCS)	//max. entries in board is 64
        return 1;
    return 0;
}

int ShowWinner(struct Player *p)
{
    /*This function shows the statistics of the winner and return 1 if first player is winner and 0 if second player is winner and -1 if tie*/
    int s1 = p->firstPlayerScore;
    int s2 = p->secondPlayerScore;
    if (s1 > s2)
    {
    	printf("%s is Winner with %d Score\n", p->firstPlayerName, s1);
    	return 1;
	}
        
    else if (s1 == s2)
    {
    	printf("Game is Tied\n");
    	return -1;
	}
        
    else
    {
    	printf("%s is Winner with %d Score\n", p->secondPlayerName, s2);
    	return 0;
	}
        

}


void SaveResult(char data[], char name[], char score[]) 			
{
	/*Funtion to store the results on the disk having date, time and result*/
	
	time_t now = time(0);
   	char *dt = ctime(&now);		/*getting the time and removing the leading \n character*/
   	dt[strlen(dt) - 1]='\0';
    
	FILE *fp;  
	fp=fopen("othello-results.txt","a");  	/*Opening the file in the append mode*/
	if(fp == NULL)
    {
        printf("Error occur while openin/creating the file.\n");    
    }
    else
    {
    	fputs(dt,fp);  	//writting the date and time
		fputs(" : ",fp); 
		fputs(data,fp); 
		fputs(name,fp); 
		fputs(" => score is => ",fp);
		fputs(score,fp); 
		printf("Result has been saved on the disk with file name \'othello-results.txt\' \n");
		fclose(fp);
	}

}

int UpdateBoard(char a[], struct Board *b, int t, int update)
{
    /*UpdateBoard is the funtion which put the effect of the player input on the board 
    and return 1 if the move is not allowed otherwise put the effect and return 0*/

    if (a[0] == 'p') /*If user input 'pass' then it check if it is passable, if it is then return 0 otherwise 1*/
    {
        if (IsMovesLeft(b, t))
        {
            printf("You can not pass your move right now.\n");
            return 1;
        }
        else
            return 0;
    }
    else
    {
        int r, c;
        r = (a[0] - '0') - 1; /*converting to int  and -1 because index starts by zero*/

        char temp = 'a'; /*temp varible having initial 'a' character, we are using it to compare and iterate*/
        for (c = 0; c < 8; c++)
        {
            if (a[1] == temp)
                break;
            else
                temp++;
        }

        char matchCharacter, InsertCharacter;
        if (t) /*if the player is Black disc, then matchCharacter is W, and insertCharacter is B*/
        {
            matchCharacter = 'B';
            InsertCharacter = 'W';
        }
        else
        {
            matchCharacter = 'W';
            InsertCharacter = 'B';
        }

        struct Board prevBoard; /*storing the current snapshot of the board*/
        int m, n;
        for (m = 0; m < 8; m++)
            for (n = 0; n < 8; n++)
                prevBoard.board[m][n] = b->board[m][n];

        int invalidMove = 1;
        if (b->board[r][c] == ' ') /*here we have 8 possible cases where can made effect on board by the user input*/
        {
            if (b->board[r][c + 1] != ' ' && b->board[r][c + 1] == matchCharacter) /*case for right side*/
            {
                int k, start = c, end = -1;
                for (k = c + 1; k < 8; k++)
                {
                    if (prevBoard.board[r][k] == matchCharacter)
                        continue;
                    else
                    {
                        if (prevBoard.board[r][k] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                if (end != -1)
                {
                    if (update) /*if this fucntion is called for update then we update the board*/
                    {
                        for (k = start; k <= end; k++)
                            b->board[r][k] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r][c - 1] != ' ' && b->board[r][c - 1] == matchCharacter) /*case for left side*/
            {
                int k, start = c, end = -1;
                for (k = c - 1; k >= 0; k--)
                {
                    if (prevBoard.board[r][k] == matchCharacter)
                        continue;
                    else
                    {
                        if (prevBoard.board[r][k] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                if (end != -1)
                {
                    if (update)
                    {
                        for (k = end; k <= start; k++)
                            b->board[r][k] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r - 1][c] != ' ' && b->board[r - 1][c] == matchCharacter) /*case for upper side*/
            {
                int k, start = r, end = -1;
                for (k = r - 1; k >= 0; k--)
                {
                    if (prevBoard.board[k][c] == matchCharacter)
                        continue;
                    else
                    {
                        if (prevBoard.board[k][c] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                if (end != -1)
                {
                    if (update)
                    {
                        for (k = end; k <= start; k++)
                            b->board[k][c] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r + 1][c] != ' ' && b->board[r + 1][c] == matchCharacter) /*case for bottom side*/
            {
                int k, start = r, end = -1;
                for (k = r + 1; k < 8; k++)
                {
                    if (prevBoard.board[k][c] == matchCharacter)
                        continue;
                    else
                    {
                        if (prevBoard.board[k][c] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                if (end != -1)
                {
                    if (update)
                    {
                        for (k = start; k <= end; k++)
                            b->board[k][c] = InsertCharacter;

                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r + 1][c + 1] != ' ' && b->board[r + 1][c + 1] == matchCharacter) /*case for lower right diagnol*/
            {
                int k, start = c, end = -1;
                int temp = r + 1;
                for (k = c + 1; k < 8; k++)
                {
                    if (prevBoard.board[temp][k] == matchCharacter)
                        temp++;
                    else
                    {
                        if (prevBoard.board[temp][k] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                temp = r;
                if (end != -1)
                {
                    if (update)
                    {
                        for (k = start; k <= end; k++)
                            b->board[temp++][k] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r - 1][c - 1] != ' ' && b->board[r - 1][c - 1] == matchCharacter) /*case for upper left diagnol*/
            {
                int k, l = r - 1, start = c, end = -1;

                for (k = c - 1; k >= 0; k--)
                {
                    if (prevBoard.board[l][k] == matchCharacter)
                    {
                        l--;
                    }
                    else
                    {
                        if (prevBoard.board[l][k] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                l = r;
                if (end != -1)
                {
                    if (update)
                    {
                        for (k = start; k != end; k--)
                            b->board[l--][k] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r - 1][c + 1] != ' ' && b->board[r - 1][c + 1] == matchCharacter) /*case for upper right side*/
            {
                int k, start = r, end = -1;

                int temp = c + 1;

                for (k = r - 1; k >= 0; k--)
                {
                    if (prevBoard.board[k][temp] == matchCharacter)
                        temp++;
                    else
                    {
                        if (prevBoard.board[k][temp] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                temp = c;
                if (end != -1)
                {
                    if (update)
                    {
                        for (k = start; k >= end; k--)
                            b->board[k][temp++] = InsertCharacter;
                    }

                    invalidMove = 0;

                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            if (b->board[r + 1][c - 1] != ' ' && b->board[r + 1][c - 1] == matchCharacter) /*case for lower left side*/
            {
                int k, start = r, end = -1;
                int temp = c - 1;
                for (k = r + 1; k < 8; k++)
                {
                    if (prevBoard.board[k][temp] == matchCharacter)
                        temp--;
                    else
                    {
                        if (prevBoard.board[k][temp] == InsertCharacter)
                            end = k;
                        break;
                    }
                }

                temp = c;
                if (end != -1)
                {
                    if (update)
                    {
                        for (k = start; k <= end; k++)
                            b->board[k][temp--] = InsertCharacter;
                    }

                    invalidMove = 0;
                }
                else
                {
                    if (invalidMove != 0)
                        invalidMove = 1;
                }
            }

            return invalidMove;

        }
        else
            return invalidMove;
    }
}

int IsMovesLeft(struct Board *b, int t)
{
    /*Function checks whether the board for specific player has available moves or not, it return 1 if moves left*/
    char r[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };
    char c[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    char temp[2];
    int i, j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            temp[0] = r[i];
            temp[1] = c[j];
            int x=UpdateBoard(temp, b, t, 0);
            if (x==0)
				return 1;
        }
    }

    return 0;
}
