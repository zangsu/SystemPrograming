#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define Srow 5
#define Scol 10
#define Maxstr 60

int main()
{
	char path[30];
	int row = Srow, col = Scol;

	while(1)
	{
		int option;
		char filename[7];
		FILE * fa = NULL;

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
			case 2:
				{
					FILE * fb = NULL;
					char schedule[Maxstr];
					int line = 1, delete;
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
					fa = fopen(filename, "r");
					if(fa == NULL)
						{
						move(row, col);
						addstr("the day has no schedule");
						refresh();
						sleep(2);
						endwin();
						exit(1);
						}
					clear();
					row = Srow;
					move(row, col); row++;
					addstr("start time / end time / shcedule");
					move(row, col); row++;
					addstr("--------------------------------");
					refresh();
					while(1)
					{
						char num[2];
						sprintf(num, "%d", line++);
						fgets(schedule,Maxstr, fa);
						if(feof(fa))
							break;
						move(row, col);
						addstr(num);
						move(row, col+1);
						addstr(".");
						move(row, col+3);row++;
						addstr(schedule);
						refresh();
						sleep(1);					
					}
					move(row, col); row++;
					addstr("Type number what you want to delete");
					move(row, col);
					refresh();
					fclose(fa);
					while(1)
					{
						scanf("%d", &delete);
						if(delete > 0 && delete < line-1)
							break;
						move(row, col);
						addstr("your input is wrong. Type again");
						refresh();
					}
					
					//remove schedule
					line = 1;
					fa = fopen(filename, "rt");
					fb = fopen("temp", "w");
					while(1)
					{
						fgets(schedule, Maxstr, fa);
						if(feof(fa))
							break;
						if(line == delete)
						{
							line++;
							continue;
						}
						line++;
						fprintf(fb, "%s", schedule);		
					}
					fclose(fa);
					fclose(fb);

					if(remove(filename) == -1)
					{
						perror("file remove fail");
						exit(1);
					}
					if(rename("temp", filename) == -1)
					{
						perror("fail to change filename");
						exit(1);
					}

					clear();
					row = Srow;
					move(row, col);
					addstr("Remove schedule successfully");
					refresh();
					sleep(1);
					endwin();
					exit(1);
				}
			case 3:
					clear();
					row = Srow;
					move(row, col);
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
	}
	endwin();
	exit(1);
}
