#define _CRT_SECUER_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>

#define startrow 5
#define startcol 8
#define Maxstr 60

void tty_mode(int);
void set_crmode();
//void wait(int signum, int, int);
int main()
{
	char message[Maxstr], temp, path[60], program[20], op[2]; //store message to print, 
	int option, pid, apipe[2]; //store option number
	FILE * fa = NULL;
	int row = 5, col = 8, timer = 5;
	signal(SIGINT, SIG_IGN);

	tty_mode(0);
	set_crmode();

	//"schedule" is directory where stored textfile relative to schedule
	if(chdir("schedule") == -1)
	{
		mkdir("schedule", 0755);
	}
	else
		chdir("..");
	getcwd(path, 1024);

	initscr();
	echo();
	clear();
	refresh();
	fa = fopen("welcome.txt", "r");

	if(fa == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	//print welcome message when program start
	while( fscanf(fa, "%[^\n]",message) != EOF)
	{
		move(row, col);
		addstr(message);
		refresh();
		row++;
		sleep(1);
		fscanf(fa, "%c", &temp);
	}
	sleep(2);
	endwin();
	//main page
	while(1)
	{
		initscr();
		clear();
		row = 0;
		move(row, 0); row++;
		addstr("<MAIN PAGE>");
		row += 4;
		move(row, col); row++;
		addstr("---------option list---------");
		move(row, col); row++;
		addstr("1. check schedule");
		move(row, col); row++;
		addstr("2. edit schedule");
		move(row, col); row++;
		addstr("3. quit program");
		move(row, col);
		refresh();

		scanf("%d", &option);

		/*
		signal(SIGALRM, wait);
		alarm(5);
		scanf("%d", &option);
		pause();*/

		switch(option)
		{
			case 1: //check schedule
				
				pid = fork();
				if(pid == 0)	//chide process
				{
					initscr();
					strcpy(program, path);
					strcat(program, "/CkSchedule");
					endwin();
					execlp(program, program, NULL);
					perror("execlp failed");
					exit(1);
				}
				else // parent process
				{
					initscr();
					clear();
					wait(NULL);
				}
				break;
			case 2:
					//edit schedule
				pid = fork();
				if(pid == 0)//child process
				{
					initscr();
					strcpy(program, path);
					strcat(program, "/EdSchedule");
					endwin();
					execlp(program, program, NULL);
					perror("execlp failed");
					exit(1);
				}
				else
				{
					initscr();
					clear();
					wait(NULL);
				}
				break;
			case 3:
				//end program
				initscr();
				clear();
				row = startrow;
				move(row, col); row++;
				addstr("...end program");
				move(row, col); row++;
				addstr("bye!");
				refresh();
				sleep(3);
				clear();
				refresh();
				endwin();
				exit(1);
				break;
			default:
				move(row, col); row++;
				addstr("your input is wrong");
				move(row, col); row++;
				addstr("type number between 1 and 3");
				refresh();
		}

		clear();
		move(row, col); row++;
	}
		sleep(5);
		fclose(fa);

		tty_mode(1);

		endwin();
		exit(1);
}
void tty_mode(int num)
{
	static struct termios or_mode;
	if(num == 0)
		tcgetattr(0, &or_mode);
	else
		tcsetattr(0, TCSANOW, &or_mode);
}
void set_crmode()
{
	struct termios t_state;

	tcgetattr( 0, &t_state);
	t_state.c_lflag &= ~ICANON;
	t_state.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &t_state);
}
/*
void wait(int signum, int row, int col)
{
	int timer = 5;
	move(row, col); 
	addstr("press input in 5 sec");
	refresh();
	sleep(1);
	signal(SIGALRM, count);
	while(timer > 0)
	{
		alarm(1);
		pause();
	}
}
void count(int signum, int timer);
{
	addstr("timer : %d         ",timer);
	refresh();
}*/
