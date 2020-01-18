#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#define ROWS 18
#define COLS 36
char board[ROWS][COLS];


//this  struct holds 2 parameters:
//	col: column in a table
//	row: row in a table
typedef struct  place {
	int col;
	int row;
}PLACE;

//a linked list 
//	data: PLACE place- coordinates on a (x,y) table, 
//	
typedef struct Snake {
	PLACE place;
	struct snake* next;
}SNAKE;


// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulatoion).

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);

//initializes the board according to set parameters
void initBoard();

//print the board
void printBoard();

//moves the snake in a direction
// parameters:
//	head: pointer to start of a linked list
//	tail: returns the last node in the list
//	move: direction to move in
void snakeMove(SNAKE* head,SNAKE** tail, char move);

//forward each link in the snake forward to the location of the link prior to it
//	tail: returns the last node in the list
void forwardSnake(SNAKE* head, SNAKE** tail);

//move snake up
void Up(SNAKE* head, SNAKE** tail);

//move snake left
void Left(SNAKE* head, SNAKE** tail);

//move snake right
void Right(SNAKE* head, SNAKE** tail);

//move snake down
void Down(SNAKE* head, SNAKE** tail);

//add new node to the end of a SNAKE linked list
//parameters:
//	head: pointer to SNAKE linked list 
//	place: coordenates of the new node
void addNode(SNAKE* head, SNAKE** tail,  PLACE place);

//print '@' in a certain location on the board
// parameters: 
//	place: coordenates for where to print
void printNode(PLACE place);

//print ' ' in a certain location on the board
// parameters: 
//	place: coordenates for where to print
void removeTail(PLACE place);

//create a new node of a SNAKE linked list
//parameters:
//	place: coordenates of the new node
SNAKE* newNode(PLACE place);

//checks if the move will end the game
//return true if the game will NOT end
//return false if game will end
//parameters:
//	head: pointer to start of a linked list
//	move: direction to check in
bool checkMove(SNAKE* head, char move);

//makes sure the move is legal
//returns the move if yes
//parameters:
//	prev_move: the last move made
//	next_move: move to check if legal
int makeMove(int prev_move, int next_move);

//increases speed by 0.03 seconds if speed is > 0.1
//parameters:
//	speed: hte speed in seconds
float speedChange(float speed);

bool ateApple(SNAKE* head, char move);

void generateApple();

//free the SNAKE memory
void freeSnake(SNAKE* head);


int main() {
	SNAKE* head;
	SNAKE* tail;
	char move = 'd';
	char temp_move = 'd';

	PLACE place;
	place.col = COLS/2;
	place.row = ROWS/2;
	bool lose = false;
	float speed;
	time_t t;
	srand((unsigned)time(&t));
	PLACE apple;
	int score;
	char newgame = 'y';
	int check;

	while (newgame == 'y') {
		init();
		initBoard();
		printBoard();
		check = 0;
		score = 0;
		speed = 0.2;
		head = newNode(place);
		place.col--;
		move = 'd';
		temp_move = 'd';
		addNode(head, &tail, place);
		generateApple();

		while (lose == false) {
			if (_kbhit()) {
				temp_move = _getch();
				move = makeMove(move, temp_move);
			}

			if (ateApple(head, move) == true) {
				addNode(head, &tail, tail->place);
				generateApple();
				score++;
				speed = speedChange(speed);
				gotoxy(2, COLS + 4);
				printf("Score:%d", score);
				gotoxy(29, 0);

			}
			if (checkMove(head, move) == false) {
				lose = true;
			}
			else {
				snakeMove(head, &tail, move);
				gotoxy(29, 0);
				

				sleep(speed);
			}
		}
		
			gotoxy(9, 12);
			printf("GAME OVER!\n");
			gotoxy(10, 13);
			printf("Score:%d", score);
			gotoxy(11, 9);
			printf("New game?:Y / N");
			while (check == 0) {
				if (_kbhit()) {
					newgame = _getch();
					if (newgame == 'y' || newgame == 'n'){
						check = 1;
						lose = false;
					}
			}
		}
	}

	freeSnake(head);
}
void freeSnake(SNAKE* head) {
	SNAKE* temp;
	while (head != NULL) {
		temp = head;
		free(temp);
		head = head->next;
	}
}


SNAKE* newNode(PLACE place) {

	SNAKE* new = (SNAKE*)malloc(sizeof(SNAKE));
	if (new == NULL) {
		printf("ERROR!");
	}
	else {
		new->next = NULL;
		new->place = place;
		return new;
	}
}

void addNode(SNAKE* head, SNAKE** tail, PLACE place) {
	SNAKE* new;
	
	while (head->next != NULL) {
		head = head->next;
	}
	new = newNode(place);
	head->next = new;
	*tail = new;
	

}

