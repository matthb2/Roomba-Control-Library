/*     
 *      Copyright 2011 Ben Matthews <ben@kc2vjw.com>
 *     
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
*/
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
			case 'j':roomba_ramp(fd,0-SPEED,STRAIGHT_RADIUS,50);break;
			case 'l':roomba_ramp(fd,SPEED,CLOCKWISE_INPLACE_RADIUS,50);break;
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

