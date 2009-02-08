#include <stdio.h>
#include <stdlib.h>
/*Debug*/
#include <termios.h>
#include <unistd.h>
/*Debug*/
#include "libroomba.h"

int main(int argc, char** argv)
{
	int fd;
	fd = roomba_init(argv[1]);
	roomba_safe(fd);
	roomba_off(fd);
	tcflush(fd,TCIOFLUSH);
	roomba_free(fd);
	return(0);
}

