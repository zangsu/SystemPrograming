#define _CRT_SECUER_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

#define startrow 5
#define startcol 8
int main()
{
	char message[50], temp, path[60], program[20]; //store message to print, 
	int option, pid, apipe[2]; //store option number
	FILE * fa = NULL;
	int row = 5, col = 8;

	//"schedule" is directory where stored textfile relative to schedule
	if(chdir("schedule") == -1)
	{
		mkdir("schedule", 0755);
	}
	else
		chdir("..");
	getcwd(path, 1024);

	initscr();
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
		sleep(3);
		fscanf(fa, "%c", &temp);
	}
	endwin();
	//main page
	while(1)
	{
		initscr();
		clear();
		row = 0;
		move(row, 0); row++;
		addstr("<MAIN PAGE>");
		row += 5;
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

		scanf("%d", &option);//notice !! < not echoing>
		
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
					move(row, col); row++;
					addstr("this is option 1 parent process");
					refresh();
					sleep(1);

				}
				break;
			case 2:
					//edit schedule
				break;
			case 3:
				//end program
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
				sleep(3);
		}

		move(row, col); row++;
		addstr("line clear");
		refresh();
		sleep(1);
	}
		sleep(5);
		fclose(fa);

		endwin();
		exit(1);
}
