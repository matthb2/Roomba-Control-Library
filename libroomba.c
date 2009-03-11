#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h> //for endianness conversion function

#include "libroomba.h"

struct termios old_settings;

int roomba_init(char* device)
{
	int port_fd;
	char buf;
	/* Setup the Port.. Roomba Discovery starts at 57600/8n1 */
	struct termios t = {
		.c_cflag = B57600 | CS8 | CLOCAL | CREAD,
		.c_iflag = IGNPAR | IGNBRK | ICRNL,
		.c_oflag = 0,
		.c_lflag = 0,
		.c_cc[VMIN] = 26, /* At most 26 char's expected from roomba */
		.c_cc[VTIME] = 5, /*Wait 1/2 a second for read */
	};
	port_fd = open(device, O_RDWR | O_NOCTTY);
	if(port_fd == 0) {
		printf("Error opening port\n");
		return 0;
	}
	tcgetattr(port_fd,&old_settings); /* Backup Old Settings for later */
	tcflush(port_fd,TCIOFLUSH);
	tcsetattr(port_fd,TCSANOW,&t);
	buf = 128;
	write(port_fd,&buf,1); /* Start SCI */
	buf = 130;
	write(port_fd,&buf,1); /* Enable Serial Control */
	tcflush(port_fd,TCIOFLUSH);
	current_speed = 0;
	return(port_fd);
}

int roomba_safe(int roomba_fd)
{
	char cmd = 131;
	if(write(roomba_fd,&cmd,1) == 1)
		return(roomba_fd);
	printf("Error Writing to Roomba\n");
	tcflush(roomba_fd,TCIOFLUSH);
	return(0);
}

int roomba_full(int roomba_fd)
{
	char cmd = 132;
	if(write(roomba_fd,&cmd,1) == 1)
		return(roomba_fd);
	printf("Error Writing to Roomba\n");
	tcflush(roomba_fd,TCIOFLUSH);
	return(0);
}

int roomba_off(int roomba_fd)
{
	char cmd=133;
	if(write(roomba_fd,&cmd,1) == 1)
		return(roomba_fd);
	printf("Error Writing to Roomba\n");
	tcflush(roomba_fd,TCIOFLUSH);
	return(0);
}
int roomba_play_song(int roomba_fd,char song)
{
	char cmd[2];
	cmd[0] = 141;
	cmd[1] = song;
	if(write(roomba_fd,&cmd,2) == 2)
		return(roomba_fd);
	printf("Error Writing to Roomba\n");
	tcflush(roomba_fd,TCIOFLUSH);
	return(0);
}
int roomba_define_song(int roomba_fd,char song, char song_data[][2], char length)
{
	if(length>16 || length < 1) {
		printf("Song not a valid length\n");
		return(0);
	}
	if(song < 0 || song > 15) {
		printf("Invalid Song\n");
		return(0);
	}
	char* cmd_str = malloc(sizeof(char)*length*2+3);
	int i,j;
	int k = 0;
	cmd_str[0] = 140;
	cmd_str[1] = song;
	cmd_str[2] = length;
	for(i=0;i<length;i++)
	{
		for(j=0;j<2;j++)
		{
			cmd_str[k] = song_data[i][j];
			k++;
			/* song_data[i][0] == note (see SCI Spec P.5) */
			/* song_data[i][1] == note length (in 1/64" 0<x<255) */
		}
	}
	write(roomba_fd,cmd_str,(length*2)+3);
	tcflush(roomba_fd,TCIOFLUSH);
	free(cmd_str);
	return(roomba_fd);
}
int roomba_free(int roomba_fd)
{
	tcflush(roomba_fd,TCIOFLUSH);
	tcsetattr(roomba_fd,TCSANOW,&old_settings);
	close(roomba_fd);
	return(0);
}

int roomba_set_motors(int roomba_fd,char motor_status)
{
	char cmd[2];
	cmd[0]=138;
	cmd[1]=motor_status;
	write(roomba_fd,cmd,2);
	tcflush(roomba_fd,TCIOFLUSH);
	return(roomba_fd);
}

