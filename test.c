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

int main(int argc, char** argv)
{
	int fd,i;
	int j=0;
	fd = roomba_init(argv[1]);
	roomba_safe(fd);
	//roomba_set_motors(fd,0x2);
	sleep(5);
	//roomba_set_motors(fd,0);
//	roomba_off(fd);
	tcflush(fd,TCIOFLUSH);
	roomba_drive(fd,50,STRAIGHT_RADIUS);
	sleep(1);
	roomba_drive(fd,0,STRAIGHT_RADIUS);
	roomba_free(fd);
	return(0);
}

