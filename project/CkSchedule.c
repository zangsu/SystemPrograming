#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
	int option, pid;
	char path[30];
	FILE * fa = NULL;
	int row = 5, col = 10;

	initscr();
	clear();
	move(0,0);
	addstr("---CHECK SCHEDULE!!---");
	move(1, 0);
	addstr("1.check today schedule.");
	move(2, 0);
	addstr("2.check another day schedule.");
	move(3, 0);
	addstr("3. go back to main");
	refresh();

	scanf("%d", &option);
	switch(option)
	{
		case 1:	//check today schedule
		{
			//get today date
			time_t timer;
			char filename[7], mon[3], day[3], schedule[40];
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
				move(row, col); row++;
				addstr("start  end  schedule");
				move(row, col); row++;
				addstr("time   time ");
				move(row, col); row++;
				addstr("----------------------------------");
				refresh();
				while(1)
				{
					fgets(schedule, 40, fa);
					if(feof(fa))
						break;
					move(row,col);row++;
					addstr(schedule);
					refresh();
					sleep(1);
				}



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
			char filename[7], schedule[40], check[2];
			clear();
			move(row, col); row++;
			addstr("Type date when you want to check the schedule");
			move(row, col); row++;
			addstr("example : '210612'");
			scanf("%s", filename);
			
		}
		default:
			break;
	}
	endwin();
	exit(1);
}
