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
	char song[16][2];
	for(i=31;i<(31+16);i++)
	{
		song[j][0]=i;
		song[j][1]=254;
		j++;
	}
	roomba_define_song(fd,1,song,16);
	roomba_play_song(fd,1);
	char cmd[2] = {138,2};
	write(fd,&cmd,2);
//	roomba_off(fd);
	tcflush(fd,TCIOFLUSH);
	roomba_free(fd);
	return(0);
}

