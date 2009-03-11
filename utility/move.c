#include <stdio.h>
#include <stdlib.h>
/*Debug*/
#include <termios.h>
#include <unistd.h>
/*Debug*/
#include "libroomba.h"
#include <curses.h>
#define SPEED 1000
int main(int argc, char** argv)
{
	initscr(); //setup curses
	noecho(); //don't echo keypresses
	cbreak(); //no line buffering

	int fd;
	fd = roomba_init(argv[1]);
	roomba_safe(fd);
	roomba_full(fd);
	tcflush(fd,TCIOFLUSH);
	while(1)
	{
		switch(getch())
		{
			case 'h':roomba_drive(fd,SPEED,CCLOCKWISE_INPLACE_RADIUS);
				 break;
			case 'k':roomba_drive(fd,SPEED,STRAIGHT_RADIUS);break;
			case 'j':roomba_ramp(fd,0-SPEED,STRAIGHT_RADIUS,100);break;
			case 'l':roomba_ramp(fd,SPEED,CLOCKWISE_INPLACE_RADIUS,100);break;
			case 's':roomba_ramp(fd,0,0,100);break;
			case ERR:roomba_drive(fd,0,0);break;
			case 'q':goto end;
			default:roomba_drive(fd,0,0);break;
		}
		tcflush(fd,TCIOFLUSH);
	}
end:
	roomba_free(fd);
	endwin(); //close curses window/reset terminal
	return(0);
}

