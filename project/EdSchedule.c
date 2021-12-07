#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#define Srow 5
#define Scop 10
int main()
{
	char path[30];
	int row = Srow, col = Scol;

	initscr();
	clear();
	move(row, col);row++;
	addstr("---EDIT SCHEDULE!!---");
	move(row, col); row++;
	addstr("1. Add schedule");
	move(row,col); row++;
	addstr("2. Delete Schedule");
	move(row, col); row++;
	addstr("3. Go back to main");
	refresh();

	scanf("%d", &option);
	switch(option)
	{
		case 1:
			{
				int new = 0;//check file is new
				int sh, sm, eh, em;
				clear();
				row = Srow;
				move(row, col); row++;
				addstr("Type date when you want to add the schedule");
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
				fa = fopen(filename, "r+");
				if(fa == NULL)
				{
					fa = fopen(filename, "w+");
					new = 1;
				}
				move(row, col); row++;
				addstr("type time when you start the schedule");
				move(row, col); row++;
				addstr("example : '06:30'");
				refresh();
				scanf("%d:%d", &sh, &sm);
				row = row-2;
				move(row, col); row++;
				addstr("type time when you end the schedule");
				move(row, col); row++;
				addstr("example : '21:30'");
				refresh();

				/*
				   check code whether that time is enable
				   */


				

				break;
			}
	}
}
