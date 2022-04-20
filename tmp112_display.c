// AUTHOR:              A. R. Ansari
//E-mail:               ansarirahim1@gmail.com
//Contact:              +919024304883/+919462225303
// VERSION HISTORY: VER 1.0.0 26th OCT 2017
// VER 1.0.1 Dated 19th April 2022
// Calibration offset command support is provided
// Command to set calibration : SET_TEMP_CALIB_DATA<SPACE><OFFSET VALUE>
// Command to read calibration: SET_TEMP_CALIB_DATA
// edit: nano -c tmp112_display.c
// build:  gcc tmp112_display.c -o tmp112_display
// run: ./tmp112_display 1000 (1000 is referesh delay)
// run with bash:
// rtcpindriver==>source /etc/environment
// bash for service: DISPLAY7_TEMP
// service: DISPLAY7_TEMP.service
// GET_VER Command is implented to get

#include <unistd.h>
#include <string.h> // this contains the std::string class.
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include "tmp112.h"
#include <limits.h>
char buf[MYBUFSIZE]; // Output of the command
char buf2[39];       // 35+];
char chardisplayvalue[10];
char TempBuffer[4];
int main(int argc, char **argv)
{
        FILE *ptr;
	int c;

        if ((ptr = popen(COMMAND_GET_TEMP_CALIB_DATA, "r")) != NULL)
                while (fread(buf, sizeof(buf), 1, ptr))
                        while ((c = getchar()) != EOF)
                                (void)pclose(ptr);
        float calibdata = atof(buf); //
                                     // Create I2C bus
        float cTemp = 0;
        int displayederror = 0;
        int delay_ms = atoi(argv[1]);
        system(ENABLE_UART4);
        while (1)
        {
                while (1)
                {
                        while (1)
                        {
                                int file;
                                char *bus = "/dev/i2c-1";
                                if ((file = open(bus, O_RDWR)) < 0)
                                {
                                        printf("Failed to open the i2c bus. \r\n");
                                        displayederror = 1;
                                        system("Display7Brightness");
                                        system("Display7Home0");
                                        system("Display7Dp0");
                                        system("Display7FErrr");
                                }
                                // Get I2C device, TMP112 I2C address is 0x48(72)
                                else
                                {
                                        ioctl(file, I2C_SLAVE, 0x48);

                                        // Select configuration register(0x01)
                                        // Continous Conversion mode, 12-Bit Resolution, Fault Queue is 1(0x60)
                                        // Polarity low, Thermostat in Comparator mode, Disables Shutdown mode(0xA0)
                                        char config[3] = {0};
                                        config[0] = 0x01;
                                        config[1] = 0x60;
                                        config[2] = 0xA0;
                                        write(file, config, 3);
                                        int i = 0;
                                        for (i = 0; i < 1000; i++)
                                                usleep(delay_ms);

                                        // Read 2 bytes of data from register(0x00)
                                        // temp msb, temp lsb
                                        char reg[1] = {0x00};
                                        write(file, reg, 1);
                                        char data[2] = {0};
                                        if (read(file, data, 2) != 2)
                                        {
                                                displayederror = 2;
                                                system("Display7Brightness");
                                                system("Display7Home0");
                                                system("Display7Dp0");
                                                system("Display7CErrr");
                                                printf("ERROR: CONNECTION ERROR\r\n");
                                        }
                                        else
                                        {
                                                // Convert the data to 12-bits
                                                int temp = (data[0] * 256 + data[1]) / 16;
                                                if (temp > 2047)
                                                {
                                                        temp -= 4096;
                                                }
                                                cTemp = temp * 0.0625;
                                                float fTemp = (cTemp * 1.8) + 32;
                                                cTemp = cTemp + calibdata;
                                                // Output data to screen
                                                printf("TEMP=%.4f\r\n", cTemp);
                                                displayederror = 0;
                                        }

                                        int decimalpt = 0;
                                        int intdecimalpt = 0;

                                        if (displayederror == 0)
                                        {
                                                if (cTemp < 10)
                                                {
                                                        decimalpt = 10;
                                                        intdecimalpt = 10 * cTemp;
                                                }

                                                else if (cTemp < 100)
                                                {
                                                        decimalpt = 100;
                                                        intdecimalpt = 100 * cTemp;
                                                }
                                                else if (cTemp < 100)
                                                {
                                                        decimalpt = 100;
                                                        intdecimalpt = 100 * cTemp;
                                                }
                                                else if (cTemp < 1000)
                                                {
                                                        decimalpt = 1000;
                                                        intdecimalpt = 1000 * cTemp;
                                                }

                                                memset(chardisplayvalue, 10, '\0'); //
                                                memset(TempBuffer, 4, '\0');        //
                                                sprintf(chardisplayvalue, "%d", intdecimalpt);
                                                i = 0;
                                                for (i = 0; i < 4; i++)
                                                        TempBuffer[i] = chardisplayvalue[i];
                                                memset(buf2, sizeof(buf2), '\0');
                                                printf("\nDisplay=%s", TempBuffer);
                                                sprintf(buf2, "%s%s",COMMAND_DISPLAY7SEGSTRING,TempBuffer); //

                                                system("Display7Brightness");
                                                system("Display7Home0");
                                                system("Display7Dp0");
                                                system(buf2); // cmd);//.c_str());
                                                if (cTemp < 10)
                                                {
                                                        system("Display7Dp1");
                                                }
                                                else if (cTemp < 100)
                                                {
                                                        system("Display7Dp2");
                                                }
                                                else if (cTemp < 1000)
                                                {
                                                        system("Display7Dp3");
                                                }
                                                else
                                                        system("Display7Dp0");
                                        }

                                        close(file);
                                }
                        }
                }
        }
}