int roomba_drive(int roomba_fd, int16_t speed, int16_t radius)
{
	/* Waste the first 8bits to get the alignment right */
	union drivecmd
	{
		uint8_t cmdstr[6];
		uint16_t cmdints[3];
	} cmd;
	cmd.cmdstr[0] = 0;
	cmd.cmdstr[1] = 137;
	cmd.cmdints[1] = htons((uint16_t)speed);
	cmd.cmdints[2] = htons((uint16_t)radius);
	write(roomba_fd,(cmd.cmdstr+1),5);
	tcflush(roomba_fd,TCIOFLUSH);
	current_speed = speed;
	return(roomba_fd);
}
int roomba_ramp_up(int roomba_fd, int16_t speed, int16_t radius, 
		int16_t increment)
{
	int16_t i;
	for(i=current_speed;i<speed;i+=(speed/increment))
	{
		if(i>speed)i=speed;
		if(roomba_drive(roomba_fd,i,radius) == 0)
		{
			printf("Error Setting Drive Speed\n");
			return(0);
		}
		usleep(RAMP_DELAY);
	}
	return roomba_fd;
}
int roomba_ramp_down(int roomba_fd, int16_t speed, int16_t radius, 
		int16_t increment)
{
	int16_t i;
	for(i=current_speed;i>=speed;i-=(speed/increment))
	{
		if(i<speed)i=speed;
		if(roomba_drive(roomba_fd,i,radius) == 0)
		{
			printf("Error Setting Drive Speed\n");
			return(0);
		}
		usleep(RAMP_DELAY);
	}
	return roomba_fd;
}

int roomba_force_seeking_dock(int roomba_fd)
{
	char cmd = 143;
	if(write(roomba_fd,&cmd,1) != 1)
		return(0);
	tcflush(roomba_fd,TCIOFLUSH);
	return(roomba_fd);
}

int roomba_read_sensor_data(int roomba_fd, struct roomba_sensor_data* data)
{
	char cmd[2] = {142,0};
	char reply[26];
	if(write(roomba_fd,cmd,2) != 2)
	{
		perror("Error Requesting Sensor Data");
		return(0);
	}
	tcflush(roomba_fd,TCIOFLUSH);
	if(read(roomba_fd,reply,26) != 26)
	{
		perror("Error Getting Sensor Data");
		return(0);
	}
#ifdef STRUCT_JUST_WORKS
	memcpy(data,reply,26);
	return(roomba_fd);
#else
	data->bumps_wheeldrops = reply[0];
	data->wall = reply[1];
	data->cliff_left = reply[2];
	data->cliff_front_left = reply[3];
	data->cliff_front_right = reply[4];
	data->cliff_right = reply[5];
	data->virtual_wall = reply[6];
	data->motor_overcurrent = reply[7];
	data->dirt_detector_left = reply[8];
	data->dirt_detector_right = reply[9];
	data->remote_opcode = reply[10];
	data->buttons = reply[11];
	data->distance = (int16_t) (0 | reply[12] << 8) | reply[13];
	data->angle = (int16_t) (0 | reply[14] << 8) | reply[15];
	data->charging_state = reply[16];
	data->voltage = (uint16_t) 0 |(reply[17] << 8) | reply[18];
	data->current = (int16_t) 0 | (reply[19] << 8) | reply[20];
	data->temperature = reply[21];
	data->charge = (uint16_t) 0 | (reply[22] << 8) | reply[23];
	data->capacity = (uint16_t) 0 | (reply[24] << 8) | reply[25];
	return(roomba_fd);
#endif
}

int roomba_set_leds(int roomba_fd,struct roomba_led_status* data,
		uint8_t power_color, uint8_t power_intensity)
{
	char cmd[4] = {139,0,0,0};
	if(data->dirt_detect)
		cmd[1] |= 0x01;
	if(data->max)
		cmd[1] |= 0x02;
	if(data->clean)
		cmd[1] |= 0x04;
	if(data->spot)
		cmd[1] |= 0x08;
	if(data->status_red)
		cmd[1] |= 0x10;
	if(data->status_red)
		cmd[1] |= 0x20;
	cmd[2] = power_color;
	cmd[3] = power_intensity;
	if(write(roomba_fd,cmd,4) != 4)
		perror("Error Writing LED Byte");
	return(roomba_fd);
}