bool checkMove(SNAKE* head,char move) {
	PLACE place = head->place;
	switch (move)
	{
	case 'w':
	{
		if (place.row == 2 || board[place.row-1][place.col] == '@')
		{
			return false;
		}
		else {

			return true;
		}

		break;
	}
	case 'a':
	{
		if (place.col == 2 || board[place.row][place.col - 1] == '@')
		{
			return false;
		}
		else {
			return true;
		}
		break;
	}
	case 'd':
	{
		if (place.col==COLS-1 || board[place.row][place.col + 1] == '@')
		{
			return false;
		}
		else {
			return true;
		}
		break;
	}
	case 's':
	{
		if (place.row == ROWS-1 || board[place.row+1][place.col] == '@')
		{
			return false;
		}
		else {
			return true;
		}
		break;
	}

	default: {

	}
		   break;
	}

}

int makeMove(int prev_move, int next_move)
{
	if (prev_move == 'w' || prev_move == 's') {
		if (next_move == 'a' || next_move == 'd') {
			prev_move = next_move;
		}
	}
	else {
		if (next_move == 'w' || next_move == 's') {
			prev_move = next_move;
		}
	}
	return prev_move;
}

float speedChange(float speed)
{
	if (speed > 0.1) {
		if (speed - 0.03 >= 0.1) {
			speed -= 0.03;
		}
		else {
			speed = 0.1;
		}
	}
	return speed;
}

bool ateApple(SNAKE* head, char move){
	PLACE place = head->place;
		switch (move)
	{


	case 'w':
	{
		if (board[place.row-1][place.col] == 'O')
		{
			return true;
		}
		else {

			return false;
		}

		break;
	}
	case 'a':
	{
		if (board[place.row][place.col - 1] == 'O')
		{
			return true;
		}
		else {

			return false;
		}
		break;
	}
	case 'd':
	{
		if (board[place.row][place.col + 1] == 'O')
		{
			return true;
		}
		else {

			return false;
		}
		break;
	}
	case 's':
	{
		if (board[place.row+1][place.col] == 'O')
		{
			return true;
		}
		else {

			return false;
		}
		break;
	}

	default: {

	}
		   break;
	}

}

void generateApple()
{
	PLACE place;
	do {
		place.col = rand() % 34;
		place.row = rand() % 16;
		if (place.col <=1) {
			place.col+=2;
		}
		if (place.row <=1) {
			place.row+=2;
		}
	} while (board[place.row][place.col] == '@'|| board[place.row][place.col] == '#');
	board[place.row][place.col] = 'O';
	gotoxy(place.row, place.col);
	printf("O");

}



void snakeMove(SNAKE* head, SNAKE** tail, char move) {
	switch (move)
	{

	case 'w':
	{
		Up(head, tail);
		break;
	}
	case 'a':
	{
		Left(head, tail);
		break;
	}
	case 'd':
	{
		Right(head, tail);
		break;
	}
	case 's':
	{
		Down(head, tail);
		break;
	}

	default: {

	}
		break;
	}
	
}

void Up(SNAKE* head, SNAKE** tail) {
	head->place.row--;
	forwardSnake(head, tail);
	
}

void Left(SNAKE* head, SNAKE** tail) {
	head->place.col--;
	forwardSnake(head, tail);
}

void Right(SNAKE* head, SNAKE** tail) {
	head->place.col++;
	forwardSnake(head, tail);
}

void Down(SNAKE* head, SNAKE** tail) {
	head->place.row++;
	forwardSnake(head, tail);
}

void printNode(PLACE place) {
	board[place.row][place.col] = '@';
	gotoxy(place.row, place.col);
	printf("@");
}

void removeTail(PLACE place) {
	board[place.row][place.col] = ' ';
	gotoxy(place.row, place.col);
	printf(" ");
}

void forwardSnake(SNAKE* head, SNAKE** tail) {
	printNode(head->place);
	PLACE temp_place1 = head->place;
	PLACE temp_place2=temp_place1;
	while (head->next != NULL) {
		head = head->next;
		temp_place2 = head->place;
		head->place = temp_place1;
		temp_place1 = temp_place2;
	}
	*tail=head;
	removeTail(temp_place2);
}

void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs * CLOCKS_PER_SEC)
		;
}

void initBoard()
{

	int i, j;

	for (i = 0; i < ROWS; i++)
	{
		if (i == 0||i==ROWS-1) {
			for (j = 0; j < COLS; j++)
			{
				board[i][j] = '#';
			}
		}
		else {


			for (j = 0; j < COLS; j++)
			{
				if (j == 0 || j == COLS - 1)
				{
					board[i][j] = '#';
				}
				else {
					board[i][j] = ' ';
				}
			}
		}
	}
}

void printBoard() {

	for (int i = 0; i < ROWS; i++) {

		for (int j = 0; j < COLS; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");

	}
}