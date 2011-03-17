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
#include <string.h>

int main(int argc, char** argv)
{
	int fd;
	uint8_t i;
	int j=0;
	fd = roomba_init(argv[1]);
	roomba_safe(fd);
	tcflush(fd,TCIOFLUSH);
	struct roomba_led_status leds;
	for(i=1;i<0x3F;i=2*i)
	{
		memcpy(&leds,&i,1);
		roomba_set_leds(fd,&leds,i,254);
		sleep(1);
	}
	roomba_free(fd);
	return(0);
}

