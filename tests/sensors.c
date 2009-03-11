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

