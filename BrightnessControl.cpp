#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
int main(int argc , char **argv){
 /*char tx_buffer[4];
/
int  counts=atoi(argv[1]);
for (int i=2;i<argc;i++)
{
tx_buffer[i-2]=(atoi(argv[i]));
}*/
unsigned char tx_buffer[2];
tx_buffer[0]=0x7A;
tx_buffer[1]=atoi(argv[1]);//0b00111000;//=56;
printf("\n%x %x",tx_buffer[0],tx_buffer[1]);

int fd = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY); //Open in non blocking read/write mode

	if (fd == -1) {
		//ERROR - CAN'T OPEN SERIAL PORT
		printf(	"Error - Unable to open UART.  Ensure it is not in use by another application\n");
return -1;
	}

write(fd, tx_buffer, 2);
close(fd);
return 0;
}
