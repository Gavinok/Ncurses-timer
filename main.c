//cd /home/gavinok/Programming/ncurses_practice/multi_stopwatch_ncurses
// gcc -o multitimer main.c -lncurses
// ./multitimer
#include <ncurses.h>
#include <time.h>
#include <stdio.h>
//#include <timer.h>
#define KEY_H 104
#define KEY_J 106
#define KEY_K 107
#define KEY_L 108
#define KEY_RENTER 10
#define NUMBER_OF_BUTTONS 3
enum states
{
	INIT,
	START,
	RESTART,
	SET,
	END,
} state;
//=====================multitimer.c=======
void initializeCurses()
{
	initscr();			// Start curses mode
	//raw();
	noecho();
	keypad(stdscr, TRUE);//allow reading form keyboard
}
bool boundryReached(bool direction, int y, int clocknum)
{
	//if at the limit return true
	if(y == clocknum+1 || y == clocknum+5)
		return true;
	else
		return false;
}
//====================timer.c========
void timer(int limit, int timerx, int timery)
{
	long int msec = 0, trigger = limit ; /* 10ms */
	clock_t before = clock();
	do {
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		mvprintw(timerx,timery,"Time taken %d seconds %d milliseconds\n",
		msec/1000, msec%1000);
		refresh();
	} while ( msec < trigger );
	{
		mvprintw(timerx,timery,"Time taken %d seconds %d milliseconds\n",
		msec/1000, msec%1000);
	}
}
int setTimer()
{
	int timerval;
	mvprintw(2, 13,"how long is the timer");
	scanf("%d", &timerval);	
	return (timerval*1000);
}
enum states checkButton(int index)
{
	//there are 6 lines for every clock the options ar at points 1,3, and 5.
	//the number clock 2 then has this at points 7,9, and 11
	//clock num 
	//number of numcloc * 3 
	//1, 2, and 3 are start, restart, and set in that order
	//0 is an error;
	int button = 0;
	if((index+1)%2 == 0)
	{
		switch(index)
		{
			case 1:
				//start timer
				return START; 
				break;
			case 3:
				//restart timer
				return RESTART;
			case 5:
				//change timer
				return SET;
			default:
				printw("failed to determin button");
		}
	}
	else{
	printw("error with button location");
	}
}
void setupDisplay(bool state, int Start_Loc)
{
	// draw the boarder starting at the location specified
	for(int y=Start_Loc;y<=(Start_Loc+6);y++){
		if(y%2 != 0){
			mvaddch(y, 0, '|');
			mvaddch(y, 9, '|');
		}else{
			for(int i=1;i<=8;i++) {
				mvaddch(y, i, '-');

			}
		}
	}
	if(state == 0)
	{
		//braw the start and stop keys
		mvprintw(Start_Loc+1,1," Start");
		mvprintw(Start_Loc+3,1," Restart");
		mvprintw(Start_Loc+5,1," Set");
	}else{
		//braw the stop and stop keys
		mvprintw(Start_Loc+1,1," Stop");
		mvprintw(Start_Loc+3,1," Zero");
	}
	refresh();
}
enum states selectOption()
{
	int timerval = 2;
	int inputChar;
	//state 1, 2, and 3 are start, reset, and set 0 is an error
	int state = 0;
	int y = 1;
	move(y,1);
	refresh();
	while(inputChar != 'q')
	{
		inputChar = getch();
		if(inputChar != KEY_RENTER){

			switch (inputChar){

				case KEY_K:// = 107
					if(y != 1){
						move(y-=2, 1);
					}
					break;

				case KEY_J:// = 106
					if(y != 5){
						move(y+=2, 1);
					}
					break;
			}
			refresh();
		}else
		{
			//3 posible locations (1,3,5) num/3
			//if y is at 3 its on start if its on 9 its on start
			state = checkButton(y);
		}
	}
	return END;

}
void stateHandler()
{
	state = INIT;
	//default timer is 2000
	int time = 2000;
	while(state != END)
	{
		switch(state)
		{
			case INIT:
				state = selectOption();
				break;
			case START:
				timer(1, 10, time);
				state = INIT;
				break;
			case RESTART:
				//not implemented
				timer(1, 10, time);
				state = INIT;
				break;
			case SET:
				time = setTimer();
				state = INIT;
				break;
		}
	}

}
int main()
{
	initializeCurses();
	setupDisplay(0,0);
	stateHandler();
	refresh();			// Print it on to the real screen
	getch();			// Wait for user input
	endwin();			// End curses mode
	return 0;
}
