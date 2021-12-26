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
void go_to_main(int);
void print_tail();

int main()
{
	char path[30];
	int row = Srow, col = Scol;

	while(1)
	{
		int option;
		char filename[7];
		FILE * fa = NULL;
		signal(SIGINT, go_to_main );

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
					char num[3];
					char schedule[Maxstr-6], line[Maxstr];
					clear();
					print_tail();
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
						fflush(stdin);
						move(row, col);
						addstr("Please type again");
						refresh();
					}
					clear();
					row = Srow;
					move(row, col); col = col + 6;
					addstr("Date :");
					move(row, col); row++;
					addstr(filename);
					refresh();
					col = Scol;
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
					while(1)
					{
						scanf("%d:%d", &sh, &sm);
						move(row,col); 
						if(sh >= 0 && sh <= 23 && sm > 0 && sm < 60)
							break;
						addstr("type again");
						refresh();
					}
					fflush(stdin);
					row = row-2;
					move(row, col); row++;
					addstr("type time when you end the schedule");
					move(row, col); row++;
					addstr("example : '21:30'");
					refresh();
					while(1)
					{
						scanf("%d:%d", &eh, &em);
						move(row, col);
						if(eh >= 0 && eh <24 && em >= 0 && em < 60) 
							break;
						addstr("type again");
						refresh();
					}
					fflush(stdin);
					move(row, col); row++;
					if(new == 1)
					{
						addstr("type schedule what you want to add");
						refresh();
						gets(line);
						fflush(stdin);
						addstr("gecha!");
						refresh();
						fprintf(fa,"%d:%d   %d:%d   %s",sh, sm, eh, em, line);
						row = Srow;
						move(row, col);
						addstr("Add Schedule successfully");
						refresh();
						sleep(1);
						endwin();
						exit(1);
					}
					else
					{
						FILE * fb = NULL;
						int ish, ism, ieh, iem, write = 0;
						char iline[Maxstr];
						while(1)
						{
							fscanf(fa, "%d:%d   %d:%d   %[^/n]", &ish, &ism, &ieh, &iem,iline);
							if(feof(fa))
								break;
							if( (100 * ish + ism < 100*sh + sm && 100*sh + sm < 100*ieh + iem) || (100 * ish + ism < 100 * eh + em && 100 * eh + em < 100 * ieh + iem) )
							{
								move(row, col); 
								addstr("schedule is already exist in that time");
								refresh();
								sleep(2);
								endwin();
								exit(1);
							}
						}
						fseek(fa, 0L, SEEK_SET);
						fb = fopen("temp", "w");
						while(1)
						{
							fscanf(fa, "%d:%d   %d:%d   %[^\n]", &ish, &ism, &ieh, &iem, iline);
							if(feof(fa))
							{
								if(write == 0)
								{
									move(row, col); row++;
									addstr("type schedule what you want to add");
									refresh();
									scanf("%[^/n]", line);
									fprintf(fb, "%d:%d   %d:%d   %s", sh, sm, eh, em, line);
								}
								break;
							}
							if(100 * ish + ism > 100 * eh + em)
							{
								move(row, col); row++;
								addstr("type schedule what you want to add");
								refresh();
								scanf("%[^/n]", line);
								fprintf(fb, "%d:%d   %d:%d   %s", sh, sm, eh, em, line);
								write = 1;
							}
							fprintf(fb, "%d:%d   %d:%d   %s", ish, ism, ieh, iem, iline);
						}
						fclose(fb);
					}

					fclose(fa);
					if(remove(filename) == -1)
					{
						perror("fail to remove");
						exit(1);
					}
					if(rename("temp", filename) == -1)
					{
						perror("fail to rename");
						exit(1);
					}
					clear();
					row = Srow;
					move(row, col);
					addstr("Add schedule successfully");
					refresh();
					sleep(1);
					endwin();
					exit(1);
				}
			case 2:
				{
					FILE * fb = NULL;
					char schedule[Maxstr];
					int line = 1, delete;
					clear();
					print_tail();
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
void print_tail()
{
	move(20, 45);
	addstr("press Ctrl + 'C' to go main");
}
