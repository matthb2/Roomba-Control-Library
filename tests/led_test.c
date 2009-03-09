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

