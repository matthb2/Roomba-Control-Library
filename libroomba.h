#ifndef __LIBROOMBA__
#define __LIBROOMBA__

int roomba_init(char* device);

/* Put the roomba into safe mode */
/* Allows full control unless charger or cliff is detected */
int roomba_safe(int);

/* Releases the port */
int roomba_free(int);

/* Powers off the Roomba */
int roomba_off(int);

/* Plays a predefined "song" */
/* Song number should be between 0 and 15 inclusive */
int roomba_play_song(int, char song);

/* Define a song
 * song: song number (0-15)
 * song_data[1-16][0] =  note (31-127 (others are assumed to be a rest))
 * song_data[1-16][1] = note duration in 1/64 seconds
 */
int roomba_define_song(int,char song, char song_data[][2], char length);
#endif
