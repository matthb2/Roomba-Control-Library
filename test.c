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
	roomba_drive(fd,10,STRAIGHT_RADIUS);
	sleep(5);
	roomba_drive(fd,0,STRAIGHT_RADIUS);
	roomba_free(fd);
	return(0);
}

