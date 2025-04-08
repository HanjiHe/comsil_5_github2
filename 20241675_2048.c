#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over = 1 , b[4][4];
FILE *fp;

int make_two_or_four();

void init(){
	int i, j;
	game_over = 0;
    // set the board to be all zeros
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			b[i][j] = 0;
		}
	}
    // the board starts with 2 random spots with either 2 or 4
	make_two_or_four();
	make_two_or_four();
}

//functions to be called when moving the board
void rotate(int arr[4][4]){
	// create a copy of the board 
    int c[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			c[i][j] = arr[i][j];
		}
	}
    // reset values of the board inversing cols and rows 
    for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			arr[i][j] = c[3-j][i];
		}
	}
}

void shift(int b[4][4], int dir){
    //left = 0, right = 1;
    int shift;
    // shift all values left
    if (dir == 0){
        while(1){
            shift = 0;
            for (int i = 0; i < 4; i++){
                for (int j = 2; j >= 0; j--){
                    if (b[i][j] == 0 && b[i][j+1] != 0){
                        b[i][j] = b[i][j+1];
                        b[i][j+1] = 0;
                        shift = 1;
                    }
                }
            } 
            if (!shift){
                break;
            }
        }
    // shift all values right
    } else if (dir == 1){
        while (1){
            int shift = 0;
            for (int i = 0; i < 4; i++){
                for (int j = 1; j < 4; j++){
                    if (b[i][j] == 0 && b[i][j-1] != 0){
                        b[i][j] = b[i][j-1];
                        b[i][j-1] = 0;
                        shift = 1;
                    } 
                }
            }
            if (!shift){
                break;
            }
        }
    }
}

void merge_right(int b[4][4]){
    // shift all values right
    shift(b, 1);
    //if there are two consecutive spots with the same value, merge them to the right
	for (int i = 0; i < 4; i++){
        for (int j = 3; j > 0; j--){
            if (b[i][j] == b[i][j-1] && b[i][j] != 0){
                b[i][j] *= 2;
                b[i][j-1] = 0;
                shift(b,1);
            } 
        }
    }
}

void merge_left(int b[4][4]){
    // shift all values left
    shift(b, 0);
    // merge any consecutive matching cells
	for (int i = 0; i < 4; i++){
        for (int j = 3; j > 0; j--){
            if (b[i][j] == b[i][j-1] && b[i][j] != 0){
                b[i][j] *= 2;
                b[i][j-1] = 0;
                shift(b,0);
            } 
        }
    }
}

// moves the board depending on command
int set_board(int dir, int b[4][4]){
	
	switch (dir)
	{
	case 0:	
		merge_right(b);
		break;
	case 1: 
        // to merge up or down , turn the board 90 degrees then merge
		rotate(b);
		merge_right(b);
        // bring back to board to its original direction
		rotate(b);
		rotate(b);
		rotate(b);
		break;
		
	case 2: 
		merge_left(b);
		break;
	case 3:
        // to merge up or down , turn the board 90 degrees then merge   
		rotate(b);
		merge_left(b);
        // bring back to board to its original direction
		rotate(b);
		rotate(b);
		rotate(b);
		break;
	default: return 0;
	}

	return 1;
}

int is_game_over(){
	// checks if there are any zeroes left
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (b[i][j] == 0){
				return 1;
			}
		}
	}
    // checks if there are any consecutive spots left to right
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			if (b[i][j] == b[i][j+1]){
				return 1;
			}
		}
	}
    // checks if there are any consecutive spots up and down
	rotate(b);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			if (b[i][j] == b[i][j+1]){
				return 1;
			}
		}
	}
	rotate(b);
	rotate(b);
	rotate(b);
	/* if game over return 0, else then return 1 */
	return 0;
}

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */
	if (command == 0){
        
	}
	/* calculate score & check sum of all block equasls variable tot */
	int tot = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			tot += b[i][j];
			if (b[i][j] > score){
				score = b[i][j];
			}
		}
	}


	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
}

int make_two_or_four(){
	// freeInd is an array that stores all indexes possible on the board, numFree counts the number of spots at 0
	int freeInd[16][2], numFree = 0;
	/* user code */
    // store indexes of spots on the board that are still zero and count them
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (b[i][j] == 0){
				freeInd[numFree][0] = i;
				freeInd[numFree][1] = j;
				numFree++;
			}
		}
	}
    // return if there are no empty spots
	if (numFree == 0){
		return 0;
	}
    // make a 
	int random = rand() % numFree;
	int i = freeInd[random][0];
	int j = freeInd[random][1];
    b[i][j] = (rand() % 3 < 2) ? 2 : 4;
	/* return 2 or 4 that makes in this times */

	/* if can not make two or four, then return 0 */
	return 1;
}


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;

	/* make init board */
	/* user code */
	init();

	draw_board(tot, -1);

	do {
		command = GetCommand();

		if (command != -1) {
            // if you can move the board
			if( set_board(command, b) ){
				/* user code */
                // checks if the game is over
				if (!is_game_over()){
					game_over = 0;
				}
                // redraw the new board
				draw_board(tot, command);
                // add 2 or 4
				make_two_or_four();
			}
		}

	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}
