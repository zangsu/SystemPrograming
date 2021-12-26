#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define Srow  5
#define Scol  10
#define Maxstr 60

void printfile(char * filename, int ,int);
void print_tail();
void go_to_main(int signum);
int main()
{
	int option, pid;
	char path[30];
	FILE * fa = NULL;
	int row = 5, col = 10;
	signal(SIGINT, go_to_main);

	initscr();
	clear();
	move(row, col); row++;
	addstr("---CHECK SCHEDULE!!---");
	move(row, col); row++;
	addstr("1.check today schedule.");
	move(row, col); row++;
	addstr("2.check another day schedule.");
	move(row, col); row++;
	addstr("3.go back to main");
	refresh();

	scanf("%d", &option);
	switch(option)
	{
		case 1:	//check today schedule
		{
			//get today date
			time_t timer;
			char filename[7], mon[3], day[3], schedule[Maxstr];
			struct tm* t;
			timer = time(NULL);
			t = localtime(&timer);
			sprintf(filename, "%d", t->tm_year - 100);
			sprintf(mon, "%d", t->tm_mon + 1);
			if(strlen(mon) == 1)
				strcat(filename, "0");
			strcat(filename, mon);
			sprintf(day, "%d", t->tm_mday);
			if(strlen(day) == 1)
				strcat(filename, "0");
			strcat(filename, day);
					
			pid = fork();
			if(pid == -1)
			{
				perror("fork error");
				exit(1);
			}
			else if(pid == 0)//child process
			{
				chdir("schedule");
				getcwd(path, 1024);
				clear();
				move(0, 0);
				fa = fopen(filename, "rt");
				if( fa == NULL)
				{
					move(row, col);
					addstr("today's schedule is empty");
					refresh();
					sleep(1);
					endwin();
					exit(1);
				}
				fclose(fa);

				printfile(filename, row, col);		

				refresh();
				sleep(3);
				endwin();
				exit(1);
			}
			else
			{
				wait(NULL);
				exit(1);
			}
		}
		case 2:
		{
			char filename[7], schedule[Maxstr], check[2];
			clear();
			print_tail();
			move(row, col); row++;
			addstr("Type date when you want to check the schedule");
			move(row, col); row++;
			addstr("example : '210612'");
			refresh();
			while(1)
			{
				scanf("%s", filename);
				if(strlen(filename) == 6)
					break;
				move(row, col);
				addstr("Please type again");
				refresh();
			}
			chdir("schedule");
			fa = fopen(filename, "rt");
			if(fa == NULL)
			{
				move(row,col);
				addstr(filename);
				addstr(" has no schedule");
				row++;
				refresh();
				sleep(2);
				endwin();
				exit(1);
			}
			printfile(filename, row, col);

			sleep(1);
			endwin();
			exit(1);
		}
		case 3:
		{
			clear();
			row = Srow;
			move(row,col);
			addstr("Going to main.");
			refresh();
			sleep(1);
			col = col+strlen("Going to main.");
			move(row, col);
			addstr(".");
			refresh();
			sleep(1);
			col++;
			move(row, col);
			addstr(".");
			refresh();
			sleep(1);
			endwin();
			exit(1);

		}
		default:
			break;
	}
	endwin();
	exit(1);
}
void printfile(char * filename, int row, int col)
{
	char schedule[Maxstr];
	FILE * fa = NULL;
	fa = fopen(filename, "rt");
	clear();
	move(row, col); row++;
	addstr("start time / end time / schedule");
	move(row, col); row++;
	addstr("--------------------------------");
	refresh();
	while(1)
	{
		fgets(schedule, Maxstr, fa);
		if(feof(fa))
			break;
		move(row, col); row++;
		addstr(schedule);
		refresh();
		sleep(1);
	}
	fclose(fa);

}
void print_tail()
{
	move(20, 45);
	addstr("press Ctrl + 'C' to go main menu");
}
void go_to_main(int signum)
{
	int row, col;
	clear();
	row = Srow;
	col = Scol;
	move(row, col);
	addstr("Going to main.");
	refresh();
	sleep(1);
	col = col + strlen("Going to main.");
	move(row, col);
	addstr(".");
	refresh();
	sleep(1);
	col++;
	move(row, col);
	addstr(".");
	refresh();
	sleep(1);
	endwin();
	exit(1);
}
