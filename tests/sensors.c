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
	char input;
	fd = roomba_init(argv[1]);
	roomba_safe(fd);
	tcflush(fd,TCIOFLUSH);

	struct roomba_sensor_data sensor_data;
	while(1)
	{
		printf("bump sensors 0x%x\n",sensor_data.bumps_wheeldrops);
		printf("wall 0x%x\n",sensor_data.wall);
		printf("left cliff sensor 0x%x\n",sensor_data.cliff_left);
		printf("front left cliff sensor 0x%x\n",sensor_data.cliff_front_left);
		printf("chage is %d\n",sensor_data.charge);
		printf("Buttons are 0x%x\n",sensor_data.buttons);
		roomba_read_sensor_data(fd,&sensor_data);
		input = getc(stdin);
		if(input=='q')
			return(0);
	}
	roomba_free(fd);
	return(0);
}

